/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   weapon.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 17:00:00 by joamiran          #+#    #+#             */
/*   Updated: 2026/04/08 01:38:57 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/weapon_internal.h"

void	render_weapon(t_cub_data *data)
{
	t_weapon_ctx	ctx;
	t_texture		*tex;
	int				x;

	tex = get_weapon_texture(data);
	set_weapon_dimensions(data, tex, &ctx);
	set_weapon_position(data, &ctx);
	x = 0;
	while (x < ctx.w_width)
	{
		draw_weapon_column(data, tex, &ctx, x);
		x++;
	}
}
