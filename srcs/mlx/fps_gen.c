/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fps_gen.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 16:36:37 by joamiran          #+#    #+#             */
/*   Updated: 2026/02/23 03:08:10 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

uint64_t get_time_ms(void) {
  struct timeval tv;

  gettimeofday(&tv, NULL);
  return ((uint64_t)(tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void init_fps_sync(t_cub_data *data) {
  data->fps.last_frame_time = get_time_ms();
  data->fps.accumulator = 0;
}

int main_loop(t_cub_data *data) {
  uint64_t current_time;
  uint64_t elapsed;

  current_time = get_time_ms();
  elapsed = current_time - data->fps.last_frame_time;
  if (elapsed < MIN_FRAME_TIME) {
    usleep((MIN_FRAME_TIME - elapsed) * 1000);
    current_time = get_time_ms();
    elapsed = current_time - data->fps.last_frame_time;
  }
  data->fps.delta_time = elapsed;
  data->fps.last_frame_time = current_time;
  data->fps.accumulator += elapsed;
  while (data->fps.accumulator >= FIXED_STEP_MS) {
    update_game_logic(data);
    data->fps.accumulator -= FIXED_STEP_MS;
  }
  render_raycasting(data);
  display_fps(data, 1000.0f / data->fps.delta_time);
  print_movements(data);
  print_player_pos(data);
  return (ERR_NO_ERROR);
}
