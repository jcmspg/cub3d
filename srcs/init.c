/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 21:19:09 by joamiran          #+#    #+#             */
/*   Updated: 2026/02/23 03:30:08 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/hud.h"
#include "../includes/inits.h"
#include <unistd.h>

static bool	init_input(t_cub_data *data)
{
	data->input = ft_calloc(sizeof(t_input), 1);
	if (!data->input)
	{
		ft_putstr_fd("Error creating input struct", STDERR_FILENO);
		return (false);
	}
	data->input->backward = false;
	data->input->forward = false;
	data->input->left = false;
	data->input->right = false;
	data->input->turn_left = false;
	data->input->turn_right = false;
	data->input->shoot = false;
	data->input->use = false;
	data->input->exit = false;
	return (true);
}

void	init_fps_sync(t_fps_data *fps)
{
	fps->last_frame_time = get_time_ms();
	fps->delta_time = 0;
	fps->target_frame_duration = (1000 / MAX_FPS);
	fps->frame_count = 0;
}

static void	init_game_state(t_cub_data *data)
{
	data->player = init_player(data);
	if (!data->player)
	{
		ft_putstr_fd("Error: Player initialization failed.\n", STDERR_FILENO);
		cleanup_and_exit(data);
	}
	data->player->stats.max_health = 100;
	data->player->stats.health = 100;
	data->player->stats.max_ammo = 200;
	data->player->stats.ammo = 20;
	data->player->stats.damage = 10;
	data->game = ft_calloc(1, sizeof(t_game));
	if (!data->game)
	{
		ft_putstr_fd("Error: Game initialization failed.\n", STDERR_FILENO);
		cleanup_and_exit(data);
	}
	data->game->fov = to_fixed32(START_FOV);
	if (!init_doors(data) || init_enemies(data) || !data->textures)
		cleanup_and_exit(data);
	data->raycasting = init_raycasting(data->mlx->width);
	if (!data->raycasting || !init_input(data))
		cleanup_and_exit(data);
}

static void	init_mlx_runtime(t_cub_data *data)
{
	void	*mlx_ptr;

	mylx_init(data);
	mlx_ptr = data->mlx->mlx_ptr;
	if (!mlx_ptr)
	{
		ft_putstr_fd("Error: MLX initialization failed.\n", STDERR_FILENO);
		exit(ERR_MLX_INIT);
	}
	if (mylx_create_window(data) != ERR_NO_ERROR)
		exit(ERR_WINDOW_CREATE);
	mylx_create_image(data);
	if (load_all_textures(data) != 0)
		ft_putstr_fd("Warning: Failed to load textures, using colors.\n",
			STDERR_FILENO);
	if (!init_hud(data))
	{
		ft_putstr_fd("Error: HUD initialization failed.\n", STDERR_FILENO);
		cleanup_and_exit(data);
	}
	mylx_clear_image(data);
	mylx_make_image(data);
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
	init_game_state(data);
	init_mlx_runtime(data);
}
