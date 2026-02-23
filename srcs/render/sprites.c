/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprites.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 16:30:00 by joamiran          #+#    #+#             */
/*   Updated: 2026/02/08 16:30:00 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/render.h"
#include <math.h>

/**
 * Draw a single sprite stripe (vertical line)
 */
static void draw_sprite_stripe(t_cub_data *data, int stripe, int draw_start,
                               int draw_end, int color) {
  int y;

  y = draw_start;
  while (y < draw_end) {
    if (y >= 0 && y < data->mlx->height)
      mylx_pixel_put(data, stripe, y, color);
    y++;
  }
}

/**
 * Render a billboard sprite at world position (sx, sy) with given color
 * Reusable for ammo boxes, enemies, etc.
 */
static void render_billboard(t_cub_data *data, float sx, float sy, int color,
                             int scale_div) {
  float spriteX = sx - from_fixed32(data->player->x);
  float spriteY = sy - from_fixed32(data->player->y);

  float dirX = from_fixed32(data->player->dir_x);
  float dirY = from_fixed32(data->player->dir_y);
  float planeX = from_fixed32(data->player->plane_x);
  float planeY = from_fixed32(data->player->plane_y);

  float invDet = 1.0 / (planeX * dirY - dirX * planeY);

  float transformX = invDet * (dirY * spriteX - dirX * spriteY);
  float transformY = invDet * (-planeY * spriteX + planeX * spriteY);

  if (transformY <= 0.1f)
    return;

  int spriteScreenX =
      (int)((data->mlx->width / 2) * (1 + transformX / transformY));

  int spriteHeight = abs((int)(data->mlx->height / transformY)) / scale_div;
  int spriteWidth = spriteHeight;

  int draw_start_y = -spriteHeight / 2 + data->mlx->height / 2;
  draw_start_y += (int)(data->mlx->height / (4 * transformY));
  if (draw_start_y < 0)
    draw_start_y = 0;
  int draw_end_y = spriteHeight / 2 + data->mlx->height / 2;
  draw_end_y += (int)(data->mlx->height / (4 * transformY));
  if (draw_end_y >= data->mlx->height)
    draw_end_y = data->mlx->height - 1;

  int draw_start_x = -spriteWidth / 2 + spriteScreenX;
  if (draw_start_x < 0)
    draw_start_x = 0;
  int draw_end_x = spriteWidth / 2 + spriteScreenX;
  if (draw_end_x >= data->mlx->width)
    draw_end_x = data->mlx->width - 1;

  for (int stripe = draw_start_x; stripe < draw_end_x; stripe++) {
    if (transformY < from_fixed32(data->raycasting->rays[stripe].perp_dist))
      draw_sprite_stripe(data, stripe, draw_start_y, draw_end_y, color);
  }
}

/**
 * Render all sprites: ammo pickups + enemies
 */
void render_sprites(t_cub_data *data) {
  int x;
  int y;
  int i;

  if (!data || !data->map || !data->raycasting)
    return;

  // Render ammo pickups
  y = 0;
  while (y < data->map->height) {
    x = 0;
    while (x < data->map->width) {
      if (data->map->map_array[y * data->map->width + x] == 'M')
        render_billboard(data, (float)x + 0.5f, (float)y + 0.5f, 0xFFD700, 4);
      x++;
    }
    y++;
  }

  // Render enemies
  if (!data->game || !data->game->enemies)
    return;
  i = 0;
  while (i < data->game->enemy_count) {
    if (data->game->enemies[i].state != ENEMY_DEAD) {
      render_billboard(data, from_fixed32(data->game->enemies[i].x),
                       from_fixed32(data->game->enemies[i].y), 0xFF0000, 2);
    }
    i++;
  }
}
