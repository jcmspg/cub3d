/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 20:31:44 by joamiran          #+#    #+#             */
/*   Updated: 2026/04/08 23:11:06 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/gamelogic.h"
#include "../../includes/mylx_hooks.h"

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
		return (cleanup_and_exit(data));
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

int	handle_close(t_cub_data *data)
{
	printf("Window close event received\n");
	return cleanup_and_exit(data);
}
