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
#define MINIMAP_CELL_SIZE 12
#define MINIMAP_OFFSET_X 10
#define MINIMAP_OFFSET_Y 10
#define RAY_COLOR 0xFFFF00
#define RAY_EDGE_COLOR 0x00FFFF
#define RAY_CENTER_COLOR 0xFFFFFF

struct s_ray_line
{
	int	x0;
	int	y0;
	int	x1;
	int	y1;
	int	color;
};

static void	step_line(struct s_ray_line *line, int *err, int dx, int dy);

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
static void	draw_line(t_cub_data *data, struct s_ray_line *line)
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

static void	step_line(struct s_ray_line *line, int *err, int dx, int dy)
{
	int	e2;
	int	sx;
	int	sy;

	sx = (line->x0 < line->x1) ? 1 : -1;
	sy = (line->y0 < line->y1) ? 1 : -1;
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
	struct s_ray_line	line;

	world_to_minimap(data->player->x, data->player->y, &start_x, &start_y);
	
	hit_x = fixed32_add(data->player->x, fixed32_mul(ray->dir_x,
				ray->perp_dist));
	hit_y = fixed32_add(data->player->y, fixed32_mul(ray->dir_y,
				ray->perp_dist));
	world_to_minimap(hit_x, hit_y, &end_x, &end_y);
	line.x0 = start_x;
	line.y0 = start_y;
	line.x1 = end_x;
	line.y1 = end_y;
	line.color = color;
	draw_line(data, &line);
}

static int	ray_color(int i, int num_rays)
{
	if (i == 0 || i == num_rays - 1)
		return (RAY_EDGE_COLOR);
	if (i == num_rays / 2)
		return (RAY_CENTER_COLOR);
	return (RAY_COLOR);
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
		color = ray_color(i, num_rays);
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
