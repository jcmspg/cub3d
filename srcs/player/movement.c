/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 21:45:00 by joao              #+#    #+#             */
/*   Updated: 2025/08/04 20:37:24 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/movements.h"

/**
 * Handle forward movement
 */
void	move_player_y(t_cub_data *data, t_fixed32 speed)
{
    t_fixed32 new_y = fixed32_add(data->player->y, speed);

    data->player->y = new_y;
    printf("Player Y: %f\n", from_fixed32(data->player->y));




    



    


    // if (data->player->y + speed > 0 && data->player->y + speed < data->map->height)
    //     data->player->y += speed;
    // else
    //     data->player->y = data->player->y > 0 ? data->map->height - 1 : 0;
}

/**
 * Handle backward movement
 */
void	move_player_x(t_cub_data *data, t_fixed32 speed)
{
    t_fixed32 new_player_x;

    new_player_x = fixed32_add(data->player->x, speed);

    data->player->x = new_player_x;
    printf("Player X: %f\n", from_fixed32(data->player->x));


    // if (data->player->x + speed > 0 && data->player->x + speed < data->map->width)
    //     data->player->x += speed;
    // else
    //     data->player->x = data->player->x > 0 ? data->map->width - 1 : 0;
}

int	is_valid_move(t_cub_data *data, t_fixed32 new_x, t_fixed32 new_y)
{
	// TODO: Implement collision detection
	(void)data;
	(void)new_x;
	(void)new_y;
	return (1);
}
