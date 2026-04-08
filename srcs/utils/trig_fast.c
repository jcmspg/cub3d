/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trig_fast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 01:40:45 by hladeiro          #+#    #+#             */
/*   Updated: 2026/04/08 01:40:45 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils.h"

static int	angle_to_index(t_fixed32 angle_degrees)
{
	t_fixed32	index_fixed;
	int			index;

	index_fixed = fixed32_mul(angle_degrees, to_fixed32(100.0f));
	index = from_fixed32(index_fixed);
	if (index < 0)
		index = 0;
	if (index >= TRIG_TABLE_SIZE)
		index = TRIG_TABLE_SIZE - 1;
	return (index);
}

static t_fixed32	handle_sin_quadrant(t_trig *trig, t_fixed32 degrees)
{
	int			index;
	t_fixed32	angle_in_quadrant;

	if (degrees <= to_fixed32(90.0f))
		return (trig->sin[angle_to_index(degrees)]);
	if (degrees <= to_fixed32(180.0f))
	{
		angle_in_quadrant = to_fixed32(180.0f) - degrees;
		index = angle_to_index(angle_in_quadrant);
		return (trig->sin[index]);
	}
	if (degrees <= to_fixed32(270.0f))
	{
		angle_in_quadrant = degrees - to_fixed32(180.0f);
		index = angle_to_index(angle_in_quadrant);
		return (-trig->sin[index]);
	}
	angle_in_quadrant = to_fixed32(360.0f) - degrees;
	index = angle_to_index(angle_in_quadrant);
	return (-trig->sin[index]);
}

static t_fixed32	handle_cos_quadrant(t_trig *trig, t_fixed32 degrees)
{
	int			index;
	t_fixed32	angle_in_quadrant;

	if (degrees <= to_fixed32(90.0f))
		return (trig->cos[angle_to_index(degrees)]);
	if (degrees <= to_fixed32(180.0f))
	{
		angle_in_quadrant = to_fixed32(180.0f) - degrees;
		index = angle_to_index(angle_in_quadrant);
		return (-trig->cos[index]);
	}
	if (degrees <= to_fixed32(270.0f))
	{
		angle_in_quadrant = degrees - to_fixed32(180.0f);
		index = angle_to_index(angle_in_quadrant);
		return (-trig->cos[index]);
	}
	angle_in_quadrant = to_fixed32(360.0f) - degrees;
	index = angle_to_index(angle_in_quadrant);
	return (trig->cos[index]);
}

t_fixed32	fast_sin(t_trig *trig, t_fixed32 degrees)
{
	if (!trig || !trig->sin)
		return (0);
	degrees = normalize_angle_degrees(degrees);
	return (handle_sin_quadrant(trig, degrees));
}

t_fixed32	fast_cos(t_trig *trig, t_fixed32 degrees)
{
	if (!trig || !trig->cos)
		return (to_fixed32(1.0f));
	degrees = normalize_angle_degrees(degrees);
	return (handle_cos_quadrant(trig, degrees));
}
