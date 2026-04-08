/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   weapon_ctx.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 01:37:13 by hladeiro          #+#    #+#             */
/*   Updated: 2026/04/08 01:38:14 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/weapon_internal.h"

bool	is_transparent_pixel(int pixel)
{
	return ((unsigned int)pixel == 0xFF000000U);
}

t_texture	*get_weapon_texture(t_cub_data *data)
{
	if (!data->textures)
		return (NULL);
	return (&data->textures->gun_pov);
}

void	set_weapon_dimensions(t_cub_data *data, t_texture *tex,
		t_weapon_ctx *ctx)
{
	if (tex && tex->loaded && tex->width > 0 && tex->height > 0)
	{
		ctx->w_height = data->mlx->height / 2;
		ctx->w_width = (ctx->w_height * tex->width) / tex->height;
	}
	else
	{
		ctx->w_width = data->mlx->width / 6;
		ctx->w_height = data->mlx->height / 3;
	}
}

static int	weapon_bob_offset(t_cub_data *data)
{
	if (data->input->forward || data->input->backward || data->input->left
		|| data->input->right)
		return (abs(data->player->bob_offset) * 2);
	return (0);
}

void	set_weapon_position(t_cub_data *data, t_weapon_ctx *ctx)
{
	ctx->x_start = (data->mlx->width / 2) - (ctx->w_width / 2);
	ctx->y_start = data->mlx->height - ctx->w_height + weapon_bob_offset(data);
	ctx->color = 0x555555;
}
