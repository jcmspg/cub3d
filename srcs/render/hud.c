/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hud.c                                              :+:      :+:    :+:   */
/*                                              +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 21:00:00 by joamiran          #+#    #+#             */
/*   Updated: 2026/02/23 02:22:15 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/hud.h"

int init_hud(t_cub_data *data) {
  data->hud = ft_calloc(1, sizeof(t_hud));
  if (!data->hud)
    return (0);
  data->hud->height = HUD_HEIGHT;
  data->hud->width = data->mlx->width;
  data->hud->x = 0;
  data->hud->y = data->mlx->height - HUD_HEIGHT;
  data->hud->bg_color = HUD_BG_COLOR;
  data->hud->enabled = true;
  return (1);
}

void render_hud(t_cub_data *data) {
  if (!data || !data->hud || !data->hud->enabled)
    return;
  draw_hud_background(data);
  draw_hud_minimap(data);
  render_hud_text(data);
}

void draw_hud_background(t_cub_data *data) {
  t_rect bg;

  bg.x = data->hud->x;
  bg.y = data->hud->y;
  bg.width = data->hud->width;
  bg.height = data->hud->height;
  bg.color = data->hud->bg_color;
  draw_filled_rect(data, bg);
  bg.height = HUD_BORDER_WIDTH;
  bg.color = HUD_BORDER_COLOR;
  draw_filled_rect(data, bg);
}

void render_hud_text(t_cub_data *data) {
  char *str;
  int y_off;

  y_off = data->hud->y + 30;
  mlx_string_put(data->mlx->mlx_ptr, data->mlx->win_ptr, 20, y_off, 0xFFFFFF,
                 "HEALTH:");
  str = ft_itoa(data->player->stats.health);
  mlx_string_put(data->mlx->mlx_ptr, data->mlx->win_ptr, 80, y_off, 0x00FF00,
                 str);
  free(str);
  y_off += 20;
  mlx_string_put(data->mlx->mlx_ptr, data->mlx->win_ptr, 20, y_off, 0xFFFFFF,
                 "AMMO:");
  str = ft_itoa(data->player->stats.ammo);
  mlx_string_put(data->mlx->mlx_ptr, data->mlx->win_ptr, 80, y_off, 0xFFFF00,
                 str);
  free(str);
}

void draw_hud_minimap(t_cub_data *data) {
  t_rect minimap;
  int cell_size;
  int view_cells;
  t_point p;
  t_point frac;
  t_point offset;
  t_point i;
  t_rect cell_draw;

  if (!data || !data->hud || !data->hud->enabled)
    return;
  minimap.height =
      data->hud->height - (HUD_MINIMAP_PADDING * 2) - HUD_BORDER_WIDTH;
  minimap.width = minimap.height;
  cell_size = HUD_MINIMAP_CELL;
  view_cells = minimap.width / cell_size;
  minimap.x = data->mlx->width - minimap.width - HUD_MINIMAP_PADDING;
  minimap.y = data->hud->y + HUD_BORDER_WIDTH + HUD_MINIMAP_PADDING;
  p.x = from_fixed32(data->player->x);
  p.y = from_fixed32(data->player->y);
  frac.x = p.x - (int)p.x;
  frac.y = p.y - (int)p.y;
  offset.x = (int)(frac.x * cell_size);
  offset.y = (int)(frac.y * cell_size);
  i.y = -1;
  while (++i.y < view_cells + 2) {
    i.x = -1;
    while (++i.x < view_cells + 2) {
      cell_draw.x = minimap.x + i.x * cell_size - offset.x;
      cell_draw.y = minimap.y + i.y * cell_size - offset.y;
      if (cell_draw.x < minimap.x + minimap.width &&
          cell_draw.x + cell_size > minimap.x &&
          cell_draw.y < minimap.y + minimap.height &&
          cell_draw.y + cell_size > minimap.y) {
        cell_draw.width = cell_size;
        cell_draw.height = cell_size;
        draw_minimap_cell_at(data,
                             (t_point){(int)p.x - view_cells / 2 - 1 + i.x,
                                       (int)p.y - view_cells / 2 - 1 + i.y},
                             cell_draw);
      }
    }
  }
  draw_hud_rays(data, minimap);
  draw_minimap_player_centered(data, minimap);
}

void draw_minimap_cell_at(t_cub_data *data, t_point map, t_rect screen) {
  int color;
  char c;
  t_point p;

  if (map.x < 0 || map.x >= data->map->width || map.y < 0 ||
      map.y >= data->map->height)
    color = 0x111111;
  else {
    c = data->map->map_array[map.y * data->map->width + map.x];
    color = 0x111111;
    if (c == '1')
      color = 0xFFFFFF;
    else if (c == 'D')
      color = 0x8B4513;
    else if (c == 'M')
      color = 0xFFD700;
    else if (c == 'X')
      color = 0xFF0000;
    else if (c == '0' || c == 'N' || c == 'S' || c == 'E' || c == 'W')
      color = 0x444444;
  }
  p.y = -1;
  while (++p.y < screen.height) {
    p.x = -1;
    while (++p.x < screen.width) {
      if (p.x == 0 || p.y == 0)
        mylx_pixel_put(data, screen.x + p.x, screen.y + p.y, 0x222222);
      else
        mylx_pixel_put(data, screen.x + p.x, screen.y + p.y, color);
    }
  }
}

void draw_minimap_player_centered(t_cub_data *data, t_rect minimap) {
  t_point center;
  t_point p;
  float dir_angle;
  float fov;
  float l_a;
  float r_a;
  float ray_len;
  int i;

  center.x = minimap.x + minimap.width / 2;
  center.y = minimap.y + minimap.height / 2;
  dir_angle = from_fixed32(data->player->dir_angle);
  fov = START_FOV;
  l_a = (dir_angle - fov / 2.0f) * M_PI / 180.0f;
  r_a = (dir_angle + fov / 2.0f) * M_PI / 180.0f;
  ray_len = minimap.width / 2.5f;
  i = -1;
  while (++i < (int)ray_len)
    mylx_pixel_put(data, center.x + (int)(cosf(l_a) * i),
                   center.y + (int)(sinf(l_a) * i), 0x00AA00);
  i = -1;
  while (++i < (int)ray_len)
    mylx_pixel_put(data, center.x + (int)(cosf(r_a) * i),
                   center.y + (int)(sinf(r_a) * i), 0x00AA00);
  i = -1;
  while (++i < (int)ray_len)
    mylx_pixel_put(data, center.x + (int)(cosf(dir_angle * M_PI / 180.0f) * i),
                   center.y + (int)(sinf(dir_angle * M_PI / 180.0f) * i),
                   0xFFFF00);
  p.y = -3;
  while (++p.y <= 2) {
    p.x = -3;
    while (++p.x <= 2)
      mylx_pixel_put(data, center.x + p.x, center.y + p.y, 0xFF0000);
  }
}

void draw_minimap_cell(t_cub_data *data, t_point p, int size) {
  t_point px;
  int color;
  char c;

  if (p.x < 0 || p.x >= data->map->width || p.y < 0 || p.y >= data->map->height)
    color = 0x111111;
  else {
    c = data->map->map_array[p.y * data->map->width + p.x];
    color = 0x444444;
    if (c == '1')
      color = 0xFFFFFF;
    else if (c == 'D')
      color = 0x8B4513;
    else if (c == 'M')
      color = 0xFFD700;
    else if (c == 'X')
      color = 0xFF0000;
  }
  px.y = -1;
  while (++px.y < size) {
    px.x = -1;
    while (++px.x < size)
      mylx_pixel_put(data, p.x * size + px.x, p.y * size + px.y, color);
  }
}

void draw_hud_rays(t_cub_data *data, t_rect minimap) {
  int i;
  t_point center;
  t_line l;
  t_ray *ray;

  if (!data->raycasting || !data->raycasting->rays)
    return;
  center.x = minimap.x + minimap.width / 2;
  center.y = minimap.y + minimap.height / 2;
  i = 0;
  while (i < data->raycasting->num_rays) {
    ray = &data->raycasting->rays[i];
    l.p0 = center;
    l.p1.x = center.x + (int)(from_fixed32(ray->dir_x) *
                              from_fixed32(ray->perp_dist) * HUD_MINIMAP_CELL);
    l.p1.y = center.y + (int)(from_fixed32(ray->dir_y) *
                              from_fixed32(ray->perp_dist) * HUD_MINIMAP_CELL);
    if (l.p1.x < minimap.x)
      l.p1.x = minimap.x;
    if (l.p1.x >= minimap.x + minimap.width)
      l.p1.x = minimap.x + minimap.width - 1;
    if (l.p1.y < minimap.y)
      l.p1.y = minimap.y;
    if (l.p1.y >= minimap.y + minimap.height)
      l.p1.y = minimap.y + minimap.height - 1;
    l.color = 0x00AA00;
    draw_hud_ray_line(data, l);
    i += 20;
  }
}

void draw_hud_ray_line(t_cub_data *data, t_line l) {
  t_bres b;
  int e2;

  b.dx = abs(l.p1.x - l.p0.x);
  b.dy = abs(l.p1.y - l.p0.y);
  b.step_x = -1;
  if (l.p0.x < l.p1.x)
    b.step_x = 1;
  b.step_y = -1;
  if (l.p0.y < l.p1.y)
    b.step_y = 1;
  b.err = b.dx - b.dy;
  while (1) {
    mylx_pixel_put(data, l.p0.x, l.p0.y, l.color);
    if (l.p0.x == l.p1.x && l.p0.y == l.p1.y)
      break;
    e2 = 2 * b.err;
    if (e2 > -b.dy) {
      b.err -= b.dy;
      l.p0.x += b.step_x;
    }
    if (e2 < b.dx) {
      b.err += b.dx;
      l.p0.y += b.step_y;
    }
  }
}
