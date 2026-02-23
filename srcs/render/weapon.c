/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   weapon.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 21:30:00 by joamiran          #+#    #+#             */
/*   Updated: 2026/02/23 02:55:10 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static void draw_weapon_rect(t_cub_data *data, t_rect rect) {
  int x;
  int y;

  y = rect.y;
  while (y < rect.y + rect.height) {
    x = rect.x;
    while (x < rect.x + rect.width) {
      mylx_pixel_put(data, x, y, rect.color);
      x++;
    }
    y++;
  }
}

void render_weapon(t_cub_data *data) {
  t_rect rect;
  int bob;

  if (!data || !data->player)
    return;
  bob = data->player->bob_offset;
  rect.width = 120;
  rect.height = 180;
  rect.x = data->mlx->width / 2 - rect.width / 2;
  rect.y = data->mlx->height - rect.height + bob;
  rect.color = 0x333333;
  draw_weapon_rect(data, rect);
  rect.width = 40;
  rect.height = 100;
  rect.x = data->mlx->width / 2 - rect.width / 2;
  rect.y = data->mlx->height - rect.height - 40 + bob;
  rect.color = 0x111111;
  draw_weapon_rect(data, rect);
}
