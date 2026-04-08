/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 20:00:00 by joamiran          #+#    #+#             */
/*   Updated: 2026/04/08 01:31:06 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

/**
 * Initialize a single texture with default color
 */
static void	init_single_texture(t_texture *tex, int color)
{
	tex->color = color;
	tex->img = NULL;
	tex->pixels = NULL;
	tex->width = 0;
	tex->height = 0;
	tex->path = NULL;
	tex->loaded = false;
}

static void	init_wall_textures(t_textures *textures)
{
	init_single_texture(&textures->walls[TEX_NORTH], COLOR_NORTH);
	init_single_texture(&textures->walls[TEX_SOUTH], COLOR_SOUTH);
	init_single_texture(&textures->walls[TEX_EAST], COLOR_EAST);
	init_single_texture(&textures->walls[TEX_WEST], COLOR_WEST);
}

static void	init_misc_textures(t_textures *textures)
{
	init_single_texture(&textures->floor, COLOR_FLOOR);
	init_single_texture(&textures->ceiling, COLOR_CEILING);
	init_single_texture(&textures->door, 0x8B4513);
	init_single_texture(&textures->ammo, 0xFFD700);
	init_single_texture(&textures->demon, 0xFF0000);
	init_single_texture(&textures->gun_pov, 0x555555);
	textures->floor_color = COLOR_FLOOR;
	textures->ceiling_color = COLOR_CEILING;
}

/**
 * Initialize all textures with default colors
 * These will be replaced when actual textures are loaded from .cub file
 */
t_textures	*init_textures(void)
{
	t_textures	*textures;

	textures = ft_calloc(1, sizeof(t_textures));
	if (!textures)
		return (NULL);
	init_wall_textures(textures);
	init_misc_textures(textures);
	return (textures);
}
