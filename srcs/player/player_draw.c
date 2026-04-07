/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_draw.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 22:30:00 by hladeiro          #+#    #+#             */
/*   Updated: 2026/04/06 21:42:46 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/player.h"

void	draw_player(t_cub_data *data)
{
	int	player_x;
	int	player_y;
	int	dir_x;
	int	dir_y;

	if (!data || !data->player)
		return ;
	draw_filled_rect(data, from_fixed32(data->player->x) * 12 + 10 - 3,
		from_fixed32(data->player->y) * 12 + 10 - 3, 6, 6, 0xFF0000);
	player_x = from_fixed32(data->player->x) * 12 + 10;
	player_y = from_fixed32(data->player->y) * 12 + 10;
	dir_x = player_x + (int)(from_fixed32(data->player->dir_x) * 12);
	dir_y = player_y + (int)(from_fixed32(data->player->dir_y) * 12);
	draw_filled_rect(data, dir_x - 2, dir_y - 2, 4, 4, 0xFFFF00);
}

void	print_player_coords(t_cub_data *data)
{
	if (!data || !data->player)
		return ;
	printf("Player position: (%.2f, %.2f)\n", from_fixed32(data->player->x),
		from_fixed32(data->player->y));
}
