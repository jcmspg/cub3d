/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks_mouse.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 22:16:42 by hladeiro          #+#    #+#             */
/*   Updated: 2026/04/06 22:17:51 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/gamelogic.h"
#include "../../includes/mylx_hooks.h"

static void	apply_mouse_rotation(t_cub_data *data, int delta_x)
{
	t_fixed32	rotation;

	rotation = to_fixed32((float)delta_x * 0.15f);
	data->player->dir_angle = fixed32_add(data->player->dir_angle, rotation);
	if (data->player->dir_angle < 0)
		data->player->dir_angle = fixed32_add(data->player->dir_angle,
				to_fixed32(360.0f));
	if (data->player->dir_angle >= to_fixed32(360.0f))
		data->player->dir_angle = fixed32_sub(data->player->dir_angle,
				to_fixed32(360.0f));
	calc_player_dirs(data);
}

int	handle_mouse_button(int button, int x, int y, t_cub_data *data)
{
	(void)x;
	(void)y;
	if (button == MOUSE_LEFT)
		data->input->shoot = true;
	return (0);
}

int	handle_mouse_release(int button, int x, int y, t_cub_data *data)
{
	(void)x;
	(void)y;
	if (button == MOUSE_LEFT)
		data->input->shoot = false;
	return (0);
}

int	handle_mouse_move(int x, int y, t_cub_data *data)
{
	static int	center_x = -1;
	static int	mouse_locked = 0;
	int			delta_x;

	if (center_x == -1)
		center_x = data->mlx->width / 2;
	if (mouse_locked)
	{
		mouse_locked = 0;
		return (0);
	}
	delta_x = x - center_x;
	if (delta_x != 0)
	{
		apply_mouse_rotation(data, delta_x);
		mouse_locked = 1;
		mlx_mouse_move(data->mlx->mlx_ptr, data->mlx->win_ptr, center_x,
			data->mlx->height / 2);
	}
	data->input->turn_left = false;
	data->input->turn_right = false;
	(void)y;
	return (0);
}
