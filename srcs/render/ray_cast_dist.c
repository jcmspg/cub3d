/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_cast_dist.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 01:09:05 by hladeiro          #+#    #+#             */
/*   Updated: 2026/04/08 01:09:06 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static void	apply_fisheye_correction(t_cub_data *data, t_ray *ray)
{
	t_fixed32	dot_product;

	dot_product = fixed32_add(fixed32_mul(ray->dir_x, data->player->dir_x),
			fixed32_mul(ray->dir_y, data->player->dir_y));
	ray->perp_dist = fixed32_mul(ray->perp_dist, dot_product);
	if (ray->door_hit)
	{
		ray->door_dist = fixed32_mul(ray->door_dist, dot_product);
		if (ray->door_dist <= 0)
			ray->door_dist = to_fixed32(0.001f);
	}
}

void	calculate_perp_distance(t_cub_data *data, t_ray *ray)
{
	if (ray->side == 0)
		ray->perp_dist = fixed32_sub(ray->side_dist_x, ray->delta_dist_x);
	else
		ray->perp_dist = fixed32_sub(ray->side_dist_y, ray->delta_dist_y);
	apply_fisheye_correction(data, ray);
	if (ray->perp_dist <= 0)
		ray->perp_dist = to_fixed32(0.001f);
}

void	cast_single_ray(t_cub_data *data, t_ray *ray, int x)
{
	reset_ray(ray);
	calculate_ray_dir(data, ray, x);
	init_dda(data, ray);
	perform_dda(data, ray);
	calculate_perp_distance(data, ray);
}
