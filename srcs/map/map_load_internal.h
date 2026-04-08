/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_load_internal.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 00:53:23 by hladeiro          #+#    #+#             */
/*   Updated: 2026/04/08 00:53:25 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAP_LOAD_INTERNAL_H
# define MAP_LOAD_INTERNAL_H

# include "../../includes/cub3d.h"

bool	map_first_pass(t_map *map, int *line_count, int *max_len);
bool	map_second_pass(t_map *map);
bool	map_create_array(t_map *map);
bool	map_convert_lines_to_array(t_map *map);

#endif
