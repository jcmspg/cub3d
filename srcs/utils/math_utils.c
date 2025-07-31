/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 20:04:40 by joamiran          #+#    #+#             */
/*   Updated: 2025/07/31 20:42:07 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils.h"
#include <math.h>

// Convert 1D array index to 2D coordinates
void index_to_coords(int index, int width, int *x, int *y)
{
    *x = index % width;
    *y = index / width;
}

// Convert 2D coordinates to 1D array index
int coords_to_index(int x, int y, int width)
{
    return (y * width + x);
}

// Get map character at world position
char get_map_char_at(t_cub_data *data, t_fixed32 world_x, t_fixed32 world_y)
{
    int map_x = from_fixed32(world_x);  // Convert fixed-point to int
    int map_y = from_fixed32(world_y);
    
    // Bounds checking
    if (map_x < 0 || map_x >= data->map->width || 
        map_y < 0 || map_y >= data->map->height)
        return ('1');  // Treat out-of-bounds as walls
    
    int index = coords_to_index(map_x, map_y, data->map->width);
    return (data->map->map_array[index]);
}

// Check if position is a wall
bool is_wall_at(t_cub_data *data, t_fixed32 world_x, t_fixed32 world_y)
{
    char c = get_map_char_at(data, world_x, world_y);
    return (c == '1');
}



#define TRIG_TABLE_SIZE 91

void	calc_trig_table(t_trig *trig)
{
    int			i;
    t_fixed32	deg_fixed;
    t_fixed32	rad_fixed;
    t_fixed32	deg_to_rad;

    // Pre-calculate degree-to-radian conversion in fixed-point
    deg_to_rad = fixed_div(FIXED_PI, to_fixed32(180.0f));
    i = 0;
    while (i < TRIG_TABLE_SIZE)
    {
        deg_fixed = to_fixed32((float)i);
        rad_fixed = fixed_mul(deg_fixed, deg_to_rad);
        
        // Use your poormanfixedpoint sin/cos functions if available
        trig->sin[i] = fixed_sin(rad_fixed);  // If pmfp has sin
        trig->cos[i] = fixed_cos(rad_fixed);  // If pmfp has cos
        i++;
    }
}

bool	init_trig_table(t_cub_data *data)
{
    data->trig.sin = (t_fixed32 *)ft_calloc(TRIG_TABLE_SIZE, sizeof(t_fixed32));
    data->trig.cos = (t_fixed32 *)ft_calloc(TRIG_TABLE_SIZE, sizeof(t_fixed32));
    if (!data->trig.sin || !data->trig.cos)
    {
        cleanup_trig_table(&data->trig);
        ft_putstr_fd("Error: Failed to allocate memory for trig table\n", 2);
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

// For fixed-point angle input
t_fixed32	fixed_sin_lookup(t_trig *trig, t_fixed32 angle_fixed)
{
    int	degrees;

    degrees = from_fixed32(angle_fixed);  // Convert fixed to int degrees
    return (fast_sin(trig, degrees));
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

