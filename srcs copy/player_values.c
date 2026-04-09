/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_values.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malourei <malourei@student.42lisboa.pt>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 01:36:32 by malourei          #+#    #+#             */
/*   Updated: 2025/06/08 01:55:23 by malourei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

static void	set_player_values(t_player *player, char c, int i, int j)
{
	player->coor_x = j;
	player->coor_y = i;
	if (c == 'N')
		player->dir = PI / 2;
	else if (c == 'S')
		player->dir = (2 * PI) / 3;
	else if (c == 'E')
		player->dir = PI;
	else if (c == 'W')
		player->dir = 2 * PI;
}

void	player_values(t_map *map)
{
	int		i;
	int		j;
	char	c;

	i = 0;
	while (map->map[i])
	{
		j = 0;
		while (map->map[i][j])
		{
			c = map->map[i][j];
			if (c == 'N' || c == 'S' || c == 'W' || c == 'E')
				set_player_values(&map->player, c, i, j);
			j++;
		}
		i++;
	}
	printf("Player x: %d\n", map->player.coor_x);
	printf("Player y: %d\n", map->player.coor_y);
	printf("Player dir: %f\n", map->player.dir);
}
