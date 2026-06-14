/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_cast_hit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 01:08:55 by hladeiro          #+#    #+#             */
/*   Updated: 2026/04/08 01:08:55 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	ray_handle_door_step(t_cub_data *data, t_ray *ray);

static void	step_dda_once(t_ray *ray)
{
	if (ray->side_dist_x < ray->side_dist_y)
	{
		ray->side_dist_x = fixed32_add(ray->side_dist_x, ray->delta_dist_x);
		ray->map_x += ray->step_x;
		ray->side = 0;
	}
	else
	{
		ray->side_dist_y = fixed32_add(ray->side_dist_y, ray->delta_dist_y);
		ray->map_y += ray->step_y;
		ray->side = 1;
	}
}

static int	handle_cell_hit(t_cub_data *data, t_ray *ray, char map_char)
{
	if (map_char == '1' || map_char == ' ')
	{
		ray->hit = 1;
		ray->hit_content = map_char;
		return (1);
	}
	if (map_char == 'D')
		return (ray_handle_door_step(data, ray));
	return (0);
}

int	perform_dda(t_cub_data *data, t_ray *ray)
{
	int		max_steps;
	int		step;
	char	map_char;

	max_steps = data->map->width + data->map->height;
	step = 0;
	while (step < max_steps)
	{
		step_dda_once(ray);
		if (ray->map_x < 0 || ray->map_x >= data->map->width || ray->map_y < 0
			|| ray->map_y >= data->map->height)
		{
			ray->hit = 1;
			return (1);
		}
		map_char = data->map->map_array[ray->map_y * data->map->width
			+ ray->map_x];
		if (handle_cell_hit(data, ray, map_char))
			return (1);
		step++;
	}
	return (0);
}
