/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extra.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 20:56:58 by joamiran          #+#    #+#             */
/*   Updated: 2026/04/08 21:11:53 by joamiran         ###   ########.fr       */
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

static void	draw_filled_rect_row(t_cub_data *data, t_draw_shape *shape,
		int row_y)
{
	int	i;
	int	pixel_x;
	int	pixel_y;

	i = 0;
	while (i < shape->width)
	{
		pixel_x = shape->x + i;
		pixel_y = row_y;
		if (pixel_x >= 0 && pixel_x < data->mlx->width && pixel_y >= 0
			&& pixel_y < data->mlx->height)
		{
			mylx_pixel_put(data, pixel_x, pixel_y, shape->color);
		}
		i++;
	}
}

void	draw_filled_rect(t_cub_data *data, t_draw_shape *shape)
{
	int	start_y;
	int	j;

	if (!data || !data->mlx || !data->mlx->img || !shape)
		return ;
	start_y = shape->y;
	j = 0;
	while (j < shape->height)
	{
		draw_filled_rect_row(data, shape, start_y + j);
		j++;
	}
}
