/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hud_minimap_view.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 01:04:16 by hladeiro          #+#    #+#             */
/*   Updated: 2026/04/08 01:05:29 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/hud_internal.h"

void	init_minimap_view(t_cub_data *data, t_minimap_view *view)
{
	float	player_x;
	float	player_y;
	float	frac_x;
	float	frac_y;

	view->minimap_size = data->hud->height - (HUD_MINIMAP_PADDING * 2)
		- HUD_BORDER_WIDTH;
	view->cell_size = HUD_MINIMAP_CELL;
	view->view_cells = view->minimap_size / view->cell_size;
	view->minimap_x = data->mlx->width - view->minimap_size
		- HUD_MINIMAP_PADDING;
	view->minimap_y = data->hud->y + HUD_BORDER_WIDTH + HUD_MINIMAP_PADDING;
	player_x = from_fixed32(data->player->x);
	player_y = from_fixed32(data->player->y);
	frac_x = player_x - (int)player_x;
	frac_y = player_y - (int)player_y;
	view->offset_x = (int)(frac_x * view->cell_size);
	view->offset_y = (int)(frac_y * view->cell_size);
	view->start_cell_x = (int)player_x - view->view_cells / 2 - 1;
	view->start_cell_y = (int)player_y - view->view_cells / 2 - 1;
}

static int	get_minimap_cell_color(t_cub_data *data, int map_x, int map_y)
{
	char	c;

	if (map_x < 0 || map_x >= data->map->width || map_y < 0
		|| map_y >= data->map->height)
		return (0x111111);
	c = data->map->map_array[map_y * data->map->width + map_x];
	if (c == '1')
		return (0xFFFFFF);
	if (c == 'D')
		return (0x8B4513);
	if (c == 'M')
		return (0xFFD700);
	if (c == '0' || c == 'N' || c == 'S' || c == 'E' || c == 'W')
		return (0x444444);
	return (0x111111);
}

void	fill_minimap_cell(t_cub_data *data, t_cell_draw *cell)
{
	int	px;
	int	py;

	py = -1;
	while (++py < cell->size)
	{
		px = -1;
		while (++px < cell->size)
		{
			if (px == 0 || py == 0)
				mylx_pixel_put(data, cell->x + px, cell->y + py, 0x222222);
			else
				mylx_pixel_put(data, cell->x + px, cell->y + py, cell->color);
		}
	}
}

static void	draw_minimap_visible_cell(t_cub_data *data, t_minimap_view *view,
		int x, int y)
{
	int			map_x;
	int			map_y;
	t_cell_draw	cell;

	cell.x = view->minimap_x + x * view->cell_size - view->offset_x;
	cell.y = view->minimap_y + y * view->cell_size - view->offset_y;
	cell.size = view->cell_size;
	if (cell.x >= view->minimap_x + view->minimap_size || cell.x
		+ view->cell_size <= view->minimap_x || cell.y >= view->minimap_y
		+ view->minimap_size || cell.y + view->cell_size <= view->minimap_y)
		return ;
	map_x = view->start_cell_x + x;
	map_y = view->start_cell_y + y;
	cell.color = get_minimap_cell_color(data, map_x, map_y);
	fill_minimap_cell(data, &cell);
}

void	draw_visible_minimap_cells(t_cub_data *data, t_minimap_view *view)
{
	int	x;
	int	y;

	y = -1;
	while (++y < view->view_cells + 2)
	{
		x = -1;
		while (++x < view->view_cells + 2)
			draw_minimap_visible_cell(data, view, x, y);
	}
}
