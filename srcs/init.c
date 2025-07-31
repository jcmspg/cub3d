/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 21:19:09 by joamiran          #+#    #+#             */
/*   Updated: 2025/07/31 21:32:30 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/inits.h"

void	init_fps_sync(t_fps_data *fps)
{
	fps->last_frame_time = get_time_ms();
	fps->delta_time = 0;
	fps->target_frame_duration = (1000 / MAX_FPS);
	fps->frame_count = 0;
}

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
	if (!init_trig_table(data))
	{
		ft_putstr_fd("Error: Lookup Tables Failed to init\n", STDERR_FILENO);
		cleanup_and_exit(data);
	}
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
