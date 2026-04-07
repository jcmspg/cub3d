/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 21:53:34 by hladeiro          #+#    #+#             */
/*   Updated: 2026/04/06 21:53:35 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/gamelogic.h"
#include "../../includes/movements.h"

bool	dirs_are_zero(t_cub_data *data)
{
	if (data->player->dir_x == 0 && data->player->dir_y == 0)
	{
		ft_putstr_fd("Error: Direction vectors are not set\n", STDERR_FILENO);
		return (true);
	}
	return (false);
}

t_fixed32	normalize_angle_fixed(t_fixed32 angle)
{
	while (angle < 0)
		angle = fixed32_add(angle, to_fixed32(360.0f));
	while (angle >= to_fixed32(360.0f))
		angle = fixed32_sub(angle, to_fixed32(360.0f));
	return (angle);
}
