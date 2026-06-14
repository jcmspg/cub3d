/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hud_minimap_hudrays.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 01:06:14 by hladeiro          #+#    #+#             */
/*   Updated: 2026/04/08 01:06:15 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/hud_internal.h"

void	draw_hud_rays_view(t_cub_data *data, t_minimap_view *view)
{
	int				i;
	t_hud_ray_ctx	ctx;

	if (!data->raycasting || !data->raycasting->rays)
		return ;
	ctx.minimap_x = view->minimap_x;
	ctx.minimap_y = view->minimap_y;
	ctx.minimap_size = view->minimap_size;
	ctx.center_x = view->minimap_x + view->minimap_size / 2;
	ctx.center_y = view->minimap_y + view->minimap_size / 2;
	ctx.cell_size = view->cell_size;
	i = 0;
	while (i < data->raycasting->num_rays)
	{
		draw_single_hud_ray(data, &ctx, &data->raycasting->rays[i]);
		i += 20;
	}
}
