/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_render.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 20:00:00 by joamiran          #+#    #+#             */
/*   Updated: 2026/01/24 20:11:05 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

/**
 * Calculate wall slice dimensions for a given ray
 * 
 * @param data       Game data
 * @param ray        The ray to calculate for
 * @param draw_start Output: Y coordinate to start drawing
 * @param draw_end   Output: Y coordinate to stop drawing
 * @return           Height of the wall slice in pixels
 */
static int	calculate_wall_slice(t_cub_data *data, t_ray *ray,
								int *draw_start, int *draw_end)
{
	int		line_height;
	float	perp_dist;
	int		view_offset;

	// Convert perpendicular distance to float for calculation
	perp_dist = from_fixed32(ray->perp_dist);
	if (perp_dist < 0.001f)
		perp_dist = 0.001f;
	// Calculate height of wall slice on screen
	line_height = (int)(data->mlx->height / perp_dist);
	// Combine jump offset + head bob offset
	view_offset = data->player->view_offset + data->player->bob_offset;
	// Calculate start and end points, centered on screen + combined offset
	*draw_start = (data->mlx->height - line_height) / 2 + view_offset;
	*draw_end = (data->mlx->height + line_height) / 2 + view_offset;
	// Clamp to screen bounds
	if (*draw_start < 0)
		*draw_start = 0;
	if (*draw_end >= data->mlx->height)
		*draw_end = data->mlx->height - 1;
	return (line_height);
}

/**
 * Apply shading based on distance and wall side
 * Makes distant walls darker and adds depth
 */
static int	apply_shading(int color, t_ray *ray)
{
	int		r;
	int		g;
	int		b;
	float	shade;
	float	dist;

	// Distance-based shading (farther = darker)
	dist = from_fixed32(ray->perp_dist);
	shade = 1.0f - (dist / 20.0f);
	if (shade < 0.3f)
		shade = 0.3f;
	if (shade > 1.0f)
		shade = 1.0f;
	// Side shading: horizontal walls slightly darker
	if (ray->side == 1)
		shade *= 0.7f;
	// Apply shade to color components
	r = (int)(((color >> 16) & 0xFF) * shade);
	g = (int)(((color >> 8) & 0xFF) * shade);
	b = (int)((color & 0xFF) * shade);
	return ((r << 16) | (g << 8) | b);
}

/**
 * Draw a single vertical wall slice
 */
static void	draw_wall_slice(t_cub_data *data, int x, t_ray *ray)
{
	int	draw_start;
	int	draw_end;
	int	y;
	int	wall_color;
	int	shaded_color;

	if (!ray->hit)
		return ;
	calculate_wall_slice(data, ray, &draw_start, &draw_end);
	// Get base wall color
	wall_color = get_wall_color(ray, data->textures);
	shaded_color = apply_shading(wall_color, ray);
	// Draw the wall slice
	y = draw_start;
	while (y <= draw_end)
	{
		mylx_pixel_put(data, x, y, shaded_color);
		y++;
	}
}

/**
 * Draw ceiling for a column
 */
static void	draw_ceiling_slice(t_cub_data *data, int x, int wall_start)
{
	int	y;
	int	color;

	color = data->textures->ceiling_color;
	y = 0;
	while (y < wall_start)
	{
		mylx_pixel_put(data, x, y, color);
		y++;
	}
}

/**
 * Draw floor for a column
 */
static void	draw_floor_slice(t_cub_data *data, int x, int wall_end)
{
	int	y;
	int	color;

	color = data->textures->floor_color;
	y = wall_end + 1;
	while (y < data->mlx->height)
	{
		mylx_pixel_put(data, x, y, color);
		y++;
	}
}

/**
 * Render a complete column (ceiling + wall + floor)
 */
static void	render_column(t_cub_data *data, int x, t_ray *ray)
{
	int	draw_start;
	int	draw_end;

	if (!ray->hit)
	{
		draw_start = data->mlx->height / 2;
		draw_end = data->mlx->height / 2;
	}
	else
		calculate_wall_slice(data, ray, &draw_start, &draw_end);
	// Draw ceiling above the wall
	draw_ceiling_slice(data, x, draw_start);
	// Draw the wall
	if (ray->hit)
		draw_wall_slice(data, x, ray);
	// Draw floor below the wall
	draw_floor_slice(data, x, draw_end);
}

/**
 * Render all walls based on raycasting data
 * Call this after cast_all_rays()
 */
void	render_walls(t_cub_data *data)
{
	int	x;

	if (!data || !data->raycasting || !data->raycasting->rays)
		return ;
	if (!data->textures)
		return ;
	x = 0;
	while (x < data->raycasting->num_rays)
	{
		render_column(data, x, &data->raycasting->rays[x]);
		x++;
	}
}
