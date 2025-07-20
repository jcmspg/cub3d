/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_game.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 21:19:09 by joamiran          #+#    #+#             */
/*   Updated: 2025/07/20 21:19:22 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inits.h"

void	init_game(t_cub_data *data)
{
	data->mlx.mlx_ptr = mlx_init();
	if (!data->mlx.mlx_ptr)
	{
		ft_putstr_fd("Error: Failed to initialize MLX.\n", 2);
		exit(ERR_GRAPHICS_INIT);
	}
	data->mlx.win_ptr = mlx_new_window(data->mlx.mlx_ptr, START_WIDTH,
			START_HEIGHT, "Cub3D");
	if (!data->mlx.win_ptr)
	{
		ft_putstr_fd("Error: Failed to create window.\n", 2);
		exit(ERR_GRAPHICS_INIT);
	}
	data->mlx.width = START_WIDTH;
	data->mlx.height = START_HEIGHT;
	data->mlx.map_file = NULL; // Set map file later
}