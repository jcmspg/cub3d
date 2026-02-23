/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprites.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 16:30:00 by joamiran          #+#    #+#             */
/*   Updated: 2026/02/23 03:52:10 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/render.h"
#include <math.h>

static void	draw_sprite_stripe(t_cub_data *data, t_sprite_render *sr)
{
	int	y;

	y = sr->d_start[1];
	while (y < sr->d_end[1])
	{
		if (y >= 0 && y < data->mlx->height)
			mylx_pixel_put(data, sr->stripe, y, sr->color);
		y++;
	}
}

static void	setup_billboard(t_cub_data *data, float sx, float sy,
		t_sprite_render *sr)
{
	float	spr[2];
	float	inv_det;
	float	p[2];
	float	d[2];
	int		off;

	spr[0] = sx - from_fixed32(data->player->x);
	spr[1] = sy - from_fixed32(data->player->y);
	p[0] = from_fixed32(data->player->plane_x);
	p[1] = from_fixed32(data->player->plane_y);
	d[0] = from_fixed32(data->player->dir_x);
	d[1] = from_fixed32(data->player->dir_y);
	inv_det = 1.0 / (p[0] * d[1] - d[0] * p[1]);
	sr->trans[0] = inv_det * (d[1] * spr[0] - d[0] * spr[1]);
	sr->trans[1] = inv_det * (-p[1] * spr[0] + p[0] * spr[1]);
	sr->spr_screen_x = (int)((data->mlx->width / 2) * (1 + sr->trans[0]
				/ sr->trans[1]));
	sr->spr_h = abs((int)(data->mlx->height / sr->trans[1])) / sr->scale_div;
	off = (int)(data->mlx->height / (4 * sr->trans[1]));
	sr->d_start[1] = -sr->spr_h / 2 + data->mlx->height / 2 + off;
	sr->d_end[1] = sr->spr_h / 2 + data->mlx->height / 2 + off;
	sr->d_start[0] = -sr->spr_h / 2 + sr->spr_screen_x;
	sr->d_end[0] = sr->spr_h / 2 + sr->spr_screen_x;
}

void	render_billboard(t_cub_data *data, float sx, float sy,
		t_sprite_render *sr)
{
	setup_billboard(data, sx, sy, sr);
	if (sr->trans[1] <= 0.1f)
		return ;
	sr->stripe = sr->d_start[0] - 1;
	while (++sr->stripe < sr->d_end[0])
	{
		if (sr->stripe >= 0 && sr->stripe < data->mlx->width && sr->trans[1]
			< from_fixed32(data->raycasting->rays[sr->stripe].perp_dist))
			draw_sprite_stripe(data, sr);
	}
}

void	render_sprites(t_cub_data *data)
{
	t_sprite_render	sr;
	int				x;
	int				y;

	if (!data || !data->map || !data->raycasting)
		return ;
	y = -1;
	while (++y < data->map->height)
	{
		x = -1;
		while (++x < data->map->width)
		{
			if (data->map->map_array[y * data->map->width + x] == 'M')
			{
				sr.color = 0xFFD700;
				sr.scale_div = 4;
				render_billboard(data, (float)x + 0.5f, (float)y + 0.5f, &sr);
			}
		}
	}
	render_enemies(data);
}

int	init_sprites(t_cub_data *data)
{
	(void)data;
	return (0);
}
