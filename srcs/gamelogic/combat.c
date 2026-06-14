/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   combat.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 17:15:00 by joamiran          #+#    #+#             */
/*   Updated: 2026/04/08 00:38:05 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/gamelogic.h"
#include "../../includes/render.h"
#include "combat_internal.h"

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
