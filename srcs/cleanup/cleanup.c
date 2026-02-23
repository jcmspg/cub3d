/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 17:32:32 by joamiran          #+#    #+#             */
/*   Updated: 2026/02/23 04:00:10 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cleanup.h"

static void	cleanup_game_components(t_cub_data *data)
{
	if (data->game)
	{
		if (data->game->doors)
			cleanup_doors(data->game->doors);
		if (data->game->enemies)
			cleanup_enemies(data->game->enemies);
		cleanup_game(data->game);
	}
	if (data->map)
		cleanup_map(data->map);
	if (data->player)
		cleanup_player(data->player);
}

static void	cleanup_graphics_components(t_cub_data *data)
{
	if (data->graphics)
		cleanup_graphics(data->graphics);
	if (data->textures)
		cleanup_textures(data->textures);
	if (data->sprites)
		cleanup_sprites(data->sprites);
	if (data->raycasting)
		cleanup_raycasting(data->raycasting);
	if (data->hud)
		cleanup_hud(data->hud);
}

int	cleanup(t_cub_data *data)
{
	if (!data)
		return (ERR_UNKNOWN);
	cleanup_trig_table(&data->trig);
	if (data->mlx)
		cleanup_mylx(data);
	cleanup_game_components(data);
	cleanup_graphics_components(data);
	if (data->input)
		cleanup_input(data->input);
	cleanup_fps(data->fps);
	return (ERR_NO_ERROR);
}

void	cleanup_and_exit(t_cub_data *data)
{
	if (data)
		cleanup(data);
	exit(0);
}
