/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parse.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 19:18:12 by joamiran          #+#    #+#             */
/*   Updated: 2025/11/01 17:55:31 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAP_PARSE_H
# define MAP_PARSE_H

# include "cub3d.h"

bool		is_texture_or_color(char *line);
bool		is_empty_line(const char *line);
bool		is_valid_map_char(char c);
bool		is_valid_map_line(const char *line);
bool		create_map_array(t_cub_data *data);
bool		open_map(char *filename, t_cub_data *data);
int			parse_cub_file(char *filename, t_cub_data *data);
int			parse_textures(char *line, t_cub_data *data);
int			parse_colors(char *line, t_cub_data *data);
bool		get_map_dimensions(t_map *map);
bool		look_for_spawn(t_map *map, int *sx, int *sy, char *dir);

#endif
