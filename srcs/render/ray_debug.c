/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_debug.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 20:00:00 by joamiran          #+#    #+#             */
/*   Updated: 2026/04/08 21:22:40 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

/*
** Minimap configuration - MUST MATCH draw_map_grid() settings!
** draw_map_grid() uses cell_size=12, start_x=10, start_y=10
*/

/**
 * Draw a single ray from player to hit point on minimap
 */
void	draw_ray_on_minimap(t_cub_data *data, t_ray *ray, int color)
{
	t_fixed32	hit_x;
	t_fixed32	hit_y;
	t_ray_line	line;

	world_to_minimap(data->player->x, data->player->y, &line.x0, &line.y0);
	hit_x = fixed32_add(data->player->x, fixed32_mul(ray->dir_x,
				ray->perp_dist));
	hit_y = fixed32_add(data->player->y, fixed32_mul(ray->dir_y,
				ray->perp_dist));
	world_to_minimap(hit_x, hit_y, &line.x1, &line.y1);
	line.color = color;
	draw_line(data, &line);
}

int	ray_color(int i, int num_rays)
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
