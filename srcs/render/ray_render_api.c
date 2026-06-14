/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_render_api.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 01:11:49 by hladeiro          #+#    #+#             */
/*   Updated: 2026/04/08 01:11:50 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ray_render_internal.h"

void	render_walls(t_cub_data *data)
{
	int	x;

	if (!data || !data->raycasting || !data->raycasting->rays)
		return ;
	if (!data->textures)
		return ;
	x = 0;
	while (x < data->raycasting->num_rays)
	{
		render_column(data, x, &data->raycasting->rays[x]);
		x++;
	}
}
