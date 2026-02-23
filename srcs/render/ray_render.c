/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_render.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 20:00:00 by joamiran          #+#    #+#             */
/*   Updated: 2026/02/23 02:55:10 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"
#include "../../includes/gamelogic.h"

static int calculate_wall_slice(t_cub_data *data, t_ray *ray, int *draw_start,
                                int *draw_end) {
  int line_height;
  float perp_dist;
  int view_offset;

  perp_dist = from_fixed32(ray->perp_dist);
  if (perp_dist < 0.001f)
    perp_dist = 0.001f;
  line_height = (int)(data->mlx->height / perp_dist);
  view_offset = data->player->view_offset + data->player->bob_offset;
  *draw_start = (data->mlx->height - line_height) / 2 + view_offset;
  *draw_end = (data->mlx->height + line_height) / 2 + view_offset;
  if (*draw_start < 0)
    *draw_start = 0;
  if (*draw_end >= data->mlx->height)
    *draw_end = data->mlx->height - 1;
  return (line_height);
}

static int apply_shading(int color, float dist, int side) {
  int r;
  int g;
  int b;
  float shade;

  shade = 1.0f - (dist / 20.0f);
  if (shade < 0.3f)
    shade = 0.3f;
  if (shade > 1.0f)
    shade = 1.0f;
  if (side == 1)
    shade *= 0.7f;
  r = (int)(((color >> 16) & 0xFF) * shade);
  g = (int)(((color >> 8) & 0xFF) * shade);
  b = (int)((color & 0xFF) * shade);
  return ((r << 16) | (g << 8) | b);
}

static void draw_wall_slice(t_cub_data *data, int x, t_ray *ray) {
  int draw_start;
  int draw_end;
  int y;
  int wall_color;

  if (!ray->hit)
    return;
  calculate_wall_slice(data, ray, &draw_start, &draw_end);
  wall_color = apply_shading(get_wall_color(ray, data->textures),
                             from_fixed32(ray->perp_dist), ray->side);
  y = draw_start;
  while (y <= draw_end) {
    mylx_pixel_put(data, x, y, wall_color);
    y++;
  }
}

static void draw_door_slice(t_cub_data *data, int x, t_ray *ray) {
  int draw_start;
  int draw_end;
  int y;
  t_door *door;
  int door_color;

  if (!ray->door_hit)
    return;
  calculate_wall_slice(data, ray, &draw_start, &draw_end);
  door = get_door_at(data, ray->door_map_x, ray->door_map_y);
  if (door)
    draw_start += (int)((draw_end - draw_start) * door->open_amount);
  if (draw_start >= draw_end)
    return;
  door_color =
      apply_shading(0x8B4513, from_fixed32(ray->door_dist), ray->door_side);
  y = draw_start;
  while (y <= draw_end) {
    mylx_pixel_put(data, x, y, door_color);
    y++;
  }
}

static void render_column(t_cub_data *data, int x, t_ray *ray) {
  int draw_start;
  int draw_end;
  int y;

  if (!ray->hit) {
    draw_start = data->mlx->height / 2;
    draw_end = data->mlx->height / 2;
  } else
    calculate_wall_slice(data, ray, &draw_start, &draw_end);
  y = -1;
  while (++y < draw_start)
    mylx_pixel_put(data, x, y, data->textures->ceiling_color);
  if (ray->hit)
    draw_wall_slice(data, x, ray);
  y = draw_end;
  while (++y < data->mlx->height)
    mylx_pixel_put(data, x, y, data->textures->floor_color);
}

void render_door_overlays(t_cub_data *data) {
  int x;

  if (!data || !data->raycasting || !data->raycasting->rays)
    return;
  x = 0;
  while (x < data->raycasting->num_rays) {
    if (data->raycasting->rays[x].door_hit)
      draw_door_slice(data, x, &data->raycasting->rays[x]);
    x++;
  }
}

void render_walls(t_cub_data *data) {
  int x;

  if (!data || !data->raycasting || !data->raycasting->rays)
    return;
  if (!data->textures)
    return;
  x = 0;
  while (x < data->raycasting->num_rays) {
    render_column(data, x, &data->raycasting->rays[x]);
    x++;
  }
}
