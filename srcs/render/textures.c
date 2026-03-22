/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 21:45:00 by joao              #+#    #+#             */
/*   Updated: 2026/03/22 17:17:01 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

/**
 * Load texture from XPM file using MLX
 * 
 * @param mlx_ptr	MLX instance pointer
 * @param texture	Texture structure to populate
 * @param path		Path to the XPM file
 * @return			0 on success, -1 on failure
 */
int	load_texture_from_file(void *mlx_ptr, t_texture *texture, char *path)
{
	t_img_data	img_data;
	int			x;
	int			y;
	
	if (!mlx_ptr || !texture || !path)
		return (-1);
	
	// Load XPM file into MLX image
	img_data.img = mlx_xpm_file_to_image(mlx_ptr, path, 
										  &texture->width, &texture->height);
	if (!img_data.img)
	{
		ft_printf_fd(STDERR_FILENO, "Error: Failed to load texture: %s\n", path);
		return (-1);
	}
	
	// Get pixel data address
	img_data.address = mlx_get_data_addr(img_data.img, 
										 &img_data.bits_per_pixel,
										 &img_data.line_length, 
										 &img_data.endian);
	if (!img_data.address)
	{
		mlx_destroy_image(mlx_ptr, img_data.img);
		return (-1);
	}
	
	// Allocate pixel array
	texture->pixels = (int *)malloc(sizeof(int) * texture->width * texture->height);
	if (!texture->pixels)
	{
		mlx_destroy_image(mlx_ptr, img_data.img);
		return (-1);
	}
	
	// Copy pixel data to our array for faster access
	y = 0;
	while (y < texture->height)
	{
		x = 0;
		while (x < texture->width)
		{
			texture->pixels[y * texture->width + x] = 
				*(int *)(img_data.address + (y * img_data.line_length + 
				x * (img_data.bits_per_pixel / 8)));
			x++;
		}
		y++;
	}
	
	// Store the image pointer for cleanup later
	texture->img = img_data.img;
	texture->loaded = true;
	
	ft_printf("Loaded texture: %s (%dx%d)\n", path, texture->width, texture->height);
	return (0);
}

/**
 * Get pixel color from texture with bounds checking
 */
int	get_texture_pixel(t_texture *texture, int x, int y)
{
	if (!texture || !texture->loaded || !texture->pixels)
		return (texture->color);
	
	// Bounds checking
	if (x < 0 || x >= texture->width || y < 0 || y >= texture->height)
		return (texture->color);
	
	return (texture->pixels[y * texture->width + x]);
}

/**
 * Load all wall textures from their paths
 */
int	load_all_textures(t_cub_data *data)
{
	int	i;
	
	if (!data || !data->mlx || !data->mlx->mlx_ptr || !data->textures)
		return (-1);
	
	// Load wall textures
	i = 0;
	while (i < 4)
	{
		if (data->textures->walls[i].path)
		{
			if (load_texture_from_file(data->mlx->mlx_ptr, 
									   &data->textures->walls[i],
									   data->textures->walls[i].path) != 0)
			{
				ft_printf_fd(STDERR_FILENO, 
							 "Error: Failed to load wall texture %d\n", i);
				return (-1);
			}
		}
		i++;
	}

	if (load_texture_from_file(data->mlx->mlx_ptr, &data->textures->door,
			"./assets/textures/_DOOR_.xpm") != 0)
		ft_printf_fd(STDERR_FILENO,
			"Warning: Failed to load ./assets/textures/_DOOR_.xpm\n");
	if (load_texture_from_file(data->mlx->mlx_ptr, &data->textures->ammo,
			"./assets/textures/_AMMO_.xpm") != 0)
		ft_printf_fd(STDERR_FILENO,
			"Warning: Failed to load ./assets/textures/_AMMO_.xpm\n");
	if (load_texture_from_file(data->mlx->mlx_ptr, &data->textures->demon,
			"./assets/textures/_DEMON_.xpm") != 0)
		ft_printf_fd(STDERR_FILENO,
			"Warning: Failed to load ./assets/textures/_DEMON_.xpm\n");
	if (load_texture_from_file(data->mlx->mlx_ptr, &data->textures->gun_pov,
			"./assets/textures/_GUN_POV_.xpm") != 0)
		ft_printf_fd(STDERR_FILENO,
			"Warning: Failed to load ./assets/textures/_GUN_POV_.xpm\n");
	
	ft_printf("All textures loaded successfully!\n");
	return (0);
}
