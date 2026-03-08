/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gamelogic.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 16:54:04 by joamiran          #+#    #+#             */
/*   Updated: 2026/01/24 20:11:05 by joamiran         ###   ########.fr       */
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
		move_player(data, -data->player->move_speed);
	if (data->input->left)
		strafe_player(data, -data->player->move_speed);
	if (data->input->right)
		strafe_player(data, data->player->move_speed);
}

static void	process_player_rotation(t_cub_data *data)
{
	if (!data || !data->player)
		return ;
	if (data->input->turn_left)
	{
		data->player->dir_angle -= data->player->rotate_speed;
		if (data->player->dir_angle < 0)
			data->player->dir_angle += to_fixed32(360);
	}
	if (data->input->turn_right)
	{
		data->player->dir_angle += data->player->rotate_speed;
		if (data->player->dir_angle >= to_fixed32(360))
			data->player->dir_angle -= to_fixed32(360);
	}
	calc_player_dirs(data);
}

static void	update_jump(t_cub_data *data)
{
	uint64_t	elapsed;
	float		progress;
	float		arc;

	if (!data->input->jumping)
	{
		data->player->view_offset = 0;
		return ;
	}
	elapsed = get_time_ms() - data->input->jump_start_time;
	if (elapsed >= JUMP_DURATION)
	{
		data->input->jumping = false;
		data->player->view_offset = 0;
		return ;
	}
	progress = (float)elapsed / (float)JUMP_DURATION;
	arc = 4.0f * progress * (1.0f - progress);
	data->player->view_offset = (int)(JUMP_HEIGHT * arc);
}

static void	update_head_bob(t_cub_data *data)
{
	bool	is_moving;
	float	phase_speed;
	float	bob_mult;

	if (!HEAD_BOB_ENABLED)
	{
		data->player->bob_offset = 0;
		return ;
	}
	is_moving = data->input->forward || data->input->backward
		|| data->input->left || data->input->right;
	if (!is_moving || data->input->jumping)
	{
		data->player->bob_phase = 0;
		data->player->bob_offset = 0;
		return ;
	}
	if (data->input->sprint > 0)
		bob_mult = 1.5f;
	else
		bob_mult = 1.0f;
	phase_speed = BOB_FREQUENCY * bob_mult * (data->fps.delta_time / 1000.0f);
	data->player->bob_phase += phase_speed * 2.0f * M_PI;
	if (data->player->bob_phase > 2.0f * M_PI)
		data->player->bob_phase -= 2.0f * M_PI;
	data->player->bob_offset = (int)(sinf(data->player->bob_phase)
			* BOB_AMPLITUDE);
}

void	update_game_logic(t_cub_data *data)
{
	float	frame_multiplier;
	float	speed_mult;

	frame_multiplier = data->fps.delta_time / 16.6f;
	if (frame_multiplier > 3.0f)
		frame_multiplier = 3.0f;
	if (frame_multiplier < 0.1f)
		frame_multiplier = 0.1f;
	speed_mult = 1.0f;
	if (data->input->sprint)
		speed_mult = SPRINT_MULTIPLIER;
	data->player->move_speed = to_fixed32(PLAYER_SPEED * speed_mult
			* (data->fps.delta_time / 16.67f));
	data->player->rotate_speed = to_fixed32(ROTATE_SPEED * (data->fps.delta_time
				/ 16.67f));
	process_player_input(data);
	process_player_rotation(data);
	update_jump(data);
	update_head_bob(data);
	update_doors(data);
	player_shoot(data);
	check_ammo_pickups(data);
}
