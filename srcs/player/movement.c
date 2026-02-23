/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 21:45:00 by joao              #+#    #+#             */
/*   Updated: 2026/02/23 02:48:10 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/gamelogic.h"
#include "../../includes/movements.h"

static bool check_collision(t_cub_data *data, int x, int y) {
  char cell;
  t_door *door;

  if (x < 0 || x >= data->map->width || y < 0 || y >= data->map->height)
    return (true);
  cell = data->map->map_array[y * data->map->width + x];
  if (cell == '1')
    return (true);
  if (cell == 'D') {
    door = get_door_at(data, x, y);
    if (door && door->open_amount < 0.7f)
      return (true);
  }
  if (cell == 'X' && get_enemy_at(data, x, y))
    return (true);
  return (false);
}

static bool is_valid_move(t_cub_data *data, t_fixed32 new_x, t_fixed32 new_y) {
  float pos_x;
  float pos_y;
  int map_x;
  int map_y;
  float buffer;

  if (!data || !data->map || !data->map->map_array)
    return (false);
  buffer = 0.2f;
  pos_x = from_fixed32(new_x);
  pos_y = from_fixed32(new_y);
  map_x = (int)(pos_x);
  map_y = (int)(pos_y);
  if (map_x < 0 || map_x >= data->map->width || map_y < 0 ||
      map_y >= data->map->height)
    return (false);
  if (check_collision(data, map_x, map_y) ||
      check_collision(data, (int)(pos_x - buffer), (int)pos_y) ||
      check_collision(data, (int)(pos_x + buffer), (int)pos_y) ||
      check_collision(data, (int)pos_x, (int)(pos_y - buffer)) ||
      check_collision(data, (int)pos_x, (int)(pos_y + buffer)))
    return (false);
  return (true);
}

void move_player_y(t_cub_data *data, t_fixed32 speed) {
  t_fixed32 new_y;

  new_y = fixed32_add(data->player->y, speed);
  if (!is_valid_move(data, data->player->x, new_y))
    return;
  data->player->y = new_y;
}

void move_player_x(t_cub_data *data, t_fixed32 speed) {
  t_fixed32 new_player_x;

  new_player_x = fixed32_add(data->player->x, speed);
  if (!is_valid_move(data, new_player_x, data->player->y))
    return;
  data->player->x = new_player_x;
}

void move_player(t_cub_data *data, t_fixed32 move_speed) {
  t_fixed32 new_x;
  t_fixed32 new_y;

  if (!data || !data->player)
    return;
  if (data->player->dir_x == 0 && data->player->dir_y == 0)
    return;
  new_x = fixed32_add(data->player->x,
                      fixed32_mul(move_speed, data->player->dir_x));
  new_y = fixed32_add(data->player->y,
                      fixed32_mul(move_speed, data->player->dir_y));
  if (!is_valid_move(data, new_x, new_y))
    return;
  data->player->x = new_x;
  data->player->y = new_y;
}

void strafe_player(t_cub_data *data, t_fixed32 strafe_speed) {
  t_fixed32 new_x;
  t_fixed32 new_y;

  if (!data || !data->player)
    return;
  if (data->player->dir_x == 0 && data->player->dir_y == 0)
    return;
  new_x = fixed32_add(data->player->x,
                      fixed32_mul(strafe_speed, -data->player->dir_y));
  new_y = fixed32_add(data->player->y,
                      fixed32_mul(strafe_speed, data->player->dir_x));
  if (!is_valid_move(data, new_x, new_y))
    return;
  data->player->x = new_x;
  data->player->y = new_y;
}

void rotate_player(t_cub_data *data, t_fixed32 rotation_angle) {
  if (!data || !data->player)
    return;
  data->player->dir_angle =
      fixed32_add(data->player->dir_angle, rotation_angle);
  data->player->dir_angle = normalize_angle_degrees(data->player->dir_angle);
  calc_player_dirs(data);
}