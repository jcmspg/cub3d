/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 19:15:05 by joamiran          #+#    #+#             */
/*   Updated: 2026/04/08 01:49:10 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include "cub3d.h"

int			ft_isspace(char c);
void		index_to_coords(int index, int width, int *x, int *y);
int			coords_to_index(int x, int y, int width);
char		get_map_char_at(t_cub_data *data, t_fixed32 world_x,
				t_fixed32 world_y);
bool		is_wall_at(t_cub_data *data, t_fixed32 world_x, t_fixed32 world_y);
void		calc_trig_table(t_trig *trig);
bool		init_trig_table(t_cub_data *data);
t_fixed32	normalize_angle_degrees(t_fixed32 angle);
t_fixed32	fast_sin(t_trig *trig, t_fixed32 degrees);
t_fixed32	fast_cos(t_trig *trig, t_fixed32 degrees);
t_fixed32	fixed_radians_to_degrees(t_fixed32 radians);
t_fixed32	fixed_sin(t_trig *trig, t_fixed32 radians);
t_fixed32	fixed_cos(t_trig *trig, t_fixed32 radians);
void		cleanup_trig_table(t_trig *trig);

#endif /* UTILS_H */