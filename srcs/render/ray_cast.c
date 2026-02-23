/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_cast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 19:30:00 by joamiran          #+#    #+#             */
/*   Updated: 2026/02/23 02:40:10 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void calculate_ray_dir(t_cub_data *data, t_ray *ray, int x) {
  t_fixed32 camera_x;
  t_fixed32 ray_angle;
  t_fixed32 half_fov;
  t_fixed32 fov_step;

  camera_x = fixed32_sub(
      fixed32_div(fixed32_mul(to_fixed32(2.0f), to_fixed32((float)x)),
                  to_fixed32((float)data->mlx->width)),
      to_fixed32(1.0f));
  half_fov = fixed32_div(data->game->fov, to_fixed32(2.0f));
  fov_step = fixed32_mul(camera_x, half_fov);
  ray_angle = fixed32_add(data->player->dir_angle, fov_step);
  ray_angle = normalize_angle_degrees(ray_angle);
  ray->dir_x = fast_cos(&data->trig, ray_angle);
  ray->dir_y = fast_sin(&data->trig, ray_angle);
}

void init_dda(t_cub_data *data, t_ray *ray) {
  t_fixed32 p_x;
  t_fixed32 p_y;

  p_x = data->player->x;
  p_y = data->player->y;
  ray->map_x = (int)from_fixed32(p_x);
  ray->map_y = (int)from_fixed32(p_y);
  if (fixed32_abs(ray->dir_x) < to_fixed32(0.0001f))
    ray->delta_dist_x = to_fixed32(10000.0f);
  else
    ray->delta_dist_x = fixed32_abs(fixed32_div(to_fixed32(1.0f), ray->dir_x));
  if (fixed32_abs(ray->dir_y) < to_fixed32(0.0001f))
    ray->delta_dist_y = to_fixed32(10000.0f);
  else
    ray->delta_dist_y = fixed32_abs(fixed32_div(to_fixed32(1.0f), ray->dir_y));
  if (ray->dir_x < 0) {
    ray->step_x = -1;
    ray->side_dist_x = fixed32_mul(
        fixed32_sub(p_x, to_fixed32((float)ray->map_x)), ray->delta_dist_x);
  } else {
    ray->step_x = 1;
    ray->side_dist_x = fixed32_mul(
        fixed32_sub(to_fixed32(1.0f),
                    fixed32_sub(p_x, to_fixed32((float)ray->map_x))),
        ray->delta_dist_x);
  }
  if (ray->dir_y < 0) {
    ray->step_y = -1;
    ray->side_dist_y = fixed32_mul(
        fixed32_sub(p_y, to_fixed32((float)ray->map_y)), ray->delta_dist_y);
  } else {
    ray->step_y = 1;
    ray->side_dist_y = fixed32_mul(
        fixed32_sub(to_fixed32(1.0f),
                    fixed32_sub(p_y, to_fixed32((float)ray->map_y))),
        ray->delta_dist_y);
  }
}

static void check_door_hit(t_cub_data *data, t_ray *ray, char map_char) {
  t_door *door;

  if (map_char == 'D') {
    door = get_door_at(data, ray->map_x, ray->map_y);
    if (door) {
      if (door->open_amount < 0.01f) {
        ray->hit = 1;
        ray->hit_content = 'D';
      } else if (door->open_amount < 1.0f && !ray->door_hit) {
        ray->door_hit = 1;
        ray->door_map_x = ray->map_x;
        ray->door_map_y = ray->map_y;
        ray->door_side = ray->side;
        if (ray->side == 0)
          ray->door_dist = fixed32_sub(ray->side_dist_x, ray->delta_dist_x);
        else
          ray->door_dist = fixed32_sub(ray->side_dist_y, ray->delta_dist_y);
      }
    }
  }
}

int perform_dda(t_cub_data *data, t_ray *ray) {
  int max_steps;
  int step;
  char map_char;

  max_steps = data->map->width + data->map->height;
  step = -1;
  while (++step < max_steps) {
    if (ray->side_dist_x < ray->side_dist_y) {
      ray->side_dist_x = fixed32_add(ray->side_dist_x, ray->delta_dist_x);
      ray->map_x += ray->step_x;
      ray->side = 0;
    } else {
      ray->side_dist_y = fixed32_add(ray->side_dist_y, ray->delta_dist_y);
      ray->map_y += ray->step_y;
      ray->side = 1;
    }
    if (ray->map_x < 0 || ray->map_x >= data->map->width || ray->map_y < 0 ||
        ray->map_y >= data->map->height) {
      ray->hit = 1;
      return (1);
    }
    map_char = data->map->map_array[ray->map_y * data->map->width + ray->map_x];
    if (map_char == '1' || map_char == ' ') {
      ray->hit = 1;
      ray->hit_content = map_char;
      return (1);
    }
    check_door_hit(data, ray, map_char);
    if (ray->hit)
      return (1);
  }
  return (0);
}

void calculate_perp_distance(t_cub_data *data, t_ray *ray) {
  t_fixed32 dot_product;

  if (ray->side == 0)
    ray->perp_dist = fixed32_sub(ray->side_dist_x, ray->delta_dist_x);
  else
    ray->perp_dist = fixed32_sub(ray->side_dist_y, ray->delta_dist_y);
  dot_product = fixed32_add(fixed32_mul(ray->dir_x, data->player->dir_x),
                            fixed32_mul(ray->dir_y, data->player->dir_y));
  ray->perp_dist = fixed32_mul(ray->perp_dist, dot_product);
  if (ray->door_hit) {
    ray->door_dist = fixed32_mul(ray->door_dist, dot_product);
    if (ray->door_dist <= 0)
      ray->door_dist = to_fixed32(0.001f);
  }
  if (ray->perp_dist <= 0)
    ray->perp_dist = to_fixed32(0.001f);
}

void cast_single_ray(t_cub_data *data, t_ray *ray, int x) {
  reset_ray(ray);
  calculate_ray_dir(data, ray, x);
  init_dda(data, ray);
  perform_dda(data, ray);
  calculate_perp_distance(data, ray);
}

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
