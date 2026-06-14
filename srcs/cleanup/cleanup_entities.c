/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_entities.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 00:35:03 by hladeiro          #+#    #+#             */
/*   Updated: 2026/04/08 00:35:04 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cleanup.h"
#include <unistd.h>

int	cleanup_game(t_game *game)
{
	ft_printf_fd(STDERR_FILENO, "cleaning t_game\n");
	if (game)
	{
		if (game->doors)
			free(game->doors);
		if (game->enemies)
			free(game->enemies);
		free(game);
	}
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
		free(map);
	}
	return (ERR_NO_ERROR);
}

int	cleanup_player(t_player *player)
{
	ft_printf_fd(STDERR_FILENO, "cleaning t_player\n");
	if (!player)
		return (ERR_PLAYER_INIT);
	free(player);
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
	if (!input)
		return (ERR_NO_ERROR);
	ft_printf_fd(STDERR_FILENO, "cleaning t_input\n");
	free(input);
	return (ERR_NO_ERROR);
}
