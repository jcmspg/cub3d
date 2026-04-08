/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 17:32:32 by joamiran          #+#    #+#             */
/*   Updated: 2026/04/08 00:31:57 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cleanup.h"
#include <unistd.h>

int	cleanup_mylx(t_cub_data *data)
{
	ft_printf_fd(STDERR_FILENO, "destroying texture images\n");
	cleanup_textures_mlx(data);
	ft_printf_fd(STDERR_FILENO, "destroying img\n");
	mylx_destroy_image(data);
	ft_printf_fd(STDERR_FILENO, "destroying window\n");
	mylx_destroy_window(data);
	ft_printf_fd(STDERR_FILENO, "destroying mlx\n");
	mylx_destroy_mlx(data);
	return (ERR_NO_ERROR);
}

int	cleanup(t_cub_data *data)
{
	if (!data)
		return (ERR_UNKNOWN);
	cleanup_trig_table(&data->trig);
	if (data->mlx)
		cleanup_mylx(data);
	if (data->game)
		cleanup_game(data->game);
	if (data->map)
		cleanup_map(data->map);
	if (data->player)
		cleanup_player(data->player);
	if (data->graphics)
		cleanup_graphics(data->graphics);
	if (data->input)
		cleanup_input(data->input);
	if (data->textures)
		cleanup_textures(data->textures);
	if (data->sprites)
		cleanup_sprites(data->sprites);
	if (data->raycasting)
		cleanup_raycasting(data->raycasting);
	if (data->hud)
		cleanup_hud(data->hud);
	cleanup_fps(data->fps);
	return (ERR_NO_ERROR);
}

int	cleanup_and_exit(t_cub_data *data)
{
	if (!data)
		return (ERR_UNKNOWN);
	if (cleanup(data) != 0)
		return (ERR_CLEAN_UP);
	return (ERR_NO_ERROR);
}
