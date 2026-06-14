/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gamelogic_view.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 00:44:15 by hladeiro          #+#    #+#             */
/*   Updated: 2026/04/08 00:44:16 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/gamelogic.h"

static bool	is_player_moving(t_cub_data *data)
{
	return (data->input->forward || data->input->backward || data->input->left
		|| data->input->right);
}

void	update_jump(t_cub_data *data)
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

void	update_head_bob(t_cub_data *data)
{
	float	phase_speed;
	float	bob_mult;

	if (!HEAD_BOB_ENABLED)
	{
		data->player->bob_offset = 0;
		return ;
	}
	if (!is_player_moving(data) || data->input->jumping)
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
