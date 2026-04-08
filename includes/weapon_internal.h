/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   weapon_internal.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 11:05:00 by hladeiro          #+#    #+#             */
/*   Updated: 2026/04/08 11:05:00 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEAPON_INTERNAL_H
# define WEAPON_INTERNAL_H

# include "render.h"

typedef struct s_weapon_ctx
{
	int		w_width;
	int		w_height;
	int		x_start;
	int		y_start;
	int		color;
}			t_weapon_ctx;

bool		is_transparent_pixel(int pixel);
t_texture	*get_weapon_texture(t_cub_data *data);
void		set_weapon_dimensions(t_cub_data *data, t_texture *tex,
				t_weapon_ctx *ctx);
void		set_weapon_position(t_cub_data *data, t_weapon_ctx *ctx);
void		draw_weapon_column(t_cub_data *data, t_texture *tex,
				t_weapon_ctx *c, int x);

#endif /* WEAPON_INTERNAL_H */
