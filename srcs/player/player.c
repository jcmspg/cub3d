/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 21:45:00 by joao              #+#    #+#             */
/*   Updated: 2025/08/04 20:39:29 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/player.h"

// init player
t_player *init_player(t_cub_data *data)
{
    t_player *player;

    if (!data || !data->map)
    {
        ft_putstr_fd("Error: Invalid data or map\n", STDERR_FILENO);
        exit(ERR_PLAYER_INIT);
    }

    player = ft_calloc(sizeof(t_player), 1);
    if (!player)
    {
        ft_putstr_fd("Error: Memory allocation failed for player\n", STDERR_FILENO);
        exit(ERR_MEMORY_ALLOCATION);
    }

    //look for player spawn
    int spawn_x = -1;
    int spawn_y = -1;

    if (!look_for_spawn(data->map, &spawn_x, &spawn_y))
    {
        ft_putstr_fd("Error: Failed to find player spawn position\n", STDERR_FILENO);
        exit(ERR_PLAYER_INIT);
    }
    if (spawn_x == -1 || spawn_y == -1)
    {
        ft_putstr_fd("Error: No player spawn position found in map\n", STDERR_FILENO);
        exit(ERR_PLAYER_INIT);
    }

       // Set player initial position and direction based on spawn,
    player->x = to_fixed32(spawn_x + 0.5f); // Center the player in the cell
    player->y = to_fixed32(spawn_y + 0.5f); // Center the player in the cell
    
    

    player->move_speed = to_fixed32(PLAYER_SPEED);

    
    return player;
}

void draw_player(t_cub_data *data)
{
    if (!data || !data->player)
    {
        ft_putstr_fd("Error: Invalid data or player\n", STDERR_FILENO);
        return;
    }


    // Draw player as a simple circle for now
    int x = (int)(from_fixed32(data->player->x) * 12) + 10 ;
    int y = (int)(from_fixed32(data->player->y) * 12) + 10; // Offset for better visibility

    draw_filled_rect(data, x, y, 12, 12, CYAN);
}

void print_player_coords(t_cub_data *data)
{
    if (!data || !data->player)
    {
        ft_putstr_fd("Error: Invalid data or player\n", STDERR_FILENO);
        return;
    }


    printf("Player position: (%.2f, %.2f)\n",
           from_fixed32(data->player->x), from_fixed32(data->player->y));
}