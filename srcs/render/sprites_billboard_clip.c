/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprites_billboard_clip.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 01:27:47 by hladeiro          #+#    #+#             */
/*   Updated: 2026/04/08 01:27:47 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sprites_internal.h"

bool	sprite_pixel_is_clipped(t_sprite_ctx *ctx, int y)
{
	return (ctx->clip_top >= 0 && y >= ctx->clip_top && y <= ctx->clip_bottom);
}

void	set_door_clip_bounds(t_cub_data *data, t_sprite_ctx *ctx)
{
	ctx->clip_top = ctx->start_y - ctx->sprite_size;
	ctx->clip_bottom = ctx->end_y - ctx->sprite_size;
	if (ctx->clip_top < ctx->start_y)
		ctx->clip_top = ctx->start_y;
	if (ctx->clip_top < 0)
		ctx->clip_top = 0;
	if (ctx->clip_bottom >= data->mlx->height)
		ctx->clip_bottom = data->mlx->height - 1;
}

void	get_door_coverage(t_cub_data *data, t_sprite_ctx *ctx, int stripe)
{
	t_ray	*ray;
	t_door	*door;

	ctx->clip_top = -1;
	ctx->clip_bottom = -1;
	ray = &data->raycasting->rays[stripe];
	if (!ray->door_hit)
		return ;
	ctx->start_y = data->player->view_offset + data->player->bob_offset;
	if (ctx->transform_y < from_fixed32(ray->door_dist))
		return ;
	door = get_door_at(data, ray->door_map_x, ray->door_map_y);
	if (!door)
		return ;
	ctx->sprite_size = (int)(data->mlx->height / from_fixed32(ray->door_dist));
	ctx->start_y = (data->mlx->height - ctx->sprite_size) / 2 + ctx->start_y;
	ctx->end_y = (data->mlx->height + ctx->sprite_size) / 2 + ctx->start_y;
	ctx->sprite_size = (int)(ctx->sprite_size * door->open_amount);
	set_door_clip_bounds(data, ctx);
}

bool	compute_billboard_span(t_cub_data *data, t_sprite_ctx *ctx)
{
	float	player_x;
	float	player_y;
	float	camera_x;
	float	camera_y;

	update_billboard_camera(data, ctx);
	ctx->inv = 1.0f / (ctx->plane_x * ctx->dir_y - ctx->dir_x * ctx->plane_y);
	player_x = get_player_x(data);
	player_y = get_player_y(data);
	camera_x = ctx->sprite_x - player_x;
	camera_y = ctx->sprite_y - player_y;
	ctx->transform_y = ctx->inv * (-ctx->plane_y * camera_x
			+ ctx->plane_x * camera_y);
	if (ctx->transform_y <= 0.1f)
		return (false);
	ctx->sprite_size = abs((int)(data->mlx->height / ctx->transform_y))
		/ ctx->sprite_scale_div;
	set_billboard_x_span(data, ctx);
	set_billboard_y_span(data, ctx);
	return (ctx->end_x > ctx->start_x && ctx->end_y > ctx->start_y);
}
