/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 21:45:00 by joao              #+#    #+#             */
/*   Updated: 2026/01/24 19:46:57 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/render.h"

/*
** Main raycasting entry point
** Called each frame to:
** 1. Reset ray data
** 2. Cast all rays using DDA
** 3. Render 3D walls (ceiling + walls + floor)
** Note: Minimap/debug overlay is drawn separately after this
*/

void	start_rays(t_cub_data *data)
{
	if (!data || !data->raycasting)
		return ;
	reset_all_rays(data->raycasting);
	cast_all_rays(data);
	render_walls(data);
	render_sprites(data);
	render_weapon(data);
}
