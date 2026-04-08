/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_debug_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 21:22:23 by joamiran          #+#    #+#             */
/*   Updated: 2026/04/08 21:25:02 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	step_line(t_ray_line *line, int *err, int dx, int dy)
{
	int	e2;
	int	sx;
	int	sy;

	if (line->x0 < line->x1)
		sx = 1;
	else
		sx = -1;
	if (line->y0 < line->y1)
		sy = 1;
	else
		sy = -1;
	e2 = 2 * (*err);
	if (e2 > -dy)
	{
		*err -= dy;
		line->x0 += sx;
	}
	if (e2 < dx)
	{
		*err += dx;
		line->y0 += sy;
	}
}

void	world_to_minimap(t_fixed32 world_x, t_fixed32 world_y,
		int *screen_x, int *screen_y)
{
	float	wx;
	float	wy;

	wx = from_fixed32(world_x);
	wy = from_fixed32(world_y);
	*screen_x = MINIMAP_OFFSET_X + (int)(wx * MINIMAP_CELL_SIZE);
	*screen_y = MINIMAP_OFFSET_Y + (int)(wy * MINIMAP_CELL_SIZE);
}

/**
 * Draw a line using Bresenham's algorithm
 * Used for drawing rays on the minimap
 */
void	draw_line(t_cub_data *data, t_ray_line *line)
{
	int	dx;
	int	dy;
	int	err;

	dx = abs(line->x1 - line->x0);
	dy = abs(line->y1 - line->y0);
	err = dx - dy;
	while (1)
	{
		if (line->x0 >= 0 && line->x0 < data->mlx->width && line->y0 >= 0
			&& line->y0 < data->mlx->height)
			mylx_pixel_put(data, line->x0, line->y0, line->color);
		if (line->x0 == line->x1 && line->y0 == line->y1)
			break ;
		step_line(line, &err, dx, dy);
	}
}
