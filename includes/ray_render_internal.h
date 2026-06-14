/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_render_internal.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 01:12:01 by hladeiro          #+#    #+#             */
/*   Updated: 2026/04/08 01:25:21 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RAY_RENDER_INTERNAL_H
# define RAY_RENDER_INTERNAL_H

# include "cub3d.h"

typedef struct s_wall_ctx
{
	int			line_height;
	int			draw_start;
	int			draw_end;
	int			y;
	int			wall_color;
	int			shaded_color;
	int			tex_x;
	int			tex_y;
	t_texture	*texture;
	t_fixed32	step;
	t_fixed32	tex_pos;
}				t_wall_ctx;

typedef struct s_door_ctx
{
	int			line_height;
	int			frame_top;
	int			frame_bottom;
	int			render_top;
	int			render_bottom;
	int			view_offset;
	int			offset;
	int			draw_start;
	int			draw_end;
	int			tex_x;
	int			tex_y;
	int			tex_color;
	int			door_color;
	int			draw_fallback;
	t_door		*door;
	t_fixed32	player_pos;
	t_fixed32	map_pos;
	t_fixed32	step_val;
	t_fixed32	dir_val;
	t_fixed32	euclidean_dist;
	t_fixed32	p_dir_x;
	t_fixed32	p_dir_y;
	t_fixed32	r_dir_x;
	t_fixed32	r_dir_y;
	t_fixed32	dot_prod;
	t_fixed32	perp_dist;
	t_fixed32	dist;
	t_fixed32	wall_x;
	t_fixed32	door_dist;
	int			door_side;
}				t_door_ctx;

int				calculate_wall_slice(t_cub_data *data, t_ray *ray,
					int *draw_start, int *draw_end);
int				apply_shading(int color, float dist, int side);
void			draw_textured_pixel(t_cub_data *data, int x, t_ray *ray,
					t_wall_ctx *ctx);
void			draw_textured_wall(t_cub_data *data, int x, t_ray *ray,
					t_wall_ctx *ctx);
void			draw_flat_wall(t_cub_data *data, int x, t_ray *ray,
					t_wall_ctx *ctx);
void			draw_wall_slice(t_cub_data *data, int x, t_ray *ray);
void			draw_door_slice(t_cub_data *data, int x, t_ray *ray);
void			draw_ceiling_slice(t_cub_data *data, int x, int wall_start);
void			draw_floor_slice(t_cub_data *data, int x, int wall_end);
void			render_column(t_cub_data *data, int x, t_ray *ray);

#endif /* RAY_RENDER_INTERNAL_H */
