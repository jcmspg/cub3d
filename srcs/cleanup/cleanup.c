/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 17:32:32 by joamiran          #+#    #+#             */
/*   Updated: 2025/07/29 19:17:33 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cleanup.h"

int	cleanup_mylx(t_cub_data *data)
{
	ft_printf_fd(STDERR_FILENO, "destroying img\n");
	mylx_destroy_image(data);
	ft_printf_fd(STDERR_FILENO, "destroying window\n");
	mylx_destroy_window(data);
	ft_printf_fd(STDERR_FILENO, "destroying mlx\n");
	mylx_destroy_mlx(data);
	return (ERR_NO_ERROR);
}

int	cleanup_game(t_game *game)
{
	ft_printf_fd(STDERR_FILENO, "cleaning t_game\n");
	// * to do * //
	(void)game;
	return (ERR_NO_ERROR);
}

int	cleanup_map(t_map *map)
{
	int	i;

	ft_printf_fd(STDERR_FILENO, "cleaning t_map\n");
	if (map)
	{
		if (map->map_array)
			free(map->map_array);
		if (map->filename)
			free(map->filename);
		if (map->map_lines)
		{
			i = 0;
			while (i < map->height)
			{
				if (map->map_lines[i])
					free(map->map_lines[i]);
				i++;
			}
			free(map->map_lines);
		}
		// Free any other allocated fields in t_map
		free(map);
	}
	return (ERR_NO_ERROR);
}

int	cleanup_player(t_player *player)
{
	ft_printf_fd(STDERR_FILENO, "cleaning t_player\n");
	// * to do * //
	(void)player;
	return (ERR_NO_ERROR);
}

int	cleanup_graphics(t_graphics *graphics)
{
	ft_printf_fd(STDERR_FILENO, "cleaning t_graphics\n");
	if (graphics->pixels)
		free(graphics->pixels);
	free(graphics);
	return (ERR_NO_ERROR);
}

int	cleanup_input(t_input *input)
{
	ft_printf_fd(STDERR_FILENO, "cleaning t_input\n");
	// * to do * //
	(void)input;
	return (ERR_NO_ERROR);
}

int	cleanup_textures(t_texture *textures)
{
	ft_printf_fd(STDERR_FILENO, "cleaning t_textures\n");
	// * to do * //
	(void)textures;
	return (ERR_NO_ERROR);
}

int	cleanup_sprites(t_sprite *sprites)
{
	ft_printf_fd(STDERR_FILENO, "cleaning t_sprites\n");
	// * to do * //
	(void)sprites;
	return (ERR_NO_ERROR);
}

int	cleanup_fps(t_fps_data fps)
{
	ft_printf_fd(STDERR_FILENO, "cleaning t_fps_data\n");
	(void)fps;
	return (ERR_NO_ERROR);
}

int	cleanup_raycasting(t_raycasting *s_raycasting)
{
	ft_printf_fd(STDERR_FILENO, "cleaning t_raycasting\n");
	// * to do * //
	(void)s_raycasting;
	return (ERR_NO_ERROR);
}

int	cleanup(t_cub_data *data)
{
	if (!data)
		return (ERR_UNKNOWN);
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
	cleanup_fps(data->fps);
	// free(data);
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