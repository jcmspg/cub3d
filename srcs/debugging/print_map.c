/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 19:07:59 by joamiran          #+#    #+#             */
/*   Updated: 2026/02/23 02:25:20 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void print_map_array(const t_map *map) {
  int x;
  int y;

  if (!map || !map->map_array)
    return;
  printf("=== MAP DISPLAY (Height: %d, Width: %d) ===\n", map->height,
         map->width);
  y = 0;
  while (y < map->height) {
    printf("Row %2d: |", y);
    x = 0;
    while (x < map->width) {
      if (map->map_array[y * map->width + x] == ' ')
        printf(".");
      else
        printf("%c", map->map_array[y * map->width + x]);
      x++;
    }
    printf("|\n");
    y++;
  }
}

void draw_filled_tri(t_cub_data *data, t_rect rect) {
  int i;
  int j;
  int pixel_x;
  int pixel_y;

  if (!data || !data->mlx || !data->mlx->img)
    return;
  j = 0;
  while (j < rect.height) {
    i = 0;
    while (i <= (rect.width * j) / rect.height) {
      pixel_x = rect.x + i;
      pixel_y = rect.y + j;
      if (pixel_x >= 0 && pixel_x < data->mlx->width && pixel_y >= 0 &&
          pixel_y < data->mlx->height)
        mylx_pixel_put(data, pixel_x, pixel_y, rect.color);
      i++;
    }
    j++;
  }
}

void draw_filled_rect(t_cub_data *data, t_rect rect) {
  int i;
  int j;
  int pixel_x;
  int pixel_y;

  if (!data || !data->mlx || !data->mlx->img)
    return;
  j = 0;
  while (j < rect.height) {
    i = 0;
    while (i < rect.width) {
      pixel_x = rect.x + i;
      pixel_y = rect.y + j;
      if (pixel_x >= 0 && pixel_x < data->mlx->width && pixel_y >= 0 &&
          pixel_y < data->mlx->height)
        mylx_pixel_put(data, pixel_x, pixel_y, rect.color);
      i++;
    }
    j++;
  }
}

void draw_map_grid(t_cub_data *data) {
  int x;
  int y;
  char c;
  t_rect rect;

  if (!data || !data->map || !data->map->map_array || !data->mlx)
    return;
  y = 0;
  while (y < data->map->height) {
    x = 0;
    while (x < data->map->width) {
      c = data->map->map_array[y * data->map->width + x];
      rect.color = 0x808080;
      if (c == '1')
        rect.color = 0xFFFFFF;
      else if (c == '0')
        rect.color = 0x000000;
      else if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
        rect.color = 0xFF0000;
      else if (c == '2')
        rect.color = 0x00FF00;
      rect.x = 10 + x * 12;
      rect.y = 10 + y * 12;
      rect.width = 12;
      rect.height = 12;
      draw_filled_rect(data, rect);
      x++;
    }
    y++;
  }
}
