/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_fps.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 17:54:52 by joamiran          #+#    #+#             */
/*   Updated: 2025/08/04 18:28:21 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_debug.h"

void	display_fps(t_cub_data *data, float fps)
{
	char	*fps_str;
	char	*label;
	char	*full_str;

	fps_str = ft_itoa((int)fps);
	if (!fps_str)
		return ;
	label = "FPS: ";
	full_str = ft_strjoin(label, fps_str);
	free(fps_str);
	if (!full_str)
		return ;
	mlx_string_put(data->mlx->mlx_ptr, data->mlx->win_ptr, 10, 10, 0xFFFFFF,
		full_str);
	free(full_str);
}

void print_movements(t_cub_data *data)
{
	if (!data || !data->input)
		return;
	char *movement_str;
	char *movement;

	movement_str = "Movement: ";
	if (data->input->forward)
		movement = "Forward ";
	else if (data->input->backward)
		movement = "Backward ";
	else if (data->input->left)
		movement = "Left ";
	else if (data->input->right)
		movement = "Right ";
	else
		movement = "None ";

	char *full_str = ft_strjoin(movement_str, movement);
	if (!full_str)
		return;
	mlx_string_put(data->mlx->mlx_ptr, data->mlx->win_ptr, 10, 30, 0xFFFFFF,
		full_str);
	free(full_str);
	if (data->input->exit)
	{
		mlx_string_put(data->mlx->mlx_ptr, data->mlx->win_ptr, 10, 50, 0xFF0000,
			"Exiting...");
	}
	else
	{
		mlx_string_put(data->mlx->mlx_ptr, data->mlx->win_ptr, 10, 50, 0x00FF00,
			"Press ESC to exit");
	}
	
}
