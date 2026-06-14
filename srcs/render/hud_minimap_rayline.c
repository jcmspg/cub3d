/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hud_minimap_rayline.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 01:05:20 by hladeiro          #+#    #+#             */
/*   Updated: 2026/04/08 01:05:21 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/hud_internal.h"

static void	clamp_ray_end(int *x, int *y, t_hud_ray_ctx *ctx)
{
	if (*x < ctx->minimap_x)
		*x = ctx->minimap_x;
	if (*x >= ctx->minimap_x + ctx->minimap_size)
		*x = ctx->minimap_x + ctx->minimap_size - 1;
	if (*y < ctx->minimap_y)
		*y = ctx->minimap_y;
	if (*y >= ctx->minimap_y + ctx->minimap_size)
		*y = ctx->minimap_y + ctx->minimap_size - 1;
}

static void	setup_bresenham(t_bresenham *line)
{
	line->dx = abs(line->x1 - line->x0);
	line->dy = abs(line->y1 - line->y0);
	line->sx = -1;
	line->sy = -1;
	if (line->x0 < line->x1)
		line->sx = 1;
	if (line->y0 < line->y1)
		line->sy = 1;
	line->err = line->dx - line->dy;
}

static void	step_bresenham(t_bresenham *line)
{
	line->e2 = 2 * line->err;
	if (line->e2 > -line->dy)
	{
		line->err -= line->dy;
		line->x0 += line->sx;
	}
	if (line->e2 < line->dx)
	{
		line->err += line->dx;
		line->y0 += line->sy;
	}
}

static void	draw_bresenham_line(t_cub_data *data, t_bresenham *line)
{
	while (1)
	{
		mylx_pixel_put(data, line->x0, line->y0, line->color);
		if (line->x0 == line->x1 && line->y0 == line->y1)
			break ;
		step_bresenham(line);
	}
}

void	draw_single_hud_ray(t_cub_data *data, t_hud_ray_ctx *ctx, t_ray *ray)
{
	float		hit_x;
	float		hit_y;
	t_bresenham	line;

	if (!ray->hit)
		return ;
	hit_x = from_fixed32(ray->dir_x) * from_fixed32(ray->perp_dist);
	hit_y = from_fixed32(ray->dir_y) * from_fixed32(ray->perp_dist);
	line.x0 = ctx->center_x;
	line.y0 = ctx->center_y;
	line.x1 = ctx->center_x + (int)(hit_x * ctx->cell_size);
	line.y1 = ctx->center_y + (int)(hit_y * ctx->cell_size);
	clamp_ray_end(&line.x1, &line.y1, ctx);
	line.color = 0x00AA00;
	setup_bresenham(&line);
	draw_bresenham_line(data, &line);
}
