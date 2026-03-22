/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemies_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 12:00:00 by joamiran          #+#    #+#             */
/*   Updated: 2026/03/22 17:51:07 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/gamelogic.h"
#include "../../includes/utils.h"

static int	count_enemies(t_map *map)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (i < map->width * map->height)
	{
		if (map->map_array[i] == 'X')
			count++;
		i++;
	}
	return (count);
}

static void	init_enemy_stats(t_enemy *enemy, int id)
{
	enemy->stats.health = ENEMY_MAX_HEALTH;
	enemy->stats.max_health = ENEMY_MAX_HEALTH;
	enemy->stats.damage = ENEMY_DAMAGE;
	enemy->stats.ammo = ENEMY_AMMO;
	enemy->stats.max_ammo = ENEMY_AMMO;
	enemy->state = ENEMY_IDLE;
	enemy->dir_x = to_fixed32(1.0f);
	enemy->dir_y = to_fixed32(0.0f);
	enemy->id = id;
	enemy->hit_time = 0;
}

static void	fill_enemies_array(t_cub_data *data)
{
	int	i;
	int	idx;
	int	x;
	int	y;

	i = 0;
	idx = 0;
	while (i < data->map->width * data->map->height)
	{
		if (data->map->map_array[i] == 'X')
		{
			index_to_coords(i, data->map->width, &x, &y);
			data->game->enemies[idx].x = to_fixed32((float)x + 0.5f);
			data->game->enemies[idx].y = to_fixed32((float)y + 0.5f);
			init_enemy_stats(&data->game->enemies[idx], idx);
			idx++;
		}
		i++;
	}
}

int	init_enemies(t_cub_data *data)
{
	if (!data || !data->map || !data->game)
		return (1);
	data->game->enemy_count = count_enemies(data->map);
	if (data->game->enemy_count == 0)
	{
		data->game->enemies = NULL;
		return (0);
	}
	data->game->enemies = ft_calloc(data->game->enemy_count, sizeof(t_enemy));
	if (!data->game->enemies)
		return (1);
	fill_enemies_array(data);
	return (0);
}

t_enemy	*get_enemy_at(t_cub_data *data, int x, int y)
{
	int	i;

	if (!data->game || !data->game->enemies)
		return (NULL);
	i = 0;
	while (i < data->game->enemy_count)
	{
		if (data->game->enemies[i].state != ENEMY_DEAD
			&& (int)from_fixed32(data->game->enemies[i].x) == x
			&& (int)from_fixed32(data->game->enemies[i].y) == y)
			return (&data->game->enemies[i]);
		i++;
	}
	return (NULL);
}
