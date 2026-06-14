/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 19:00:00 by joamiran          #+#    #+#             */
/*   Updated: 2026/01/24 19:13:26 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

/**
 * Allocate the raycasting structure and ray array
 * Returns NULL on failure
 */
t_raycasting	*init_raycasting(int screen_width)
{
	t_raycasting	*rc;

	rc = malloc(sizeof(t_raycasting));
	if (!rc)
		return (NULL);
	rc->num_rays = screen_width;
	rc->rays = malloc(sizeof(t_ray) * screen_width);
	if (!rc->rays)
	{
		free(rc);
		return (NULL);
	}
	return (rc);
}

/**
 * Reset a single ray to default values before casting
 */
void	reset_ray(t_ray *ray)
{
	ray->dir_x = 0;
	ray->dir_y = 0;
	ray->map_x = 0;
	ray->map_y = 0;
	ray->step_x = 0;
	ray->step_y = 0;
	ray->side_dist_x = 0;
	ray->side_dist_y = 0;
	ray->delta_dist_x = 0;
	ray->delta_dist_y = 0;
	ray->perp_dist = 0;
	ray->side = 0;
	ray->hit = 0;
	ray->door_hit = 0;
	ray->door_side = 0;
	ray->door_dist = 0;
	ray->door_map_x = 0;
	ray->door_map_y = 0;
	ray->hit_content = 0;
}

/**
 * Reset all rays in the array before a new frame
 */
void	reset_all_rays(t_raycasting *rc)
{
	int	i;

	i = 0;
	while (i < rc->num_rays)
	{
		reset_ray(&rc->rays[i]);
		i++;
	}
}

/**
 * Free the raycasting structure and ray array
 */
void	free_raycasting(t_raycasting *rc)
{
	if (!rc)
		return ;
	if (rc->rays)
		free(rc->rays);
	free(rc);
}
