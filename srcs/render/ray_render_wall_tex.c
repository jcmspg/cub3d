/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_render_wall_tex.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 01:11:52 by hladeiro          #+#    #+#             */
/*   Updated: 2026/04/08 01:25:36 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ray_render_internal.h"

static t_texture	*get_wall_texture(t_ray *ray, t_textures *textures)
{
	if (ray->hit_content == 'D')
		return (NULL);
	if (ray->side == 0)
	{
		if (ray->step_x > 0)
			return (&textures->walls[TEX_WEST]);
		return (&textures->walls[TEX_EAST]);
	}
	if (ray->step_y > 0)
		return (&textures->walls[TEX_NORTH]);
	return (&textures->walls[TEX_SOUTH]);
}

static t_fixed32	calculate_wall_hit_x(t_cub_data *data, t_ray *ray)
{
	t_fixed32	raw_dist;
	t_fixed32	wall_x;

	if (ray->side == 0)
	{
		raw_dist = fixed32_sub(ray->side_dist_x, ray->delta_dist_x);
		wall_x = fixed32_add(data->player->y, fixed32_mul(raw_dist,
					ray->dir_y));
	}
	else
	{
		raw_dist = fixed32_sub(ray->side_dist_y, ray->delta_dist_y);
		wall_x = fixed32_add(data->player->x, fixed32_mul(raw_dist,
					ray->dir_x));
	}
	wall_x = fixed32_sub(wall_x, to_fixed32((float)(int)from_fixed32(wall_x)));
	return (wall_x);
}

static int	calculate_texture_x(t_cub_data *data, t_ray *ray,
		t_texture *texture)
{
	t_fixed32	wall_x;
	int			tex_x;

	if (!texture || !texture->loaded || texture->width == 0)
		return (0);
	wall_x = calculate_wall_hit_x(data, ray);
	tex_x = (int)from_fixed32(fixed32_mul(wall_x, to_fixed32(texture->width)));
	if ((ray->side == 0 && ray->dir_x > 0) || (ray->side == 1
			&& ray->dir_y < 0))
		tex_x = texture->width - tex_x - 1;
	if (tex_x < 0)
		tex_x = 0;
	if (tex_x >= texture->width)
		tex_x = texture->width - 1;
	return (tex_x);
}

void	draw_wall_slice(t_cub_data *data, int x, t_ray *ray)
{
	t_wall_ctx	ctx;

	if (!ray->hit)
		return ;
	ctx.line_height = calculate_wall_slice(data, ray, &ctx.draw_start,
			&ctx.draw_end);
	ctx.texture = get_wall_texture(ray, data->textures);
	if (!ctx.texture || !ctx.texture->loaded || !ctx.texture->pixels)
	{
		draw_flat_wall(data, x, ray, &ctx);
		return ;
	}
	ctx.tex_x = calculate_texture_x(data, ray, ctx.texture);
	ctx.step = fixed32_div(to_fixed32(ctx.texture->height),
			to_fixed32(ctx.line_height));
	ctx.tex_pos = fixed32_mul(to_fixed32(ctx.draw_start - (data->mlx->height
					- ctx.line_height) / 2 - data->player->view_offset
				- data->player->bob_offset), ctx.step);
	draw_textured_wall(data, x, ray, &ctx);
}
