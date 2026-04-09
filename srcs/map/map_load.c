/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_load.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 00:54:37 by hladeiro          #+#    #+#             */
/*   Updated: 2026/04/09 02:22:04 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map_load_internal.h"

bool	create_map_array(t_cub_data *data)
{
	int		line_count;
	int		max_len;
	t_map	*map;

	if (!data || !data->map || data->map->fd < 0)
		return (false);
	map = data->map;
	line_count = 0;
	max_len = 0;
	if (!map_first_pass(map, &line_count, &max_len) || line_count == 0)
	{
		close(map->fd);
		map->fd = -1;
		return (false);
	}
	close(map->fd);
	map->fd = -1;
	map->height = line_count;
	map->width = max_len;
	map->map_lines = ft_calloc(map->height + 1, sizeof(char *));
	if (!map->map_lines)
		return (false);
	map->fd = open(map->filename, O_RDONLY);
	if (map->fd < 0)
		return (false);
	if (!map_second_pass(map))
	{
		close(map->fd);
		map->fd = -1;
		return (false);
	}
	close(map->fd);
	map->fd = -1;
	if (!map_create_array(map) || !map_convert_lines_to_array(map))
		return (false);
	return (true);
}
