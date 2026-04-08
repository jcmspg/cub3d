/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_cleanup_mlx.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 01:31:42 by hladeiro          #+#    #+#             */
/*   Updated: 2026/04/08 01:31:42 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static void	destroy_wall_images(t_cub_data *data)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		if (data->textures->walls[i].img)
		{
			mlx_destroy_image(data->mlx->mlx_ptr, data->textures->walls[i].img);
			data->textures->walls[i].img = NULL;
		}
		i++;
	}
}

static void	destroy_texture_image(t_cub_data *data, t_texture *texture)
{
	if (texture->img)
	{
		mlx_destroy_image(data->mlx->mlx_ptr, texture->img);
		texture->img = NULL;
	}
}

void	cleanup_textures_mlx(t_cub_data *data)
{
	if (!data || !data->textures || !data->mlx || !data->mlx->mlx_ptr)
		return ;
	destroy_wall_images(data);
	destroy_texture_image(data, &data->textures->floor);
	destroy_texture_image(data, &data->textures->ceiling);
	destroy_texture_image(data, &data->textures->door);
	destroy_texture_image(data, &data->textures->ammo);
	destroy_texture_image(data, &data->textures->demon);
	destroy_texture_image(data, &data->textures->gun_pov);
}
