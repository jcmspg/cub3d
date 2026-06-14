/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprites.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 01:28:21 by hladeiro          #+#    #+#             */
/*   Updated: 2026/04/08 01:28:22 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sprites_internal.h"

static void	render_map_pickups(t_cub_data *data)
{
	int	x;
	int	y;

	y = 0;
	while (y < data->map->height)
	{
		x = 0;
		while (x < data->map->width)
		{
			if (data->map->map_array[y * data->map->width + x] == 'M')
				render_ammo_sprite(data, (float)x + 0.5f, (float)y + 0.5f);
			x++;
		}
		y++;
	}
}

void	render_sprites(t_cub_data *data)
{
	if (!data || !data->map || !data->raycasting)
		return ;
	render_map_pickups(data);
	render_enemies(data);
}
