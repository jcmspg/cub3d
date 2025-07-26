/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fps_gen.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 16:36:37 by joamiran          #+#    #+#             */
/*   Updated: 2025/07/26 22:21:45 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/fps_gen.h"

// get time
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
	data->fps.target_frame_duration = MIN_FRAME_TIME;
	elapsed = current_time - data->fps.last_frame_time;
	if (elapsed < data->fps.target_frame_duration)
		return (ERR_NO_ERROR);
	data->fps.delta_time = elapsed;
	data->fps.last_frame_time = current_time;
	data->fps.frame_count++;
	mylx_update_scene(data);
	// fps overay
	fps = 1000.0f / data->fps.delta_time;
	display_fps(data, fps); // Draw FPS overlay on screen
	return (ERR_NO_ERROR);
}
