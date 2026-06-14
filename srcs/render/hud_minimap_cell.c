/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hud_minimap_cell.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 01:05:16 by hladeiro          #+#    #+#             */
/*   Updated: 2026/04/08 01:05:18 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/hud_internal.h"

static int	get_legacy_minimap_cell_color(char c)
{
	if (c == '1')
		return (0xFFFFFF);
	if (c == '0' || c == 'N' || c == 'S' || c == 'E' || c == 'W')
		return (0x333333);
	return (HUD_BG_COLOR);
}

void	draw_minimap_cell(t_cub_data *data, int map_x, int map_y, int cell_size)
{
	char		c;
	t_cell_draw	cell;

	c = data->map->map_array[map_y * data->map->width + map_x];
	cell.color = get_legacy_minimap_cell_color(c);
	cell.x = data->hud->minimap_x + (map_x * cell_size);
	cell.y = data->hud->minimap_y + (map_y * cell_size);
	cell.size = cell_size;
	fill_minimap_cell(data, &cell);
}
