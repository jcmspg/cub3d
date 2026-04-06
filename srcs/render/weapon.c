/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   weapon.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 17:00:00 by joamiran          #+#    #+#             */
/*   Updated: 2026/04/06 19:45:00 by copilot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/render.h"

struct s_weapon_ctx
{
	int	w_width;
	int	w_height;
	int	x_start;
	int	y_start;
	int	color;
};

static bool	is_transparent_pixel(int pixel)
{
	return ((unsigned int)pixel == 0xFF000000U);
}

static t_texture	*get_weapon_texture(t_cub_data *data)
{
	if (!data->textures)
		return (NULL);
	return (&data->textures->gun_pov);
}

static void	set_weapon_dimensions(t_cub_data *data, t_texture *tex,
		struct s_weapon_ctx *ctx)
{
	if (tex && tex->loaded && tex->width > 0 && tex->height > 0)
	{
		ctx->w_height = data->mlx->height / 2;
		ctx->w_width = (ctx->w_height * tex->width) / tex->height;
	}
	else
	{
		ctx->w_width = data->mlx->width / 6;
		ctx->w_height = data->mlx->height / 3;
	}
}

static int	weapon_bob_offset(t_cub_data *data)
{
	if (data->input->forward || data->input->backward || data->input->left
		|| data->input->right)
		return (abs(data->player->bob_offset) * 2);
	return (0);
}

static void	set_weapon_position(t_cub_data *data, struct s_weapon_ctx *ctx)
{
	ctx->x_start = (data->mlx->width / 2) - (ctx->w_width / 2);
	ctx->y_start = data->mlx->height - ctx->w_height + weapon_bob_offset(data);
	ctx->color = 0x555555;
}

static void	draw_weapon_fallback_pixel(t_cub_data *data, struct s_weapon_ctx *c,
		int x, int y)
{
	if (x == 0 || x == c->w_width - 1 || y == 0)
		mylx_pixel_put(data, c->x_start + x, c->y_start + y, 0x222222);
	else
		mylx_pixel_put(data, c->x_start + x, c->y_start + y, c->color);
}

static void	draw_weapon_texture_pixel(t_cub_data *data, t_texture *tex,
		struct s_weapon_ctx *c, int x)
{
	int	y;
	int	xs;
	int	ys;
	int	tx;
	int	ty;
	int	pixel;

	y = 0;
	while (y < c->w_height)
	{
		xs = c->x_start + x;
		ys = c->y_start + y;
		if (xs >= 0 && xs < data->mlx->width && ys >= 0 && ys < data->mlx->height)
		{
			tx = (x * tex->width) / c->w_width;
			ty = (y * tex->height) / c->w_height;
			pixel = get_texture_pixel(tex, tx, tty);
			if (!is_transparent_pixel(pixel))
				mylx_pixel_put(data, xs, ys, pixel);
		}
		y++;
	}
}

static void	draw_weapon_column(t_cub_data *data, t_texture *tex,
		struct s_weapon_ctx *c, int x)
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
		if (xs >= 0 && xs < data->mlx->width && ys >= 0 && ys < data->mlx->height)
			draw_weapon_fallback_pixel(data, c, x, y);
		y++;
	}
}

void	render_weapon(t_cub_data *data)
{
	struct s_weapon_ctx	ctx;
	t_texture			*tex;
	int				x;

	tex = get_weapon_texture(data);
	set_weapon_dimensions(data, tex, &ctx);
	set_weapon_position(data, &ctx);
	x = 0;
	while (x < ctx.w_width)
	{
		draw_weapon_column(data, tex, &ctx, x);
		x++;
	}
}
