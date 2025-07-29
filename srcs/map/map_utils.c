/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 21:45:00 by joao              #+#    #+#             */
/*   Updated: 2025/07/29 19:32:23 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

// Checks if a line is a texture or color definition
bool is_texture_or_color(char *line)
{
	if (ft_strncmp(line, "NO ", 3) == 0 || 
		ft_strncmp(line, "SO ", 3) == 0 || 
		ft_strncmp(line, "WE ", 3) == 0 || 
		ft_strncmp(line, "EA ", 3) == 0 || 
		ft_strncmp(line, "F ", 2) == 0 || 
		ft_strncmp(line, "C ", 2) == 0)
	{
		return true;
	}
	return false;
}

bool is_empty_line(const char *line)
{
    if (!line)
        return true;
        
    // Remove newline for checking
    char *clean = ft_strdup(line);
    if (!clean)
        return false;
    
    int len = ft_strlen(clean);
    if (len > 0 && clean[len-1] == '\n')
        clean[len-1] = '\0';
    
    // Check if line contains only whitespace
    for (int i = 0; clean[i]; i++)
    {
        if (clean[i] != ' ' && clean[i] != '\t')
        {
            free(clean);
            return false;  // Not empty - contains non-whitespace
        }
    }
    
    free(clean);
    return true;  // Empty or only whitespace
}

// Checks if a character is valid in the map
bool	is_valid_map_char(char c)
{
	if (c == '0' || c == '1' || c == '2' || c == 'N' || c == 'S' || 
		c == 'E' || c == 'W' || c == ' ')
		return true;
	return false;
}

bool is_valid_map_line(const char *line)
{
    if (!line || !*line)
        return false;
    
    // Remove newline for checking
    char *clean = ft_strdup(line);
    if (!clean)
        return false;
    
    int len = ft_strlen(clean);
    if (len > 0 && clean[len-1] == '\n')
        clean[len-1] = '\0';
    
    // Must have at least one character after removing newline
    if (ft_strlen(clean) == 0)
    {
        free(clean);
        return false;
    }
    
    // Use your existing validation function
    for (int i = 0; clean[i]; i++)
    {
        if (!is_valid_map_char(clean[i]))
        {
            free(clean);
            return false;
        }
    }
    
    free(clean);
    return true;
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
// bool	get_map_dimensions(t_map *map)
// {
// 	if (!map)
// 		return (false);
// 	if (!skip_header(map))
// 		return (false);
// 	if (!scan_map(map))
// 		return (false);
// }

