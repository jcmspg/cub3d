/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   combat_hit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 00:39:19 by hladeiro          #+#    #+#             */
/*   Updated: 2026/04/08 00:39:24 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/gamelogic.h"
#include "../../includes/render.h"
#include "combat_internal.h"
#include <math.h>

static bool	init_shot_view(t_cub_data *data, t_shot_view *view)
{
	view->center_idx = data->mlx->width / 2;
	if (view->center_idx < 0 || view->center_idx >= data->raycasting->num_rays)
		return (false);
	view->screen_width = data->mlx->width;
	view->center_min = view->screen_width / 2 - view->screen_width / 10;
	view->center_max = view->screen_width / 2 + view->screen_width / 10;
	view->dir_x = from_fixed32(data->player->dir_x);
	view->dir_y = from_fixed32(data->player->dir_y);
	view->plane_x = from_fixed32(data->player->plane_x);
	view->plane_y = from_fixed32(data->player->plane_y);
	view->wall_dist = from_fixed32(
			data->raycasting->rays[view->center_idx].perp_dist);
	return (true);
}

static bool	project_enemy(t_cub_data *data, t_enemy *enemy, t_shot_view *view,
		t_enemy_proj *proj)
{
	float	inv_det;

	proj->sprite_x = from_fixed32(enemy->x) - from_fixed32(data->player->x);
	proj->sprite_y = from_fixed32(enemy->y) - from_fixed32(data->player->y);
	inv_det = 1.0f / (view->plane_x * view->dir_y - view->dir_x
			* view->plane_y);
	proj->transform_x = inv_det * (view->dir_y * proj->sprite_x - view->dir_x
			* proj->sprite_y);
	proj->transform_y = inv_det * (-view->plane_y * proj->sprite_x
			+ view->plane_x * proj->sprite_y);
	if (proj->transform_y <= 0.1f)
		return (false);
	proj->screen_x = (int)((view->screen_width / 2) * (1 + proj->transform_x
				/ proj->transform_y));
	return (true);
}

static void	register_enemy_hit(t_cub_data *data, t_enemy *enemy)
{
	enemy->stats.health -= BULLET_DMG;
	printf("Enemy %d hit! HP: %d\n", enemy->id, enemy->stats.health);
	enemy->state = ENEMY_HIT;
	enemy->hit_time = data->fps.last_frame_time;
	if (enemy->stats.health <= 0)
	{
		enemy->blink_count = 2;
		printf("Enemy %d killed!\n", enemy->id);
	}
	else
		enemy->blink_count = 1;
	enemy->blink_phase = 1;
}

static bool	try_hit_enemy(t_cub_data *data, t_enemy *enemy, t_shot_view *view)
{
	t_enemy_proj	proj;
	float			enemy_dist;

	if (enemy->state == ENEMY_DEAD)
		return (false);
	if (!project_enemy(data, enemy, view, &proj))
		return (false);
	if (proj.screen_x < view->center_min || proj.screen_x > view->center_max)
		return (false);
	enemy_dist = sqrtf(proj.sprite_x * proj.sprite_x + proj.sprite_y
			* proj.sprite_y);
	if (enemy_dist >= view->wall_dist)
		return (false);
	register_enemy_hit(data, enemy);
	return (true);
}

void	process_shot_hits(t_cub_data *data)
{
	t_shot_view	view;
	int			i;

	if (!init_shot_view(data, &view))
		return ;
	i = 0;
	while (i < data->game->enemy_count)
	{
		if (try_hit_enemy(data, &data->game->enemies[i], &view))
			break ;
		i++;
	}
}
