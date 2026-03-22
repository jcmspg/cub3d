/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_render.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 20:00:00 by joamiran          #+#    #+#             */
/*   Updated: 2026/03/22 17:29:58 by joamiran         ###   ########.fr       */
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
static int calculate_wall_slice(t_cub_data *data, t_ray *ray, int *draw_start,
                                int *draw_end) {
  int line_height;
  float perp_dist;
  int view_offset;

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
static int apply_shading(int color, float dist, int side) {
  int r;
  int g;
  int b;
  float shade;

  // Distance-based shading (farther = darker)
  shade = 1.0f - (dist / 20.0f);
  if (shade < 0.3f)
    shade = 0.3f;
  if (shade > 1.0f)
    shade = 1.0f;
  // Side shading: horizontal walls slightly darker
  if (side == 1)
    shade *= 0.7f;
  // Apply shade to color components
  r = (int)(((color >> 16) & 0xFF) * shade);
  g = (int)(((color >> 8) & 0xFF) * shade);
  b = (int)((color & 0xFF) * shade);
  return ((r << 16) | (g << 8) | b);
}

/**
 * Get the texture for this wall face based on ray direction
 */
static t_texture *get_wall_texture(t_ray *ray, t_textures *textures)
{
	if (ray->hit_content == 'D')
		return (NULL);  // Doors handled separately
	
	if (ray->side == 0)
	{
		// Vertical wall (E/W face)
		if (ray->step_x > 0)
			return (&textures->walls[TEX_WEST]);
		else
			return (&textures->walls[TEX_EAST]);
	}
	else
	{
		// Horizontal wall (N/S face)
		if (ray->step_y > 0)
			return (&textures->walls[TEX_NORTH]);
		else
			return (&textures->walls[TEX_SOUTH]);
	}
}

/**
 * Calculate texture X coordinate based on where ray hit the wall
 */
static int calculate_texture_x(t_cub_data *data, t_ray *ray, t_texture *texture)
{
	float wall_x;
	float raw_dist;
	int tex_x;
	
	if (!texture || !texture->loaded || texture->width == 0)
		return (0);
	
	// Use raw DDA distance (before fisheye correction) to find wall hit point
	if (ray->side == 0)
	{
		raw_dist = from_fixed32(fixed32_sub(ray->side_dist_x, ray->delta_dist_x));
		wall_x = from_fixed32(data->player->y) + raw_dist * from_fixed32(ray->dir_y);
	}
	else
	{
		raw_dist = from_fixed32(fixed32_sub(ray->side_dist_y, ray->delta_dist_y));
		wall_x = from_fixed32(data->player->x) + raw_dist * from_fixed32(ray->dir_x);
	}
	
	// Get fractional part
	wall_x -= floor(wall_x);
	
	// Convert to texture coordinate
	tex_x = (int)(wall_x * (float)texture->width);
	
	// Flip texture for certain faces to maintain consistency
	if ((ray->side == 0 && ray->dir_x > 0) || (ray->side == 1 && ray->dir_y < 0))
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
static void draw_wall_slice(t_cub_data *data, int x, t_ray *ray) {
	int draw_start;
	int draw_end;
	int y;
	int wall_color;
	int shaded_color;
	t_texture *texture;
	int tex_x;
	int tex_y;
	int line_height;
	float step;
	float tex_pos;
	
	if (!ray->hit)
		return;
	
	line_height = calculate_wall_slice(data, ray, &draw_start, &draw_end);
	
	// Get the texture for this wall
	texture = get_wall_texture(ray, data->textures);
	
	// If texture is loaded, use texture mapping
	if (texture && texture->loaded && texture->pixels)
	{
		// Calculate texture X coordinate
		tex_x = calculate_texture_x(data, ray, texture);
		
		// Calculate texture Y step per screen pixel
		step = (float)texture->height / (float)line_height;
		
		// Starting texture position (accounting for y clipping)
		tex_pos = (draw_start - (data->mlx->height - line_height) / 2 - 
				   data->player->view_offset - data->player->bob_offset) * step;
		
		// Draw textured wall slice
		y = draw_start;
		while (y <= draw_end)
		{
			// Calculate texture Y coordinate
			tex_y = (int)tex_pos;
			if (tex_y < 0)
				tex_y = 0;
			if (tex_y >= texture->height)
				tex_y = texture->height - 1;
			
			tex_pos += step;
			
			// Get pixel from texture
			wall_color = texture->pixels[tex_y * texture->width + tex_x];
			
			// Apply shading
			shaded_color = apply_shading(wall_color, from_fixed32(ray->perp_dist), ray->side);
			
			mylx_pixel_put(data, x, y, shaded_color);
			y++;
		}
	}
	else
	{
		// Fallback to solid color if texture not loaded
		wall_color = get_wall_color(ray, data->textures);
		shaded_color = apply_shading(wall_color, from_fixed32(ray->perp_dist), ray->side);
		
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
static void draw_ceiling_slice(t_cub_data *data, int x, int wall_start) {
  int y;
  int color;

  color = data->textures->ceiling_color;
  y = 0;
  while (y < wall_start) {
    mylx_pixel_put(data, x, y, color);
    y++;
  }
}

/**
 * Draw floor for a column
 */
static void draw_floor_slice(t_cub_data *data, int x, int wall_end) {
  int y;
  int color;

  color = data->textures->floor_color;
  y = wall_end + 1;
  while (y < data->mlx->height) {
    mylx_pixel_put(data, x, y, color);
    y++;
  }
}

/**
 * Draw the door slice (transparent/animating)
 */
static void draw_door_slice(t_cub_data *data, int x, t_ray *ray) {
  int line_height;
  int frame_top;
  int frame_bottom;
  int render_top;
  int render_bottom;
  int view_offset;
  int y;
  int tex_x;
  int tex_y;
  int tex_color;
  t_door *door;
  int door_color;


  // 1. Calculate the static door frame dimensions (where a closed door would
  // be)
  // 1. Calculate relative geometrical distance to the door plane
  // This avoids reliance on DDA accumulators which might drift or have
  // precision issues Formula: (map - pos + (1 - step)/2) / dir
  float player_pos;
  float map_pos;
  float step_val;
  float dir_val;
  float euclidean_dist;

  if (ray->door_side == 0) {
    player_pos = from_fixed32(data->player->x);
    map_pos = (float)ray->door_map_x;
    step_val = (float)ray->step_x;
    dir_val = from_fixed32(ray->dir_x);
  } else {
    player_pos = from_fixed32(data->player->y);
    map_pos = (float)ray->door_map_y;
    step_val = (float)ray->step_y;
    dir_val = from_fixed32(ray->dir_y);
  }

  // Avoid div by zero
  if (fabs(dir_val) < 0.00001f)
    dir_val = 0.00001f;

  euclidean_dist = (map_pos - player_pos + (1.0f - step_val) / 2.0f) / dir_val;

  // 2. Apply Fisheye Correction: dist * cos(angle_diff)
  // We can re-derive cos(angle) via dot product of ray_dir and player_dir
  float p_dir_x = from_fixed32(data->player->dir_x);
  float p_dir_y = from_fixed32(data->player->dir_y);
  float r_dir_x = from_fixed32(ray->dir_x);
  float r_dir_y = from_fixed32(ray->dir_y);

  float dot_prod = r_dir_x * p_dir_x + r_dir_y * p_dir_y;
  float perp_dist = euclidean_dist * dot_prod;

  // 3. Near clipping
  if (perp_dist < 0.1f)
    perp_dist = 0.1f;

  float dist = perp_dist;

  line_height = (int)(data->mlx->height / dist);
  view_offset = data->player->view_offset + data->player->bob_offset;

  frame_top = (data->mlx->height - line_height) / 2 + view_offset;
  frame_bottom = (data->mlx->height + line_height) / 2 + view_offset;

  // 2. Apply offset based on open_amount
  door = get_door_at(data, ray->door_map_x, ray->door_map_y);
  int offset = 0;
  if (door)
    offset = (int)(line_height * door->open_amount); // Shift UP

  // The door moves UP.
  // The bottom of the door moves from frame_bottom to (frame_bottom - offset).
  // The top of the door moves from frame_top to (frame_top - offset).
  render_bottom = frame_bottom - offset;
  render_top = frame_top - offset;

  // 3. Clip to the door frame TOP (Strictly slide INTO the ceiling)
  // We only draw pixels that are BELOW the frame_top.
  // Actually, wait, if we are inside the door frame, we draw.
  // The door is solid.
  // If we shift it up, the top part goes above frame_top. We should CLIP that.
  // The bottom part rises. We draw from the new bottom up to the CLIPPED top.
  int draw_start = render_top;
  int draw_end = render_bottom;

  // Clip top against the original frame top
  if (draw_start < frame_top)
    draw_start = frame_top;

  // Also clip against screen bounds
  if (draw_start < 0)
    draw_start = 0;
  if (draw_end >= data->mlx->height)
    draw_end = data->mlx->height - 1;

  // If fully open, draw_start might coincide with draw_end or cross it.
  if (draw_start > draw_end)
    return;

  // Texture X coordinate from exact hit point on door plane
  float wall_x;
  if (ray->door_side == 0)
    wall_x = from_fixed32(data->player->y) + euclidean_dist * r_dir_y;
  else
    wall_x = from_fixed32(data->player->x) + euclidean_dist * r_dir_x;
  wall_x -= floor(wall_x);

  tex_x = 0;
  if (data->textures && data->textures->door.loaded &&
      data->textures->door.width > 0) {
    tex_x = (int)(wall_x * (float)data->textures->door.width);
    if ((ray->door_side == 0 && r_dir_x > 0) ||
        (ray->door_side == 1 && r_dir_y < 0))
      tex_x = data->textures->door.width - tex_x - 1;
    if (tex_x < 0)
      tex_x = 0;
    if (tex_x >= data->textures->door.width)
      tex_x = data->textures->door.width - 1;
  }

  // 4. Draw
  door_color = 0x8B4513; // SaddleBrown
  door_color =
      apply_shading(door_color, from_fixed32(ray->door_dist), ray->door_side);

  y = draw_start;
  while (y <= draw_end) {
    int draw_fallback = 0;
    if (data->textures && data->textures->door.loaded &&
        data->textures->door.width > 0 && data->textures->door.height > 0) {
      tex_y = ((y - render_top) * data->textures->door.height) / line_height;
      if (tex_y < 0)
        tex_y = 0;
      if (tex_y >= data->textures->door.height)
        tex_y = data->textures->door.height - 1;
      tex_color = get_texture_pixel(&data->textures->door, tex_x, tex_y);
      // If pixel is fully transparent, use fallback color
      if ((unsigned int)tex_color == 0xFF000000U)
        draw_fallback = 1;
      else {
        tex_color = apply_shading(tex_color, from_fixed32(ray->door_dist), ray->door_side);
        mylx_pixel_put(data, x, y, tex_color);
      }
    } else {
      draw_fallback = 1;
    }
    if (draw_fallback) {
      mylx_pixel_put(data, x, y, door_color);
    }
    y++;
  }
}

/**
 * Render a complete column (ceiling + wall + floor)
 */
static void render_column(t_cub_data *data, int x, t_ray *ray) {
  int draw_start;
  int draw_end;

  if (!ray->hit) {
    draw_start = data->mlx->height / 2;
    draw_end = data->mlx->height / 2;
  } else
    calculate_wall_slice(data, ray, &draw_start, &draw_end);
  // Draw ceiling above the wall
  draw_ceiling_slice(data, x, draw_start);
  // Draw the wall or door
  if (ray->hit) {
    if (ray->hit_content == 'D')
      draw_door_slice(data, x, ray);
    else
      draw_wall_slice(data, x, ray);
  }
  // Draw floor below the wall
  draw_floor_slice(data, x, draw_end);

  // OVERLAY: Draw door if we hit one and it's not the main hit (i.e., animating/partially open)
  if (ray->door_hit && ray->hit_content != 'D')
    draw_door_slice(data, x, ray);
}

/**
 * Render all walls based on raycasting data
 * Call this after cast_all_rays()
 */
void render_walls(t_cub_data *data) {
  int x;

  if (!data || !data->raycasting || !data->raycasting->rays)
    return;
  if (!data->textures)
    return;
  x = 0;
  while (x < data->raycasting->num_rays) {
    render_column(data, x, &data->raycasting->rays[x]);
    x++;
  }
}
