/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parse.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 19:50:51 by joamiran          #+#    #+#             */
/*   Updated: 2025/07/29 17:54:08 by joamiran         ###   ########.fr       */
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

// Function to count the number of lines in a file
bool		count_lines_in_file(t_map *map);

// Function to count the number of tiles in the map
bool	count_map_tiles(t_map *map);

// Function to create Y arrays for the map
bool	create_y_arrays(t_map *map);

// Function to create X arrays for the map
bool	create_x_arrays(t_map *map);

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

#endif /* MAP_PARSE_H */