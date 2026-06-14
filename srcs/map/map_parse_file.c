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

static bool	is_texture_line(char *line)
{
	if (ft_strncmp(line, "NO ", 3) == 0 || ft_strncmp(line, "SO ", 3) == 0)
		return (true);
	if (ft_strncmp(line, "WE ", 3) == 0 || ft_strncmp(line, "EA ", 3) == 0)
		return (true);
	return (false);
}

static bool	is_color_line(char *line)
{
	if (ft_strncmp(line, "F ", 2) == 0 || ft_strncmp(line, "C ", 2) == 0)
		return (true);
	return (false);
}

static int	parse_line(char *line, t_cub_data *data, int fd)
{
	if (is_texture_line(line))
	{
		if (parse_textures(line, data) != 0)
		{
			free(line);
			close(fd);
			return (ERR_TEXTURE_PARSE);
		}
	}
	else if (is_color_line(line))
	{
		if (parse_colors(line, data) != 0)
		{
			free(line);
			close(fd);
			return (ERR_TEXTURE_PARSE);
		}
	}
	return (ERR_NO_ERROR);
}

int	parse_cub_file(char *filename, t_cub_data *data)
{
	int		fd;
	char	*line;
	int		status;

	if (!filename || !data)
		return (ERR_INVALID_ARG);
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (ERR_FILE_NOT_FOUND);
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		status = parse_line(line, data, fd);
		if (status != ERR_NO_ERROR)
			return (status);
		free(line);
	}
	close(fd);
	if (!open_map(filename, data))
		return (ERR_FILE_NOT_FOUND);
	return (ERR_NO_ERROR);
}
