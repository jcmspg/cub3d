/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 20:00:00 by joamiran          #+#    #+#             */
/*   Updated: 2026/02/23 03:30:10 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

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

int	init_textures(t_cub_data *data)
{
	data->textures = ft_calloc(1, sizeof(t_textures));
	if (!data->textures)
		return (1);
	init_single_texture(&data->textures->walls[TEX_NORTH], COLOR_NORTH);
	init_single_texture(&data->textures->walls[TEX_SOUTH], COLOR_SOUTH);
	init_single_texture(&data->textures->walls[TEX_EAST], COLOR_EAST);
	init_single_texture(&data->textures->walls[TEX_WEST], COLOR_WEST);
	init_single_texture(&data->textures->floor, COLOR_FLOOR);
	init_single_texture(&data->textures->ceiling, COLOR_CEILING);
	data->textures->floor_color = COLOR_FLOOR;
	data->textures->ceiling_color = COLOR_CEILING;
	return (0);
}

int	get_wall_color(t_ray *ray, t_textures *textures)
{
	if (ray->hit_content == 'D')
		return (0x8B4513);
	if (ray->side == 0)
	{
		if (ray->step_x > 0)
			return (textures->walls[TEX_WEST].color);
		else
			return (textures->walls[TEX_EAST].color);
	}
	else
	{
		if (ray->step_y > 0)
			return (textures->walls[TEX_NORTH].color);
		else
			return (textures->walls[TEX_SOUTH].color);
	}
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
		i++;
	}
	if (textures->floor.path)
		free(textures->floor.path);
	if (textures->ceiling.path)
		free(textures->ceiling.path);
	free(textures);
}
