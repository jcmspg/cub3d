/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 20:00:00 by joamiran          #+#    #+#             */
/*   Updated: 2026/01/24 19:48:31 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

/**
 * Initialize a single texture with default color
 */
static void init_single_texture(t_texture *tex, int color) {
  tex->color = color;
  tex->img = NULL;
  tex->pixels = NULL;
  tex->width = 0;
  tex->height = 0;
  tex->path = NULL;
  tex->loaded = false;
}

/**
 * Initialize all textures with default colors
 * These will be replaced when actual textures are loaded from .cub file
 */
t_textures *init_textures(void) {
  t_textures *textures;

  textures = ft_calloc(1, sizeof(t_textures));
  if (!textures)
    return (NULL);
  // Initialize wall textures with default colors
  init_single_texture(&textures->walls[TEX_NORTH], COLOR_NORTH);
  init_single_texture(&textures->walls[TEX_SOUTH], COLOR_SOUTH);
  init_single_texture(&textures->walls[TEX_EAST], COLOR_EAST);
  init_single_texture(&textures->walls[TEX_WEST], COLOR_WEST);
  // Initialize floor and ceiling
  init_single_texture(&textures->floor, COLOR_FLOOR);
  init_single_texture(&textures->ceiling, COLOR_CEILING);
  // Default parsed colors (can be overridden by .cub file)
  textures->floor_color = COLOR_FLOOR;
  textures->ceiling_color = COLOR_CEILING;
  return (textures);
}

/**
 * Get the wall color based on ray hit side and direction
 *
 * @param ray       The ray that hit the wall
 * @param textures  Texture configuration
 * @return          Color for this wall
 *
 * Note: When a ray going EAST hits a wall, we see the WEST face of that wall!
 * The wall face is opposite to the ray direction.
 */
int get_wall_color(t_ray *ray, t_textures *textures) {
  // side == 0 means we hit a vertical wall (E/W face)
  // side == 1 means we hit a horizontal wall (N/S face)

  if (ray->hit_content == 'D')
    return (0x8B4513); // Door color (SaddleBrown)

  if (ray->side == 0) {
    // Hit vertical wall - ray going East sees West face, ray going West sees
    // East face
    if (ray->step_x > 0)
      return (textures->walls[TEX_WEST].color);
    else
      return (textures->walls[TEX_EAST].color);
  } else {
    // Hit horizontal wall - ray going South sees North face, ray going North
    // sees South face
    if (ray->step_y > 0)
      return (textures->walls[TEX_NORTH].color);
    else
      return (textures->walls[TEX_SOUTH].color);
  }
}

/**
 * Free all texture resources
 */
void free_textures(t_textures *textures) {
  int i;

  if (!textures)
    return;
  // Free wall texture resources
  i = 0;
  while (i < 4) {
    if (textures->walls[i].path)
      free(textures->walls[i].path);
    if (textures->walls[i].pixels)
      free(textures->walls[i].pixels);
    // Note: img destruction should be handled by cleanup_textures_mlx
    i++;
  }
  if (textures->floor.path)
    free(textures->floor.path);
  if (textures->ceiling.path)
    free(textures->ceiling.path);
  free(textures);
}

/**
 * Destroy MLX texture images
 * Call this before free_textures and before destroying MLX
 */
void cleanup_textures_mlx(t_cub_data *data) {
  int i;

  if (!data || !data->textures || !data->mlx || !data->mlx->mlx_ptr)
    return;
  
  // Destroy wall texture images
  i = 0;
  while (i < 4) {
    if (data->textures->walls[i].img) {
      mlx_destroy_image(data->mlx->mlx_ptr, data->textures->walls[i].img);
      data->textures->walls[i].img = NULL;
    }
    i++;
  }
  
  // Destroy floor/ceiling texture images if any
  if (data->textures->floor.img) {
    mlx_destroy_image(data->mlx->mlx_ptr, data->textures->floor.img);
    data->textures->floor.img = NULL;
  }
  if (data->textures->ceiling.img) {
    mlx_destroy_image(data->mlx->mlx_ptr, data->textures->ceiling.img);
    data->textures->ceiling.img = NULL;
  }
}
