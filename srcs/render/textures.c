/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao <joao@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 21:45:00 by joao              #+#    #+#             */
/*   Updated: 2025/07/20 21:45:00 by joao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

#define DEFAULT_WALL_TEXTURE_PATH "assets/wall_north.xpm"

static int	load_texture(t_cub_data *data, t_texture *texture, const char *path)
{
	if (!data || !data->mlx || !data->mlx->mlx_ptr || !texture || !path)
		return (ERR_TEXTURE_LOAD);
	texture->img = mlx_xpm_file_to_image(data->mlx->mlx_ptr, (char *)path,
			&texture->width, &texture->height);
	if (!texture->img)
		return (ERR_TEXTURE_LOAD);
	texture->address = mlx_get_data_addr(texture->img, &texture->bits_per_pixel,
			&texture->line_length, &texture->endian);
	if (!texture->address)
	{
		mlx_destroy_image(data->mlx->mlx_ptr, texture->img);
		texture->img = NULL;
		return (ERR_TEXTURE_LOAD);
	}
	return (ERR_NO_ERROR);
}

int	init_textures(t_cub_data *data)
{
	int	status;

	if (!data || !data->mlx || !data->mlx->mlx_ptr)
		return (ERR_TEXTURE_LOAD);
	data->textures = ft_calloc(1, sizeof(t_texture));
	if (!data->textures)
		return (ERR_MEMORY_ALLOCATION);
	status = load_texture(data, &data->textures[0], DEFAULT_WALL_TEXTURE_PATH);
	if (status != ERR_NO_ERROR)
	{
		free(data->textures);
		data->textures = NULL;
		return (status);
	}
	return (ERR_NO_ERROR);
}

static int	get_texture_pixel(t_texture *texture, int x, int y)
{
	char	*pixel;

	if (!texture || !texture->address || texture->width <= 0
		|| texture->height <= 0)
		return (0);
	if (x < 0)
		x = 0;
	if (y < 0)
		y = 0;
	if (x >= texture->width)
		x = texture->width - 1;
	if (y >= texture->height)
		y = texture->height - 1;
	pixel = texture->address + (y * texture->line_length
			+ x * (texture->bits_per_pixel / 8));
	return (*(unsigned int *)pixel);
}

static void	apply_wall_texture(t_cub_data *data, int x, int wall_start,
		int wall_end, t_ray *ray, t_texture *texture)
{
	double	wall_x;
	double	step;
	double	tex_pos;
	int		tex_x;
	int		tex_y;
	int		y;

	if (!data || !ray || !texture || !texture->img)
		return ;
	if (ray->side == 0)
		wall_x = from_fixed32(ray->hit_y);
	else
		wall_x = from_fixed32(ray->hit_x);
	wall_x -= floor(wall_x);
	tex_x = (int)(wall_x * (double)texture->width);
	if (tex_x < 0)
		tex_x = 0;
	if (tex_x >= texture->width)
		tex_x = texture->width - 1;
	if (wall_start < 0)
		wall_start = 0;
	if (wall_end >= data->mlx->height)
		wall_end = data->mlx->height - 1;
	step = (double)texture->height / (double)(wall_end - wall_start + 1);
	tex_pos = 0.0;
	y = wall_start;
	while (y <= wall_end)
	{
		tex_y = (int)tex_pos;
		mylx_pixel_put(data, x, y, get_texture_pixel(texture, tex_x, tex_y));
		tex_pos += step;
		y++;
	}
}

void	draw_walls_textured(t_cub_data *data)
{
	int		x;
	int		max_x;
	double	distance;
	int		wall_height;
	int		wall_start;
	int		wall_end;

	if (!data || !data->mlx || !data->raycasting || !data->raycasting->rays
		|| !data->textures || !data->textures[0].img)
		return ;
	max_x = data->mlx->width;
	if (data->raycasting->num_rays < max_x)
		max_x = data->raycasting->num_rays;
	x = 0;
	while (x < max_x)
	{
		distance = from_fixed32(data->raycasting->rays[x].distance);
		if (distance > 0.0)
		{
			wall_height = (int)((double)data->mlx->height / distance);
			wall_start = (data->mlx->height - wall_height) / 2;
			wall_end = wall_start + wall_height - 1;
			apply_wall_texture(data, x, wall_start, wall_end,
				&data->raycasting->rays[x], &data->textures[0]);
		}
		x++;
	}
}
