/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 20:04:40 by joamiran          #+#    #+#             */
/*   Updated: 2026/04/08 01:41:04 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/gamelogic.h"
#include "../../includes/utils.h"

void	index_to_coords(int index, int width, int *x, int *y)
{
	*x = index % width;
	*y = index / width;
}

int	coords_to_index(int x, int y, int width)
{
	return (y * width + x);
}

char	get_map_char_at(t_cub_data *data, t_fixed32 world_x, t_fixed32 world_y)
{
	int	map_x;
	int	map_y;
	int	index;

	map_x = from_fixed32(world_x);
	map_y = from_fixed32(world_y);
	if (map_x < 0 || map_x >= data->map->width || map_y < 0
		|| map_y >= data->map->height)
		return ('1');
	index = coords_to_index(map_x, map_y, data->map->width);
	return (data->map->map_array[index]);
}

bool	is_wall_at(t_cub_data *data, t_fixed32 world_x, t_fixed32 world_y)
{
	char	c;
	int		x;
	int		y;
	t_door	*door;

	c = get_map_char_at(data, world_x, world_y);
	if (c == '1')
		return (true);
	if (c == 'D')
	{
		x = from_fixed32(world_x);
		y = from_fixed32(world_y);
		if (!data || !data->game)
			return (true);
		door = get_door_at(data, x, y);
		if (door && door->open_amount < 1.0f)
			return (true);
	}
	return (false);
}
