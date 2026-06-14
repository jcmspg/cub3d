/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_pixel.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 01:33:46 by hladeiro          #+#    #+#             */
/*   Updated: 2026/04/08 01:33:47 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	get_texture_pixel(t_texture *texture, int x, int y)
{
	if (!texture || !texture->loaded || !texture->pixels)
		return (texture->color);
	if (x < 0 || x >= texture->width || y < 0 || y >= texture->height)
		return (texture->color);
	return (texture->pixels[y * texture->width + x]);
}
