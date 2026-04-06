/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pickups.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 16:10:00 by joamiran          #+#    #+#             */
/*   Updated: 2026/02/08 16:10:00 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/gamelogic.h"

/**
 * Check if player is standing on an ammo pickup and collect it
 */
void	check_ammo_pickups(t_cub_data *data)
{
	int		player_x;
	int		player_y;
	char	*cell;

	if (!data || !data->player || !data->map)
		return ;
	
	player_x = (int)from_fixed32(data->player->x);
	player_y = (int)from_fixed32(data->player->y);
	
	if (player_x < 0 || player_x >= data->map->width || player_y < 0
		|| player_y >= data->map->height)
		return ;
	
	cell = &data->map->map_array[player_y * data->map->width + player_x];
	
	if (*cell == 'M')
	{
		
		data->player->stats.ammo += 42;
		if (data->player->stats.ammo > data->player->stats.max_ammo)
			data->player->stats.ammo = data->player->stats.max_ammo;
		
		*cell = '0';
		printf("Picked up ammo! Total: %d\n", data->player->stats.ammo);
	}
}
