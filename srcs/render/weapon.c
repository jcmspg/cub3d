/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   weapon.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 17:00:00 by joamiran          #+#    #+#             */
/*   Updated: 2026/02/08 17:00:00 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/render.h"

/**
 * Render a placeholder weapon (rectangle)
 */
void render_weapon(t_cub_data *data) {
  int w_width;
  int w_height;
  int x_start;
  int y_start;
  int x, y;
  int color;

  // Dimensions relative to screen
  w_width = data->mlx->width / 6;
  w_height = data->mlx->height / 3;

  // Position: Bottom center
  x_start = (data->mlx->width / 2) - (w_width / 2);
  // slightly offset to right like Doom/Wolf3D often is, or just center?
  // User asked for "weapon texture placeholder.. for now leave it as a
  // rectangle" Let's stick to center-ish right or center. Center is classic
  // Wolf3D. Actually Wolf3D is center. Doom is center.

  // Let's add extensive bobbing to make it feel "alive" later,
  // for now static or simple bob from player

  int bob_y = 0;
  // Simple bobbing if moving
  if (data->input->forward || data->input->backward || data->input->left ||
      data->input->right)
    bob_y = abs(data->player->bob_offset) * 2;

  y_start = data->mlx->height - w_height + bob_y;

  color = 0x555555; // Grey gun

  // Draw rectangle
  for (y = 0; y < w_height; y++) {
    for (x = 0; x < w_width; x++) {
      int screen_x = x_start + x;
      int screen_y = y_start + y;

      // Simple border check
      if (x == 0 || x == w_width - 1 || y == 0)
        mylx_pixel_put(data, screen_x, screen_y, 0x222222);
      else
        mylx_pixel_put(data, screen_x, screen_y, color);
    }
  }

  // Draw "muzzle" or something to distinguish front?
  // Nah, just a box as requested.
}
