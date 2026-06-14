/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hud_minimap_api.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 01:05:04 by hladeiro          #+#    #+#             */
/*   Updated: 2026/04/08 01:05:05 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/hud_internal.h"

void	draw_hud_minimap(t_cub_data *data)
{
	t_minimap_view	view;

	if (!data || !data->hud || !data->hud->enabled)
		return ;
	if (!data->map || !data->map->map_array)
		return ;
	init_minimap_view(data, &view);
	draw_visible_minimap_cells(data, &view);
	draw_hud_rays_view(data, &view);
	draw_minimap_player_centered_view(data, &view);
}
