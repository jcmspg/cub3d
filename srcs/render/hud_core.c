/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hud_core.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 01:01:02 by hladeiro          #+#    #+#             */
/*   Updated: 2026/04/08 01:01:03 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/hud_internal.h"

int	init_hud(t_cub_data *data)
{
	if (!data)
		return (0);
	data->hud = ft_calloc(1, sizeof(t_hud));
	if (!data->hud)
		return (0);
	data->hud->x = 0;
	data->hud->y = data->mlx->height - HUD_HEIGHT;
	data->hud->width = data->mlx->width;
	data->hud->height = HUD_HEIGHT;
	data->hud->bg_color = HUD_BG_COLOR;
	data->hud->bg_img = NULL;
	data->hud->enabled = true;
	return (1);
}

int	cleanup_hud(t_hud *hud)
{
	if (!hud)
		return (ERR_NO_ERROR);
	if (hud->bg_img)
		hud->bg_img = NULL;
	free(hud);
	return (ERR_NO_ERROR);
}

void	render_hud(t_cub_data *data)
{
	if (!data || !data->hud || !data->hud->enabled)
		return ;
	draw_hud_background(data);
	draw_hud_minimap(data);
}

void	render_hud_text(t_cub_data *data)
{
	if (!data || !data->hud || !data->hud->enabled)
		return ;
	draw_hud_stats(data);
}
