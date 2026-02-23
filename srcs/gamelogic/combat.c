/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   combat.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 17:00:00 by joamiran          #+#    #+#             */
/*   Updated: 2026/02/23 01:45:10 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	player_shoot(t_cub_data *data)
{
	if (data->player->stats.ammo > 0)
	{
		data->player->stats.ammo--;
		handle_player_combat(data);
	}
	else
		ft_putstr_fd("Out of ammo!\n", 1);
}

void	handle_player_combat(t_cub_data *data)
{
	int		i;
	t_enemy	*enemy;

	i = 0;
	while (i < data->game->enemy_count)
	{
		enemy = &data->game->enemies[i];
		if (enemy->state != ENEMY_DEAD)
		{
			enemy->stats.health -= data->player->stats.damage;
			if (enemy->stats.health <= 0)
				enemy->state = ENEMY_DEAD;
		}
		i++;
	}
}
