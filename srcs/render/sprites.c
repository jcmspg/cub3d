/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprites.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 16:30:00 by joamiran          #+#    #+#             */
/*   Updated: 2026/03/22 19:36:00 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/render.h"
#include <math.h>

struct s_door_clip
{
	int	top;
	int	bottom;
};

struct s_billboard
{
	float	sx;
	float	sy;
	int		color;
	int		scale_div;
	t_texture	*texture;
};

struct s_draw_span
{
	int	start_x;
	int	end_x;
	int	start_y;
	int	end_y;
	float	transform_y;
};

static bool	is_transparent_pixel(int pixel)
{
	return ((unsigned int)pixel == 0xFF000000U);
}

static void	get_door_coverage(t_cub_data *data, int stripe, float sprite_dist,
		struct s_door_clip *clip)
{
	t_ray	*ray;
	t_door	*door;
	float	door_dist;
	int		line_height;
	int		frame_top;
	int		offset;
	int		view_offset;

	clip->top = -1;
	clip->bottom = -1;
	ray = &data->raycasting->rays[stripe];
	if (!ray->door_hit)
		return ;
	door_dist = from_fixed32(ray->door_dist);
	if (sprite_dist < door_dist)
		return ;
	door = get_door_at(data, ray->door_map_x, ray->door_map_y);
	if (!door)
		return ;
	line_height = (int)(data->mlx->height / door_dist);
	view_offset = data->player->view_offset + data->player->bob_offset;
	frame_top = (data->mlx->height - line_height) / 2 + view_offset;
	offset = (int)(line_height * door->open_amount);
	clip->top = frame_top - offset;
	clip->bottom = (data->mlx->height + line_height) / 2 + view_offset - offset;
	if (clip->top < frame_top)
		clip->top = frame_top;
	if (clip->top < 0)
		clip->top = 0;
	if (clip->bottom >= data->mlx->height)
		clip->bottom = data->mlx->height - 1;
}

static void	draw_sprite_stripe_color(t_cub_data *data, int stripe,
		struct s_draw_span *span, struct s_door_clip *clip, int color)
{
	int	y;

	y = span->start_y;
	while (y < span->end_y)
	{
		if (y >= 0 && y < data->mlx->height)
		{
			if (clip->top >= 0 && y >= clip->top && y <= clip->bottom)
			{
				y++;
				continue ;
			}
			mylx_pixel_put(data, stripe, y, color);
		}
		y++;
	}
}

static void	draw_sprite_stripe_tex(t_cub_data *data, int stripe,
		struct s_draw_span *span, struct s_door_clip *clip, t_texture *texture)
{
	int	y;
	int	tx;
	int	ty;
	int	pixel;

	tx = ((stripe - span->start_x) * texture->width) / (span->end_x
			- span->start_x);
	y = span->start_y;
	while (y < span->end_y)
	{
		if (y >= 0 && y < data->mlx->height && !(clip->top >= 0
				&& y >= clip->top && y <= clip->bottom))
		{
			ty = ((y - span->start_y) * texture->height) / (span->end_y
					- span->start_y);
			if (ty < 0)
				ty = 0;
			if (ty >= texture->height)
				ty = texture->height - 1;
			pixel = get_texture_pixel(texture, tx, ty);
			if (!is_transparent_pixel(pixel))
				mylx_pixel_put(data, stripe, y, pixel);
		}
		y++;
	}
}

static bool	compute_billboard_span(t_cub_data *data, struct s_billboard *bb,
		struct s_draw_span *span)
{
	float	sx;
	float	sy;
	float	dir_x;
	float	dir_y;
	float	pl_x;
	float	pl_y;
	float	inv;
	int		size;

	sx = bb->sx - from_fixed32(data->player->x);
	sy = bb->sy - from_fixed32(data->player->y);
	dir_x = from_fixed32(data->player->dir_x);
	dir_y = from_fixed32(data->player->dir_y);
	pl_x = from_fixed32(data->player->plane_x);
	pl_y = from_fixed32(data->player->plane_y);
	inv = 1.0f / (pl_x * dir_y - dir_x * pl_y);
	span->transform_y = inv * (-pl_y * sx + pl_x * sy);
	if (span->transform_y <= 0.1f)
		return (false);
	size = abs((int)(data->mlx->height / span->transform_y)) / bb->scale_div;
	span->start_x = -(size / 2) + (int)((data->mlx->width / 2) * (1 + inv
					* (dir_y * sx - dir_x * sy) / span->transform_y));
	span->end_x = (size / 2) + (int)((data->mlx->width / 2) * (1 + inv
					* (dir_y * sx - dir_x * sy) / span->transform_y));
	span->start_y = -size / 2 + data->mlx->height / 2
		+ data->player->view_offset + data->player->bob_offset
		+ (int)(data->mlx->height / (4 * span->transform_y));
	span->end_y = size / 2 + data->mlx->height / 2
		+ data->player->view_offset + data->player->bob_offset
		+ (int)(data->mlx->height / (4 * span->transform_y));
	if (span->start_x < 0)
		span->start_x = 0;
	if (span->end_x >= data->mlx->width)
		span->end_x = data->mlx->width - 1;
	if (span->start_y < 0)
		span->start_y = 0;
	if (span->end_y >= data->mlx->height)
		span->end_y = data->mlx->height - 1;
	return (span->end_x > span->start_x && span->end_y > span->start_y);
}

static void	render_billboard(t_cub_data *data, struct s_billboard *bb)
{
	int				stripe;
	float			wall_dist;
	struct s_door_clip	clip;
	struct s_draw_span	span;

	if (!compute_billboard_span(data, bb, &span))
		return ;
	stripe = span.start_x;
	while (stripe < span.end_x)
	{
		wall_dist = from_fixed32(data->raycasting->rays[stripe].perp_dist);
		if (span.transform_y < wall_dist)
		{
			get_door_coverage(data, stripe, span.transform_y, &clip);
			if (bb->texture && bb->texture->loaded && bb->texture->width > 0
				&& bb->texture->height > 0)
				draw_sprite_stripe_tex(data, stripe, &span, &clip, bb->texture);
			else
				draw_sprite_stripe_color(data, stripe, &span, &clip, bb->color);
		}
		stripe++;
	}
}

static void	render_ammo_sprite(t_cub_data *data, float sx, float sy)
{
	struct s_billboard	bb;

	bb.sx = sx;
	bb.sy = sy;
	bb.color = 0xFFD700;
	bb.scale_div = 4;
	bb.texture = NULL;
	if (data->textures)
		bb.texture = &data->textures->ammo;
	render_billboard(data, &bb);
}

static void	render_enemy_sprite(t_cub_data *data, t_enemy *enemy,
		t_texture *demon_texture)
{
	struct s_billboard	bb;

	bb.sx = from_fixed32(enemy->x);
	bb.sy = from_fixed32(enemy->y);
	bb.color = 0xFF0000;
	bb.scale_div = 2;
	bb.texture = demon_texture;
	render_billboard(data, &bb);
}

static bool	enemy_should_draw(t_enemy *enemy, uint64_t now)
{
	uint64_t	elapsed;
	int			period;
	int			total;

	if (enemy->state == ENEMY_DEAD)
		return (false);
	if (enemy->state != ENEMY_HIT)
		return (true);
	elapsed = now - enemy->hit_time;
	period = 300;
	total = enemy->blink_count * period;
	if ((int)elapsed >= total)
	{
		if (enemy->blink_count == 2)
			enemy->state = ENEMY_DEAD;
		else
			enemy->state = ENEMY_IDLE;
		return (false);
	}
	return (((int)elapsed % period) < (period / 2));
}

static void	render_enemies(t_cub_data *data)
{
	int				i;
	t_texture		*demon_texture;
	uint64_t		now;
	t_enemy			*enemy;

	if (!data->game || !data->game->enemies)
		return ;
	demon_texture = NULL;
	if (data->textures)
		demon_texture = &data->textures->demon;
	now = data->fps.last_frame_time;
	i = 0;
	while (i < data->game->enemy_count)
	{
		enemy = &data->game->enemies[i];
		if (enemy_should_draw(enemy, now))
			render_enemy_sprite(data, enemy, demon_texture);
		i++;
	}
}

void	render_sprites(t_cub_data *data)
{
	int	x;
	int	y;

	if (!data || !data->map || !data->raycasting)
		return ;
	y = 0;
	while (y < data->map->height)
	{
		x = 0;
		while (x < data->map->width)
		{
			if (data->map->map_array[y * data->map->width + x] == 'M')
				render_ammo_sprite(data, (float)x + 0.5f, (float)y + 0.5f);
			x++;
		}
		y++;
	}
	render_enemies(data);
}
