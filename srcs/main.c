/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 20:39:30 by joamiran          #+#    #+#             */
/*   Updated: 2026/03/08 20:14:35 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"
#include "../includes/inits.h"

static int	init_parse_map(char *filename, t_cub_data *data)
{
	data->textures = init_textures();
	if (!data->textures)
	{
		ft_putstr_fd("Error: Failed to initialize textures.\n", STDERR_FILENO);
		return (ERR_MEMORY_ALLOCATION);
	}
	if (parse_cub_file(filename, data) != ERR_NO_ERROR)
	{
		ft_putstr_fd("Error parsing .cub file.\n", STDERR_FILENO);
		return (ERR_FILE_NOT_FOUND);
	}
	print_map_array(data->map);
	if (!init_trig_table(data))
	{
		ft_putstr_fd("Error: Failed to initialize trig tables\n",
			STDERR_FILENO);
		return (ERR_MEMORY_ALLOCATION);
	}
	run_all_math_tests(data);
	return (ERR_NO_ERROR);
}

static void	setup_mlx_hooks(t_cub_data *data)
{
	mlx_loop_hook(data->mlx->mlx_ptr, main_render_loop, data);
	mlx_hook(data->mlx->win_ptr, 2, 1L << 0, handle_key_press, data);
	mlx_hook(data->mlx->win_ptr, 3, 1L << 1, handle_key_release, data);
	mlx_hook(data->mlx->win_ptr, 17, 0L, handle_close, data);
	mlx_hook(data->mlx->win_ptr, 6, 1L << 6, handle_mouse_move, data);
	mlx_hook(data->mlx->win_ptr, 4, 1L << 2, handle_mouse_button, data);
	mlx_hook(data->mlx->win_ptr, 5, 1L << 3, handle_mouse_release, data);
}

int	main(int argc, char **argv)
{
	t_cub_data	data;
	int			err;

	ft_memset(&data, 0, sizeof(t_cub_data));
	srand(time(NULL));
	if (argc != 2)
	{
		ft_putstr_fd("Usage: ./cub3d <map_file>\n", 2);
		return (ERR_INVALID_ARG);
	}
	ft_putstr_fd("Game initialized with map: ", 1);
	ft_putstr_fd(argv[1], 1);
	ft_putchar_fd('\n', 1);
	err = init_parse_map(argv[1], &data);
	if (err != ERR_NO_ERROR)
		return (err);
	init_game_window(&data);
	graphics_init(&data);
	init_fps_sync(&data.fps);
	mlx_mouse_hide(data.mlx->mlx_ptr, data.mlx->win_ptr);
	mlx_mouse_move(data.mlx->mlx_ptr, data.mlx->win_ptr, data.mlx->width / 2,
		data.mlx->height / 2);
	setup_mlx_hooks(&data);
	mlx_loop(data.mlx->mlx_ptr);
	cleanup(&data);
	return (ERR_NO_ERROR);
}
