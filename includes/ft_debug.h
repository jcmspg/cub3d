/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_debug.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 17:55:35 by joamiran          #+#    #+#             */
/*   Updated: 2025/08/04 20:16:13 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_DEBUG_H
# define FT_DEBUG_H

# include "cub3d.h"

void	display_fps(t_cub_data *data, float fps);

int		test_sync(t_cub_data *data);

int		graphics_init(t_cub_data *data);

void print_map_array(const t_map *map);


void draw_map_grid(t_cub_data *data);

/* Math testing functions */
void test_fixed_point_math(void);
void test_trig_tables(t_cub_data *data);
void test_coordinate_conversions(t_cub_data *data);
void run_all_math_tests(t_cub_data *data);

/* Dynamic map testing functions */
void init_dynamic_map_test(t_cub_data *data);
void update_dynamic_map_test(t_cub_data *data);
void stop_dynamic_map_test(void);


void print_movements(t_cub_data *data);

void draw_filled_rect(t_cub_data *data, int x, int y, int width, int height, int color);


#endif /* FT_DEBUG_H */