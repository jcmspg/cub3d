/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   combat_internal.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 00:39:32 by hladeiro          #+#    #+#             */
/*   Updated: 2026/04/08 00:39:34 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMBAT_INTERNAL_H
# define COMBAT_INTERNAL_H

# include "../../includes/cub3d.h"

typedef struct s_shot_view
{
	int		center_idx;
	int		screen_width;
	int		center_min;
	int		center_max;
	float	dir_x;
	float	dir_y;
	float	plane_x;
	float	plane_y;
	float	wall_dist;
}			t_shot_view;

typedef struct s_enemy_proj
{
	float	sprite_x;
	float	sprite_y;
	float	transform_x;
	float	transform_y;
	int		screen_x;
}			t_enemy_proj;

void		process_shot_hits(t_cub_data *data);

#endif
