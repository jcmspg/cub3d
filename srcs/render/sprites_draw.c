/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprites_draw.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 01:27:50 by hladeiro          #+#    #+#             */
/*   Updated: 2026/04/08 01:27:51 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sprites_internal.h"

static bool	is_transparent_pixel(int pixel)
{
	return ((unsigned int)pixel == 0xFF000000U);
}

void	draw_sprite_stripe_color(t_cub_data *data, t_sprite_ctx *ctx,
		int stripe)
{
	int	y;

	y = ctx->start_y;
	while (y < ctx->end_y)
	{
		if (y >= 0 && y < data->mlx->height && !sprite_pixel_is_clipped(ctx, y))
			mylx_pixel_put(data, stripe, y, ctx->sprite_color);
		y++;
	}
}

void	draw_sprite_stripe_tex(t_cub_data *data, t_sprite_ctx *ctx, int stripe)
{
	int	y;
	int	tx;
	int	ty;
	int	pixel;

	tx = ((stripe - ctx->start_x) * ctx->sprite_texture->width)
		/ (ctx->end_x - ctx->start_x);
	y = ctx->start_y;
	while (y < ctx->end_y)
	{
		if (y >= 0 && y < data->mlx->height && !sprite_pixel_is_clipped(ctx, y))
		{
			ty = ((y - ctx->start_y) * ctx->sprite_texture->height)
				/ (ctx->end_y - ctx->start_y);
			if (ty < 0)
				ty = 0;
			if (ty >= ctx->sprite_texture->height)
				ty = ctx->sprite_texture->height - 1;
			pixel = get_texture_pixel(ctx->sprite_texture, tx, ty);
			if (!is_transparent_pixel(pixel))
				mylx_pixel_put(data, stripe, y, pixel);
		}
		y++;
	}
}

void	render_billboard(t_cub_data *data, t_sprite_ctx *ctx)
{
	int		stripe;
	float	wall_dist;

	if (!compute_billboard_span(data, ctx))
		return ;
	stripe = ctx->start_x;
	while (stripe < ctx->end_x)
	{
		wall_dist = from_fixed32(data->raycasting->rays[stripe].perp_dist);
		if (ctx->transform_y < wall_dist)
		{
			get_door_coverage(data, ctx, stripe);
			if (ctx->sprite_texture && ctx->sprite_texture->loaded
				&& ctx->sprite_texture->width > 0
				&& ctx->sprite_texture->height > 0)
				draw_sprite_stripe_tex(data, ctx, stripe);
			else
				draw_sprite_stripe_color(data, ctx, stripe);
		}
		stripe++;
	}
}
