/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pickups.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 16:10:00 by joamiran          #+#    #+#             */
/*   Updated: 2026/02/08 16:10:00 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/gamelogic.h"

/**
 * Check if player is standing on an ammo pickup and collect it
 */
void check_ammo_pickups(t_cub_data *data) {
  int player_x;
  int player_y;
  char *cell;

  if (!data || !data->player || !data->map)
    return;

  // Get player's grid position
  player_x = (int)from_fixed32(data->player->x);
  player_y = (int)from_fixed32(data->player->y);

  // Check bounds
  if (player_x < 0 || player_x >= data->map->width || player_y < 0 ||
      player_y >= data->map->height)
    return;

  // Get the cell at player position
  cell = &data->map->map_array[player_y * data->map->width + player_x];

  // If it's an ammo pickup
  if (*cell == 'M') {
    // Add ammo
    data->player->stats.ammo += AMMO_PICKUP_AMOUNT;
    if (data->player->stats.ammo > data->player->stats.max_ammo)
      data->player->stats.ammo = data->player->stats.max_ammo;

    // Remove pickup from map
    *cell = '0';

    printf("Picked up ammo! Total: %d\n", data->player->stats.ammo);
  }
}
