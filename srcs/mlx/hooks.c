/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 20:31:44 by joamiran          #+#    #+#             */
/*   Updated: 2025/08/27 21:03:06 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mylx_hooks.h"

int handle_key_press(int keycode, t_cub_data *data)
{
  if (keycode == KEY_ESC) // esc for exit
    data->input->exit = true;
  else if (keycode == KEY_W)
    data->input->forward = true;
  else if (keycode == KEY_S)
    data->input->backward = true;
  else if (keycode == KEY_A)
    data->input->left = true;
  else if (keycode == KEY_D)
    data->input->right = true;

  if (data->input->exit)
    mlx_loop_end(data->mlx->mlx_ptr);
  return (ERR_NO_ERROR);
}

int	handle_key_release(int keycode, t_cub_data *data)
{
	if (keycode == KEY_W)
	    data->input->forward = false;
	else if (keycode == KEY_S)
	    data->input->backward = false;
	else if (keycode == KEY_A)
	    data->input->left = false;
	else if (keycode == KEY_D)
	    data->input->right = false;
	return (ERR_NO_ERROR);
} 
// mouse hooks for rotation left and right
int handle_mouse_move(int x, int y, t_cub_data *data)
{
    static int last_x = -1;

    if (last_x == -1) // Initialize the first position
        last_x = x;

    int delta_x = x - last_x; // Calculate the difference in X position

    // Enable the appropriate turn flag based on mouse movement
    if (delta_x < 0)
    {
        data->input->turn_left = true;
        data->input->turn_right = false;
    }
    else if (delta_x > 0)
    {
        data->input->turn_right = true;
        data->input->turn_left = false;
    }
    else
    {
        data->input->turn_left = false;
        data->input->turn_right = false;
    }

    last_x = x; // Update the last X position
    (void)y; // Ignore Y-axis
    return (0);
}

int	handle_close(t_cub_data *data)
{
	mlx_loop_end(data->mlx->mlx_ptr);
	return (ERR_NO_ERROR);
}
