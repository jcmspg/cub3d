/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 19:46:00 by joamiran          #+#    #+#             */
/*   Updated: 2026/04/08 01:49:10 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RENDER_H
# define RENDER_H

# include "cub3d.h"

/*
** =============================================================================
** RAY INITIALIZATION (ray_init.c)
** =============================================================================
*/

// Allocate raycasting structure and ray array
t_raycasting	*init_raycasting(int screen_width);

// Reset a single ray before casting
void			reset_ray(t_ray *ray);

// Reset all rays before a new frame
void			reset_all_rays(t_raycasting *rc);

// Free raycasting structure
void			free_raycasting(t_raycasting *rc);

/*
** =============================================================================
** RAY CASTING - DDA Algorithm (ray_cast.c)
** =============================================================================
*/

// Calculate ray direction for a screen column
void			calculate_ray_dir(t_cub_data *data, t_ray *ray, int x);

// Initialize DDA parameters (step directions, side distances)
void			init_dda(t_cub_data *data, t_ray *ray);

// Perform DDA stepping until wall hit
int				perform_dda(t_cub_data *data, t_ray *ray);

// Calculate perpendicular distance (no fish-eye)
void			calculate_perp_distance(t_cub_data *data, t_ray *ray);

// Cast a single ray
void			cast_single_ray(t_cub_data *data, t_ray *ray, int x);

// Cast all rays for the frame
void			cast_all_rays(t_cub_data *data);

/*
** =============================================================================
** RAY DEBUG VISUALIZATION (ray_debug.c)
** =============================================================================
*/

// Draw all rays on the minimap (full FOV fan)
void			draw_rays_on_minimap(t_cub_data *data);

// Convert world fixed coordinates to minimap screen coordinates
void			world_to_minimap(t_fixed32 world_x, t_fixed32 world_y,
					int *screen_x, int *screen_y);

// Draw a line on screen (used by minimap ray debug)
void			draw_line(t_cub_data *data, t_ray_line *line);

// Complete ray debug visualization (call after cast_all_rays)
void			draw_ray_debug(t_cub_data *data);

/*
** =============================================================================
** RAYCASTING MAIN ENTRY POINT (raycasting.c)
** =============================================================================
*/

// Main raycasting function called each frame
void			start_rays(t_cub_data *data);

/*
** =============================================================================
** TEXTURE INITIALIZATION (texture_init.c)
** =============================================================================
*/

// Initialize all textures with default colors
t_textures		*init_textures(void);

// Get wall color based on ray direction
int				get_wall_color(t_ray *ray, t_textures *textures);

// Free all texture resources
void			free_textures(t_textures *textures);

// Cleanup texture MLX images
void			cleanup_textures_mlx(t_cub_data *data);

/*
** =============================================================================
** TEXTURE LOADING (textures.c)
** =============================================================================
*/

// Load a texture from an XPM file
int				load_texture_from_file(void *mlx_ptr, t_texture *texture,
					char *path);

// Get a pixel color from a texture
int				get_texture_pixel(t_texture *texture, int x, int y);

// Load all wall textures
int				load_all_textures(t_cub_data *data);

/*
** =============================================================================
** WALL RENDERING (ray_render.c)
** =============================================================================
*/

// Render all walls (ceiling + walls + floor) for the frame
void			render_walls(t_cub_data *data);

// Render all sprites (ammo, etc.)
void			render_sprites(t_cub_data *data);

// Render placeholder weapon (rect)
void			render_weapon(t_cub_data *data);

#endif /* RENDER_H */