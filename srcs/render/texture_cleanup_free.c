/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_cleanup_free.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 01:31:39 by hladeiro          #+#    #+#             */
/*   Updated: 2026/04/08 01:31:39 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static void	free_texture_paths(t_textures *textures)
{
	if (textures->floor.path)
		free(textures->floor.path);
	if (textures->ceiling.path)
		free(textures->ceiling.path);
	if (textures->door.path)
		free(textures->door.path);
	if (textures->ammo.path)
		free(textures->ammo.path);
	if (textures->demon.path)
		free(textures->demon.path);
	if (textures->gun_pov.path)
		free(textures->gun_pov.path);
}

static void	free_texture_pixels(t_textures *textures)
{
	if (textures->floor.pixels)
		free(textures->floor.pixels);
	if (textures->ceiling.pixels)
		free(textures->ceiling.pixels);
	if (textures->door.pixels)
		free(textures->door.pixels);
	if (textures->ammo.pixels)
		free(textures->ammo.pixels);
	if (textures->demon.pixels)
		free(textures->demon.pixels);
	if (textures->gun_pov.pixels)
		free(textures->gun_pov.pixels);
}

void	free_textures(t_textures *textures)
{
	int	i;

	if (!textures)
		return ;
	i = 0;
	while (i < 4)
	{
		if (textures->walls[i].path)
			free(textures->walls[i].path);
		if (textures->walls[i].pixels)
			free(textures->walls[i].pixels);
		i++;
	}
	free_texture_paths(textures);
	free_texture_pixels(textures);
	free(textures);
}
