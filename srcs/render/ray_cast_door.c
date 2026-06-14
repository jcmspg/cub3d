/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_cast_door.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 01:09:00 by hladeiro          #+#    #+#             */
/*   Updated: 2026/04/08 01:09:01 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static int	handle_closed_door_hit(t_ray *ray)
{
	ray->hit = 1;
	ray->hit_content = 'D';
	ray->door_hit = 1;
	ray->door_map_x = ray->map_x;
	ray->door_map_y = ray->map_y;
	ray->door_side = ray->side;
	if (ray->side == 0)
		ray->door_dist = fixed32_sub(ray->side_dist_x, ray->delta_dist_x);
	else
		ray->door_dist = fixed32_sub(ray->side_dist_y, ray->delta_dist_y);
	return (1);
}

static void	store_partial_door_hit(t_ray *ray)
{
	if (!ray->door_hit)
	{
		ray->door_hit = 1;
		ray->door_map_x = ray->map_x;
		ray->door_map_y = ray->map_y;
		ray->door_side = ray->side;
		if (ray->side == 0)
			ray->door_dist = fixed32_sub(ray->side_dist_x, ray->delta_dist_x);
		else
			ray->door_dist = fixed32_sub(ray->side_dist_y, ray->delta_dist_y);
	}
}

int	ray_handle_door_step(t_cub_data *data, t_ray *ray)
{
	t_door	*door;

	door = get_door_at(data, ray->map_x, ray->map_y);
	if (!door)
		return (0);
	if (door->open_amount < 0.01f)
		return (handle_closed_door_hit(ray));
	if (door->open_amount < 1.0f)
		store_partial_door_hit(ray);
	return (0);
}
