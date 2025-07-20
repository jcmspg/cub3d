/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joao <joao@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 21:45:00 by joao              #+#    #+#             */
/*   Updated: 2025/07/20 21:45:00 by joao             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

/**
 * Check if character is valid for map
 */
int	is_valid_map_char(char c)
{
	return (c == '0' || c == '1' || c == 'N' || c == 'S' || 
			c == 'E' || c == 'W' || c == ' ');
}

/**
 * Find player starting position
 */
int	find_player_position(t_game *game)
{
	// TODO: Implement player position finding
	(void)game;
	return (0);
}

/**
 * Get map dimensions
 */
void	get_map_dimensions(char **map, int *width, int *height)
{
	// TODO: Implement map dimension calculation
	(void)map;
	(void)width;
	(void)height;
}
