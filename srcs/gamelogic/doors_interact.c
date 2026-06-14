/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doors_interact.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 00:41:43 by hladeiro          #+#    #+#             */
/*   Updated: 2026/04/08 00:41:44 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static t_door	*door_at_distance(t_cub_data *data, float dist)
{
	t_fixed32	check_x;
	t_fixed32	check_y;

	check_x = fixed32_add(data->player->x, fixed32_mul(data->player->dir_x,
				to_fixed32(dist)));
	check_y = fixed32_add(data->player->y, fixed32_mul(data->player->dir_y,
				to_fixed32(dist)));
	return (get_door_at(data, from_fixed32(check_x), from_fixed32(check_y)));
}

static void	toggle_door(t_cub_data *data, t_door *door)
{
	int	player_map_x;
	int	player_map_y;

	player_map_x = from_fixed32(data->player->x);
	player_map_y = from_fixed32(data->player->y);
	if (door->is_open || door->is_opening)
	{
		if (player_map_x == door->x && player_map_y == door->y)
			return ;
		door->is_closing = true;
		door->is_opening = false;
		door->is_open = false;
	}
	else
	{
		door->is_opening = true;
		door->is_closing = false;
	}
}

void	interact_doors(t_cub_data *data)
{
	t_door	*door;
	float	dist;

	dist = 0.5f;
	while (dist <= 1.5f)
	{
		door = door_at_distance(data, dist);
		if (door)
		{
			toggle_door(data, door);
			return ;
		}
		dist += 0.5f;
	}
}
