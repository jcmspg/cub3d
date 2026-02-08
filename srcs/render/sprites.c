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
 * Render a single ammo box at (mx, my) formatted specifically as a small
 * rectangle
 */
static void render_ammo_sprite(t_cub_data *data, float sx, float sy) {
  float spriteX = sx - from_fixed32(data->player->x);
  float spriteY = sy - from_fixed32(data->player->y);

  // Transform sprite with inverse camera matrix
  // [ planeX   dirX ] -1                                       [ dirY -dirX ]
  // [ planeY   dirY ]       =  1/(planeX*dirY-dirX*planeY) *   [ -planeY planeX
  // ]

  float dirX = from_fixed32(data->player->dir_x);
  float dirY = from_fixed32(data->player->dir_y);
  float planeX = from_fixed32(data->player->plane_x);
  float planeY = from_fixed32(data->player->plane_y);

  float invDet = 1.0 / (planeX * dirY - dirX * planeY);

  float transformX = invDet * (dirY * spriteX - dirX * spriteY);
  float transformY = invDet * (-planeY * spriteX + planeX * spriteY);

  if (transformY <= 0.1f) // Behind player or too close
    return;

  int spriteScreenX =
      (int)((data->mlx->width / 2) * (1 + transformX / transformY));

  // Calculate height of the sprite on screen
  // Using 0.2f as a scale factor for "small rectangle"
  int spriteHeight = abs((int)(data->mlx->height / transformY)) / 4;
  int spriteWidth = spriteHeight; // Square for now

  int draw_start_y = -spriteHeight / 2 + data->mlx->height / 2;
  // Lower it towards the ground
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

  // Golden color for ammo
  int color = 0xFFD700;

  // Loop through every vertical stripe of the sprite on screen
  for (int stripe = draw_start_x; stripe < draw_end_x; stripe++) {
    // Check Z-buffer
    if (transformY < from_fixed32(data->raycasting->rays[stripe].perp_dist)) {
      draw_sprite_stripe(data, stripe, draw_start_y, draw_end_y, color);
    }
  }
}

/**
 * Scan map for 'M' and render them as billboards
 */
void render_sprites(t_cub_data *data) {
  if (!data || !data->map || !data->raycasting)
    return;

  for (int y = 0; y < data->map->height; y++) {
    for (int x = 0; x < data->map->width; x++) {
      if (data->map->map_array[y * data->map->width + x] == 'M') {
        render_ammo_sprite(data, (float)x + 0.5f, (float)y + 0.5f);
      }
    }
  }
}
