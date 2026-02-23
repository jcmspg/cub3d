/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doors.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 17:00:00 by joamiran          #+#    #+#             */
/*   Updated: 2026/02/23 04:10:10 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static void	update_door_state(t_door *door, float delta_time)
{
	if (door->is_opening)
	{
		door->open_amount += delta_time * 2.0f;
		if (door->open_amount >= 1.0f)
		{
			door->open_amount = 1.0f;
			door->is_opening = false;
			door->is_open = true;
		}
	}
	else if (door->is_closing)
	{
		door->open_amount -= delta_time * 2.0f;
		if (door->open_amount <= 0.0f)
		{
			door->open_amount = 0.0f;
			door->is_closing = false;
			door->is_open = false;
		}
	}
}

void	update_doors(t_cub_data *data, float delta_time)
{
	int		i;
	t_door	*door;

	i = 0;
	while (i < data->game->door_count)
	{
		door = &data->game->doors[i];
		update_door_state(door, delta_time);
		i++;
	}
}

void	toggle_door(t_cub_data *data)
{
	int		i;
	t_door	*door;

	i = 0;
	while (i < data->game->door_count)
	{
		door = &data->game->doors[i];
		if (abs(door->x - (int)from_fixed32(data->player->x)) <= 1
			&& abs(door->y - (int)from_fixed32(data->player->y)) <= 1)
		{
			if (door->is_open)
			{
				door->is_open = false;
				door->is_closing = true;
			}
			else
				door->is_opening = true;
		}
		i++;
	}
}

t_door	*get_door_at(t_cub_data *data, int x, int y)
{
	int	i;

	if (!data || !data->game || !data->game->doors)
		return (NULL);
	i = 0;
	while (i < data->game->door_count)
	{
		if (data->game->doors[i].x == x && data->game->doors[i].y == y)
			return (&data->game->doors[i]);
		i++;
	}
	return (NULL);
}

void	interact_doors(t_cub_data *data)
{
	toggle_door(data);
}
