/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemies_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 16:00:00 by joamiran          #+#    #+#             */
/*   Updated: 2026/02/23 04:10:10 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	init_enemies(t_cub_data *data)
{
	int	x;
	int	y;
	int	count;

	count = 0;
	y = 0;
	while (y < data->map->height)
	{
		x = -1;
		while (++x < data->map->width)
			if (data->map->map_array[y * data->map->width + x] == 'E')
				count++;
		y++;
	}
	data->game->enemy_count = count;
	data->game->enemies = ft_calloc(count, sizeof(t_enemy));
	if (!data->game->enemies)
		return (0);
	return (1);
}

void	update_enemies(t_cub_data *data, float delta_time)
{
	int		i;
	t_enemy	*enemy;

	(void)delta_time;
	i = 0;
	while (i < data->game->enemy_count)
	{
		enemy = &data->game->enemies[i];
		if (enemy->state != ENEMY_DEAD)
		{
		}
		i++;
	}
}

t_enemy	*get_enemy_at(t_cub_data *data, int x, int y)
{
	int	i;

	if (!data || !data->game || !data->game->enemies)
		return (NULL);
	i = 0;
	while (i < data->game->enemy_count)
	{
		if ((int)from_fixed32(data->game->enemies[i].x) == x
			&& (int)from_fixed32(data->game->enemies[i].y) == y)
			return (&data->game->enemies[i]);
		i++;
	}
	return (NULL);
}
