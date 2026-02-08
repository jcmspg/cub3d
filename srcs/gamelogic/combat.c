/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   combat.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 17:15:00 by joamiran          #+#    #+#             */
/*   Updated: 2026/02/08 16:07:00 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/gamelogic.h"
#include "../../includes/render.h" // for raycasting access if needed

/**
 * Handle player shooting - SEMI-AUTOMATIC
 * Only fires once per button press (requires release and re-press for next
 * shot)
 * 1. Detect rising edge (button press transition)
 * 2. Check ammo
 * 3. Fire ray (center of screen)
 * 4. Check intersection with enemies (placeholder)
 */
void player_shoot(t_cub_data *data) {
  static bool was_shooting = false;

  if (!data || !data->player || !data->input)
    return;

  // Semi-automatic: only fire on the RISING EDGE (button press transition)
  // If button is held, don't keep firing
  if (data->input->shoot && !was_shooting) {
    // Check ammo
    if (data->player->stats.ammo <= 0) {
      printf("*click* Out of ammo!\n");
      was_shooting = data->input->shoot;
      return;
    }

    // Fire!
    data->player->stats.ammo--;
    printf("BANG! Ammo left: %d\n", data->player->stats.ammo);

    // Hitscan Logic
    // The center ray is at index: screen_width / 2
    // We can reuse the raycasting result from `render_walls` which populated
    // `data->raycasting->rays`

    int center_idx = data->mlx->width / 2;
    if (center_idx >= 0 && center_idx < data->raycasting->num_rays) {
      // t_ray *center_ray = &data->raycasting->rays[center_idx];

      // This gives us the wall distance
      // printf("Hit wall at distance: %.2f\n",
      // from_fixed32(center_ray->perp_dist));

      // TODO: Check against enemies
      // Iterate through enemy list (when we have one)
      // Check if enemy is within the "center 1/3" cone AND closer than the
      // wall. float enemy_angle = atan2(enemy.y - player.y, enemy.x - player.x)
      // float diff = angle_diff(player_angle, enemy_angle)
      // if (abs(diff) < threshold && enemy_dist < center_ray->perp_dist) -> HIT
    }
  }

  // Remember previous state for edge detection
  was_shooting = data->input->shoot;
}
