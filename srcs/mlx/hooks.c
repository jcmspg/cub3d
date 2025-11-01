/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 20:31:44 by joamiran          #+#    #+#             */
/*   Updated: 2025/09/04 18:34:37 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mylx_hooks.h"

int	handle_key_press(int keycode, t_cub_data *data)
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
	else if (keycode == KEY_LEFT)
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
    static int center_x = -1;
    static int mouse_locked = 0;
    
    // Initialize center position once
    if (center_x == -1)
    {
        center_x = data->mlx->width / 2;
    }
    
    // Ignore events when we're warping the mouse (prevents infinite loop)
    if (mouse_locked)
    {
        mouse_locked = 0; // Reset the lock
        return (0);
    }
    
    int delta_x = x - center_x;
    
    // Only process significant mouse movement (reduces sensitivity)
    if (abs(delta_x) > 2)
    {
        // Set rotation flags based on mouse delta (same as before but cleaner)
        if (delta_x < 0)
        {
            data->input->turn_left = true;
            data->input->turn_right = false;
        }
        else
        {
            data->input->turn_right = true;
            data->input->turn_left = false;
        }
        
        // Lock mouse events and warp back to center
        mouse_locked = 1;
        mlx_mouse_move(data->mlx->mlx_ptr, data->mlx->win_ptr, center_x, data->mlx->height / 2);
    }
    else
    {
        // No significant movement, clear flags
        data->input->turn_left = false;
        data->input->turn_right = false;
    }
    
    (void)y; // Ignore Y-axis movement
    return (0);
}


int	handle_close(t_cub_data *data)
{
	mlx_loop_end(data->mlx->mlx_ptr);
	return (ERR_NO_ERROR);
}