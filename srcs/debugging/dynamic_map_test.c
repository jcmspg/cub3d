/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dynamic_map_test.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/08 14:00:00 by joamiran          #+#    #+#             */
/*   Updated: 2026/02/23 04:15:10 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	test_dynamic_map(t_cub_data *data)
{
	int	x;
	int	y;

	y = 0;
	while (y < data->map->height)
	{
		x = -1;
		while (++x < data->map->width)
		{
			if (data->map->map_array[y * data->map->width + x] == 'D')
			{
				/* Test logic */
			}
		}
		y++;
	}
}
