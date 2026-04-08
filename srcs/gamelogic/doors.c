/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doors.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 21:15:00 by joamiran          #+#    #+#             */
/*   Updated: 2026/04/08 00:40:59 by hladeiro         ###   ########.fr       */
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

static void	update_opening_door(t_door *door)
{
	door->open_amount += 0.02f;
	if (door->open_amount >= 1.0f)
	{
		door->open_amount = 1.0f;
		door->is_opening = false;
		door->is_open = true;
	}
}

static void	update_closing_door(t_door *door)
{
	door->open_amount -= 0.02f;
	if (door->open_amount <= 0.0f)
	{
		door->open_amount = 0.0f;
		door->is_closing = false;
		door->is_open = false;
	}
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
			update_opening_door(door);
		else if (door->is_closing)
			update_closing_door(door);
		i++;
	}
}
