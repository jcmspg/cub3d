/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 21:07:31 by joamiran          #+#    #+#             */
/*   Updated: 2026/02/23 03:20:10 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int	main(int argc, char **argv)
{
	t_cub_data	data;

	if (argc != 2)
	{
		ft_putstr_fd("Usage: ./cub3d <map.cub>\n", 1);
		return (1);
	}
	ft_memset(&data, 0, sizeof(t_cub_data));
	if (!parse_cub_file(argv[1], &data))
		return (1);
	init_game_window(&data);
	graphics_init(&data);
	init_fps_sync(&data);
	mlx_loop_hook(data.mlx->mlx_ptr, main_loop, &data);
	mlx_hook(data.mlx->win_ptr, 2, 1L << 0, handle_key_press, &data);
	mlx_hook(data.mlx->win_ptr, 3, 1L << 1, handle_key_release, &data);
	mlx_hook(data.mlx->win_ptr, 17, 0L, handle_close, &data);
	mlx_hook(data.mlx->win_ptr, 6, 1L << 6, handle_mouse_move, &data);
	mlx_hook(data.mlx->win_ptr, 4, 1L << 2, handle_mouse_button, &data);
	mlx_hook(data.mlx->win_ptr, 5, 1L << 3, handle_mouse_release, &data);
	mlx_loop(data.mlx->mlx_ptr);
	cleanup_and_exit(&data);
	return (0);
}
