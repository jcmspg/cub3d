/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_systems.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 00:35:22 by hladeiro          #+#    #+#             */
/*   Updated: 2026/04/08 23:23:37 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cleanup.h"
#include <unistd.h>

int	cleanup_textures(t_textures *textures)
{
	ft_printf_fd(STDERR_FILENO, "cleaning t_textures\n");
	if (textures)
		free_textures(textures);
	return (ERR_NO_ERROR);
}

int	cleanup_sprites(t_sprite *sprites)
{
	ft_printf_fd(STDERR_FILENO, "cleaning t_sprites\n");
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
	if (s_raycasting)
	{
		if (s_raycasting->rays)
			free(s_raycasting->rays);
		free(s_raycasting);
	}
	return (ERR_NO_ERROR);
}

int	cleanup_mlx_struct(t_mlx *mlx)
{
	ft_printf_fd(STDERR_FILENO, "cleaning t_mlx struct\n");
	if (mlx)
		free(mlx);
	return (ERR_NO_ERROR);
}
