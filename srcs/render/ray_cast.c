/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_cast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 01:09:29 by hladeiro          #+#    #+#             */
/*   Updated: 2026/04/08 01:09:30 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	cast_all_rays(t_cub_data *data)
{
	int	x;

	if (!data->raycasting || !data->raycasting->rays)
		return ;
	x = 0;
	while (x < data->raycasting->num_rays)
	{
		cast_single_ray(data, &data->raycasting->rays[x], x);
		x++;
	}
}
