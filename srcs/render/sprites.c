/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprites.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 16:30:00 by joamiran          #+#    #+#             */
/*   Updated: 2026/04/06 21:51:42 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/render.h"
#include <math.h>

static int	g_clip_top;
static int	g_clip_bottom;
static int	g_start_x;
static int	g_end_x;
static int	g_start_y;
static int	g_end_y;
static int	g_sprite_scale_div;
static int	g_sprite_color;
static int	g_sprite_size;
static float	g_sprite_x;
static float	g_sprite_y;
static float	g_transform_y;
static float	g_inv;
static float	g_dir_x;
static float	g_dir_y;
static float	g_plane_x;
static float	g_plane_y;
static t_texture	*g_sprite_texture;

static bool	is_transparent_pixel(int pixel)
{
	return ((unsigned int)pixel == 0xFF000000U);
}

static void	update_billboard_camera(t_cub_data *data)
{
	g_dir_x = from_fixed32(data->player->dir_x);
	g_dir_y = from_fixed32(data->player->dir_y);
	g_plane_x = from_fixed32(data->player->plane_x);
	g_plane_y = from_fixed32(data->player->plane_y);
}

static void	set_billboard_x_span(t_cub_data *data)
{
	g_start_x = -(g_sprite_size / 2) + (int)((data->mlx->width / 2)
		* (1 + g_inv * (g_dir_y * (g_sprite_x - from_fixed32(data->player->x))
		- g_dir_x * (g_sprite_y - from_fixed32(data->player->y)))
		/ g_transform_y));
	g_end_x = (g_sprite_size / 2) + (int)((data->mlx->width / 2)
		* (1 + g_inv * (g_dir_y * (g_sprite_x - from_fixed32(data->player->x))
		- g_dir_x * (g_sprite_y - from_fixed32(data->player->y)))
		/ g_transform_y));
	if (g_start_x < 0)
		g_start_x = 0;
	if (g_end_x >= data->mlx->width)
		g_end_x = data->mlx->width - 1;
}

static void	set_billboard_y_span(t_cub_data *data)
{
	g_start_y = -g_sprite_size / 2 + data->mlx->height / 2
		+ data->player->view_offset + data->player->bob_offset
		+ (int)(data->mlx->height / (4 * g_transform_y));
	g_end_y = g_sprite_size / 2 + data->mlx->height / 2
		+ data->player->view_offset + data->player->bob_offset
		+ (int)(data->mlx->height / (4 * g_transform_y));
	if (g_start_y < 0)
		g_start_y = 0;
	if (g_end_y >= data->mlx->height)
		g_end_y = data->mlx->height - 1;
}

static void	get_door_coverage(t_cub_data *data, int stripe)
{
	t_ray	*ray;
	t_door	*door;

	g_clip_top = -1;
	g_clip_bottom = -1;
	ray = &data->raycasting->rays[stripe];
	if (!ray->door_hit)
		return ;
	g_start_y = data->player->view_offset + data->player->bob_offset;
	if (g_transform_y < from_fixed32(ray->door_dist))
		return ;
	door = get_door_at(data, ray->door_map_x, ray->door_map_y);
	if (!door)
		return ;
	g_sprite_size = (int)(data->mlx->height / from_fixed32(ray->door_dist));
	g_start_y = (data->mlx->height - g_sprite_size) / 2 + g_start_y;
	g_end_y = (data->mlx->height + g_sprite_size) / 2 + g_start_y;
	g_sprite_size = (int)(g_sprite_size * door->open_amount);
	g_clip_top = g_start_y - g_sprite_size;
	g_clip_bottom = g_end_y - g_sprite_size;
	if (g_clip_top < g_start_y)
		g_clip_top = g_start_y;
	if (g_clip_top < 0)
		g_clip_top = 0;
	if (g_clip_bottom >= data->mlx->height)
		g_clip_bottom = data->mlx->height - 1;
}

static bool	compute_billboard_span(t_cub_data *data)
{
	update_billboard_camera(data);
	g_inv = 1.0f / (g_plane_x * g_dir_y - g_dir_x * g_plane_y);
	g_transform_y = g_inv * (-g_plane_y * (g_sprite_x - from_fixed32(data->player->x))
		+ g_plane_x * (g_sprite_y - from_fixed32(data->player->y)));
	if (g_transform_y <= 0.1f)
		return (false);
	g_sprite_size = abs((int)(data->mlx->height / g_transform_y))
		/ g_sprite_scale_div;
	set_billboard_x_span(data);
	set_billboard_y_span(data);
	return (g_end_x > g_start_x && g_end_y > g_start_y);
}

static void	draw_sprite_stripe_color(t_cub_data *data, int stripe)
{
	int	y;

	y = g_start_y;
	while (y < g_end_y)
	{
		if (y >= 0 && y < data->mlx->height)
		{
			if (g_clip_top >= 0 && y >= g_clip_top && y <= g_clip_bottom)
			{
				y++;
				continue ;
			}
			mylx_pixel_put(data, stripe, y, g_sprite_color);
		}
		y++;
	}
}

static void	draw_sprite_stripe_tex(t_cub_data *data, int stripe)
{
	int	y;
	int	tx;
	int	ty;
	int	pixel;

	tx = ((stripe - g_start_x) * g_sprite_texture->width)
		/ (g_end_x - g_start_x);
	y = g_start_y;
	while (y < g_end_y)
	{
		if (y >= 0 && y < data->mlx->height && !(g_clip_top >= 0
				&& y >= g_clip_top && y <= g_clip_bottom))
		{
			ty = ((y - g_start_y) * g_sprite_texture->height)
				/ (g_end_y - g_start_y);
			if (ty < 0)
				ty = 0;
			if (ty >= g_sprite_texture->height)
				ty = g_sprite_texture->height - 1;
			pixel = get_texture_pixel(g_sprite_texture, tx, ty);
			if (!is_transparent_pixel(pixel))
				mylx_pixel_put(data, stripe, y, pixel);
		}
		y++;
	}
}

static void	render_billboard(t_cub_data *data)
{
	int	stripe;
	float	wall_dist;

	if (!compute_billboard_span(data))
		return ;
	stripe = g_start_x;
	while (stripe < g_end_x)
	{
		wall_dist = from_fixed32(data->raycasting->rays[stripe].perp_dist);
		if (g_transform_y < wall_dist)
		{
			get_door_coverage(data, stripe);
			if (g_sprite_texture && g_sprite_texture->loaded
				&& g_sprite_texture->width > 0 && g_sprite_texture->height > 0)
				draw_sprite_stripe_tex(data, stripe);
			else
				draw_sprite_stripe_color(data, stripe);
		}
		stripe++;
	}
}

static void	render_ammo_sprite(t_cub_data *data, float sx, float sy)
{
	g_sprite_x = sx;
	g_sprite_y = sy;
	g_sprite_scale_div = 4;
	g_sprite_color = 0xFFD700;
	g_sprite_texture = NULL;
	if (data->textures)
		g_sprite_texture = &data->textures->ammo;
	render_billboard(data);
}

static void	render_enemy_sprite(t_cub_data *data, t_enemy *enemy,
		t_texture *demon_texture)
{
	g_sprite_x = from_fixed32(enemy->x);
	g_sprite_y = from_fixed32(enemy->y);
	g_sprite_scale_div = 2;
	g_sprite_color = 0xFF0000;
	g_sprite_texture = demon_texture;
	render_billboard(data);
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
