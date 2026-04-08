/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fps_gen.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 16:36:37 by joamiran          #+#    #+#             */
/*   Updated: 2026/04/08 21:13:00 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/fps_gen.h"

uint64_t	get_time_ms(void);

static void	fps_wait_frame(t_cub_data *data, uint64_t *current_time,
		uint64_t *elapsed)
{
	data->fps.target_frame_duration = MIN_FRAME_TIME;
	*elapsed = *current_time - data->fps.last_frame_time;
	if (*elapsed < data->fps.target_frame_duration)
	{
		usleep((data->fps.target_frame_duration - *elapsed) * 1000);
		*current_time = get_time_ms();
		*elapsed = *current_time - data->fps.last_frame_time;
	}
}

static void	fps_update_logic(t_cub_data *data, uint64_t elapsed,
		uint64_t current_time)
{
	data->fps.delta_time = elapsed;
	data->fps.last_frame_time = current_time;
	data->fps.accumulator += elapsed;
	while (data->fps.accumulator >= FIXED_STEP_MS)
	{
		update_game_logic(data);
		data->fps.accumulator -= FIXED_STEP_MS;
	}
}

uint64_t	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((uint64_t)(tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

int	main_render_loop(t_cub_data *data)
{
	uint64_t	current_time;
	uint64_t	elapsed;
	float		fps;

	current_time = get_time_ms();
	fps_wait_frame(data, &current_time, &elapsed);
	fps_update_logic(data, elapsed, current_time);
	mylx_update_scene(data);
	fps = 1000.0f / data->fps.delta_time;
	return (ERR_NO_ERROR);
}
