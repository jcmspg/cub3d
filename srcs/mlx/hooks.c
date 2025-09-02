/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 20:31:44 by joamiran          #+#    #+#             */
/*   Updated: 2025/09/02 21:44:28 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mylx_hooks.h"

int handle_key_press(int keycode, t_cub_data *data)
{
  if (keycode == KEY_ESC) // esc for exit
    data->input->exit = true;
  else if (keycode == KEY_W || keycode == KEY_UP)
    data->input->forward = true;
  else if (keycode == KEY_S || keycode == KEY_DOWN)
    data->input->backward = true;
  else if (keycode == KEY_A)
    data->input->left = true;
  else if (keycode == KEY_D)
    data->input->right = true;
  else if ( keycode == KEY_LEFT)
    data->input->turn_left = true;
  else if (keycode == KEY_RIGHT)
    data->input->turn_right = true;

  if (data->input->exit)
    mlx_loop_end(data->mlx->mlx_ptr);
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

    // Warp the mouse back to the center of the window
    mlx_mouse_move(data->mlx->mlx_ptr, data->mlx->win_ptr ,data->mlx->width / 2, data->mlx->height / 2);
    last_x = data->mlx->width / 2; // Reset last_x to the center

    last_x = x; // Update the last X position
    (void)y; // Ignore Y-axis
    return (0);
}

int	handle_close(t_cub_data *data)
{
	mlx_loop_end(data->mlx->mlx_ptr);
	return (ERR_NO_ERROR);
}