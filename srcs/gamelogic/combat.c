/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   combat.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 17:15:00 by joamiran          #+#    #+#             */
/*   Updated: 2026/03/22 18:18:22 by joamiran         ###   ########.fr       */
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
void	player_shoot(t_cub_data *data)
{
	static bool	was_shooting = false;
	int			center_idx;
void	player_shoot(t_cub_data *data)
{
	static bool	was_shooting = false;
	int			center_idx;

	if (!data || !data->player || !data->input)
		return ;
	// Semi-automatic: only fire on the RISING EDGE (button press transition)
	// If button is held, don't keep firing
	if (data->input->shoot && !was_shooting)
	{
		// Check ammo
		if (data->player->stats.ammo <= 0)
		{
			printf("*click* Out of ammo!\n");
			was_shooting = data->input->shoot;
			return ;
		}
		// Fire!
		data->player->stats.ammo--;
		printf("BANG! Ammo left: %d\n", data->player->stats.ammo);
		// Hitscan Logic
		// The center ray is at index: screen_width / 2
		// We can reuse the raycasting result from `render_walls` which populated
		// `data->raycasting->rays`
		center_idx = data->mlx->width / 2;
		       if (center_idx >= 0 && center_idx < data->raycasting->num_rays)
		       {
			       int screen_width = data->mlx->width;
			       int center_min = screen_width / 2 - screen_width / 10;
			       int center_max = screen_width / 2 + screen_width / 10;
			       int i;
			       for (i = 0; i < data->game->enemy_count; i++) {
				       t_enemy *enemy = &data->game->enemies[i];
				       if (enemy->state == ENEMY_DEAD)
					       continue;
				       // Project enemy position to screen X (reuse billboard logic)
				       float spriteX = from_fixed32(enemy->x) - from_fixed32(data->player->x);
				       float spriteY = from_fixed32(enemy->y) - from_fixed32(data->player->y);
				       float dirX = from_fixed32(data->player->dir_x);
				       float dirY = from_fixed32(data->player->dir_y);
				       float planeX = from_fixed32(data->player->plane_x);
				       float planeY = from_fixed32(data->player->plane_y);
				       float invDet = 1.0f / (planeX * dirY - dirX * planeY);
				       float transformX = invDet * (dirY * spriteX - dirX * spriteY);
				       float transformY = invDet * (-planeY * spriteX + planeX * spriteY);
				       if (transformY <= 0.1f)
					       continue; // Behind player
				       int spriteScreenX = (int)((screen_width / 2) * (1 + transformX / transformY));
				       // Check if enemy is in the center 1/5th of the screen
				       if (spriteScreenX >= center_min && spriteScreenX <= center_max) {
					       // Optional: check if enemy is closer than wall
					       float enemy_dist = sqrtf(spriteX * spriteX + spriteY * spriteY);
					       float wall_dist = from_fixed32(data->raycasting->rays[center_idx].perp_dist);
					       if (enemy_dist < wall_dist) {
						       enemy->stats.health -= BULLET_DMG;
						       printf("Enemy %d hit! HP: %d\n", enemy->id, enemy->stats.health);
							       if (enemy->stats.health <= 0) {
								       enemy->state = ENEMY_HIT;
								       enemy->hit_time = data->fps.last_frame_time;
								       printf("Enemy %d killed!\n", enemy->id);
							       } else {
								       enemy->state = ENEMY_HIT;
								       enemy->hit_time = data->fps.last_frame_time;
							       }
						       // Only hit one enemy per shot
						       break;
					       }
				       }
			       }
		       }
	}
	// Remember previous state for edge detection
	was_shooting = data->input->shoot;
}
