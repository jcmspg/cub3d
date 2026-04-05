/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_render.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 20:00:00 by joamiran          #+#    #+#             */
/*   Updated: 2026/03/22 20:36:49 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"
#include "../../includes/gamelogic.h"

/**
 * Calculate wall slice dimensions for a given ray
 *
 * @param data       Game data
 * @param ray        The ray to calculate for
 * @param draw_start Output: Y coordinate to start drawing
 * @param draw_end   Output: Y coordinate to stop drawing
 * @return           Height of the wall slice in pixels
 */
static int	calculate_wall_slice(t_cub_data *data, t_ray *ray, int *draw_start,
		int *draw_end)
{
	int			line_height;
	t_fixed32	perp_dist;
	int			view_offset;

	perp_dist = ray->perp_dist;
	if (perp_dist < to_fixed32(0.001f))
		perp_dist = to_fixed32(0.001f);
	line_height = (int)from_fixed32(fixed32_div(to_fixed32(data->mlx->height),
				perp_dist));
	view_offset = data->player->view_offset + data->player->bob_offset;
	*draw_start = (data->mlx->height - line_height) / 2 + view_offset;
	*draw_end = (data->mlx->height + line_height) / 2 + view_offset;
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
static int	apply_shading(int color, float dist, int side)
{
	t_fixed32	shade;

	int r, g, b;
	shade = fixed32_sub(to_fixed32(1.0f), fixed32_div(to_fixed32(dist),
				to_fixed32(20.0f)));
	if (shade < to_fixed32(0.3f))
		shade = to_fixed32(0.3f);
	if (shade > to_fixed32(1.0f))
		shade = to_fixed32(1.0f);
	if (side == 1)
		shade = fixed32_mul(shade, to_fixed32(0.7f));
	r = (int)(((color >> 16) & 0xFF) * from_fixed32(shade));
	g = (int)(((color >> 8) & 0xFF) * from_fixed32(shade));
	b = (int)((color & 0xFF) * from_fixed32(shade));
	return ((r << 16) | (g << 8) | b);
}

/**
 * Get the texture for this wall face based on ray direction
 */
static t_texture	*get_wall_texture(t_ray *ray, t_textures *textures)
{
	if (ray->hit_content == 'D')
		return (NULL);
	if (ray->side == 0)
	{
		if (ray->step_x > 0)
			return (&textures->walls[TEX_WEST]);
		return (&textures->walls[TEX_EAST]);
	}
	else
	{
		if (ray->step_y > 0)
			return (&textures->walls[TEX_NORTH]);
		return (&textures->walls[TEX_SOUTH]);
	}
}

/**
 * Calculate texture X coordinate based on where ray hit the wall
 */
static int	calculate_texture_x(t_cub_data *data, t_ray *ray,
		t_texture *texture)
{
	t_fixed32	wall_x;
	t_fixed32	raw_dist;
	int			tex_x;

	if (!texture || !texture->loaded || texture->width == 0)
		return (0);
	// Use raw DDA distance (before fisheye correction) to find wall hit point
	if (ray->side == 0)
	{
		raw_dist = fixed32_sub(ray->side_dist_x, ray->delta_dist_x);
		wall_x = fixed32_add(data->player->y, fixed32_mul(raw_dist,
					ray->dir_y));
	}
	else
	{
		raw_dist = fixed32_sub(ray->side_dist_y, ray->delta_dist_y);
		wall_x = fixed32_add(data->player->x, fixed32_mul(raw_dist,
					ray->dir_x));
	}
	// Get fractional part (wall_x - floor(wall_x))
	wall_x = fixed32_sub(wall_x, to_fixed32((float)(int)from_fixed32(wall_x)));
	// Convert to texture coordinate
	tex_x = (int)from_fixed32(fixed32_mul(wall_x, to_fixed32(texture->width)));
	// Flip texture for certain faces to maintain consistency
	if ((ray->side == 0 && ray->dir_x > 0) || (ray->side == 1
			&& ray->dir_y < 0))
		tex_x = texture->width - tex_x - 1;
	// Bounds check
	if (tex_x < 0)
		tex_x = 0;
	if (tex_x >= texture->width)
		tex_x = texture->width - 1;
	return (tex_x);
}

/**
 * Draw a single vertical wall slice with texture mapping
 */
static void	draw_wall_slice(t_cub_data *data, int x, t_ray *ray)
{
	int			draw_start;
	int			draw_end;
	int			y;
	int			wall_color;
	int			shaded_color;
	t_texture	*texture;
	int			tex_x;
	int			tex_y;
	int			line_height;
	t_fixed32	step;
	t_fixed32	tex_pos;

	if (!ray->hit)
		return ;
	line_height = calculate_wall_slice(data, ray, &draw_start, &draw_end);
	// Get the texture for this wall
	texture = get_wall_texture(ray, data->textures);
	// If texture is loaded, use texture mapping
	if (texture && texture->loaded && texture->pixels)
	{
		// Calculate texture X coordinate
		tex_x = calculate_texture_x(data, ray, texture);
		// Calculate texture Y step per screen pixel (fixed-point)
		step = fixed32_div(to_fixed32(texture->height),
				to_fixed32(line_height));
		// Starting texture position (accounting for y clipping)
		tex_pos = fixed32_mul(to_fixed32(draw_start - (data->mlx->height
						- line_height) / 2 - data->player->view_offset
					- data->player->bob_offset), step);
		// Draw textured wall slice
		y = draw_start;
		while (y <= draw_end)
		{
			// Calculate texture Y coordinate
			tex_y = (int)from_fixed32(tex_pos);
			if (tex_y < 0)
				tex_y = 0;
			if (tex_y >= texture->height)
				tex_y = texture->height - 1;
			tex_pos = fixed32_add(tex_pos, step);
			// Get pixel from texture
			wall_color = texture->pixels[tex_y * texture->width + tex_x];
			// Apply shading
			shaded_color = apply_shading(wall_color,
					from_fixed32(ray->perp_dist), ray->side);
			mylx_pixel_put(data, x, y, shaded_color);
			y++;
		}
	}
	else
	{
		// Fallback to solid color if texture not loaded
		wall_color = get_wall_color(ray, data->textures);
		shaded_color = apply_shading(wall_color, from_fixed32(ray->perp_dist),
				ray->side);
		y = draw_start;
		while (y <= draw_end)
		{
			mylx_pixel_put(data, x, y, shaded_color);
			y++;
		}
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
 * Draw the door slice (transparent/animating)
 */
static void	draw_door_slice(t_cub_data *data, int x, t_ray *ray)
{
	int			line_height;
	int			frame_top;
	int			frame_bottom;
	int			render_top;
	int			render_bottom;
	int			view_offset;
	int			y;
	int			tex_x;
	int			tex_y;
	int			tex_color;
	t_door		*door;
	int			door_color;
	t_fixed32	player_pos;
	t_fixed32	map_pos;
	t_fixed32	step_val;
	t_fixed32	dir_val;
	t_fixed32	euclidean_dist;
	t_fixed32	p_dir_x;
	t_fixed32	p_dir_y;
	t_fixed32	r_dir_x;
	t_fixed32	r_dir_y;
	t_fixed32	dot_prod;
	t_fixed32	perp_dist;
	t_fixed32	dist;
	int			offset;
	int			draw_start;
	int			draw_end;
	t_fixed32	wall_x;
	int			draw_fallback;

	// 1. Calculate the static door frame dimensions (where a closed door would
	// be)
	// 1. Calculate relative geometrical distance to the door plane
	// This avoids reliance on DDA accumulators which might drift or have
	// precision issues Formula: (map - pos + (1 - step)/2) / dir
	if (ray->door_side == 0)
	{
		player_pos = data->player->x;
		map_pos = to_fixed32(ray->door_map_x);
		step_val = to_fixed32(ray->step_x);
		dir_val = ray->dir_x;
	}
	else
	{
		player_pos = data->player->y;
		map_pos = to_fixed32(ray->door_map_y);
		step_val = to_fixed32(ray->step_y);
		dir_val = ray->dir_y;
	}
	// Avoid div by zero
	if (dir_val == 0)
		dir_val = 1;
	// (map - pos + (1 - step)/2) / dir
	euclidean_dist = fixed32_div(fixed32_add(fixed32_sub(map_pos, player_pos),
				fixed32_div(fixed32_sub(to_fixed32(1.0f), step_val),
					to_fixed32(2.0f))), dir_val);
	// Fisheye Correction: dist * cos(angle_diff)
	p_dir_x = data->player->dir_x;
	p_dir_y = data->player->dir_y;
	r_dir_x = ray->dir_x;
	r_dir_y = ray->dir_y;
	dot_prod = fixed32_add(fixed32_mul(r_dir_x, p_dir_x), fixed32_mul(r_dir_y,
				p_dir_y));
	perp_dist = fixed32_mul(euclidean_dist, dot_prod);
	// Near clipping
	if (perp_dist < to_fixed32(0.1f))
		perp_dist = to_fixed32(0.1f);
	dist = perp_dist;
	line_height = (int)from_fixed32(fixed32_div(to_fixed32(data->mlx->height),
				dist));
	view_offset = data->player->view_offset + data->player->bob_offset;
	frame_top = (data->mlx->height - line_height) / 2 + view_offset;
	frame_bottom = (data->mlx->height + line_height) / 2 + view_offset;
	// Apply offset based on open_amount
	door = get_door_at(data, ray->door_map_x, ray->door_map_y);
	offset = 0;
	if (door)
		offset = (int)(line_height * door->open_amount); // Shift UP
	render_bottom = frame_bottom - offset;
	render_top = frame_top - offset;
	draw_start = render_top;
	draw_end = render_bottom;
	if (draw_start < frame_top)
		draw_start = frame_top;
	if (draw_start < 0)
		draw_start = 0;
	if (draw_end >= data->mlx->height)
		draw_end = data->mlx->height - 1;
	if (draw_start > draw_end)
		return ;
	// Texture X coordinate from exact hit point on door plane
	if (ray->door_side == 0)
		wall_x = fixed32_add(data->player->y, fixed32_mul(euclidean_dist,
					r_dir_y));
	else
		wall_x = fixed32_add(data->player->x, fixed32_mul(euclidean_dist,
					r_dir_x));
	wall_x = fixed32_sub(wall_x, to_fixed32((float)(int)from_fixed32(wall_x)));
	tex_x = (int)from_fixed32(fixed32_mul(wall_x,
				to_fixed32(data->textures->door.width)));
	if ((ray->door_side == 0 && r_dir_x > 0) || (ray->door_side == 1
			&& r_dir_y < 0))
		tex_x = data->textures->door.width - tex_x - 1;
	if (tex_x < 0)
		tex_x = 0;
	if (tex_x >= data->textures->door.width)
		tex_x = data->textures->door.width - 1;
	// 4. Draw
	door_color = 0x8B4513; // SaddleBrown
	door_color = apply_shading(door_color, from_fixed32(ray->door_dist),
			ray->door_side);
	y = draw_start;
	while (y <= draw_end)
	{
		draw_fallback = 0;
		if (data->textures && data->textures->door.loaded
			&& data->textures->door.width > 0
			&& data->textures->door.height > 0)
		{
			tex_y = ((y - render_top) * data->textures->door.height)
				/ line_height;
			if (tex_y < 0)
				tex_y = 0;
			if (tex_y >= data->textures->door.height)
				tex_y = data->textures->door.height - 1;
			tex_color = get_texture_pixel(&data->textures->door, tex_x, tex_y);
			// If pixel is fully transparent, use fallback color
			if ((unsigned int)tex_color == 0xFF000000U)
				draw_fallback = 1;
			else
			{
				tex_color = apply_shading(tex_color,
						from_fixed32(ray->door_dist), ray->door_side);
				mylx_pixel_put(data, x, y, tex_color);
			}
		}
		else
		{
			draw_fallback = 1;
		}
		if (draw_fallback)
		{
			mylx_pixel_put(data, x, y, door_color);
		}
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
	// Draw the wall or door
	if (ray->hit)
	{
		if (ray->hit_content == 'D')
			draw_door_slice(data, x, ray);
		else
			draw_wall_slice(data, x, ray);
	}
	// Draw floor below the wall
	draw_floor_slice(data, x, draw_end);
	// OVERLAY: Draw door if we hit one and it's not the main hit (i.e.,
		animating/partially open)
	if (ray->door_hit && ray->hit_content != 'D')
		draw_door_slice(data, x, ray);
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
