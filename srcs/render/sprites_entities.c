/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprites_entities.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 01:27:53 by hladeiro          #+#    #+#             */
/*   Updated: 2026/04/08 01:27:53 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sprites_internal.h"

void	render_ammo_sprite(t_cub_data *data, float sx, float sy)
{
	t_sprite_ctx	ctx;

	ctx.sprite_x = sx;
	ctx.sprite_y = sy;
	ctx.sprite_scale_div = 4;
	ctx.sprite_color = 0xFFD700;
	ctx.sprite_texture = NULL;
	if (data->textures)
		ctx.sprite_texture = &data->textures->ammo;
	render_billboard(data, &ctx);
}

void	render_enemy_sprite(t_cub_data *data, t_enemy *enemy,
		t_texture *demon_texture)
{
	t_sprite_ctx	ctx;

	ctx.sprite_x = from_fixed32(enemy->x);
	ctx.sprite_y = from_fixed32(enemy->y);
	ctx.sprite_scale_div = 2;
	ctx.sprite_color = 0xFF0000;
	ctx.sprite_texture = demon_texture;
	render_billboard(data, &ctx);
}

bool	enemy_should_draw(t_enemy *enemy, uint64_t now)
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

void	render_enemies(t_cub_data *data)
{
	int			i;
	t_texture	*demon_texture;
	uint64_t	now;
	t_enemy		*enemy;

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
