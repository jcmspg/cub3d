/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 21:45:00 by joao              #+#    #+#             */
/*   Updated: 2025/09/05 20:20:54 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/render.h"

/* 1 setup screen columns 
for a screen width of w pixels, iterate x=0.....witdth-1
calc angle ray for each column

2. compute ray direction
fixed points
rayDirX = quickcos(ray_angle)
rayDirY = quicksin(ray_angle)

3. step along the ray with DDA (incremental steps)
step size = small fraction of a world unit (t_fixed32 -> 0.001)
advance current x += rayDirX * step size
advance current y += rayDirY * step size

4. collision check
map currentX, currentY -> map grid cell
if cell is wall, stop
else continue stepping

5. calculate distance to wall
distance = sqrt((currentX - playerX)^2 + (currentY - playerY)^2)

*/

/**
 * Cast rays for each column of pixels
 */


void	start_rays(t_cub_data *data)
{
		



	(void)data;
}

/**
 * Calculate ray direction and distance
 */
void	calculate_ray(t_game *game, int x)
{
	// TODO: Implement ray calculation
	(void)game;
	(void)x;
}

/**
 * Perform DDA algorithm for wall detection
 */
void	perform_dda(t_game *game)
{
	// TODO: Implement DDA algorithm
	(void)game;
}

