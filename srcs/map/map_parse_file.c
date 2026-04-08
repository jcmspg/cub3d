/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parse_file.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 00:47:33 by hladeiro          #+#    #+#             */
/*   Updated: 2026/04/08 00:47:34 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

typedef struct s_parse_state
{
	bool	floor_seen;
	bool	ceiling_seen;
}t_parse_state;

static char	*skip_ws(char *line)
{
	while (*line == ' ' || *line == '\t')
		line++;
	return (line);
}

static bool	is_texture_line(char *line)
{
	line = skip_ws(line);
	if (ft_strncmp(line, "NO ", 3) == 0 || ft_strncmp(line, "SO ", 3) == 0)
		return (true);
	if (ft_strncmp(line, "WE ", 3) == 0 || ft_strncmp(line, "EA ", 3) == 0)
		return (true);
	return (false);
}

static bool	is_color_line(char *line)
{
	line = skip_ws(line);
	if (ft_strncmp(line, "F ", 2) == 0 || ft_strncmp(line, "C ", 2) == 0)
		return (true);
	return (false);
}

static int	parse_line(char *line, t_cub_data *data, t_parse_state *state)
{
	char	*clean;

	clean = skip_ws(line);
	if (is_texture_line(line))
	{
		if (parse_textures(clean, data) != 0)
			return (ERR_TEXTURE_PARSE);
	}
	else if (is_color_line(line))
	{
		if (ft_strncmp(clean, "F ", 2) == 0)
		{
			if (state->floor_seen)
				return (ERR_TEXTURE_PARSE);
			state->floor_seen = true;
		}
		else if (ft_strncmp(clean, "C ", 2) == 0)
		{
			if (state->ceiling_seen)
				return (ERR_TEXTURE_PARSE);
			state->ceiling_seen = true;
		}
		if (parse_colors(clean, data) != 0)
			return (ERR_TEXTURE_PARSE);
	}
	return (ERR_NO_ERROR);
}

static bool	has_required_metadata(const t_cub_data *data,
		const t_parse_state *state)
{
	if (!data || !data->textures)
		return (false);
	if (!data->textures->walls[TEX_NORTH].path
		|| !data->textures->walls[TEX_SOUTH].path
		|| !data->textures->walls[TEX_EAST].path
		|| !data->textures->walls[TEX_WEST].path)
		return (false);
	if (!state->floor_seen || !state->ceiling_seen)
		return (false);
	return (true);
}

int	parse_cub_file(char *filename, t_cub_data *data)
{
	int		fd;
	char	*line;
	int		status;
	t_parse_state	state;

	if (!filename || !data)
		return (ERR_INVALID_ARG);
	state.floor_seen = false;
	state.ceiling_seen = false;
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (ERR_FILE_NOT_FOUND);
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		status = parse_line(line, data, &state);
		if (status != ERR_NO_ERROR)
			return (free(line), close(fd), status);
		free(line);
	}
	close(fd);
	if (!has_required_metadata(data, &state))
		return (ERR_TEXTURE_PARSE);
	if (!open_map(filename, data))
		return (ERR_FILE_NOT_FOUND);
	return (ERR_NO_ERROR);
}
