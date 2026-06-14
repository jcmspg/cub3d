/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hud_internal.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 01:03:26 by hladeiro          #+#    #+#             */
/*   Updated: 2026/04/08 01:03:57 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HUD_INTERNAL_H
# define HUD_INTERNAL_H

# include "hud.h"

typedef struct s_minimap_view
{
	int		minimap_size;
	int		cell_size;
	int		view_cells;
	int		minimap_x;
	int		minimap_y;
	int		offset_x;
	int		offset_y;
	int		start_cell_x;
	int		start_cell_y;
}			t_minimap_view;

typedef struct s_minimap_ray_ctx
{
	int		center_x;
	int		center_y;
	float	ray_len;
}			t_minimap_ray_ctx;

typedef struct s_hud_ray_ctx
{
	int		minimap_x;
	int		minimap_y;
	int		minimap_size;
	int		center_x;
	int		center_y;
	int		cell_size;
}			t_hud_ray_ctx;

typedef struct s_cell_draw
{
	int		x;
	int		y;
	int		size;
	int		color;
}			t_cell_draw;

typedef struct s_bresenham
{
	int		x0;
	int		y0;
	int		x1;
	int		y1;
	int		dx;
	int		dy;
	int		sx;
	int		sy;
	int		err;
	int		e2;
	int		color;
}			t_bresenham;

void		draw_hud_stats(t_cub_data *data);
void		init_minimap_view(t_cub_data *data, t_minimap_view *view);
void		fill_minimap_cell(t_cub_data *data, t_cell_draw *cell);
void		draw_visible_minimap_cells(t_cub_data *data, t_minimap_view *view);
void		draw_minimap_player_centered_view(t_cub_data *data,
				t_minimap_view *view);
void		draw_single_hud_ray(t_cub_data *data, t_hud_ray_ctx *ctx,
				t_ray *ray);
void		draw_hud_rays_view(t_cub_data *data, t_minimap_view *view);

#endif /* HUD_INTERNAL_H */
