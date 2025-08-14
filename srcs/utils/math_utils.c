/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 20:04:40 by joamiran          #+#    #+#             */
/*   Updated: 2025/08/14 21:55:06 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils.h"
#include <math.h>

void	index_to_coords(int index, int width, int *x, int *y)
{
    *x = index % width;
    *y = index / width;
}

int	coords_to_index(int x, int y, int width)
{
    return (y * width + x);
}

char	get_map_char_at(t_cub_data *data, t_fixed32 world_x, t_fixed32 world_y)
{
    int	map_x;
    int	map_y;
    int	index;

    map_x = from_fixed32(world_x);
    map_y = from_fixed32(world_y);
    if (map_x < 0 || map_x >= data->map->width || 
        map_y < 0 || map_y >= data->map->height)
        return ('1');
    index = coords_to_index(map_x, map_y, data->map->width);
    return (data->map->map_array[index]);
}

bool	is_wall_at(t_cub_data *data, t_fixed32 world_x, t_fixed32 world_y)
{
    char	c;

    c = get_map_char_at(data, world_x, world_y);
    return (c == '1');
}

void	calc_trig_table(t_trig *trig)
{
    int		i;
    float	rad;

    i = 0;
    while (i < TRIG_TABLE_SIZE)
    {
        rad = (float)i * (M_PI / 180.0f);
        trig->sin[i] = to_fixed32(sinf(rad));
        trig->cos[i] = to_fixed32(cosf(rad));
        i++;
    }
}

bool	init_trig_table(t_cub_data *data)
{
    // Check if already initialized to prevent memory leaks
    if (data->trig.sin != NULL || data->trig.cos != NULL)
    {
        // Already initialized, just return success
        return (true);
    }
    
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

int	normalize_angle_degrees(int angle)
{
    while (angle < 0)
        angle += 360;
    while (angle >= 360)
        angle -= 360;
    return (angle);
}

static t_fixed32	handle_sin_quadrant(t_trig *trig, int degrees)
{
    if (degrees <= 90)
        return (trig->sin[degrees]);
    else if (degrees <= 180)
        return (trig->sin[180 - degrees]);
    else if (degrees <= 270)
        return (-trig->sin[degrees - 180]);
    else
        return (-trig->sin[360 - degrees]);
}

t_fixed32	fast_sin(t_trig *trig, int degrees)
{
    degrees = normalize_angle_degrees(degrees);
    return (handle_sin_quadrant(trig, degrees));
}

static t_fixed32	handle_cos_quadrant(t_trig *trig, int degrees)
{
    if (degrees <= 90)
        return (trig->cos[degrees]);
    else if (degrees <= 180)
        return (-trig->cos[180 - degrees]);
    else if (degrees <= 270)
        return (-trig->cos[degrees - 180]);
    else
        return (trig->cos[360 - degrees]);
}

t_fixed32	fast_cos(t_trig *trig, int degrees)
{
    degrees = normalize_angle_degrees(degrees);
    return (handle_cos_quadrant(trig, degrees));
}

int	fixed_radians_to_degrees(t_fixed32 radians)
{
    float	rad_float;

    rad_float = from_fixed32(radians);
    return ((int)(rad_float * 180.0f / M_PI));
}

t_fixed32	fixed_sin(t_trig *trig, t_fixed32 radians)
{
    int	degrees;

    degrees = fixed_radians_to_degrees(radians);
    return (fast_sin(trig, degrees));
}

t_fixed32	fixed_cos(t_trig *trig, t_fixed32 radians)
{
    int	degrees;

    degrees = fixed_radians_to_degrees(radians);
    return (fast_cos(trig, degrees));
}

t_fixed32	fixed_wrap_pi(t_fixed32 angle)
{
    while (angle < -FIXED_PI)
        angle += FIXED_TWO_PI;
    while (angle > FIXED_PI)
        angle -= FIXED_TWO_PI;
    return (angle);
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

