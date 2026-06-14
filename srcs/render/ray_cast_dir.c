/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_cast_dir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 01:09:09 by hladeiro          #+#    #+#             */
/*   Updated: 2026/04/08 01:09:10 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	calculate_ray_dir(t_cub_data *data, t_ray *ray, int x)
{
	t_fixed32	camera_x;
	t_fixed32	ray_angle;
	t_fixed32	half_fov;
	t_fixed32	fov_step;

	camera_x = fixed32_sub(fixed32_div(fixed32_mul(to_fixed32(2.0f),
					to_fixed32((float)x)), to_fixed32((float)data->mlx->width)),
			to_fixed32(1.0f));
	half_fov = fixed32_div(data->game->fov, to_fixed32(2.0f));
	fov_step = fixed32_mul(camera_x, half_fov);
	ray_angle = fixed32_add(data->player->dir_angle, fov_step);
	ray_angle = normalize_angle_degrees(ray_angle);
	ray->dir_x = fast_cos(&data->trig, ray_angle);
	ray->dir_y = fast_sin(&data->trig, ray_angle);
}
