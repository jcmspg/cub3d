/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 21:45:00 by joao              #+#    #+#             */
/*   Updated: 2025/07/29 19:04:57 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

// line normalization so 1d array can work
char *normalize_line(const char *line, int width)
{
    int len = ft_strlen(line);
    char *norm = malloc(width + 1);
    if (!norm)
        return NULL;
    ft_memcpy(norm, line, len);
    ft_memset(norm + len, ' ', width - len);  // or '~' or some defined VOID_SYMBOL
    norm[width] = '\0';
    return norm;
}


// process map line
static bool	process_map_line(char **line, int *count, int *max_len)
{
	int	len;

	if (is_empty_line(*line) || !is_valid_map_line(*line))
	{
		ft_printf_fd(STDERR_FILENO, "invalid map line [%s]\n", *line);
		free(*line);
		return (false);
	}
	(*count)++;
	len = ft_strlen(*line);
	if (len > *max_len)
		*max_len = len;
	free(*line);
	return (true);
}

// skip texture and color
static bool	process_pre_map_line(char **line, bool *map_started, int *count)
{
	if (is_texture_or_color(*line) || is_empty_line(*line))
	{
		free(*line);
		return (true);
	}
	else if (is_valid_map_line(*line))
	{
		*map_started = true;
		(*count)++;
		free(*line);
		return (true);
	}
	else
	{
		ft_printf_fd(STDERR_FILENO, "invalid line [%s]\n", *line);
		free(*line);
		return (false);
	}
}

static bool	process_line(char **line, bool *map_started, int *count,
		int *max_len)
{
	if (!*map_started)
		return (process_pre_map_line(line, map_started, count));
	return (process_map_line(line, count, max_len));
}

static bool	populate_line(char **line, char **dest, t_map *map)
{
	if (is_empty_line(*line) || !is_valid_map_line(*line))
	{
		ft_printf_fd(STDERR_FILENO, "invalid map line [%s]\n", *line);
		free(*line);
		return (false);
	}
	*dest = normalize_line(*line, map->width);
	if (!*dest)
	{
		ft_printf_fd(STDERR_FILENO,
			"Memory error while duplicating line [%s]\n", *line);
		free(*line);
		return (false);
	}
	free(*line);
	return (true);
}

static bool	populate_map_lines(char **line, bool *map_started, char **line2, t_map *map)
{
	if (!map_started)
		return (process_pre_map_line(line, map_started, NULL));
	return (populate_line(line, line2, map));
}

// Function to calculate the number of lines in the file
static bool	count_lines_in_file(t_map *map)
{
	char	*line;
	bool	map_started;
	int		line_count;
	int		max_len;
	int		i;

	line = NULL;
	map_started = false;
	line_count = 0;
	max_len = 0;
	while ((line = get_next_line(map->fd)) != NULL)
	{
		if (!process_line(&line, &map_started, &line_count, &max_len))
			return (ERR_INVALID_MAP);
	}
	close(map->fd);
	line = NULL;
	map_started = false;
	i = 0;
	map->fd = open(map->filename, O_RDONLY);
	if (map->fd < 0)
	{
		ft_putstr_fd("Error: Failed to reopen map file.\n", STDERR_FILENO);
		return (false);
	}
	map->height = line_count;
	map->width = max_len;
	map->map_lines = ft_calloc(line_count + 1, sizeof(char *));
	if (!map->map_lines)
		return (ft_putstr_fd("Error: Memory allocation failed for map lines.\n",
				STDERR_FILENO), false);
	while ((line = get_next_line(map->fd)) != NULL)
	{
		if (i >= map->height)
		{
			ft_putstr_fd("Error: More lines than expected in map.\n",
				STDERR_FILENO);
			free(line);
			return (false);
		}
		if (!populate_map_lines(&line, &map_started, &map->map_lines[i]))
			return (false);
		i++;
	}
	close(map->fd);
	return (true);
}

// Function to create the Y arrays
static bool	create_array(t_map *map)
{
	map->map_array = ft_calloc((map->height * map->width), sizeof(char));
	if (!map->map_array)
		return (ft_putstr_fd("Error: Memory allocation failed for map array.\n",
				STDERR_FILENO), false);
	return (true);
}

// Function to create a map array from the file
bool	create_map_array(t_cub_data *data)
{
	if (!data || !data->map || !data->map->fd)
		return (ft_putstr_fd("Error: Invalid data structure.\n", STDERR_FILENO),
			false);
	if (!count_lines_in_file(data->map))
		return (ft_putstr_fd("Error: Could not count map lines.\n",
				STDERR_FILENO), false);
	if (!create_array(data->map))
		return (ft_putstr_fd("Error: Could not create map array.\n",
				STDERR_FILENO), false);
	if (!data->map->map_array)
		return (ft_putstr_fd("Error: Map array is NULL.\n", STDERR_FILENO),
			false);
	else
		return (true);
}

bool	open_map(char *filename, t_cub_data *data)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (ft_putstr_fd("Error: Could not open map file.\n",
				STDERR_FILENO), false);
	data->map = ft_calloc(1, sizeof(t_map));
	if (!data->map)
	{
		close(fd);
		return (ft_putstr_fd("Error: Memory allocation failed for map.\n",
				STDERR_FILENO), false);
	}
	// Initialize map properties
	data->map->fd = fd;
	data->map->filename = ft_strdup(filename);
	if (!data->map->filename)
	{
		close(fd);
		return (ft_putstr_fd("Error: Memory allocation failed.\n",
				STDERR_FILENO), false);
	}
	if (!create_map_array(data))
	{
		close(fd);
		return (ft_putstr_fd("Error: Could not create map array.\n",
				STDERR_FILENO), false);
	}
	return (true);
}

/**
 * Parse .cub file and extract map data
 */
int	parse_cub_file(char *filename, t_cub_data *data)
{
	if (!filename || !data)
		return (ft_printf_fd(STDERR_FILENO, "Error: Invalid arguments\n"),
			ERR_INVALID_ARG);
	if (!open_map(filename, data))
		return (ft_printf_fd(STDERR_FILENO, "Error opening map file\n"),
			ERR_FILE_NOT_FOUND);
	else
		return (ERR_NO_ERROR);
}







// /**
//  * Parse texture paths from .cub file
//  */
// int	parse_textures(char *line, t_cub_data *data)
// {
// 	// TODO: Implement texture parsing
// 	(void)line;
// 	(void)data;
// 	return (0);
// }

// /**
//  * Parse color values (floor/ceiling) from .cub file
//  */
// int	parse_colors(char *line, t_cub_data *data)
// {
// 	// TODO: Implement color parsing
// 	(void)line;
// 	(void)data;
// 	return (0);
// }
