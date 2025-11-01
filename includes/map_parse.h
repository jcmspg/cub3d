/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parse.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 19:50:51 by joamiran          #+#    #+#             */
/*   Updated: 2025/08/27 18:05:55 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAP_PARSE_H
# define MAP_PARSE_H

# include "cub3d.h"

// checks if a line is a texture or color definition
bool	is_texture_or_color(char *line);

// checks if a line is empty
bool	is_empty_line(const char *line);

// checks if a character is valid in the map
bool	is_valid_map_char(char c);

// checks if a map line is valid
bool    is_valid_map_line(const char *line);






// Function to create a map array from the file
bool	create_map_array(t_cub_data *data);

// Function to open a map file and initialize the map data
bool	open_map(char *filename, t_cub_data *data);

// Function to parse the .cub file and extract map data
int		parse_cub_file(char *filename, t_cub_data *data);

// Function to parse texture paths from the .cub file
int		parse_textures(char *line, t_cub_data *data);

// Fubction to parse color values from the .cub file
int		parse_colors(char *line, t_cub_data *data);

// function to get the map dims
bool get_map_dimensions(t_map *map);



// grab player spawn 
bool look_for_spawn(t_map *map, int *spawn_x, int *spawn_y, char *direction);


#endif /* MAP_PARSE_H */