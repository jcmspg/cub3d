/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hud_minimap_fov.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 01:04:23 by hladeiro          #+#    #+#             */
/*   Updated: 2026/04/08 01:04:24 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/hud_internal.h"

static void	draw_minimap_ray(t_cub_data *data, t_minimap_ray_ctx *ray,
		float angle, int color)
{
	int	i;
	int	px;
	int	py;

	i = 0;
	while (i < (int)ray->ray_len)
	{
		px = ray->center_x + (int)(cosf(angle) * i);
		py = ray->center_y + (int)(sinf(angle) * i);
		mylx_pixel_put(data, px, py, color);
		i++;
	}
}

static void	draw_minimap_center_dot(t_cub_data *data, int center_x,
		int center_y)
{
	int	px;
	int	py;

	py = -2;
	while (py <= 2)
	{
		px = -2;
		while (px <= 2)
		{
			mylx_pixel_put(data, center_x + px, center_y + py, 0xFF0000);
			px++;
		}
		py++;
	}
}

void	draw_minimap_player_centered_view(t_cub_data *data,
		t_minimap_view *view)
{
	t_minimap_ray_ctx	ray;
	float				dir_angle;
	float				left_angle;
	float				right_angle;

	ray.center_x = view->minimap_x + view->minimap_size / 2;
	ray.center_y = view->minimap_y + view->minimap_size / 2;
	ray.ray_len = view->minimap_size / 2.5f;
	dir_angle = from_fixed32(data->player->dir_angle);
	left_angle = (dir_angle - START_FOV / 2.0f) * M_PI / 180.0f;
	right_angle = (dir_angle + START_FOV / 2.0f) * M_PI / 180.0f;
	draw_minimap_ray(data, &ray, left_angle, 0x00AA00);
	draw_minimap_ray(data, &ray, right_angle, 0x00AA00);
	draw_minimap_ray(data, &ray, dir_angle * M_PI / 180.0f, 0xFFFF00);
	draw_minimap_center_dot(data, ray.center_x, ray.center_y);
}
