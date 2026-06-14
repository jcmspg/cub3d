/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprites_internal.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 11:05:00 by hladeiro          #+#    #+#             */
/*   Updated: 2026/04/08 11:05:00 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SPRITES_INTERNAL_H
# define SPRITES_INTERNAL_H

# include "render.h"

typedef struct s_sprite_ctx
{
	int			clip_top;
	int			clip_bottom;
	int			start_x;
	int			end_x;
	int			start_y;
	int			end_y;
	int			sprite_scale_div;
	int			sprite_color;
	int			sprite_size;
	float		sprite_x;
	float		sprite_y;
	float		transform_y;
	float		inv;
	float		dir_x;
	float		dir_y;
	float		plane_x;
	float		plane_y;
	t_texture	*sprite_texture;
}				t_sprite_ctx;

float			get_player_x(t_cub_data *data);
float			get_player_y(t_cub_data *data);
void			update_billboard_camera(t_cub_data *data, t_sprite_ctx *ctx);
void			set_billboard_x_span(t_cub_data *data, t_sprite_ctx *ctx);
void			set_billboard_y_span(t_cub_data *data, t_sprite_ctx *ctx);
bool			sprite_pixel_is_clipped(t_sprite_ctx *ctx, int y);
void			set_door_clip_bounds(t_cub_data *data, t_sprite_ctx *ctx);
void			get_door_coverage(t_cub_data *data, t_sprite_ctx *ctx,
					int stripe);
bool			compute_billboard_span(t_cub_data *data, t_sprite_ctx *ctx);
void			draw_sprite_stripe_color(t_cub_data *data, t_sprite_ctx *ctx,
					int stripe);
void			draw_sprite_stripe_tex(t_cub_data *data, t_sprite_ctx *ctx,
					int stripe);
void			render_billboard(t_cub_data *data, t_sprite_ctx *ctx);
void			render_ammo_sprite(t_cub_data *data, float sx, float sy);
void			render_enemy_sprite(t_cub_data *data, t_enemy *enemy,
					t_texture *demon_texture);
bool			enemy_should_draw(t_enemy *enemy, uint64_t now);
void			render_enemies(t_cub_data *data);

#endif /* SPRITES_INTERNAL_H */
