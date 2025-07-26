/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rng_pixelart.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 21:40:44 by joamiran          #+#    #+#             */
/*   Updated: 2025/07/26 22:18:54 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_debug.h"

int	graphics_init(t_cub_data *data)
{
	int	px_total;

	if (!data->graphics)
	{
		data->graphics = malloc(sizeof(t_graphics));
		if (!data->graphics)
			return (ERR_MEMORY_ALLOCATION);
	}
	px_total = data->mlx->width * data->mlx->height;
	data->graphics->pixels = ft_calloc(px_total, sizeof(int));
	if (!data->graphics->pixels)
		return (ERR_MEMORY_ALLOCATION);
	return (ERR_NO_ERROR);
}

int	test_sync(t_cub_data *data)
{
	if (!data || !data->graphics || !data->mlx)
		return (ERR_GRAPHICS_INIT);

	int px_total;
	int i;

	px_total = (data->mlx->height * data->mlx->width);
	i = 0;
	while (i < px_total)
	{
		data->graphics->x = i % data->mlx->width;
		data->graphics->y = i / data->mlx->width;
		data->graphics->color = 0xFF000000 | (rand() % 256) << 16 | (rand()
				% 256) << 8 | (rand() % 256);
		data->graphics->pixels[i] = data->graphics->color;

		mylx_pixel_put(data, data->graphics->x, data->graphics->y,
			data->graphics->color);
		i++;
	}
	return (ERR_NO_ERROR);
}