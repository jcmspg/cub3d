/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   weapon.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 17:00:00 by joamiran          #+#    #+#             */
/*   Updated: 2026/03/22 17:17:01 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/render.h"

static bool	is_transparent_pixel(int pixel)
{
	return ((unsigned int)pixel == 0xFF000000U);
}

/**
 * Render a placeholder weapon (rectangle)
 */
void	render_weapon(t_cub_data *data)
{
	int			w_width;
	int			w_height;
	int			x_start;
	int			y_start;
	int			x;
	int			y;
	int			color;
	t_texture	*weapon_texture;
	int			tex_x;
	int			tex_y;
	int			tex_color;
	int			bob_y;
	int			screen_x;
	int			screen_y;

	weapon_texture = NULL;
	if (data->textures)
		weapon_texture = &data->textures->gun_pov;
	// Dimensions relative to screen
	if (weapon_texture && weapon_texture->loaded && weapon_texture->width > 0
		&& weapon_texture->height > 0)
	{
		w_height = data->mlx->height / 2;
		w_width = (w_height * weapon_texture->width) / weapon_texture->height;
	}
	else
	{
		w_width = data->mlx->width / 6;
		w_height = data->mlx->height / 3;
	}
	// Position: Bottom center
	x_start = (data->mlx->width / 2) - (w_width / 2);
	// slightly offset to right like Doom/Wolf3D often is, or just center?
	// User asked for "weapon texture placeholder.. for now leave it as a
	// rectangle" Let's stick to center-ish right or center. Center is classic
	// Wolf3D. Actually Wolf3D is center. Doom is center.
	// Let's add extensive bobbing to make it feel "alive" later,
	// for now static or simple bob from player
	bob_y = 0;
	// Simple bobbing if moving
	if (data->input->forward || data->input->backward || data->input->left
		|| data->input->right)
		bob_y = abs(data->player->bob_offset) * 2;
	y_start = data->mlx->height - w_height + bob_y;
	color = 0x555555; // Grey gun fallback
	for (y = 0; y < w_height; y++)
	{
		for (x = 0; x < w_width; x++)
		{
			screen_x = x_start + x;
			screen_y = y_start + y;
			if (screen_x < 0 || screen_x >= data->mlx->width || screen_y < 0
				|| screen_y >= data->mlx->height)
				continue ;
			if (weapon_texture && weapon_texture->loaded
				&& weapon_texture->width > 0 && weapon_texture->height > 0)
			{
				tex_x = (x * weapon_texture->width) / w_width;
				tex_y = (y * weapon_texture->height) / w_height;
				tex_color = get_texture_pixel(weapon_texture, tex_x, tex_y);
				if (!is_transparent_pixel(tex_color))
					mylx_pixel_put(data, screen_x, screen_y, tex_color);
				continue ;
			}
			// Simple border check
			if (x == 0 || x == w_width - 1 || y == 0)
				mylx_pixel_put(data, screen_x, screen_y, 0x222222);
			else
				mylx_pixel_put(data, screen_x, screen_y, color);
		}
	}
}
