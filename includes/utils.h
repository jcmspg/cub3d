/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 19:15:05 by joamiran          #+#    #+#             */
/*   Updated: 2025/07/31 21:30:10 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
#define UTILS_H

# include "cub3d.h"

int ft_isspace(char c);


//math_utils.c
void index_to_coords(int index, int width, int *x, int *y);
int coords_to_index(int x, int y, int width);
char get_map_char_at(t_cub_data *data, t_fixed32 world_x, t_fixed32 world_y);
bool is_wall_at(t_cub_data *data, t_fixed32 world_x, t_fixed32 world_y);
void calc_trig_table(t_trig *trig);
bool init_trig_table(t_cub_data *data);
int	normalize_angle_degrees(int angle);
t_fixed32	fast_sin(t_trig *trig, int degrees);
t_fixed32	fixed_sin_lookup(t_trig *trig, t_fixed32 angle_fixed);
t_fixed32	fast_cos(t_trig *trig, int degrees);
int	fixed_radians_to_degrees(t_fixed32 radians);
t_fixed32	fixed_sin(t_trig *trig, t_fixed32 radians);
t_fixed32	fixed_cos(t_trig *trig, t_fixed32 radians);
t_fixed32	fixed_wrap_pi(t_fixed32 angle);
void	cleanup_trig_table(t_trig *trig);


#endif /* UTILS_H */