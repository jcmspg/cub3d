/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hud_minimap_player.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 01:05:12 by hladeiro          #+#    #+#             */
/*   Updated: 2026/04/08 01:05:12 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/hud_internal.h"

static void	draw_minimap_player_square(t_cub_data *data, int player_x,
		int player_y, int size)
{
	int	px;
	int	py;

	py = -size / 2;
	while (py <= size / 2)
	{
		px = -size / 2;
		while (px <= size / 2)
		{
			mylx_pixel_put(data, player_x + px, player_y + py, 0xFF0000);
			px++;
		}
		py++;
	}
}

static void	draw_minimap_player_direction(t_cub_data *data, int player_x,
		int player_y, int cell_size)
{
	float	dir_x;
	float	dir_y;
	int		i;
	int		px;
	int		py;

	dir_x = from_fixed32(data->player->dir_x);
	dir_y = from_fixed32(data->player->dir_y);
	i = 0;
	while (i < cell_size * 2)
	{
		px = player_x + (int)(dir_x * i);
		py = player_y + (int)(dir_y * i);
		mylx_pixel_put(data, px, py, 0xFFFF00);
		i++;
	}
}

void	draw_minimap_player(t_cub_data *data, int cell_size)
{
	int	player_x;
	int	player_y;
	int	size;

	player_x = data->hud->minimap_x + (int)(from_fixed32(data->player->x)
			* cell_size);
	player_y = data->hud->minimap_y + (int)(from_fixed32(data->player->y)
			* cell_size);
	size = cell_size / 2;
	if (size < 3)
		size = 3;
	draw_minimap_player_square(data, player_x, player_y, size);
	draw_minimap_player_direction(data, player_x, player_y, cell_size);
}
