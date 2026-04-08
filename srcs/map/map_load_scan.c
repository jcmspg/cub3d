/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_load_scan.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 00:54:23 by hladeiro          #+#    #+#             */
/*   Updated: 2026/04/08 00:54:25 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static bool	process_pre_map_line(char *line, bool *map_started, int *count)
{
	if (is_texture_or_color(line) || is_empty_line(line))
		return ((true));
	if (!is_valid_map_line(line))
	{
		ft_printf_fd(STDERR_FILENO, "invalid line [%s]\n", line);
		return ((false));
	}
	*map_started = true;
	if (count)
		(*count)++;
	return ((true));
}

static bool	process_map_line(char *line, int *count, int *max_len)
{
	int	len;

	if (is_empty_line(line) || !is_valid_map_line(line))
		return ((false));
	(*count)++;
	len = ft_strlen(line);
	if (len > *max_len)
		*max_len = len;
	return ((true));
}

bool	map_first_pass(t_map *map, int *line_count, int *max_len)
{
	char	*line;
	bool	map_started;
	bool	ok;

	map_started = false;
	while (1)
	{
		line = get_next_line(map->fd);
		if (!line)
			break ;
		if (!map_started)
			ok = process_pre_map_line(line, &map_started, line_count);
		else
			ok = process_map_line(line, line_count, max_len);
		free(line);
		if (!ok)
			return ((false));
	}
	return ((true));
}
