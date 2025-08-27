/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 21:45:00 by joao              #+#    #+#             */
/*   Updated: 2025/08/27 21:17:27 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/player.h"

static t_fixed32 direction_to_angle(char direction)
{
    if (direction == 'N')
        return to_fixed32(270.0f); // Facing up
    else if (direction == 'S')
        return to_fixed32(90.0f);  // Facing down
    else if (direction == 'E')
        return to_fixed32(0.0f);   // Facing right
    else if (direction == 'W')
        return to_fixed32(180.0f); // Facing left
    else
        return to_fixed32(0.0f);   // Default to facing right
}
// player direction math and vectors
void calc_player_dirs(t_cub_data *data)
{
    if (!data || !data->player)
    {
        ft_putstr_fd("Error: Invalid data or player\n", STDERR_FILENO);
        return;
    }

    // Convert angle to radians for trig functions
    t_fixed32 dir_rad = fixed32_mul(data->player->dir_angle, to_fixed32(M_PI / 180.0f));

    // Calculate direction vector
    data->player->dir_x = fixed32_cos(dir_rad);
    data->player->dir_y = fixed32_sin(dir_rad);

    // Calculate camera plane (perpendicular to direction)
    // FOV of 66 degrees, so plane length is tan(33 degrees) ~ 0.66
    t_fixed32 plane_length = fixed32_tan(to_fixed32(33.0f * (M_PI / 180.0f)));
    data->player->plane_x = fixed32_mul(data->player->dir_y, plane_length);
    data->player->plane_y = fixed32_mul(data->player->dir_x, -plane_length);
}

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
    char direction = '\0';

    if (!look_for_spawn(data->map, &spawn_x, &spawn_y, &direction))
    {
        ft_putstr_fd("Error: Failed to find player spawn position\n", STDERR_FILENO);
        exit(ERR_PLAYER_INIT);
    }
    if (spawn_x == -1 || spawn_y == -1)
    {
        ft_putstr_fd("Error: No player spawn position found in map\n", STDERR_FILENO);
        exit(ERR_PLAYER_INIT);
    }

    // Set player initial position and direction based on spawn
    player->x = to_fixed32(spawn_x + 0.5f); // Center the player in the cell
    player->y = to_fixed32(spawn_y + 0.5f); // Center the player in the cell
    player->dir_angle = direction_to_angle(direction);
    player->move_speed = to_fixed32(PLAYER_SPEED);
    player->rotate_speed = to_fixed32(ROTATE_SPEED);

    // Calculate initial direction vectors
    calc_player_dirs(data);

    return player;
}

void draw_player(t_cub_data *data)
{
    if (!data || !data->player)
    {
        ft_putstr_fd("Error: Invalid data or player\n", STDERR_FILENO);
        return;
    }

    draw_filled_rect(data,
                     from_fixed32(data->player->x) * 12 + 10 - 3,
                     from_fixed32(data->player->y) * 12 + 10 - 3,
                     6, 6, 0xFF0000);
    

    // draw a dot indicating direction
    // Draw a rectangle indicating the direction the player is facing
    int player_x = from_fixed32(data->player->x) * 12 + 10;
    int player_y = from_fixed32(data->player->y) * 12 + 10;

    // Calculate the endpoint of the direction rectangle
    int dir_x = player_x + (int)(from_fixed32(data->player->dir_x) * 12);
    int dir_y = player_y + (int)(from_fixed32(data->player->dir_y) * 12);

    // Draw the direction rectangle as a yellow line
    draw_filled_rect(data, dir_x - 2, dir_y - 2, 4, 4, 0xFFFF00);

    print_player_coords(data);
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