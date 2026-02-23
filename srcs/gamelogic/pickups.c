/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pickups.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 21:15:00 by joamiran          #+#    #+#             */
/*   Updated: 2026/02/23 03:05:10 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void check_ammo_pickups(t_cub_data *data) {
  int px;
  int py;

  px = (int)from_fixed32(data->player->x);
  py = (int)from_fixed32(data->player->y);
  if (data->map->map_array[py * data->map->width + px] == 'A') {
    data->player->stats.ammo += AMMO_PICKUP_AMOUNT;
    data->map->map_array[py * data->map->width + px] = '0';
  }
}
