/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_wall_color.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 01:31:46 by hladeiro          #+#    #+#             */
/*   Updated: 2026/04/08 01:31:46 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	get_wall_color(t_ray *ray, t_textures *textures)
{
	if (ray->hit_content == 'D')
		return (0x8B4513);
	if (ray->side == 0)
	{
		if (ray->step_x > 0)
			return (textures->walls[TEX_WEST].color);
		return (textures->walls[TEX_EAST].color);
	}
	if (ray->step_y > 0)
		return (textures->walls[TEX_NORTH].color);
	return (textures->walls[TEX_SOUTH].color);
}
