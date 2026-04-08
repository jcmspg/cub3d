/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_draw.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 22:30:00 by hladeiro          #+#    #+#             */
/*   Updated: 2026/04/08 21:14:24 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/player.h"

void	draw_player(t_cub_data *data)
{
	int				player_x;
	int				player_y;
	int				dir_x;
	int				dir_y;
	t_draw_shape	shape;

	if (!data || !data->player)
		return ;
	shape.x = from_fixed32(data->player->x) * 12 + 10 - 3;
	shape.y = from_fixed32(data->player->y) * 12 + 10 - 3;
	shape.width = 6;
	shape.height = 6;
	shape.color = 0xFF0000;
	draw_filled_rect(data, &shape);
	player_x = from_fixed32(data->player->x) * 12 + 10;
	player_y = from_fixed32(data->player->y) * 12 + 10;
	dir_x = player_x + (int)(from_fixed32(data->player->dir_x) * 12);
	dir_y = player_y + (int)(from_fixed32(data->player->dir_y) * 12);
	shape.x = dir_x - 2;
	shape.y = dir_y - 2;
	shape.width = 4;
	shape.height = 4;
	shape.color = 0xFFFF00;
	draw_filled_rect(data, &shape);
}

void	print_player_coords(t_cub_data *data)
{
	if (!data || !data->player)
		return ;
	printf("Player position: (%.2f, %.2f)\n", from_fixed32(data->player->x),
		from_fixed32(data->player->y));
}
