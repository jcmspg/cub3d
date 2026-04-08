/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_render_column.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 01:11:55 by hladeiro          #+#    #+#             */
/*   Updated: 2026/04/08 01:11:56 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ray_render_internal.h"

void	draw_ceiling_slice(t_cub_data *data, int x, int wall_start)
{
	int	y;
	int	color;

	color = data->textures->ceiling_color;
	y = 0;
	while (y < wall_start)
	{
		mylx_pixel_put(data, x, y, color);
		y++;
	}
}

void	draw_floor_slice(t_cub_data *data, int x, int wall_end)
{
	int	y;
	int	color;

	color = data->textures->floor_color;
	y = wall_end + 1;
	while (y < data->mlx->height)
	{
		mylx_pixel_put(data, x, y, color);
		y++;
	}
}

void	render_column(t_cub_data *data, int x, t_ray *ray)
{
	int	draw_start;
	int	draw_end;

	if (!ray->hit)
	{
		draw_start = data->mlx->height / 2;
		draw_end = data->mlx->height / 2;
	}
	else
		calculate_wall_slice(data, ray, &draw_start, &draw_end);
	draw_ceiling_slice(data, x, draw_start);
	if (ray->hit)
	{
		if (ray->hit_content == 'D')
			draw_door_slice(data, x, ray);
		else
			draw_wall_slice(data, x, ray);
	}
	draw_floor_slice(data, x, draw_end);
	if (ray->door_hit && ray->hit_content != 'D')
		draw_door_slice(data, x, ray);
}
