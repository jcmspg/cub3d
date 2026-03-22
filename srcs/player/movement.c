/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 21:45:00 by joao              #+#    #+#             */
/*   Updated: 2026/03/22 20:36:49 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/gamelogic.h"
#include "../../includes/movements.h"

/**
 * Function to check if the move is valid based on collision detection.
 */

static bool check_collision(t_cub_data *data, int x, int y) {
  char cell;
  t_door *door;
  t_enemy *enemy;

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
  // Check for living enemy at this position (skip if game not initialized)
  if (data->game && data->game->enemies) {
    enemy = get_enemy_at(data, x, y);
    if (enemy)
      return (true);
  }
  return (false);
}


static bool is_valid_move(t_cub_data *data, t_fixed32 new_x, t_fixed32 new_y) {
  t_fixed32 pos_x;
  t_fixed32 pos_y;
  int map_x;
  int map_y;
  const t_fixed32 buffer = to_fixed32(0.2f); // Fixed-point buffer

  if (!data || !data->map || !data->map->map_array)
    return (false);
  // Calculate the player's position with the buffer
  pos_x = new_x;
  pos_y = new_y;
  // Calculate the grid cell the player is in
  map_x = (int)from_fixed32(pos_x);
  map_y = (int)from_fixed32(pos_y);
  // Ensure the position is within map bounds
  if (map_x < 0 || map_x >= data->map->width || map_y < 0 ||
      map_y >= data->map->height)
    return (false);

  // Check center
  if (check_collision(data, map_x, map_y))
    return (false);

  // Check corners/buffer points (fixed-point math)
  if (check_collision(data, (int)from_fixed32(fixed32_sub(pos_x, buffer)), (int)from_fixed32(pos_y)))
    return false;
  if (check_collision(data, (int)from_fixed32(fixed32_add(pos_x, buffer)), (int)from_fixed32(pos_y)))
    return false;
  if (check_collision(data, (int)from_fixed32(pos_x), (int)from_fixed32(fixed32_sub(pos_y, buffer))))
    return false;
  if (check_collision(data, (int)from_fixed32(pos_x), (int)from_fixed32(fixed32_add(pos_y, buffer))))
    return false;

  return (true);
}

/**
 * Handle forward movement
 */
void move_player_y(t_cub_data *data, t_fixed32 speed) {
  t_fixed32 new_y;

  new_y = fixed32_add(data->player->y, speed);
  if (!is_valid_move(data, data->player->x, new_y))
    return;
  data->player->y = new_y;
}

/**
 * Handle backward movement
 */
void move_player_x(t_cub_data *data, t_fixed32 speed) {
  t_fixed32 new_player_x;

  new_player_x = fixed32_add(data->player->x, speed);
  if (!is_valid_move(data, new_player_x, data->player->y))
    return;
  data->player->x = new_player_x;
}

// Move player in the direction they are facing
void move_player(t_cub_data *data, t_fixed32 move_speed) {
  t_fixed32 new_x;
  t_fixed32 new_y;
  t_fixed32 newX;
  t_fixed32 newY;

  if (!data || !data->player)
    return;
  // Ensure direction vectors are valid
  if (data->player->dir_x == 0 && data->player->dir_y == 0) {
    printf("ERROR: Direction vectors are ZERO! dir_x=%.4f, dir_y=%.4f, "
           "angle=%.1f\n",
           from_fixed32(data->player->dir_x), from_fixed32(data->player->dir_y),
           from_fixed32(data->player->dir_angle));
    ft_putstr_fd("Error: Direction vectors are not set\n", STDERR_FILENO);
    return;
  }
  // Movement debug removed for cleaner output
  // Calculate the new position based on the current direction vector
  new_x = fixed32_mul(move_speed, data->player->dir_x);
  new_y = fixed32_mul(move_speed, data->player->dir_y);
  newX = fixed32_add(data->player->x, new_x);
  newY = fixed32_add(data->player->y, new_y);
  // Check if the new position is valid
  if (!is_valid_move(data, newX, newY))
    return;
  // Update the player's position
  data->player->x = newX;
  data->player->y = newY;
}

// Strafe player left or right
void strafe_player(t_cub_data *data, t_fixed32 strafe_speed) {
  t_fixed32 new_x;
  t_fixed32 new_y;
  t_fixed32 newX;
  t_fixed32 newY;

  if (!data || !data->player)
    return;
  // Ensure direction vectors are valid
  if (data->player->dir_x == 0 && data->player->dir_y == 0) {
    ft_putstr_fd("Error: Direction vectors are not set\n", STDERR_FILENO);
    return;
  }
  // move, but perpendicular to the direction vector
  t_fixed32 perp_x = -data->player->dir_y; // Perpendicular vector
  t_fixed32 perp_y = data->player->dir_x;  // Perpendicular vector
  new_x = fixed32_mul(strafe_speed, perp_x);
  new_y = fixed32_mul(strafe_speed, perp_y);
  newX = fixed32_add(data->player->x, new_x);
  newY = fixed32_add(data->player->y, new_y);
  // Check if the new position is valid
  if (!is_valid_move(data, newX, newY))
    return;
  // Update the player's position
  data->player->x = newX;
  data->player->y = newY;
}

static t_fixed32 normalize_angle_fixed(t_fixed32 angle) {
  while (angle < 0)
    angle = fixed32_add(angle, to_fixed32(360.0f));
  while (angle >= to_fixed32(360.0f))
    angle = fixed32_sub(angle, to_fixed32(360.0f));
  return (angle);
}

void rotate_player(t_cub_data *data, t_fixed32 rotation_angle) {
  if (!data || !data->player)
    return;

  // Update the player's angle
  data->player->dir_angle =
      fixed32_add(data->player->dir_angle, rotation_angle);

  // Normalize the angle to keep it within 0-360 degrees
  data->player->dir_angle = normalize_angle_fixed(data->player->dir_angle);

  // Recalculate the direction vectors based on the new angle
  calc_player_dirs(data);
}