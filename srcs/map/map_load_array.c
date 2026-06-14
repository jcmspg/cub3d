/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_load_array.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 00:57:51 by hladeiro          #+#    #+#             */
/*   Updated: 2026/04/08 00:57:53 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map_load_internal.h"

bool	map_create_array(t_map *map)
{
	map->map_array = ft_calloc((map->height * map->width), sizeof(char));
	if (!map->map_array)
		return ((false));
	return ((true));
}

bool	map_convert_lines_to_array(t_map *map)
{
	int		y;
	int		x;
	size_t	line_len;
	char	c;

	if (!map || !map->map_lines || !map->map_array)
		return ((false));
	y = 0;
	while (y < map->height)
	{
		line_len = ft_strlen(map->map_lines[y]);
		x = 0;
		while (x < map->width)
		{
			if ((size_t)x < line_len)
				c = map->map_lines[y][x];
			else
				c = ' ';
			map->map_array[y * map->width + x] = c;
			x++;
		}
		y++;
	}
	return ((true));
}
