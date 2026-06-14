/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hud_background.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 01:00:41 by hladeiro          #+#    #+#             */
/*   Updated: 2026/04/08 01:00:42 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/hud_internal.h"

void	draw_hud_background(t_cub_data *data)
{
	int	x;
	int	y;
	int	color;

	if (!data || !data->hud || !data->hud->enabled)
		return ;
	y = data->hud->y - 1;
	while (++y < data->mlx->height)
	{
		x = -1;
		while (++x < data->hud->width)
		{
			if (y < data->hud->y + HUD_BORDER_WIDTH)
				color = HUD_BORDER_COLOR;
			else
				color = data->hud->bg_color;
			mylx_pixel_put(data, x, y, color);
		}
	}
}
