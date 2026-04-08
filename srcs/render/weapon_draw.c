/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   weapon_draw.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 01:37:23 by hladeiro          #+#    #+#             */
/*   Updated: 2026/04/08 01:37:23 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/weapon_internal.h"

static void	draw_weapon_fallback_pixel(t_cub_data *data, t_weapon_ctx *c,
		int x, int y)
{
	if (x == 0 || x == c->w_width - 1 || y == 0)
		mylx_pixel_put(data, c->x_start + x, c->y_start + y, 0x222222);
	else
		mylx_pixel_put(data, c->x_start + x, c->y_start + y, c->color);
}

static void	draw_weapon_texture_pixel(t_cub_data *data, t_texture *tex,
		t_weapon_ctx *c, int x)
{
	int	y;
	int	pixel;

	y = 0;
	while (y < c->w_height)
	{
		if (c->x_start + x >= 0 && c->x_start + x < data->mlx->width
			&& c->y_start + y >= 0 && c->y_start + y < data->mlx->height)
		{
			pixel = get_texture_pixel(tex, (x * tex->width) / c->w_width,
					(y * tex->height) / c->w_height);
			if (!is_transparent_pixel(pixel))
				mylx_pixel_put(data, c->x_start + x, c->y_start + y, pixel);
		}
		y++;
	}
}

void	draw_weapon_column(t_cub_data *data, t_texture *tex, t_weapon_ctx *c,
		int x)
{
	int	y;
	int	xs;
	int	ys;

	if (tex && tex->loaded && tex->width > 0 && tex->height > 0)
	{
		draw_weapon_texture_pixel(data, tex, c, x);
		return ;
	}
	y = 0;
	while (y < c->w_height)
	{
		xs = c->x_start + x;
		ys = c->y_start + y;
		if (xs >= 0 && xs < data->mlx->width
			&& ys >= 0 && ys < data->mlx->height)
			draw_weapon_fallback_pixel(data, c, x, y);
		y++;
	}
}
