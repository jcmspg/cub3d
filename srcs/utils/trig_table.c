/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trig_table.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 01:40:47 by hladeiro          #+#    #+#             */
/*   Updated: 2026/04/08 01:40:48 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils.h"
#include <math.h>

void	calc_trig_table(t_trig *trig)
{
	int		i;
	double	angle_degrees;
	double	rad;

	i = 0;
	while (i < TRIG_TABLE_SIZE)
	{
		angle_degrees = (double)i * 0.01;
		rad = angle_degrees * (M_PI / 180.0);
		trig->sin[i] = to_fixed32((float)sin(rad));
		trig->cos[i] = to_fixed32((float)cos(rad));
		i++;
	}
}

bool	init_trig_table(t_cub_data *data)
{
	if (data->trig.sin != NULL || data->trig.cos != NULL)
		return (true);
	data->trig.sin = (t_fixed32 *)ft_calloc(TRIG_TABLE_SIZE, sizeof(t_fixed32));
	if (!data->trig.sin)
		return (false);
	data->trig.cos = (t_fixed32 *)ft_calloc(TRIG_TABLE_SIZE, sizeof(t_fixed32));
	if (!data->trig.cos)
	{
		free(data->trig.sin);
		data->trig.sin = NULL;
		return (false);
	}
	calc_trig_table(&data->trig);
	return (true);
}

void	cleanup_trig_table(t_trig *trig)
{
	if (trig->sin)
	{
		free(trig->sin);
		trig->sin = NULL;
	}
	if (trig->cos)
	{
		free(trig->cos);
		trig->cos = NULL;
	}
}
