/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_render_door.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 01:11:58 by hladeiro          #+#    #+#             */
/*   Updated: 2026/04/08 01:21:24 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/gamelogic.h"
#include "../../includes/ray_render_internal.h"

static void	set_door_distance_data(t_cub_data *data, t_ray *ray,
		t_door_ctx *ctx)
{
	ctx->player_pos = data->player->y;
	ctx->map_pos = to_fixed32(ray->door_map_y);
	ctx->step_val = to_fixed32(ray->step_y);
	ctx->dir_val = ray->dir_y;
	if (ray->door_side == 0)
	{
		ctx->player_pos = data->player->x;
		ctx->map_pos = to_fixed32(ray->door_map_x);
		ctx->step_val = to_fixed32(ray->step_x);
		ctx->dir_val = ray->dir_x;
	}
	if (ctx->dir_val == 0)
		ctx->dir_val = 1;
	ctx->euclidean_dist = fixed32_div(fixed32_add(fixed32_sub(ctx->map_pos,
					ctx->player_pos), fixed32_div(fixed32_sub(to_fixed32(1.0f),
						ctx->step_val), to_fixed32(2.0f))),
			ctx->dir_val);
	ctx->dot_prod = fixed32_add(fixed32_mul(ray->dir_x, data->player->dir_x),
			fixed32_mul(ray->dir_y, data->player->dir_y));
	ctx->perp_dist = fixed32_mul(ctx->euclidean_dist, ctx->dot_prod);
	if (ctx->perp_dist < to_fixed32(0.1f))
		ctx->perp_dist = to_fixed32(0.1f);
	ctx->dist = ctx->perp_dist;
	ctx->door_dist = ray->door_dist;
	ctx->door_side = ray->door_side;
}

static void	set_door_screen_bounds(t_cub_data *data, t_ray *ray,
		t_door_ctx *ctx)
{
	ctx->line_height = (int)from_fixed32(fixed32_div(
				to_fixed32(data->mlx->height), ctx->dist));
	ctx->view_offset = data->player->view_offset + data->player->bob_offset;
	ctx->frame_top = (data->mlx->height - ctx->line_height) / 2
		+ ctx->view_offset;
	ctx->frame_bottom = (data->mlx->height + ctx->line_height) / 2
		+ ctx->view_offset;
	ctx->door = get_door_at(data, ray->door_map_x, ray->door_map_y);
	ctx->offset = 0;
	if (ctx->door)
		ctx->offset = (int)(ctx->line_height * ctx->door->open_amount);
	ctx->render_bottom = ctx->frame_bottom - ctx->offset;
	ctx->render_top = ctx->frame_top - ctx->offset;
	ctx->draw_start = ctx->render_top;
	ctx->draw_end = ctx->render_bottom;
	if (ctx->draw_start < ctx->frame_top)
		ctx->draw_start = ctx->frame_top;
	if (ctx->draw_start < 0)
		ctx->draw_start = 0;
	if (ctx->draw_end >= data->mlx->height)
		ctx->draw_end = data->mlx->height - 1;
}

static void	draw_door_pixel(t_cub_data *data, int x, int y, t_door_ctx *ctx)
{
	ctx->draw_fallback = 0;
	if (data->textures && data->textures->door.loaded
		&& data->textures->door.width > 0 && data->textures->door.height > 0)
	{
		ctx->tex_y = ((y - ctx->render_top) * data->textures->door.height)
			/ ctx->line_height;
		if (ctx->tex_y < 0)
			ctx->tex_y = 0;
		if (ctx->tex_y >= data->textures->door.height)
			ctx->tex_y = data->textures->door.height - 1;
		ctx->tex_color = get_texture_pixel(&data->textures->door, ctx->tex_x,
				ctx->tex_y);
		if ((unsigned int)ctx->tex_color == 0xFF000000U)
			ctx->draw_fallback = 1;
		else
		{
			ctx->tex_color = apply_shading(ctx->tex_color,
					from_fixed32(ctx->door_dist), ctx->door_side);
			mylx_pixel_put(data, x, y, ctx->tex_color);
		}
	}
	else
		ctx->draw_fallback = 1;
	if (ctx->draw_fallback)
		mylx_pixel_put(data, x, y, ctx->door_color);
}

static void	set_door_texture_x(t_cub_data *data, t_ray *ray, t_door_ctx *ctx)
{
	if (ray->door_side == 0)
		ctx->wall_x = fixed32_add(data->player->y,
				fixed32_mul(ctx->euclidean_dist, ray->dir_y));
	else
		ctx->wall_x = fixed32_add(data->player->x,
				fixed32_mul(ctx->euclidean_dist, ray->dir_x));
	ctx->wall_x = fixed32_sub(ctx->wall_x,
			to_fixed32((float)(int)from_fixed32(ctx->wall_x)));
	ctx->tex_x = (int)from_fixed32(fixed32_mul(ctx->wall_x,
				to_fixed32(data->textures->door.width)));
	if ((ray->door_side == 0 && ray->dir_x > 0) || (ray->door_side == 1
			&& ray->dir_y < 0))
		ctx->tex_x = data->textures->door.width - ctx->tex_x - 1;
	if (ctx->tex_x < 0)
		ctx->tex_x = 0;
	if (ctx->tex_x >= data->textures->door.width)
		ctx->tex_x = data->textures->door.width - 1;
}

void	draw_door_slice(t_cub_data *data, int x, t_ray *ray)
{
	t_door_ctx	ctx;
	int			i;

	set_door_distance_data(data, ray, &ctx);
	set_door_screen_bounds(data, ray, &ctx);
	if (ctx.draw_start > ctx.draw_end)
		return ;
	set_door_texture_x(data, ray, &ctx);
	ctx.door_color = apply_shading(0x8B4513, from_fixed32(ctx.door_dist),
			ctx.door_side);
	i = ctx.draw_start;
	while (i <= ctx.draw_end)
	{
		draw_door_pixel(data, x, i, &ctx);
		i++;
	}
}
