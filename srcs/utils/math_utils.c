/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 20:04:40 by joamiran          #+#    #+#             */
/*   Updated: 2025/11/01 19:43:05 by joamiran         ###   ########.fr       */
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

// Optimized table calculation with better memory access patterns
void	calc_trig_table(t_trig *trig)
{
    int		i;
    double	angle_degrees;  // Use double for initial calculation precision
    double	rad;

    i = 0;
    while (i < TRIG_TABLE_SIZE)
    {
        // Convert index to degrees with 0.01° precision
        angle_degrees = (double)i * 0.01;  // 0.00, 0.01, 0.02, ..., 90.00
        rad = angle_degrees * (M_PI / 180.0);
        
        // Calculate both sin and cos together for better cache usage
        trig->sin[i] = to_fixed32((float)sin(rad));
        trig->cos[i] = to_fixed32((float)cos(rad));
        i++;
    }
    
    printf("Enhanced trigonometric table initialized: %d entries\n", TRIG_TABLE_SIZE);
    printf("Memory usage: %.2f KB\n", (TRIG_TABLE_SIZE * 2 * sizeof(t_fixed32)) / 1024.0f);
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

// Enhanced angle normalization for fractional degrees
t_fixed32	normalize_angle_degrees(t_fixed32 angle)
{
    while (angle < to_fixed32(0.0f))
        angle += to_fixed32(360.0f);
    while (angle >= to_fixed32(360.0f))
        angle -= to_fixed32(360.0f);
    return (angle);
}

// Improved angle-to-index conversion with better precision handling
static int	angle_to_index(t_fixed32 angle_degrees)
{
    t_fixed32 index_fixed;
    int index;
    
    // Convert to hundredths of degrees with proper rounding
    index_fixed = fixed32_mul(angle_degrees, to_fixed32(100.0f));
    index = from_fixed32(index_fixed);
    
    // Ensure we don't go out of bounds
    if (index < 0)
        index = 0;
    if (index >= TRIG_TABLE_SIZE)
        index = TRIG_TABLE_SIZE - 1;
    
    return (index);
}

// Optional: Linear interpolation between table entries for even higher precision
static t_fixed32	interpolate_sin(t_trig *trig, t_fixed32 angle_degrees)
{
    t_fixed32 index_fixed = fixed32_mul(angle_degrees, to_fixed32(100.0f));
    int index = from_fixed32(index_fixed);
    t_fixed32 fraction = index_fixed - to_fixed32((float)index);
    
    if (index >= TRIG_TABLE_SIZE - 1)
        return (trig->sin[TRIG_TABLE_SIZE - 1]);
    
    // Linear interpolation: sin[i] + fraction * (sin[i+1] - sin[i])
    t_fixed32 delta = trig->sin[index + 1] - trig->sin[index];
    return (trig->sin[index] + fixed32_mul(fraction, delta));
}

// Improved quadrant handling
static t_fixed32	handle_sin_quadrant(t_trig *trig, t_fixed32 degrees)
{
    int index;
    t_fixed32 angle_in_quadrant;
    
    if (degrees <= to_fixed32(90.0f))
    {
        // First quadrant: 0° to 90°
        index = angle_to_index(degrees);
        return (trig->sin[index]);
    }
    else if (degrees <= to_fixed32(180.0f))
    {
        // Second quadrant: 90° to 180° (sin is positive, use 180° - angle)
        angle_in_quadrant = to_fixed32(180.0f) - degrees;
        index = angle_to_index(angle_in_quadrant);
        return (trig->sin[index]);
    }
    else if (degrees <= to_fixed32(270.0f))
    {
        // Third quadrant: 180° to 270° (sin is negative)
        angle_in_quadrant = degrees - to_fixed32(180.0f);
        index = angle_to_index(angle_in_quadrant);
        return (-trig->sin[index]);
    }
    else
    {
        // Fourth quadrant: 270° to 360° (sin is negative, use 360° - angle)
        angle_in_quadrant = to_fixed32(360.0f) - degrees;
        index = angle_to_index(angle_in_quadrant);
        return (-trig->sin[index]);
    }
}

// Enhanced fast sine with fractional degree support
t_fixed32	fast_sin(t_trig *trig, t_fixed32 degrees)
{
    degrees = normalize_angle_degrees
(degrees);
    return (handle_sin_quadrant
    (trig, degrees));
}

// Improved quadrant handling
static t_fixed32	handle_cos_quadrant(t_trig *trig, t_fixed32 degrees)
{
    int index;
    t_fixed32 angle_in_quadrant;
    
    if (degrees <= to_fixed32(90.0f))
    {
        // First quadrant: 0° to 90°
        index = angle_to_index(degrees);
        return (trig->cos[index]);
    }
    else if (degrees <= to_fixed32(180.0f))
    {
        // Second quadrant: 90° to 180° (cos is negative)
        angle_in_quadrant = to_fixed32(180.0f) - degrees;
        index = angle_to_index(angle_in_quadrant);
        return (-trig->cos[index]);
    }
    else if (degrees <= to_fixed32(270.0f))
    {
        // Third quadrant: 180° to 270° (cos is negative)
        angle_in_quadrant = degrees - to_fixed32(180.0f);
        index = angle_to_index(angle_in_quadrant);
        return (-trig->cos[index]);
    }
    else
    {
        // Fourth quadrant: 270° to 360° (cos is positive)
        angle_in_quadrant = to_fixed32(360.0f) - degrees;
        index = angle_to_index(angle_in_quadrant);
        return (trig->cos[index]);
    }
}

// Enhanced fast cosine with fractional degree support
t_fixed32	fast_cos(t_trig *trig, t_fixed32 degrees)
{
    degrees = normalize_angle_degrees
(degrees);
    return (handle_cos_quadrant
    (trig, degrees));
}

// Convert fixed-point radians to fixed-point degrees with precision
t_fixed32	fixed_radians_to_degrees(t_fixed32 radians)
{
    return (fixed32_mul(radians, to_fixed32(180.0f / M_PI)));
}

// Enhanced sine function from radians with 0.01° precision
t_fixed32	fixed_sin(t_trig *trig, t_fixed32 radians)
{
    t_fixed32 degrees;

    degrees = fixed_radians_to_degrees
(radians);
    return (fast_sin
    (trig, degrees));
}

// Enhanced cosine function from radians with 0.01° precision
t_fixed32	fixed_cos(t_trig *trig, t_fixed32 radians)
{
    t_fixed32 degrees;

    degrees = fixed_radians_to_degrees
(radians);
    return (fast_cos
    (trig, degrees));
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

