/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_debug.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 20:00:00 by joamiran          #+#    #+#             */
/*   Updated: 2026/01/24 19:35:30 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

/*
** Minimap configuration - MUST MATCH draw_map_grid() settings!
** draw_map_grid() uses cell_size=12, start_x=10, start_y=10
*/
# define MINIMAP_CELL_SIZE	12
# define MINIMAP_OFFSET_X	10
# define MINIMAP_OFFSET_Y	10
# define RAY_COLOR			0xFFFF00
# define RAY_EDGE_COLOR		0x00FFFF
# define RAY_CENTER_COLOR	0xFFFFFF

/**
 * Convert world coordinates to minimap pixel coordinates
 * Player position is in map units (e.g., 5.5 means middle of cell 5)
 */
static void	world_to_minimap(t_fixed32 world_x, t_fixed32 world_y,
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
static void	draw_line(t_cub_data *data, int x0, int y0, int x1, int y1,
						int color)
{
	int	dx;
	int	dy;
	int	sx;
	int	sy;
	int	err;
	int	e2;

	dx = abs(x1 - x0);
	dy = abs(y1 - y0);
	sx = (x0 < x1) ? 1 : -1;
	sy = (y0 < y1) ? 1 : -1;
	err = dx - dy;
	while (1)
	{
		if (x0 >= 0 && x0 < data->mlx->width
			&& y0 >= 0 && y0 < data->mlx->height)
			mylx_pixel_put(data, x0, y0, color);
		if (x0 == x1 && y0 == y1)
			break ;
		e2 = 2 * err;
		if (e2 > -dy)
		{
			err -= dy;
			x0 += sx;
		}
		if (e2 < dx)
		{
			err += dx;
			y0 += sy;
		}
	}
}

/**
 * Draw a single ray from player to hit point on minimap
 */
static void	draw_ray_on_minimap(t_cub_data *data, t_ray *ray, int color)
{
	int			start_x;
	int			start_y;
	int			end_x;
	int			end_y;
	t_fixed32	hit_x;
	t_fixed32	hit_y;

	world_to_minimap(data->player->x, data->player->y, &start_x, &start_y);
	// Calculate hit point: player_pos + ray_dir * perp_dist
	hit_x = fixed32_add(data->player->x,
			fixed32_mul(ray->dir_x, ray->perp_dist));
	hit_y = fixed32_add(data->player->y,
			fixed32_mul(ray->dir_y, ray->perp_dist));
	world_to_minimap(hit_x, hit_y, &end_x, &end_y);
	draw_line(data, start_x, start_y, end_x, end_y, color);
}

/**
 * Draw ALL rays on the minimap - creates the full FOV visualization
 * Rays are colored based on distance for a gradient effect
 */
void	draw_rays_on_minimap(t_cub_data *data)
{
	int	i;
	int	color;
	int	num_rays;

	if (!data || !data->raycasting || !data->raycasting->rays)
		return ;
	num_rays = data->raycasting->num_rays;
	i = 0;
	while (i < num_rays)
	{
		// Color coding: edges = cyan, center = white, rest = yellow
		if (i == 0 || i == num_rays - 1)
			color = RAY_EDGE_COLOR;
		else if (i == num_rays / 2)
			color = RAY_CENTER_COLOR;
		else
			color = RAY_COLOR;
		// Only draw if ray hit something
		if (data->raycasting->rays[i].hit)
			draw_ray_on_minimap(data, &data->raycasting->rays[i], color);
		i++;
	}
}

/**
 * Draw complete ray debug visualization
 * Call this after cast_all_rays() and after drawing the map grid
 */
void	draw_ray_debug(t_cub_data *data)
{
	if (!data || !data->player || !data->raycasting)
		return ;
	draw_rays_on_minimap(data);
}
