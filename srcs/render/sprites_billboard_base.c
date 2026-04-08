/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprites_billboard_base.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 01:27:43 by hladeiro          #+#    #+#             */
/*   Updated: 2026/04/08 01:34:35 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sprites_internal.h"

float	get_player_x(t_cub_data *data)
{
	return (from_fixed32(data->player->x));
}

float	get_player_y(t_cub_data *data)
{
	return (from_fixed32(data->player->y));
}

void	update_billboard_camera(t_cub_data *data, t_sprite_ctx *ctx)
{
	ctx->dir_x = from_fixed32(data->player->dir_x);
	ctx->dir_y = from_fixed32(data->player->dir_y);
	ctx->plane_x = from_fixed32(data->player->plane_x);
	ctx->plane_y = from_fixed32(data->player->plane_y);
}

void	set_billboard_x_span(t_cub_data *data, t_sprite_ctx *ctx)
{
	float	player_x;
	float	player_y;
	float	camera_x;
	float	camera_y;
	int		center_x;

	player_x = get_player_x(data);
	player_y = get_player_y(data);
	camera_x = ctx->sprite_x - player_x;
	camera_y = ctx->sprite_y - player_y;
	center_x = (int)((data->mlx->width / 2) * (1 + ctx->inv
				* (ctx->dir_y * camera_x - ctx->dir_x * camera_y)
				/ ctx->transform_y));
	ctx->start_x = -(ctx->sprite_size / 2) + center_x;
	ctx->end_x = (ctx->sprite_size / 2) + center_x;
	if (ctx->start_x < 0)
		ctx->start_x = 0;
	if (ctx->end_x >= data->mlx->width)
		ctx->end_x = data->mlx->width - 1;
}

void	set_billboard_y_span(t_cub_data *data, t_sprite_ctx *ctx)
{
	int	view_offset;
	int	height_offset;

	view_offset = data->player->view_offset + data->player->bob_offset;
	height_offset = (int)(data->mlx->height / (4 * ctx->transform_y));
	ctx->start_y = -ctx->sprite_size / 2 + data->mlx->height / 2 + view_offset
		+ height_offset;
	ctx->end_y = ctx->sprite_size / 2 + data->mlx->height / 2 + view_offset
		+ height_offset;
	if (ctx->start_y < 0)
		ctx->start_y = 0;
	if (ctx->end_y >= data->mlx->height)
		ctx->end_y = data->mlx->height - 1;
}
