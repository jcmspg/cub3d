/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   combat.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 17:15:00 by joamiran          #+#    #+#             */
/*   Updated: 2026/03/22 19:36:00 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/gamelogic.h"
#include "../../includes/render.h"

typedef struct s_shot_view
{
	int		center_idx;
	int		screen_width;
	int		center_min;
	int		center_max;
	float		dir_x;
	float		dir_y;
	float		plane_x;
	float		plane_y;
	float		wall_dist;
}	t_shot_view;

typedef struct s_enemy_proj
{
	float		sprite_x;
	float		sprite_y;
	float		transform_x;
	float		transform_y;
	int		screen_x;
}	t_enemy_proj;

static bool	can_process_shot(t_cub_data *data)
{
	if (!data || !data->player || !data->input)
		return (false);
	if (!data->mlx || !data->raycasting || !data->raycasting->rays)
		return (false);
	if (!data->game || !data->game->enemies)
		return (false);
	return (true);
}

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
	view->wall_dist = from_fixed32(data->raycasting->rays[view->center_idx].perp_dist);
	return (true);
}

static bool	project_enemy(t_cub_data *data, t_enemy *enemy, t_shot_view *view,
		t_enemy_proj *proj)
{
	float	inv_det;

	proj->sprite_x = from_fixed32(enemy->x) - from_fixed32(data->player->x);
	proj->sprite_y = from_fixed32(enemy->y) - from_fixed32(data->player->y);
	inv_det = 1.0f / (view->plane_x * view->dir_y - view->dir_x * view->plane_y);
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

static void	process_shot_hits(t_cub_data *data)
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

static bool	consume_shot_if_pressed(t_cub_data *data, bool *was_shooting)
{
	if (!data->input->shoot || *was_shooting)
		return (false);
	if (data->player->stats.ammo <= 0)
	{
		printf("*click* Out of ammo!\n");
		*was_shooting = data->input->shoot;
		return (false);
	}
	data->player->stats.ammo--;
	printf("BANG! Ammo left: %d\n", data->player->stats.ammo);
	return (true);
}

void	player_shoot(t_cub_data *data)
{
	static bool	was_shooting = false;

	if (!can_process_shot(data))
		return ;
	if (consume_shot_if_pressed(data, &was_shooting))
		process_shot_hits(data);
	was_shooting = data->input->shoot;
}
