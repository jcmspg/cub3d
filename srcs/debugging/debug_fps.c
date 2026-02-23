/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_fps.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 14:00:00 by joamiran          #+#    #+#             */
/*   Updated: 2026/02/23 05:00:10 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	draw_fps(t_cub_data *data)
{
	char	*fps_str;
	char	*tmp;

	tmp = ft_itoa((int)data->fps.delta_time);
	fps_str = ft_strjoin("FPS: ", tmp);
	free(tmp);
	mlx_string_put(data->mlx->mlx_ptr, data->mlx->win_ptr, 10, 20, 0xFFFFFF,
		fps_str);
	free(fps_str);
}

void	display_fps(t_cub_data *data, float fps)
{
	(void)data;
	(void)fps;
}

void	print_movements(t_cub_data *data)
{
	(void)data;
}

void	print_player_pos(t_cub_data *data)
{
	(void)data;
}
