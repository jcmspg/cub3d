/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprites.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 16:30:00 by joamiran          #+#    #+#             */
/*   Updated: 2026/03/22 19:36:00 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/render.h"
#include <math.h>

/**
 * Calculate where a door covers on screen for a given stripe
 * Returns the Y range that the door occupies (door_top to door_bottom)
 * If no door or sprite is in front, returns -1 for both
 */
static void get_door_coverage(t_cub_data *data, int stripe, float sprite_dist,
                              int *door_top, int *door_bottom) {
  t_ray *ray;
  t_door *door;
  float door_dist;
  int line_height;
  int frame_top;
  int frame_bottom;
  int offset;
  int view_offset;

  *door_top = -1;
  *door_bottom = -1;
  ray = &data->raycasting->rays[stripe];
  if (!ray->door_hit)
    return;
  door_dist = from_fixed32(ray->door_dist);
  // Sprite is in front of door - no occlusion needed
  if (sprite_dist < door_dist)
    return;
  door = get_door_at(data, ray->door_map_x, ray->door_map_y);
  if (!door)
    return;
  // Calculate door screen position (same logic as ray_render.c)
  line_height = (int)(data->mlx->height / door_dist);
  view_offset = data->player->view_offset + data->player->bob_offset;
  frame_top = (data->mlx->height - line_height) / 2 + view_offset;
  frame_bottom = (data->mlx->height + line_height) / 2 + view_offset;
  // Door slides up based on open_amount
  offset = (int)(line_height * door->open_amount);
  *door_top = frame_top - offset;
  *door_bottom = frame_bottom - offset;
  // Clip to frame
  if (*door_top < frame_top)
    *door_top = frame_top;
  if (*door_top < 0)
    *door_top = 0;
  if (*door_bottom >= data->mlx->height)
    *door_bottom = data->mlx->height - 1;
}

/**
 * Draw a single sprite stripe, respecting door occlusion
 */
static void draw_sprite_stripe_occluded(t_cub_data *data, int stripe,
                                        int draw_start, int draw_end,
                                        int color, int door_top,
                                        int door_bottom) {
  int y;

  y = draw_start;
  while (y < draw_end) {
    if (y >= 0 && y < data->mlx->height) {
      // Skip pixels covered by door
      if (door_top >= 0 && y >= door_top && y <= door_bottom) {
        y++;
        continue;
      }
      mylx_pixel_put(data, stripe, y, color);
    }
    y++;
  }
}

static bool is_transparent_pixel(int pixel) {
  return ((unsigned int)pixel == 0xFF000000U);
}

