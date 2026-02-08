/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_cast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 19:30:00 by joamiran          #+#    #+#             */
/*   Updated: 2026/01/24 19:59:18 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

/**
 * Calculate the ray direction for a given screen column
 *
 * @param data    Game data with player and FOV info
 * @param ray     Ray to set direction for
 * @param x       Screen column (0 to screen_width - 1)
 */
void calculate_ray_dir(t_cub_data *data, t_ray *ray, int x) {
  t_fixed32 camera_x;
  t_fixed32 ray_angle;
  t_fixed32 half_fov;
  t_fixed32 fov_step;

  // camera_x goes from -1 (left) to +1 (right)
  // Formula: 2 * x / width - 1
  camera_x = fixed32_sub(
      fixed32_div(fixed32_mul(to_fixed32(2.0f), to_fixed32((float)x)),
                  to_fixed32((float)data->mlx->width)),
      to_fixed32(1.0f));
  // Calculate ray angle: player_angle + (camera_x * half_fov)
  half_fov = fixed32_div(data->game->fov, to_fixed32(2.0f));
  fov_step = fixed32_mul(camera_x, half_fov);
  ray_angle = fixed32_add(data->player->dir_angle, fov_step);
  ray_angle = normalize_angle_degrees(ray_angle);
  // Set ray direction using trig tables
  ray->dir_x = fast_cos(&data->trig, ray_angle);
  ray->dir_y = fast_sin(&data->trig, ray_angle);
}

/**
 * Initialize DDA algorithm parameters
 * Sets up step directions and initial side distances
 */
void init_dda(t_cub_data *data, t_ray *ray) {
  t_fixed32 player_x;
  t_fixed32 player_y;
  t_fixed32 frac_x;
  t_fixed32 frac_y;

  player_x = data->player->x;
  player_y = data->player->y;
  // Current map cell (cast float to int to get grid cell)
  ray->map_x = (int)from_fixed32(player_x);
  ray->map_y = (int)from_fixed32(player_y);
  // Delta distances: how far to travel to cross one grid cell
  // Using absolute values to avoid issues with direction
  // IMPORTANT: Max safe value for t_fixed32 (16.16) is ~32767
  // Use 10000.0 as "infinity" - large enough but won't overflow
  if (fixed32_abs(ray->dir_x) < to_fixed32(0.0001f))
    ray->delta_dist_x = to_fixed32(10000.0f);
  else
    ray->delta_dist_x = fixed32_abs(fixed32_div(to_fixed32(1.0f), ray->dir_x));
  if (fixed32_abs(ray->dir_y) < to_fixed32(0.0001f))
    ray->delta_dist_y = to_fixed32(10000.0f);
  else
    ray->delta_dist_y = fixed32_abs(fixed32_div(to_fixed32(1.0f), ray->dir_y));
  // Fractional position within current cell
  frac_x = fixed32_sub(player_x, to_fixed32((float)ray->map_x));
  frac_y = fixed32_sub(player_y, to_fixed32((float)ray->map_y));
  // Step direction and initial side distances
  if (ray->dir_x < 0) {
    ray->step_x = -1;
    ray->side_dist_x = fixed32_mul(frac_x, ray->delta_dist_x);
  } else {
    ray->step_x = 1;
    ray->side_dist_x =
        fixed32_mul(fixed32_sub(to_fixed32(1.0f), frac_x), ray->delta_dist_x);
  }
  if (ray->dir_y < 0) {
    ray->step_y = -1;
    ray->side_dist_y = fixed32_mul(frac_y, ray->delta_dist_y);
  } else {
    ray->step_y = 1;
    ray->side_dist_y =
        fixed32_mul(fixed32_sub(to_fixed32(1.0f), frac_y), ray->delta_dist_y);
  }
}

/**
 * Perform DDA: step through the grid until we hit a wall
 * Returns 1 if wall hit, 0 if max distance reached
 */
