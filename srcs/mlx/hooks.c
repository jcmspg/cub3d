/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 20:31:44 by joamiran          #+#    #+#             */
/*   Updated: 2026/03/08 20:14:35 by joamiran         ###   ########.fr       */
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

static void	set_key_press_state(int keycode, t_cub_data *data)
{
	if (keycode == KEY_W || keycode == KEY_UP)
		data->input->forward = true;
	else if (keycode == KEY_S || keycode == KEY_DOWN)
		data->input->backward = true;
	else if (keycode == KEY_A)
		data->input->left = true;
	else if (keycode == KEY_D)
		data->input->right = true;
	else if (keycode == KEY_LEFT)
		data->input->turn_left = true;
	else if (keycode == KEY_RIGHT)
		data->input->turn_right = true;
	else if (keycode == KEY_SHIFT)
		data->input->sprint = true;
}

int	handle_key_press(int keycode, t_cub_data *data)
{
	if (keycode == KEY_ESC)
		data->input->exit = true;
	else if (keycode == KEY_SPACE && !data->input->jumping)
	{
		data->input->jumping = true;
		data->input->jump_start_time = get_time_ms();
	}
	else
		set_key_press_state(keycode, data);
	if (data->input->exit)
		exit(0);
	return (ERR_NO_ERROR);
}

int	handle_key_release(int keycode, t_cub_data *data)
{
	if (keycode == KEY_W || keycode == KEY_UP)
		data->input->forward = false;
	else if (keycode == KEY_S || keycode == KEY_DOWN)
		data->input->backward = false;
	else if (keycode == KEY_A)
		data->input->left = false;
	else if (keycode == KEY_D)
		data->input->right = false;
	else if (keycode == KEY_LEFT)
		data->input->turn_left = false;
	else if (keycode == KEY_RIGHT)
		data->input->turn_right = false;
	else if (keycode == KEY_SHIFT)
		data->input->sprint = false;
	else if (keycode == KEY_E)
		interact_doors(data);
	return (ERR_NO_ERROR);
}

// Mouse button handler for shooting
int	handle_mouse_button(int button, int x, int y, t_cub_data *data)
{
	(void)x;
	(void)y;
	if (button == MOUSE_LEFT)
		data->input->shoot = true;
	return (0);
}

// Mouse button release handler
int	handle_mouse_release(int button, int x, int y, t_cub_data *data)
{
	(void)x;
	(void)y;
	if (button == MOUSE_LEFT)
		data->input->shoot = false;
	return (0);
}

// mouse hooks for rotation - proportional smooth rotation
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

int	handle_close(t_cub_data *data)
{
	(void)data;
	exit(0);
	return (ERR_NO_ERROR);
}