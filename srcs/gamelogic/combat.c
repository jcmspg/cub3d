/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   combat.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 17:15:00 by joamiran          #+#    #+#             */
/*   Updated: 2026/03/22 19:36:00 by joamiran         ###   ########.fr       */
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
	int			screen_width;
	int			center_min;
	int			center_max;
			int i;
	t_enemy		*enemy;
	float		spriteX;
	float		spriteY;
	float		dirX;
	float		dirY;
	float		planeX;
	float		planeY;
	float		invDet;
	float		transformX;
	float		transformY;
	int			spriteScreenX;
	float		enemy_dist;
	float		wall_dist;

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
			screen_width = data->mlx->width;
			center_min = screen_width / 2 - screen_width / 10;
			center_max = screen_width / 2 + screen_width / 10;
			for (i = 0; i < data->game->enemy_count; i++)
			{
				enemy = &data->game->enemies[i];
				if (enemy->state == ENEMY_DEAD)
					continue ;
				// Project enemy position to screen X (reuse billboard logic)
				spriteX = from_fixed32(enemy->x)
					- from_fixed32(data->player->x);
				spriteY = from_fixed32(enemy->y)
					- from_fixed32(data->player->y);
				dirX = from_fixed32(data->player->dir_x);
				dirY = from_fixed32(data->player->dir_y);
				planeX = from_fixed32(data->player->plane_x);
				planeY = from_fixed32(data->player->plane_y);
				invDet = 1.0f / (planeX * dirY - dirX * planeY);
				transformX = invDet * (dirY * spriteX - dirX * spriteY);
				transformY = invDet * (-planeY * spriteX + planeX * spriteY);
				if (transformY <= 0.1f)
					continue ; // Behind player
				spriteScreenX = (int)((screen_width / 2) * (1 + transformX
							/ transformY));
				// Check if enemy is in the center 1/5th of the screen
				if (spriteScreenX >= center_min && spriteScreenX <= center_max)
				{
					// Optional: check if enemy is closer than wall
					enemy_dist = sqrtf(spriteX * spriteX + spriteY * spriteY);
					wall_dist = from_fixed32(data->raycasting->rays[center_idx].perp_dist);
					if (enemy_dist < wall_dist)
					{
						enemy->stats.health -= BULLET_DMG;
						printf("Enemy %d hit! HP: %d\n", enemy->id,
							enemy->stats.health);
						enemy->state = ENEMY_HIT;
						enemy->hit_time = data->fps.last_frame_time;
						if (enemy->stats.health <= 0)
						{
							enemy->blink_count = 2; // death: blink twice
							printf("Enemy %d killed!\n", enemy->id);
						}
						else
						{
							enemy->blink_count = 1; // hit: blink once
						}
						enemy->blink_phase = 1; // not used in render,
							but kept for future
						break ;
					}
				}
			}
		}
	}
	// Remember previous state for edge detection
	was_shooting = data->input->shoot;
}
