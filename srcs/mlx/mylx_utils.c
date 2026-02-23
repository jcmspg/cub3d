/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mylx_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 21:07:31 by joamiran          #+#    #+#             */
/*   Updated: 2026/02/23 02:52:10 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mylx_utils.h"

void mylx_pixel_put(t_cub_data *data, int x, int y, int color) {
  char *dst;

  if (x < 0 || x >= data->mlx->width || y < 0 || y >= data->mlx->height)
    return;
  dst = data->mlx->img->address + (y * data->mlx->img->line_length +
                                   x * (data->mlx->img->bits_per_pixel / 8));
  *(unsigned int *)dst = color;
}

int mylx_get_pixel(t_img_data *img, int x, int y) {
  char *dst;

  dst = img->address + (y * img->line_length + x * (img->bits_per_pixel / 8));
  return (*(unsigned int *)dst);
}

void mylx_clear_image(t_img_data *img, int color) {
  int x;
  int y;
  int width;
  int height;

  width = START_WIDTH;
  height = START_HEIGHT;
  y = -1;
  while (++y < height) {
    x = -1;
    while (++x < width)
      *(unsigned int *)(img->address + (y * img->line_length +
                                        x * (img->bits_per_pixel / 8))) = color;
  }
}

void mylx_destroy_image(t_cub_data *data) {
  if (data->mlx && data->mlx->img && data->mlx->img->img) {
    mlx_destroy_image(data->mlx->mlx_ptr, data->mlx->img->img);
    free(data->mlx->img);
    data->mlx->img = NULL;
  }
}

void mylx_destroy_window(t_cub_data *data) {
  if (data->mlx && data->mlx->win_ptr) {
    mlx_destroy_window(data->mlx->mlx_ptr, data->mlx->win_ptr);
    data->mlx->win_ptr = NULL;
  }
}

void mylx_destroy_mlx(t_cub_data *data) {
  if (data->mlx && data->mlx->mlx_ptr) {
    mlx_destroy_display(data->mlx->mlx_ptr);
    free(data->mlx->mlx_ptr);
    data->mlx->mlx_ptr = NULL;
  }
}