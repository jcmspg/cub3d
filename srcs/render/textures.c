/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 21:45:00 by joao              #+#    #+#             */
/*   Updated: 2026/04/06 19:45:00 by copilot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	load_texture_from_file(void *mlx_ptr, t_texture *texture, char *path);

static int	load_wall_textures(t_cub_data *data)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		if (data->textures->walls[i].path && load_texture_from_file(
					data->mlx->mlx_ptr, &data->textures->walls[i],
					data->textures->walls[i].path) != 0)
			return (-1);
		i++;
	}
	return (0);
}

static void	load_optional_textures(t_cub_data *data)
{
	load_texture_from_file(data->mlx->mlx_ptr, &data->textures->door,
		"./assets/textures/_DOOR_.xpm");
	load_texture_from_file(data->mlx->mlx_ptr, &data->textures->ammo,
		"./assets/textures/_AMMO_.xpm");
	load_texture_from_file(data->mlx->mlx_ptr, &data->textures->demon,
		"./assets/textures/_DEMON_.xpm");
	load_texture_from_file(data->mlx->mlx_ptr, &data->textures->gun_pov,
		"./assets/textures/_GUN_POV_.xpm");
}

static int	copy_texture_pixels(t_texture *texture, t_img_data *img_data)
{
	int	x;
	int	y;

	y = 0;
	while (y < texture->height)
	{
		x = 0;
		while (x < texture->width)
		{
			texture->pixels[y * texture->width + x] = *(int *)(img_data->address
					+ (y * img_data->line_length
						+ x * (img_data->bits_per_pixel / 8)));
			x++;
		}
		y++;
	}
	return (0);
}

int	load_texture_from_file(void *mlx_ptr, t_texture *texture, char *path)
{
	t_img_data	img_data;

	if (!mlx_ptr || !texture || !path)
		return (-1);
	img_data.img = mlx_xpm_file_to_image(mlx_ptr, path, &texture->width,
			&texture->height);
	if (!img_data.img)
		return (-1);
	img_data.address = mlx_get_data_addr(img_data.img, &img_data.bits_per_pixel,
			&img_data.line_length, &img_data.endian);
	if (!img_data.address)
		return (mlx_destroy_image(mlx_ptr, img_data.img), -1);
	texture->pixels = (int *)malloc(sizeof(int) * texture->width
			* texture->height);
	if (!texture->pixels)
		return (mlx_destroy_image(mlx_ptr, img_data.img), -1);
	copy_texture_pixels(texture, &img_data);
	texture->img = img_data.img;
	texture->loaded = true;
	return (0);
}

int	get_texture_pixel(t_texture *texture, int x, int y)
{
	if (!texture || !texture->loaded || !texture->pixels)
		return (texture->color);
	if (x < 0 || x >= texture->width || y < 0 || y >= texture->height)
		return (texture->color);
	return (texture->pixels[y * texture->width + x]);
}

int	load_all_textures(t_cub_data *data)
{
	if (!data || !data->mlx || !data->mlx->mlx_ptr || !data->textures)
		return (-1);
	if (load_wall_textures(data) != 0)
		return (-1);
	load_optional_textures(data);
	return (0);
}
