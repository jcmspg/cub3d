/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doors.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 21:15:00 by joamiran          #+#    #+#             */
/*   Updated: 2026/01/24 21:15:00 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

t_door	*get_door_at(t_cub_data *data, int x, int y)
{
	int	i;

	if (!data->game->doors)
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

void	update_doors(t_cub_data *data)
{
	int		i;
	t_door	*door;

	if (!data->game->doors)
		return ;
	i = 0;
	while (i < data->game->door_count)
	{
		door = &data->game->doors[i];
		if (door->is_opening)
		{
			door->open_amount += 0.02f; // Adjust speed as needed
			if (door->open_amount >= 1.0f)
			{
				printf("DEBUG: Door at (%d,%d) fully OPEN\n", door->x, door->y);
				door->open_amount = 1.0f;
				door->is_opening = false;
				door->is_open = true;
			}
		}
		else if (door->is_closing)
		{
			door->open_amount -= 0.02f;
			if (door->open_amount <= 0.0f)
			{
				printf("DEBUG: Door at (%d,%d) fully CLOSED\n", door->x,
					door->y);
				door->open_amount = 0.0f;
				door->is_closing = false;
				door->is_open = false;
			}
		}
		i++;
	}
}

static void	toggle_door(t_cub_data *data, t_door *door)
{
	int	player_map_x;
	int	player_map_y;

	player_map_x = from_fixed32(data->player->x);
	player_map_y = from_fixed32(data->player->y);
	if (door->is_open || door->is_opening)
	{
		// Safety check: Don't close if player is in the door tile!
		if (player_map_x == door->x && player_map_y == door->y)
		{
			printf("DEBUG: Cannot close door - player inside!\n");
			return ;
		}
		door->is_closing = true;
		door->is_opening = false;
		door->is_open = false;
			// Immediately treat as solid for collision? Maybe wait.
	}
	else
	{
		door->is_opening = true;
		door->is_closing = false;
	}
}

void	interact_doors(t_cub_data *data)
{
	t_fixed32	check_x;
	t_fixed32	check_y;
	int			map_x;
	int			map_y;
	t_door		*door;
	float		dist;

	// Check at incremental distances to avoid overshooting
	// Check at 0.5, 1.0, 1.5 units
	for (dist = 0.5f; dist <= 1.5f; dist += 0.5f)
	{
		check_x = fixed32_add(data->player->x, fixed32_mul(data->player->dir_x,
					to_fixed32(dist)));
		check_y = fixed32_add(data->player->y, fixed32_mul(data->player->dir_y,
					to_fixed32(dist)));
		map_x = from_fixed32(check_x);
		map_y = from_fixed32(check_y);
		// If we found a door, toggle it and stop (don't toggle multiple doors)
		door = get_door_at(data, map_x, map_y);
		if (door)
		{
			toggle_door(data, door);
			return ;
		}
	}
}
