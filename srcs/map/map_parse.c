/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 21:45:00 by joao              #+#    #+#             */
/*   Updated: 2025/07/24 20:37:07 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

// Function to calculate the number of lines in the file
int	count_lines_in_file(int fd)
{
	char	*line;
	int		line_count;

	line_count = 0;
	line = NULL;
	while ((line = get_next_line(fd)) != NULL)
	{
		free(line);
		line_count++;
	}
	return (line_count);
}

// Function to count the number of tiles in the map
bool	count_map_tiles(t_map *map)
{
	int		x;
	int		y;
	char	*line;

	line = NULL;
	x = 0;
	y = 0;
	while ((line = get_next_line(map->fd)) != NULL)
	{
		if (line[0] == '\0' || line[0] == '\n')
		{
			free(line);
			continue ; // Skip empty lines
		}
		if (x < (int)ft_strlen(line))
			x = (int)ft_strlen(line); // Update max width
		y++;                          // Increment line count
		free(line);
	}
	if (x <= 0 || y <= 0)
		return (ft_putstr_fd("Error: Invalid map dimensions.\n", STDERR_FILENO),
			false);
	map->width = x;
	map->height = y;
	if ((map->width > INT_MAX || map->height > INT_MAX) || (map->width < 0
			|| map->height < 0))
		return (ft_putstr_fd("Error: Map dimensions exceed limits.\n",
				STDERR_FILENO), false);
	map->map_array = NULL; // Initialize map_array to NULL
	free(line);
	return (true);
}

// Function to create the Y arrays
bool	create_y_array(t_map *map)
{
	map->map_array = ft_calloc(map->height, sizeof(char *));
	if (!map->map_array)
		return (ft_putstr_fd("Error: Memory allocation failed for map array.\n",
				STDERR_FILENO), false);
	return (true);
}

// Function to populate the X arrays in the map
bool	create_x_array(t_map *map)
{
	int	i;

	i = 0;
	while (i < map->height)
	{
		map->map_array[i] = ft_calloc(map->width + 1, sizeof(char));
		if (!map->map_array[i])
		{
			while (i > 0)
			{
				free(map->map_array[--i]);
			}
			free(map->map_array);
			return (ft_putstr_fd("Error: Memory allocation failed for map row.\n",
					STDERR_FILENO), false);
		}
		i++;
	}
	return (true);
}

// Function to create a map array from the file
bool	create_map_array(t_cub_data *data)
{
	if (!data || !data->map || !data->map->fd)
		return (ft_putstr_fd("Error: Invalid data structure.\n", STDERR_FILENO),
			false);
	if (!count_map_tiles(data->map))
		return (ft_putstr_fd("Error: Could not count map tiles.\n",
				STDERR_FILENO), false);
	if (!create_y_array(data->map) || !create_x_array(data->map))
		return (ft_putstr_fd("Error: Could not create Y arrays or X arrays.\n",
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
	close(fd);
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

/**
 * Parse texture paths from .cub file
 */
int	parse_textures(char *line, t_cub_data *data)
{
	// TODO: Implement texture parsing
	(void)line;
	(void)data;
	return (0);
}

/**
 * Parse color values (floor/ceiling) from .cub file
 */
int	parse_colors(char *line, t_cub_data *data)
{
	// TODO: Implement color parsing
	(void)line;
	(void)data;
	return (0);
}