static void draw_sprite_textured_stripe(t_cub_data *data, int stripe,
                                        int draw_start, int draw_end,
                                        int tex_x, t_texture *texture,
                                        int door_top, int door_bottom) {
  int y;
  int tex_y;
  int tex_color;

  if (!texture || draw_end <= draw_start)
    return;
  y = draw_start;
  while (y < draw_end) {
    if (y >= 0 && y < data->mlx->height) {
      if (door_top >= 0 && y >= door_top && y <= door_bottom) {
        y++;
        continue;
      }
      tex_y = ((y - draw_start) * texture->height) / (draw_end - draw_start);
      if (tex_y < 0)
        tex_y = 0;
      if (tex_y >= texture->height)
        tex_y = texture->height - 1;
      tex_color = get_texture_pixel(texture, tex_x, tex_y);
      if (!is_transparent_pixel(tex_color))
        mylx_pixel_put(data, stripe, y, tex_color);
    }
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
  int view_offset = data->player->view_offset + data->player->bob_offset;

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

  int draw_start_y = -spriteHeight / 2 + data->mlx->height / 2 + view_offset;
  // Lower it towards the ground
  draw_start_y += (int)(data->mlx->height / (4 * transformY));
  if (draw_start_y < 0)
    draw_start_y = 0;
  int draw_end_y = spriteHeight / 2 + data->mlx->height / 2 + view_offset;
  draw_end_y += (int)(data->mlx->height / (4 * transformY));
  if (draw_end_y >= data->mlx->height)
    draw_end_y = data->mlx->height - 1;

  int draw_start_x = -spriteWidth / 2 + spriteScreenX;
  if (draw_start_x < 0)
    draw_start_x = 0;
  int draw_end_x = spriteWidth / 2 + spriteScreenX;
  if (draw_end_x >= data->mlx->width)
    draw_end_x = data->mlx->width - 1;

  t_texture *ammo_texture;

  ammo_texture = NULL;
  if (data->textures)
    ammo_texture = &data->textures->ammo;

  // Loop through every vertical stripe of the sprite on screen
  for (int stripe = draw_start_x; stripe < draw_end_x; stripe++) {
    int tex_x;
    float wall_dist = from_fixed32(data->raycasting->rays[stripe].perp_dist);
    int door_top = -1;
    int door_bottom = -1;
    // Sprite must be in front of wall
    if (transformY >= wall_dist)
      continue;
    // Get door coverage for this stripe
    get_door_coverage(data, stripe, transformY, &door_top, &door_bottom);
    if (ammo_texture && ammo_texture->loaded && ammo_texture->width > 0 &&
        ammo_texture->height > 0 && draw_end_x > draw_start_x) {
      tex_x = ((stripe - draw_start_x) * ammo_texture->width) /
              (draw_end_x - draw_start_x);
      if (tex_x < 0)
        tex_x = 0;
      if (tex_x >= ammo_texture->width)
        tex_x = ammo_texture->width - 1;
      draw_sprite_textured_stripe(data, stripe, draw_start_y, draw_end_y, tex_x,
                                  ammo_texture, door_top, door_bottom);
    } else {
      draw_sprite_stripe_occluded(data, stripe, draw_start_y, draw_end_y,
                                  0xFFD700, door_top, door_bottom);
    }
  }
}

/**
 * Generic billboard renderer with configurable color and scale
 */
static void render_billboard(t_cub_data *data, float sx, float sy, int color,
                             int scale_div, t_texture *texture) {
  float spriteX = sx - from_fixed32(data->player->x);
  float spriteY = sy - from_fixed32(data->player->y);
  int view_offset = data->player->view_offset + data->player->bob_offset;

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

  int draw_start_y = -spriteHeight / 2 + data->mlx->height / 2 + view_offset;
  draw_start_y += (int)(data->mlx->height / (4 * transformY));
  if (draw_start_y < 0)
    draw_start_y = 0;
  int draw_end_y = spriteHeight / 2 + data->mlx->height / 2 + view_offset;
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
    int tex_x;
    float wall_dist = from_fixed32(data->raycasting->rays[stripe].perp_dist);
    int door_top = -1;
    int door_bottom = -1;
    // Sprite must be in front of wall
    if (transformY >= wall_dist)
      continue;
    // Get door coverage for this stripe
    get_door_coverage(data, stripe, transformY, &door_top, &door_bottom);
    if (texture && texture->loaded && texture->width > 0 && texture->height > 0 &&
        draw_end_x > draw_start_x) {
      tex_x = ((stripe - draw_start_x) * texture->width) /
              (draw_end_x - draw_start_x);
      if (tex_x < 0)
        tex_x = 0;
      if (tex_x >= texture->width)
        tex_x = texture->width - 1;
      draw_sprite_textured_stripe(data, stripe, draw_start_y, draw_end_y, tex_x,
                                  texture, door_top, door_bottom);
    } else {
      draw_sprite_stripe_occluded(data, stripe, draw_start_y, draw_end_y, color,
                                  door_top, door_bottom);
    }
  }
}

/**
 * Render enemies as red billboards
 */
static void render_enemies(t_cub_data *data) {
  int i;
  t_texture *demon_texture;

  if (!data->game || !data->game->enemies)
    return;
  demon_texture = NULL;
  if (data->textures)
    demon_texture = &data->textures->demon;
  i = 0;
  uint64_t now = data->fps.last_frame_time;
  while (i < data->game->enemy_count) {
    t_enemy *enemy = &data->game->enemies[i];
    if (enemy->state == ENEMY_DEAD) {
      i++;
      continue;
    }
    if (enemy->state == ENEMY_HIT) {
      // Blinking logic: each blink lasts 150ms (on), 150ms (off)
      uint64_t elapsed = now - enemy->hit_time;
      int blink_period = 300; // ms for one blink (on+off)
      int total_blink_time = enemy->blink_count * blink_period;
      if ((int)elapsed >= total_blink_time) {
        if (enemy->blink_count == 2) {
          // Death: after 2 blinks, destroy
          enemy->state = ENEMY_DEAD;
        } else {
          // Hit: after 1 blink, return to idle
          enemy->state = ENEMY_IDLE;
        }
        i++;
        continue;
      }
      // Determine if visible (blink on or off)
      int blink_on = (((int)elapsed % blink_period) < (blink_period / 2));
      if (blink_on) {
        render_billboard(data, from_fixed32(enemy->x),
                         from_fixed32(enemy->y), 0xFF0000, 2,
                         demon_texture);
      }
    } else {
      render_billboard(data, from_fixed32(enemy->x),
                       from_fixed32(enemy->y), 0xFF0000, 2,
                       demon_texture);
    }
    i++;
  }
}

/**
 * Scan map for 'M' and render them as billboards, then render enemies
 */
void render_sprites(t_cub_data *data) {
  if (!data || !data->map || !data->raycasting)
    return;

  // Render ammo pickups
  for (int y = 0; y < data->map->height; y++) {
    for (int x = 0; x < data->map->width; x++) {
      if (data->map->map_array[y * data->map->width + x] == 'M') {
        render_ammo_sprite(data, (float)x + 0.5f, (float)y + 0.5f);
      }
    }
  }

  // Render enemies
  render_enemies(data);
}
