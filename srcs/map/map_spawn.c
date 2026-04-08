/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_spawn.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 00:55:03 by hladeiro          #+#    #+#             */
/*   Updated: 2026/04/08 00:55:05 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

bool	look_for_spawn(t_map *map, int *spawn_x, int *spawn_y, char *direction)
{
	int	i;
	int	total;

	if (!map || !map->map_array)
		return (false);
	total = map->width * map->height;
	i = 0;
	while (i < total)
	{
		if (map->map_array[i] == 'N' || map->map_array[i] == 'S'
			|| map->map_array[i] == 'E' || map->map_array[i] == 'W')
		{
			*direction = map->map_array[i];
			*spawn_x = i % map->width;
			*spawn_y = i / map->width;
			return (true);
		}
		i++;
	}
	*spawn_x = -1;
	*spawn_y = -1;
	*direction = '\0';
	return (false);
}
