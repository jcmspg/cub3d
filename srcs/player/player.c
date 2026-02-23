/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 21:45:00 by joao              #+#    #+#             */
/*   Updated: 2026/02/23 02:25:20 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static t_fixed32	direction_to_angle(char direction)
{
	t_fixed32	angle;

	if (direction == 'N')
		angle = to_fixed32(270.0f);
	else if (direction == 'S')
		angle = to_fixed32(90.0f);
	else if (direction == 'E')
		angle = to_fixed32(0.0f);
	else if (direction == 'W')
		angle = to_fixed32(180.0f);
	else
		angle = to_fixed32(0.0f);
	return (angle);
}

void	calc_player_dirs(t_cub_data *data)
{
	static t_fixed32	cached_plane_len = 0;
	static t_fixed32	last_angle = 999123456;
	t_fixed32			angle;

	if (!data || !data->player)
		return ;
	angle = data->player->dir_angle;
	if (angle == last_angle)
		return ;
	if (cached_plane_len == 0)
		cached_plane_len = fixed32_div(fast_sin(&data->trig, to_fixed32(33.0f)),
				fast_cos(&data->trig, to_fixed32(33.0f)));
	data->player->dir_x = fast_cos(&data->trig, angle);
	data->player->dir_y = fast_sin(&data->trig, angle);
	last_angle = angle;
	data->player->plane_x = fixed32_mul(-data->player->dir_y, cached_plane_len);
	data->player->plane_y = fixed32_mul(data->player->dir_x, cached_plane_len);
}

t_player	*init_player(t_cub_data *data)
{
	t_player	*player;
	int			spawn_x;
	int			spawn_y;
	char		direction;

	if (!data || !data->map)
		return (NULL);
	player = ft_calloc(sizeof(t_player), 1);
	if (!player)
		return (NULL);
	spawn_x = -1;
	spawn_y = -1;
	direction = 'X';
	if (!look_for_spawn(data->map, &spawn_x, &spawn_y, &direction))
		return (NULL);
	player->x = to_fixed32(spawn_x + 0.5f);
	player->y = to_fixed32(spawn_y + 0.5f);
	player->dir_angle = direction_to_angle(direction);
	player->move_speed = to_fixed32(PLAYER_SPEED);
	player->rotate_speed = to_fixed32(ROTATE_SPEED);
	calc_player_dirs(data);
	return (player);
}

void	draw_player(t_cub_data *data)
{
	t_rect	rect;

	if (!data || !data->player)
		return ;
	rect.x = from_fixed32(data->player->x) * 12 + 10 - 3;
	rect.y = from_fixed32(data->player->y) * 12 + 10 - 3;
	rect.width = 6;
	rect.height = 6;
	rect.color = 0xFF0000;
	draw_filled_rect(data, rect);
	rect.x = from_fixed32(data->player->x) * 12 + 10
		+ (int)(from_fixed32(data->player->dir_x) * 12) - 2;
	rect.y = from_fixed32(data->player->y) * 12 + 10
		+ (int)(from_fixed32(data->player->dir_y) * 12) - 2;
	rect.width = 4;
	rect.height = 4;
	rect.color = 0xFFFF00;
	draw_filled_rect(data, rect);
}

void	print_player_coords(t_cub_data *data)
{
	if (!data || !data->player)
		return ;
	printf("Player position: (%.2f, %.2f)\n", from_fixed32(data->player->x),
		from_fixed32(data->player->y));
}