int perform_dda(t_cub_data *data, t_ray *ray) {
  int max_steps;
  int step;

  max_steps = data->map->width + data->map->height;
  step = 0;
  while (step < max_steps) {
    // Step to the next grid cell (whichever is closer)
    if (ray->side_dist_x < ray->side_dist_y) {
      ray->side_dist_x = fixed32_add(ray->side_dist_x, ray->delta_dist_x);
      ray->map_x += ray->step_x;
      ray->side = 0;
    } else {
      ray->side_dist_y = fixed32_add(ray->side_dist_y, ray->delta_dist_y);
      ray->map_y += ray->step_y;
      ray->side = 1;
    }
    // Check if out of bounds (treat as wall)
    if (ray->map_x < 0 || ray->map_x >= data->map->width || ray->map_y < 0 ||
        ray->map_y >= data->map->height) {
      ray->hit = 1;
      return (1);
    }
    // Check if we hit a wall or void (space)
    // '1' = wall, ' ' = void/outside map - both stop rays
    // 'D' = door
    char map_char =
        data->map->map_array[ray->map_y * data->map->width + ray->map_x];

    if (map_char == '1' || map_char == ' ') {
      ray->hit = 1;
      ray->hit_content = map_char;
      return (1);
    } else if (map_char == 'D') {
      t_door *door = get_door_at(data, ray->map_x, ray->map_y);

      if (door) {
        // If closed (open_amount == 0), it's a wall.
        if (door->open_amount < 0.01f) {
          ray->hit = 1;
          ray->hit_content = 'D';
          return (1);
        }
        // If animating (0 < open_amount < 1), record it but CONTINUE.
        else if (door->open_amount < 1.0f) {
          if (!ray->door_hit) {
            ray->door_hit = 1;
            ray->door_map_x = ray->map_x;
            ray->door_map_y = ray->map_y;
            ray->door_side = ray->side;
            // Calculate distance to the door
            if (ray->side == 0)
              ray->door_dist = fixed32_sub(ray->side_dist_x, ray->delta_dist_x);
            else
              ray->door_dist = fixed32_sub(ray->side_dist_y, ray->delta_dist_y);
          }
          // CONTINUE: Ray passes through the empty part of the door frame
        }
        // If fully open (>= 1.0f), treat as empty space (continue)
      }
    }
    step++;
  }
  return (0);
}

/**
 * Calculate perpendicular distance to avoid fish-eye effect
 * Uses the side distance minus one delta (since we stepped past the wall)
 */
void calculate_perp_distance(t_cub_data *data, t_ray *ray) {
  t_fixed32 dot_product;

  if (ray->side == 0)
    ray->perp_dist = fixed32_sub(ray->side_dist_x, ray->delta_dist_x);
  else
    ray->perp_dist = fixed32_sub(ray->side_dist_y, ray->delta_dist_y);

  // Fisheye correction: dist = euclidean_dist * cos(angle_diff)
  // cos(angle_diff) = dot(ray_dir, player_dir)
  // We use the dot product because both vectors are normalized
  dot_product = fixed32_add(fixed32_mul(ray->dir_x, data->player->dir_x),
                            fixed32_mul(ray->dir_y, data->player->dir_y));

  ray->perp_dist = fixed32_mul(ray->perp_dist, dot_product);

  // Correct door distance if we hit one
  if (ray->door_hit) {
    ray->door_dist = fixed32_mul(ray->door_dist, dot_product);
    if (ray->door_dist <= 0)
      ray->door_dist = to_fixed32(0.001f);
  }

  // Prevent division by zero later
  if (ray->perp_dist <= 0)
    ray->perp_dist = to_fixed32(0.001f);
}

/**
 * Cast a single ray from the player position
 */
void cast_single_ray(t_cub_data *data, t_ray *ray, int x) {
  reset_ray(ray);
  calculate_ray_dir(data, ray, x);
  init_dda(data, ray);
  perform_dda(data, ray);
  calculate_perp_distance(data, ray);
}

/**
 * Cast all rays for the current frame
 */
void cast_all_rays(t_cub_data *data) {
  int x;

  if (!data->raycasting || !data->raycasting->rays)
    return;
  x = 0;
  while (x < data->raycasting->num_rays) {
    cast_single_ray(data, &data->raycasting->rays[x], x);
    x++;
  }
}
