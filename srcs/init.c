/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 21:19:09 by joamiran          #+#    #+#             */
/*   Updated: 2026/02/23 03:25:10 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

static int	init_input_struct(t_cub_data *data)
{
	data->input = ft_calloc(sizeof(t_input), 1);
	if (!data->input)
	{
		ft_putstr_fd("Error creating input struct", STDERR_FILENO);
		return (0);
	}
	return (1);
}

static int	init_game_essentials(t_cub_data *data)
{
	data->mlx = ft_calloc(1, sizeof(t_mlx));
	if (!data->mlx)
		return (0);
	data->mlx->width = START_WIDTH;
	data->mlx->height = START_HEIGHT;
	data->mlx->title = "Cub3D Game";
	if (!init_trig_table(data))
		return (0);
	data->player = init_player(data);
	if (!data->player)
		return (0);
	data->player->stats.max_health = 100;
	data->player->stats.health = 100;
	data->player->stats.max_ammo = 200;
	data->player->stats.ammo = 20;
	data->player->stats.damage = 10;
	data->game = ft_calloc(1, sizeof(t_game));
	if (!data->game)
		return (0);
	data->game->fov = to_fixed32(START_FOV);
	return (1);
}

void	init_fps(t_cub_data *data)
{
	data->fps.last_frame_time = get_time_ms();
	data->fps.delta_time = 0;
	data->fps.target_frame_duration = (1000 / MAX_FPS);
	data->fps.frame_count = 0;
}

void	init_game_window(t_cub_data *data)
{
	if (!init_game_essentials(data) || !init_doors(data) || !init_enemies(data))
		cleanup_and_exit(data);
	if (init_textures(data))
		cleanup_and_exit(data);
	data->raycasting = init_raycasting(data->mlx->width);
	if (!data->raycasting || !init_input_struct(data))
		cleanup_and_exit(data);
	mylx_init(data);
	mylx_create_window(data);
	mylx_create_image(data);
	if (!init_hud(data))
		cleanup_and_exit(data);
	mylx_clear_image(data->mlx->img, 0);
	mylx_make_image(data);
}
