/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 21:45:00 by joao              #+#    #+#             */
/*   Updated: 2026/04/06 21:51:36 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/gamelogic.h"
#include "../../includes/movements.h"

bool		is_valid_move(t_cub_data *data, t_fixed32 new_x, t_fixed32 new_y);
bool		dirs_are_zero(t_cub_data *data);
t_fixed32	normalize_angle_fixed(t_fixed32 angle);
void		apply_strafe(t_cub_data *data, t_fixed32 strafe_speed,
				t_fixed32 perp_x, t_fixed32 perp_y);

void	move_player_y(t_cub_data *data, t_fixed32 speed)
{
	t_fixed32	new_y;

	new_y = fixed32_add(data->player->y, speed);
	if (!is_valid_move(data, data->player->x, new_y))
		return ;
	data->player->y = new_y;
}

void	move_player_x(t_cub_data *data, t_fixed32 speed)
{
	t_fixed32	new_player_x;

	new_player_x = fixed32_add(data->player->x, speed);
	if (!is_valid_move(data, new_player_x, data->player->y))
		return ;
	data->player->x = new_player_x;
}

void	move_player(t_cub_data *data, t_fixed32 move_speed)
{
	t_fixed32	new_x;
	t_fixed32	new_y;
	t_fixed32	new_pos_x;
	t_fixed32	new_pos_y;

	if (!data || !data->player)
		return ;
	if (dirs_are_zero(data))
		return ;
	new_x = fixed32_mul(move_speed, data->player->dir_x);
	new_y = fixed32_mul(move_speed, data->player->dir_y);
	new_pos_x = fixed32_add(data->player->x, new_x);
	new_pos_y = fixed32_add(data->player->y, new_y);
	if (!is_valid_move(data, new_pos_x, new_pos_y))
		return ;
	data->player->x = new_pos_x;
	data->player->y = new_pos_y;
}

void	strafe_player(t_cub_data *data, t_fixed32 strafe_speed)
{
	t_fixed32	perp_x;
	t_fixed32	perp_y;

	if (!data || !data->player)
		return ;
	if (data->player->dir_x == 0 && data->player->dir_y == 0)
	{
		ft_putstr_fd("Error: Direction vectors are not set\n", STDERR_FILENO);
		return ;
	}
	perp_x = -data->player->dir_y;
	perp_y = data->player->dir_x;
	apply_strafe(data, strafe_speed, perp_x, perp_y);
}

void	rotate_player(t_cub_data *data, t_fixed32 rotation_angle)
{
	if (!data || !data->player)
		return ;
	data->player->dir_angle = fixed32_add(data->player->dir_angle,
			rotation_angle);
	data->player->dir_angle = normalize_angle_fixed(data->player->dir_angle);
	calc_player_dirs(data);
}
