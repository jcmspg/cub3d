/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 21:19:09 by joamiran          #+#    #+#             */
/*   Updated: 2025/07/24 20:31:23 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/inits.h"

void	init_game_window(t_cub_data *data)
{
	data->mlx = malloc(sizeof(t_mlx));
	if (!data->mlx)
	{
		ft_putstr_fd("Error: Memory allocation failed for MLX.\n",
			STDERR_FILENO);
		exit(ERR_MEMORY_ALLOCATION);
	}
	data->mlx->width = START_WIDTH;
	data->mlx->height = START_HEIGHT;
	data->mlx->title = "Cub3D Game";
	mylx_init(data);
	if (!data->mlx->mlx_ptr)
	{
		ft_putstr_fd("Error: MLX initialization failed.\n", STDERR_FILENO);
		exit(ERR_MLX_INIT);
	}
	if (mylx_create_window(data) != ERR_NO_ERROR)
		exit(ERR_WINDOW_CREATE);
	mylx_create_image(data);
	mylx_clear_image(data);
	mylx_make_image(data);
}
