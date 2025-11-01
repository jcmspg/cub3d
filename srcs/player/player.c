/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 21:45:00 by joao              #+#    #+#             */
/*   Updated: 2025/11/01 20:58:06 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/player.h"

static t_fixed32 direction_to_angle(char direction)
{
    t_fixed32 angle;
    
    if (direction == 'N')
        angle = to_fixed32(270.0f); // Facing up
    else if (direction == 'S')
        angle = to_fixed32(90.0f);  // Facing down
    else if (direction == 'E')
        angle = to_fixed32(0.0f);   // Facing right
    else if (direction == 'W')
        angle = to_fixed32(180.0f); // Facing left
    else
        angle = to_fixed32(0.0f);   // Default to facing right
        
    // Debug print for direction to angle conversion
    printf("🧭 DIRECTION TO ANGLE: '%c' -> %.1f degrees\n", 
           direction, from_fixed32(angle));
           
    return angle;
}

// SUPER EFFICIENT CACHING - avoid all calculations when possible
static t_fixed32 cached_plane_length = 0;
static t_fixed32 last_angle = 999 << 16; // Invalid initial value (raw fixed32)

// player direction math and vectors - SUPER OPTIMIZED (FIXED)
void calc_player_dirs(t_cub_data *data)
{
    if (!data || !data->player)
        return;

    // Keep full precision - DON'T convert to int!
    t_fixed32 angle_degrees = data->player->dir_angle;
    
    // OPTIMIZATION: Skip ALL calculations if angle hasn't changed!
    // Compare with small epsilon for floating point precision
    t_fixed32 angle_diff = angle_degrees - last_angle;
    if (angle_diff < to_fixed32(0.01f) && angle_diff > to_fixed32(-0.01f))
    {
        return; // Direction vectors are already correct
    }
        
    // Cache the plane length on first call (use fast lookup tables!)
    if (cached_plane_length == 0)
    {
        // tan(33°) = sin(33°) / cos(33°) - using your fast lookup tables
        // FIX: Pass t_fixed32, not int!
        t_fixed32 sin_33 = fast_sin(&data->trig, to_fixed32(33.0f));
        t_fixed32 cos_33 = fast_cos(&data->trig, to_fixed32(33.0f));
        cached_plane_length = fixed32_div(sin_33, cos_33);
    }

    // FIX: Use t_fixed32 angle, not int!
    data->player->dir_x = fast_cos(&data->trig, angle_degrees);
    data->player->dir_y = fast_sin(&data->trig, angle_degrees);
      
    // Cache this angle to avoid recalculating
    last_angle = angle_degrees;

    // Calculate camera plane (perpendicular to direction)
    data->player->plane_x = fixed32_mul(-data->player->dir_y, cached_plane_length);
    data->player->plane_y = fixed32_mul(data->player->dir_x, cached_plane_length);

    // Debug output to verify calculations
    printf("🧮 CALC_PLAYER_DIRS: angle=%.2f° | dir_x=%.4f | dir_y=%.4f\n",
           from_fixed32(angle_degrees),
           from_fixed32(data->player->dir_x),
           from_fixed32(data->player->dir_y));
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
    char direction = 'X';

    printf("🔍 PLAYER INIT: Searching for spawn position...\n");
    
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
    
    printf("📍 SPAWN FOUND: Grid position (%d, %d) with direction '%c'\n", 
           spawn_x, spawn_y, direction);
    
    // Set player initial position and direction based on spawn
    player->x = to_fixed32(spawn_x + 0.5f); // Center the player in the cell
    player->y = to_fixed32(spawn_y + 0.5f); // Center the player in the cell
    player->dir_angle = direction_to_angle(direction);
    player->move_speed = to_fixed32(PLAYER_SPEED);
    player->rotate_speed = to_fixed32(ROTATE_SPEED);

    printf("🎮 PLAYER INITIALIZED:\n");
    printf("   World Position: (%.3f, %.3f)\n", 
           from_fixed32(player->x), from_fixed32(player->y));
    printf("   Direction Angle: %.1f degrees\n", 
           from_fixed32(player->dir_angle));
    printf("   Move Speed: %.3f\n", 
           from_fixed32(player->move_speed));
    printf("   Rotate Speed: %.3f\n", 
           from_fixed32(player->rotate_speed));

    // Calculate initial direction vectors
    printf("🧮 CALCULATING INITIAL DIRECTION VECTORS...\n");
    calc_player_dirs(data);

    printf("✅ PLAYER INITIALIZATION COMPLETE!\n\n");
    
    return player;
}

void draw_player(t_cub_data *data)
{
    if (!data || !data->player)
    {
        // Silently return instead of spamming error messages
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

    // Uncommenting the print_player_coords function to display player coordinates
    // print_player_coords(data);
}

void print_player_coords(t_cub_data *data)
{
    if (!data || !data->player)
    {
        // Silently return instead of spamming error messages
        return;
    }
    printf("Player position: (%.2f, %.2f)\n",
           from_fixed32(data->player->x), from_fixed32(data->player->y));
}