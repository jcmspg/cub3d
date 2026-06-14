/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 21:45:00 by joao              #+#    #+#             */
/*   Updated: 2026/04/06 21:43:06 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/player.h"

static void	init_cached_plane_length(t_cub_data *data, t_fixed32 *plane_len)
{
	t_fixed32	sin_33;
	t_fixed32	cos_33;

	if (*plane_len != 0)
		return ;
	sin_33 = fast_sin(&data->trig, to_fixed32(33.0f));
	cos_33 = fast_cos(&data->trig, to_fixed32(33.0f));
	*plane_len = fixed32_div(sin_33, cos_33);
}

static t_fixed32	*cached_plane_len_ref(void)
{
	static t_fixed32	plane_len = 0;

	return (&plane_len);
}

static t_fixed32	*cached_last_angle_ref(void)
{
	static t_fixed32	last_angle = 999 << 16;

	return (&last_angle);
}

void	calc_player_dirs(t_cub_data *data)
{
	t_fixed32	*plane_len;
	t_fixed32	*last_angle;
	t_fixed32	angle_degrees;
	t_fixed32	angle_diff;

	if (!data || !data->player)
		return ;
	plane_len = cached_plane_len_ref();
	last_angle = cached_last_angle_ref();
	angle_degrees = data->player->dir_angle;
	angle_diff = angle_degrees - *last_angle;
	if (angle_diff < to_fixed32(0.01f) && angle_diff > to_fixed32(-0.01f))
		return ;
	init_cached_plane_length(data, plane_len);
	data->player->dir_x = fast_cos(&data->trig, angle_degrees);
	data->player->dir_y = fast_sin(&data->trig, angle_degrees);
	*last_angle = angle_degrees;
	data->player->plane_x = fixed32_mul(-data->player->dir_y,
			*plane_len);
	data->player->plane_y = fixed32_mul(data->player->dir_x,
			*plane_len);
}
