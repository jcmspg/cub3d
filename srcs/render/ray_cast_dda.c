/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_cast_dda.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 01:08:49 by hladeiro          #+#    #+#             */
/*   Updated: 2026/04/09 02:57:22 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static t_fixed32	abs_fixed32(t_fixed32 value)
{
	if (value < 0)
		return (-value);
	return (value);
}

static void	set_dda_deltas(t_ray *ray)
{
	if (abs_fixed32(ray->dir_x) < to_fixed32(0.0001f))
		ray->delta_dist_x = to_fixed32(10000.0f);
	else
		ray->delta_dist_x = abs_fixed32(fixed32_div(to_fixed32(1.0f),
					ray->dir_x));
	if (abs_fixed32(ray->dir_y) < to_fixed32(0.0001f))
		ray->delta_dist_y = to_fixed32(10000.0f);
	else
		ray->delta_dist_y = abs_fixed32(fixed32_div(to_fixed32(1.0f),
					ray->dir_y));
}

static void	set_dda_step_x(t_ray *ray, t_fixed32 frac_x)
{
	if (ray->dir_x < 0)
	{
		ray->step_x = -1;
		ray->side_dist_x = fixed32_mul(frac_x, ray->delta_dist_x);
	}
	else
	{
		ray->step_x = 1;
		ray->side_dist_x = fixed32_mul(fixed32_sub(to_fixed32(1.0f), frac_x),
				ray->delta_dist_x);
	}
}

static void	set_dda_step_y(t_ray *ray, t_fixed32 frac_y)
{
	if (ray->dir_y < 0)
	{
		ray->step_y = -1;
		ray->side_dist_y = fixed32_mul(frac_y, ray->delta_dist_y);
	}
	else
	{
		ray->step_y = 1;
		ray->side_dist_y = fixed32_mul(fixed32_sub(to_fixed32(1.0f), frac_y),
				ray->delta_dist_y);
	}
}

void	init_dda(t_cub_data *data, t_ray *ray)
{
	t_fixed32	player_x;
	t_fixed32	player_y;
	t_fixed32	frac_x;
	t_fixed32	frac_y;

	player_x = data->player->x;
	player_y = data->player->y;
	ray->map_x = (int)from_fixed32(player_x);
	ray->map_y = (int)from_fixed32(player_y);
	set_dda_deltas(ray);
	frac_x = fixed32_sub(player_x, to_fixed32((float)ray->map_x));
	frac_y = fixed32_sub(player_y, to_fixed32((float)ray->map_y));
	set_dda_step_x(ray, frac_x);
	set_dda_step_y(ray, frac_y);
}
