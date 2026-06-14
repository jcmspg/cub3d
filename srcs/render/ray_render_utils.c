/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_render_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 01:14:44 by hladeiro          #+#    #+#             */
/*   Updated: 2026/04/08 01:25:21 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ray_render_internal.h"

int	calculate_wall_slice(t_cub_data *data, t_ray *ray, int *draw_start,
		int *draw_end)
{
	int			line_height;
	t_fixed32	perp_dist;
	int			view_offset;

	perp_dist = ray->perp_dist;
	if (perp_dist < to_fixed32(0.001f))
		perp_dist = to_fixed32(0.001f);
	line_height = (int)from_fixed32(fixed32_div(to_fixed32(data->mlx->height),
				perp_dist));
	view_offset = data->player->view_offset + data->player->bob_offset;
	*draw_start = (data->mlx->height - line_height) / 2 + view_offset;
	*draw_end = (data->mlx->height + line_height) / 2 + view_offset;
	if (*draw_start < 0)
		*draw_start = 0;
	if (*draw_end >= data->mlx->height)
		*draw_end = data->mlx->height - 1;
	return (line_height);
}

int	apply_shading(int color, float dist, int side)
{
	t_fixed32	shade;
	int			r;
	int			g;
	int			b;

	shade = fixed32_sub(to_fixed32(1.0f), fixed32_div(to_fixed32(dist),
				to_fixed32(20.0f)));
	if (shade < to_fixed32(0.3f))
		shade = to_fixed32(0.3f);
	if (shade > to_fixed32(1.0f))
		shade = to_fixed32(1.0f);
	if (side == 1)
		shade = fixed32_mul(shade, to_fixed32(0.7f));
	r = (int)(((color >> 16) & 0xFF) * from_fixed32(shade));
	g = (int)(((color >> 8) & 0xFF) * from_fixed32(shade));
	b = (int)((color & 0xFF) * from_fixed32(shade));
	return ((r << 16) | (g << 8) | b);
}

void	draw_textured_pixel(t_cub_data *data, int x, t_ray *ray,
		t_wall_ctx *ctx)
{
	ctx->tex_y = (int)from_fixed32(ctx->tex_pos);
	if (ctx->tex_y < 0)
		ctx->tex_y = 0;
	if (ctx->tex_y >= ctx->texture->height)
		ctx->tex_y = ctx->texture->height - 1;
	ctx->tex_pos = fixed32_add(ctx->tex_pos, ctx->step);
	ctx->wall_color = ctx->texture->pixels[ctx->tex_y * ctx->texture->width
		+ ctx->tex_x];
	ctx->shaded_color = apply_shading(ctx->wall_color,
			from_fixed32(ray->perp_dist), ray->side);
	mylx_pixel_put(data, x, ctx->y, ctx->shaded_color);
}

void	draw_textured_wall(t_cub_data *data, int x, t_ray *ray, t_wall_ctx *ctx)
{
	ctx->y = ctx->draw_start;
	while (ctx->y <= ctx->draw_end)
	{
		draw_textured_pixel(data, x, ray, ctx);
		ctx->y++;
	}
}

void	draw_flat_wall(t_cub_data *data, int x, t_ray *ray, t_wall_ctx *ctx)
{
	ctx->wall_color = get_wall_color(ray, data->textures);
	ctx->shaded_color = apply_shading(ctx->wall_color,
			from_fixed32(ray->perp_dist), ray->side);
	ctx->y = ctx->draw_start;
	while (ctx->y <= ctx->draw_end)
	{
		mylx_pixel_put(data, x, ctx->y, ctx->shaded_color);
		ctx->y++;
	}
}
