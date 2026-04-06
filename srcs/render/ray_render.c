/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_render.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 20:00:00 by joamiran          #+#    #+#             */
/*   Updated: 2026/04/06 18:35:00 by copilot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"
#include "../../includes/gamelogic.h"

struct				s_wall_ctx
{
	int				line_height;
	int				draw_start;
	int				draw_end;
	int				y;
	int				wall_color;
	int				shaded_color;
	int				tex_x;
	int				tex_y;
	t_texture		*texture;
	t_fixed32		step;
	t_fixed32		tex_pos;
};

struct				s_door_ctx
{
	int				line_height;
	int				frame_top;
	int				frame_bottom;
	int				render_top;
	int				render_bottom;
	int				view_offset;
	int				offset;
	int				draw_start;
	int				draw_end;
	int				tex_x;
	int				tex_y;
	int				tex_color;
	int				door_color;
	int				draw_fallback;
	t_door			*door;
	t_fixed32		player_pos;
	t_fixed32		map_pos;
	t_fixed32		step_val;
	t_fixed32		dir_val;
	t_fixed32		euclidean_dist;
	t_fixed32		p_dir_x;
	t_fixed32		p_dir_y;
	t_fixed32		r_dir_x;
	t_fixed32		r_dir_y;
	t_fixed32		dot_prod;
	t_fixed32		perp_dist;
	t_fixed32		dist;
	t_fixed32		wall_x;
	t_fixed32		door_dist;
	int				door_side;
};

static int	calculate_wall_slice(t_cub_data *data, t_ray *ray, int *draw_start,
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

static int	apply_shading(int color, float dist, int side)
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

static int	calculate_texture_x(t_cub_data *data, t_ray *ray,
		t_texture *texture)
{
	t_fixed32	wall_x;
	t_fixed32	raw_dist;
	int			tex_x;

	if (!texture || !texture->loaded || texture->width == 0)
		return (0);
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

static void	init_wall_ctx(t_cub_data *data, t_ray *ray, struct s_wall_ctx *ctx)
{
	ctx->line_height = calculate_wall_slice(data, ray, &ctx->draw_start,
			&ctx->draw_end);
	ctx->texture = get_wall_texture(ray, data->textures);
}

static void	draw_textured_wall(t_cub_data *data, int x, t_ray *ray,
		struct s_wall_ctx *ctx)
{
	ctx->tex_x = calculate_texture_x(data, ray, ctx->texture);
	ctx->step = fixed32_div(to_fixed32(ctx->texture->height),
			to_fixed32(ctx->line_height));
	ctx->tex_pos = fixed32_mul(to_fixed32(ctx->draw_start - (data->mlx->height
					- ctx->line_height) / 2 - data->player->view_offset
				- data->player->bob_offset), ctx->step);
	ctx->y = ctx->draw_start;
	while (ctx->y <= ctx->draw_end)
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
		ctx->y++;
	}
}

static void	draw_flat_wall(t_cub_data *data, int x, t_ray *ray,
		struct s_wall_ctx *ctx)
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

static void	draw_wall_slice(t_cub_data *data, int x, t_ray *ray)
{
	struct s_wall_ctx	ctx;

	if (!ray->hit)
		return ;
	init_wall_ctx(data, ray, &ctx);
	if (ctx.texture && ctx.texture->loaded && ctx.texture->pixels)
		draw_textured_wall(data, x, ray, &ctx);
	else
		draw_flat_wall(data, x, ray, &ctx);
}

static void	draw_ceiling_slice(t_cub_data *data, int x, int wall_start)
{
	int	y;
	int	color;

	color = data->textures->ceiling_color;
	y = 0;
	while (y < wall_start)
	{
		mylx_pixel_put(data, x, y, color);
		y++;
	}
}

static void	draw_floor_slice(t_cub_data *data, int x, int wall_end)
{
	int	y;
	int	color;

	color = data->textures->floor_color;
	y = wall_end + 1;
	while (y < data->mlx->height)
	{
		mylx_pixel_put(data, x, y, color);
		y++;
	}
}

static void	set_door_axis_values(t_cub_data *data, t_ray *ray,
		struct s_door_ctx *ctx)
{
	if (ray->door_side == 0)
	{
		ctx->player_pos = data->player->x;
		ctx->map_pos = to_fixed32(ray->door_map_x);
		ctx->step_val = to_fixed32(ray->step_x);
		ctx->dir_val = ray->dir_x;
	}
	else
	{
		ctx->player_pos = data->player->y;
		ctx->map_pos = to_fixed32(ray->door_map_y);
		ctx->step_val = to_fixed32(ray->step_y);
		ctx->dir_val = ray->dir_y;
	}
	if (ctx->dir_val == 0)
		ctx->dir_val = 1;
}

static void	set_door_distance_data(t_cub_data *data, t_ray *ray,
		struct s_door_ctx *ctx)
{
	set_door_axis_values(data, ray, ctx);
	ctx->euclidean_dist = fixed32_div(fixed32_add(fixed32_sub(ctx->map_pos,
					ctx->player_pos), fixed32_div(fixed32_sub(to_fixed32(1.0f),
						ctx->step_val), to_fixed32(2.0f))), ctx->dir_val);
	ctx->p_dir_x = data->player->dir_x;
	ctx->p_dir_y = data->player->dir_y;
	ctx->r_dir_x = ray->dir_x;
	ctx->r_dir_y = ray->dir_y;
	ctx->dot_prod = fixed32_add(fixed32_mul(ctx->r_dir_x, ctx->p_dir_x),
			fixed32_mul(ctx->r_dir_y, ctx->p_dir_y));
	ctx->perp_dist = fixed32_mul(ctx->euclidean_dist, ctx->dot_prod);
	if (ctx->perp_dist < to_fixed32(0.1f))
		ctx->perp_dist = to_fixed32(0.1f);
	ctx->dist = ctx->perp_dist;
	ctx->door_dist = ray->door_dist;
	ctx->door_side = ray->door_side;
}

static void	set_door_screen_bounds(t_cub_data *data, t_ray *ray,
		struct s_door_ctx *ctx)
{
	ctx->line_height = (int)from_fixed32(fixed32_div(to_fixed32(data->mlx->height),
				ctx->dist));
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

static void	set_door_texture_x(t_cub_data *data, t_ray *ray,
		struct s_door_ctx *ctx)
{
	if (ray->door_side == 0)
		ctx->wall_x = fixed32_add(data->player->y,
				fixed32_mul(ctx->euclidean_dist, ctx->r_dir_y));
	else
		ctx->wall_x = fixed32_add(data->player->x,
				fixed32_mul(ctx->euclidean_dist, ctx->r_dir_x));
	ctx->wall_x = fixed32_sub(ctx->wall_x,
			to_fixed32((float)(int)from_fixed32(ctx->wall_x)));
	ctx->tex_x = (int)from_fixed32(fixed32_mul(ctx->wall_x,
				to_fixed32(data->textures->door.width)));
	if ((ray->door_side == 0 && ctx->r_dir_x > 0) || (ray->door_side == 1
			&& ctx->r_dir_y < 0))
		ctx->tex_x = data->textures->door.width - ctx->tex_x - 1;
	if (ctx->tex_x < 0)
		ctx->tex_x = 0;
	if (ctx->tex_x >= data->textures->door.width)
		ctx->tex_x = data->textures->door.width - 1;
}

static void	draw_door_pixel(t_cub_data *data, int x, int y,
		struct s_door_ctx *ctx)
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

static void	draw_door_column(t_cub_data *data, int x, t_ray *ray,
		struct s_door_ctx *ctx)
{
	int	y;

	y = ctx->draw_start;
	while (y <= ctx->draw_end)
	{
		draw_door_pixel(data, x, y, ctx);
		y++;
	}
}

static void	draw_door_slice(t_cub_data *data, int x, t_ray *ray)
{
	struct s_door_ctx	ctx;

	set_door_distance_data(data, ray, &ctx);
	set_door_screen_bounds(data, ray, &ctx);
	if (ctx.draw_start > ctx.draw_end)
		return ;
	set_door_texture_x(data, ray, &ctx);
	ctx.door_color = apply_shading(0x8B4513, from_fixed32(ctx.door_dist),
			ctx.door_side);
	draw_door_column(data, x, ray, &ctx);
}

static void	render_column(t_cub_data *data, int x, t_ray *ray)
{
	int	draw_start;
	int	draw_end;

	if (!ray->hit)
	{
		draw_start = data->mlx->height / 2;
		draw_end = data->mlx->height / 2;
	}
	else
		calculate_wall_slice(data, ray, &draw_start, &draw_end);
	draw_ceiling_slice(data, x, draw_start);
	if (ray->hit)
	{
		if (ray->hit_content == 'D')
			draw_door_slice(data, x, ray);
		else
			draw_wall_slice(data, x, ray);
	}
	draw_floor_slice(data, x, draw_end);
	if (ray->door_hit && ray->hit_content != 'D')
		draw_door_slice(data, x, ray);
}

void	render_walls(t_cub_data *data)
{
	int	x;

	if (!data || !data->raycasting || !data->raycasting->rays)
		return ;
	if (!data->textures)
		return ;
	x = 0;
	while (x < data->raycasting->num_rays)
	{
		render_column(data, x, &data->raycasting->rays[x]);
		x++;
	}
}
