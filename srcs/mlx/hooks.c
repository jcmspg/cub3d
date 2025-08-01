/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 20:31:44 by joamiran          #+#    #+#             */
/*   Updated: 2025/07/27 18:46:46 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mylx_hooks.h"

int	handle_key_press(int keycode, t_cub_data *data)
{
	if (keycode == KEY_ESC) // esc for exit
		mlx_loop_end(data->mlx->mlx_ptr);
	return (ERR_NO_ERROR);
}

int	handle_close(t_cub_data *data)
{
	mlx_loop_end(data->mlx->mlx_ptr);
	return (ERR_NO_ERROR);
}