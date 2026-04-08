/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trig_angles.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 01:40:42 by hladeiro          #+#    #+#             */
/*   Updated: 2026/04/08 01:40:42 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils.h"
#include <math.h>

t_fixed32	normalize_angle_degrees(t_fixed32 angle)
{
	while (angle < 0)
		angle = fixed32_add(angle, to_fixed32(360.0f));
	while (angle >= to_fixed32(360.0f))
		angle = fixed32_sub(angle, to_fixed32(360.0f));
	return (angle);
}

t_fixed32	fixed_radians_to_degrees(t_fixed32 radians)
{
	return (fixed32_mul(radians, to_fixed32(180.0f / M_PI)));
}

t_fixed32	fixed_sin(t_trig *trig, t_fixed32 radians)
{
	t_fixed32	degrees;

	degrees = fixed_radians_to_degrees(radians);
	return (fast_sin(trig, degrees));
}

t_fixed32	fixed_cos(t_trig *trig, t_fixed32 radians)
{
	t_fixed32	degrees;

	degrees = fixed_radians_to_degrees(radians);
	return (fast_cos(trig, degrees));
}
