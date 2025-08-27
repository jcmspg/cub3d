/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 21:45:00 by joao              #+#    #+#             */
/*   Updated: 2025/08/27 21:17:41 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/movements.h"

/** 
 * Function to check if the move is valid based on collision detection.
 */

static bool	is_valid_move(t_cub_data *data, t_fixed32 new_x, t_fixed32 new_y)
{
    if (!data || !data->map || !data->map->map_array)
        return (false);

    // Define a buffer zone to avoid touching walls
    const float buffer = 0.3f; // Adjust this value as needed

    // Calculate the player's position with the buffer
    float pos_x = from_fixed32(new_x);
    float pos_y = from_fixed32(new_y);

    // Calculate the grid cell the player is in
    int map_x = (int)(pos_x);
    int map_y = (int)(pos_y);

    // Ensure the position is within map bounds
    if (map_x < 0 || map_x >= data->map->width || map_y < 0 || map_y >= data->map->height)
        return (false);

    // Check if the player is too close to a wall
    if (data->map->map_array[coords_to_index(map_x, map_y, data->map->width)] == '1')
        return (false);

    // Check if the player is within the buffer distance of a wall
    if ((pos_x - map_x < buffer && data->map->map_array[coords_to_index(map_x - 1, map_y, data->map->width)] == '1') ||
        (map_x + 1 - pos_x < buffer && data->map->map_array[coords_to_index(map_x + 1, map_y, data->map->width)] == '1') ||
        (pos_y - map_y < buffer && data->map->map_array[coords_to_index(map_x, map_y - 1, data->map->width)] == '1') ||
        (map_y + 1 - pos_y < buffer && data->map->map_array[coords_to_index(map_x, map_y + 1, data->map->width)] == '1'))
        return (false);

    return (true);
}

/**
 * Handle forward movement
 */
void	move_player_y(t_cub_data *data, t_fixed32 speed)
{
    t_fixed32 new_y = fixed32_add(data->player->y, speed);

    if (!is_valid_move(data, data->player->x, new_y))
        return ;
    data->player->y = new_y;
    printf("Player Y: %f\n", from_fixed32(data->player->y));

}

/**
 * Handle backward movement
 */
void	move_player_x(t_cub_data *data, t_fixed32 speed)
{
    t_fixed32 new_player_x;

    new_player_x = fixed32_add(data->player->x, speed);

    if (!is_valid_move(data, new_player_x, data->player->y))
        return ;
    data->player->x = new_player_x;
    printf("Player X: %f\n", from_fixed32(data->player->x));
}


// Move player in the direction they are facing
void move_player(t_cub_data *data, t_fixed32 move_speed)
{
    if (!data || !data->player)
        return;

    // Ensure direction vectors are valid
    if (data->player->dir_x == 0 && data->player->dir_y == 0)
    {
        ft_putstr_fd("Error: Direction vectors are not set\n", STDERR_FILENO);
        return;
    }

    // Calculate the new position based on the current direction vector
    t_fixed32 new_x = fixed32_mul(move_speed, data->player->dir_x);
    t_fixed32 new_y = fixed32_mul(move_speed, data->player->dir_y);

    t_fixed32 newX = fixed32_add(data->player->x, new_x);
    t_fixed32 newY = fixed32_add(data->player->y, new_y);

    // Check if the new position is valid
    if (!is_valid_move(data, newX, newY))
        return;

    // Update the player's position
    data->player->x = newX;
    data->player->y = newY;

    printf("Player moved to position: (%f, %f)\n", from_fixed32(data->player->x), from_fixed32(data->player->y));
}

static t_fixed32 normalize_angle_fixed(t_fixed32 angle)
{
    while (angle < 0)
        angle = fixed32_add(angle, to_fixed32(360.0f));
    while (angle >= to_fixed32(360.0f))
        angle = fixed32_sub(angle, to_fixed32(360.0f));
    return angle;
}

void rotate_player(t_cub_data *data, t_fixed32 rotation_angle)
{
    if (!data || !data->player)
        return;

    // Update the player's angle
    data->player->dir_angle = fixed32_add(data->player->dir_angle, rotation_angle);

    // Normalize the angle to keep it within 0-360 degrees
    data->player->dir_angle = normalize_angle_fixed(data->player->dir_angle);

    // Recalculate the direction vectors based on the new angle
    calc_player_dirs(data);

    printf("Player rotated. New direction: [%f, %f]\n",
           from_fixed32(data->player->dir_x), from_fixed32(data->player->dir_y));
}