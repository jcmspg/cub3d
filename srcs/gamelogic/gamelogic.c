/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gamelogic.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 16:54:04 by joamiran          #+#    #+#             */
/*   Updated: 2026/02/23 04:55:10 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/gamelogic.h"

static void	process_player_input(t_cub_data *data)
{
	if (!data || !data->player)
		return ;
	if (data->input->forward)
		move_player(data, data->player->move_speed);
	if (data->input->backward)
		move_player(data, fixed32_mul(data->player->move_speed,
				to_fixed32(-1.0f)));
	if (data->input->left)
		strafe_player(data, fixed32_mul(data->player->move_speed,
				to_fixed32(-1.0f)));
	if (data->input->right)
		strafe_player(data, data->player->move_speed);
}

static void	update_player_movement(t_cub_data *data)
{
	float	speed_mult;

	speed_mult = 1.0f;
	if (data->input->sprint)
		speed_mult = SPRINT_MULTIPLIER;
	data->player->move_speed = to_fixed32(PLAYER_SPEED * speed_mult
			* (data->fps.delta_time / 16.67f));
	data->player->rotate_speed = to_fixed32(ROTATE_SPEED
			* (data->fps.delta_time / 16.67f));
	process_player_input(data);
	if (data->input->turn_left)
		rotate_player(data, fixed32_mul(data->player->rotate_speed,
				to_fixed32(-1.0f)));
	if (data->input->turn_right)
		rotate_player(data, data->player->rotate_speed);
}

void	update_game_logic(t_cub_data *data)
{
	update_player_movement(data);
	update_jump(data);
	update_head_bob(data);
	update_doors(data, data->fps.delta_time);
	update_enemies(data, data->fps.delta_time);
	player_shoot(data);
	check_ammo_pickups(data);
}
