/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 21:45:00 by joao              #+#    #+#             */
/*   Updated: 2025/07/31 21:52:52 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

/**
 * Find player spawn position in map array
 */


// static bool	find_player_spawn(t_cub_data *data, int *spawn_x, int *spawn_y)
// {
//     int	i;
//     int	grid_x;
//     int	grid_y;

//     i = 0;
//     while (i < data->map->width * data->map->height)
//     {
//         char c = data->map->map_array[i];
//         if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
//         {
//             // Convert 1D array index to 2D grid coordinates
//             index_to_coords(i, data->map->width, &grid_x, &grid_y);
            
//             *spawn_x = grid_x;
//             *spawn_y = grid_y;
            
//             // Replace player character with empty space
//             data->map->map_array[i] = '0';
            
//             printf("Player spawn found at grid (%d, %d)\n", grid_x, grid_y);
//             return (true);
//         }
//         i++;
//     }
//     return (false);
// }

// /**
//  * Initialize player position from map data
//  */
// bool	init_player_from_map(t_cub_data *data)
// {
//     int	spawn_x;
//     int	spawn_y;

//     // Find player spawn position in map
//     if (!find_player_spawn(data, &spawn_x, &spawn_y))
//     {
//         printf("Error: No player spawn position found in map\n");
//         return (false);
//     }

//     // Set player position in world coordinates (center of grid cell)
//     data->player.x = to_fixed32((float)spawn_x + 0.5f);
//     data->player.y = to_fixed32((float)spawn_y + 0.5f);

//     printf("Player initialized at world position (%.2f, %.2f)\n",
//            from_fixed32(data->player.x), from_fixed32(data->player.y));

//     return (true);
// }

// /**
//  * Move player in X direction
//  */
// bool	move_player_x(t_cub_data *data, t_fixed32 delta_x)
// {
//     t_fixed32	new_x;

//     new_x = fixed32_add(data->player.x, delta_x);
    
//     // Check collision at new position
//     if (!is_wall_at(data, new_x, data->player.y))
//     {
//         data->player.x = new_x;
//         return (true);
//     }
//     return (false);
// }

// /**
//  * Move player in Y direction
//  */
// bool	move_player_y(t_cub_data *data, t_fixed32 delta_y)
// {
//     t_fixed32	new_y;

//     new_y = fixed32_add(data->player.y, delta_y);
    
//     // Check collision at new position
//     if (!is_wall_at(data, data->player.x, new_y))
//     {
//         data->player.y = new_y;
//         return (true);
//     }
//     return (false);
// }

// /**
//  * Move player to specific world coordinates
//  */
// bool	move_player_to(t_cub_data *data, t_fixed32 world_x, t_fixed32 world_y)
// {
//     // Check if destination is valid
//     if (!is_wall_at(data, world_x, world_y))
//     {
//         data->player.x = world_x;
//         data->player.y = world_y;
//         return (true);
//     }
//     return (false);
// }

// /**
//  * Get player position in world coordinates
//  */
// void	get_player_position(t_cub_data *data, float *world_x, float *world_y)
// {
//     *world_x = from_fixed32(data->player.x);
//     *world_y = from_fixed32(data->player.y);
// }

// /**
//  * Get player position in grid coordinates
//  */
// void	get_player_grid_position(t_cub_data *data, int *grid_x, int *grid_y)
// {
//     *grid_x = from_fixed32(data->player.x);  // Truncates to integer
//     *grid_y = from_fixed32(data->player.y);
// }

// /**
//  * Check if player can move to a specific position (with safety margin)
//  */
// bool	can_player_move_to(t_cub_data *data, t_fixed32 new_x, t_fixed32 new_y)
// {
//     t_fixed32	buffer;
    
//     buffer = to_fixed32(0.1f);  // Small safety margin
    
//     // Check collision with a small buffer around player
//     if (is_wall_at(data, fixed32_add(new_x, buffer), fixed32_add(new_y, buffer)) ||
//         is_wall_at(data, fixed32_sub(new_x, buffer), fixed32_add(new_y, buffer)) ||
//         is_wall_at(data, fixed32_add(new_x, buffer), fixed32_sub(new_y, buffer)) ||
//         is_wall_at(data, fixed32_sub(new_x, buffer), fixed32_sub(new_y, buffer)))
//         return (false);
    
//     return (true);
// }
/**
 * Initialize player with default values
 */
/**
 * Legacy function - kept for compatibility but deprecated
 */
void	init_player(t_game *game, int x, int y, char direction)
{
    (void)game;
    (void)x;
    (void)y;
    (void)direction;
    printf("Warning: init_player() is deprecated. Use init_player_from_map() instead.\n");
}

/**
 * Simple update function - will be expanded when input hooks are added
 */
void	update_player(t_game *game)
{
    (void)game;
    // This will be implemented when we add input handling
}

/**
 * Simple rotation function - placeholder for future 3D implementation
 */
void	rotate_player(t_game *game, double angle)
{
    (void)game;
    (void)angle;
    // This will be implemented when we add 3D raycasting
}
