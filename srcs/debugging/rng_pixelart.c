/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rng_pixelart.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 22:15:00 by joamiran          #+#    #+#             */
/*   Updated: 2026/02/23 03:10:10 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void draw_rng_pixelart(t_cub_data *data) {
  int x;
  int y;
  int color;

  y = -1;
  while (++y < data->mlx->height) {
    x = -1;
    while (++x < data->mlx->width) {
      color = (rand() % 0xFFFFFF);
      mylx_pixel_put(data, x, y, color);
    }
  }
}