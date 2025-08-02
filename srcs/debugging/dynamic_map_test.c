/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dynamic_map_test.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 23:20:00 by joamiran          #+#    #+#             */
/*   Updated: 2025/08/02 18:11:55 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_debug.h"

static uint64_t	g_last_animation_time = 0;
static int		g_animation_phase = 0;
static bool		g_animation_enabled = false;

void	init_dynamic_map_test(t_cub_data *data)
{
    if (!data || !data->map)
        return ;
    g_animation_enabled = true;
    g_last_animation_time = get_time_ms();
    g_animation_phase = 0;
    ft_putstr_fd("Dynamic map test initialized! Rapid animations!\n", 1);
}

static void	animate_wave_pattern(t_cub_data *data, int phase)
{
    int	y;
    int	x;
    int	index;
    int	wave;

    if (!data || !data->map || !data->map->map_array)
        return ;
    y = 1;
    while (y < data->map->height - 1)
    {
        x = 1;
        while (x < data->map->width - 1)
        {
            index = y * data->map->width + x;
            if (data->map->map_array[index] == '0' 
                || data->map->map_array[index] == '2')
            {
                wave = (int)(sin((x + y + phase) * 1.5) * 3);
                if (wave > 0)
                    data->map->map_array[index] = '1';
                else
                    data->map->map_array[index] = '2';
            }
            x++;
        }
        y++;
    }
}

static void	animate_border_blink(t_cub_data *data, int phase)
{
    int		x;
    int		y;
    int		bottom_index;
    int		left_index;
    int		right_index;
    bool	blink_on;

    if (!data || !data->map || !data->map->map_array)
        return ;
    blink_on = (phase % 10 < 5);
    x = 0;
    while (x < data->map->width)
    {
        if (data->map->map_array[x] == '1')
            data->map->map_array[x] = blink_on ? '1' : '0';
        bottom_index = (data->map->height - 1) * data->map->width + x;
        if (data->map->map_array[bottom_index] == '1')
            data->map->map_array[bottom_index] = blink_on ? '1' : '0';
        x++;
    }
    y = 0;
    while (y < data->map->height)
    {
        left_index = y * data->map->width;
        right_index = y * data->map->width + (data->map->width - 1);
        if (data->map->map_array[left_index] == '1')
            data->map->map_array[left_index] = blink_on ? '1' : '0';
        if (data->map->map_array[right_index] == '1')
            data->map->map_array[right_index] = blink_on ? '1' : '0';
        y++;
    }
}

static void	clear_rotation_area(t_cub_data *data, int center_x, int center_y)
{
    int	y;
    int	x;
    int	index;

    y = center_y - 4;
    while (y <= center_y + 4)
    {
        x = center_x - 4;
        while (x <= center_x + 4)
        {
            if (x >= 0 && x < data->map->width 
                && y >= 0 && y < data->map->height)
            {
                index = y * data->map->width + x;
                if (data->map->map_array[index] != '1')
                    data->map->map_array[index] = '0';
            }
            x++;
        }
        y++;
    }
}

static void	draw_rotating_arms(t_cub_data *data, int center_x, 
        int center_y, float angle)
{
    int		arm;
    int		dist;
    float	arm_angle;
    int		x;
    int		y;
    int		index;

    arm = 0;
    while (arm < 6)
    {
        arm_angle = angle + (arm * M_PI / 3);
        dist = 1;
        while (dist <= 4)
        {
            x = center_x + (int)(cos(arm_angle) * dist);
            y = center_y + (int)(sin(arm_angle) * dist);
            if (x >= 0 && x < data->map->width 
                && y >= 0 && y < data->map->height)
            {
                index = y * data->map->width + x;
                data->map->map_array[index] = '1';
            }
            dist++;
        }
        arm++;
    }
}

static void	animate_rotation_pattern(t_cub_data *data, int phase)
{
    int		center_x;
    int		center_y;
    float	angle;

    if (!data || !data->map || !data->map->map_array)
        return ;
    center_x = data->map->width / 2;
    center_y = data->map->height / 2;
    clear_rotation_area(data, center_x, center_y);
    angle = (phase * 0.5f);
    draw_rotating_arms(data, center_x, center_y, angle);
}

static void	animate_plasma_pattern(t_cub_data *data, int phase)
{
    int		y;
    int		x;
    int		index;
    float	plasma;

    if (!data || !data->map || !data->map->map_array)
        return ;
    y = 0;
    while (y < data->map->height)
    {
        x = 0;
        while (x < data->map->width)
        {
            index = y * data->map->width + x;
            if (x == 0 || x == data->map->width - 1 
                || y == 0 || y == data->map->height - 1)
            {
                x++;
                continue ;
            }
            plasma = sin(x * 0.5f + phase * 0.3f) 
                + sin(y * 0.3f + phase * 0.2f) 
                + sin((x + y) * 0.25f + phase * 0.4f);
            if (plasma > 0)
                data->map->map_array[index] = '1';
            else
                data->map->map_array[index] = '0';
            x++;
        }
        y++;
    }
}

static void	run_animation_pattern(t_cub_data *data, int pattern)
{
    if (pattern == 0)
        animate_wave_pattern(data, g_animation_phase);
    else if (pattern == 1)
        animate_border_blink(data, g_animation_phase);
    else if (pattern == 2)
        animate_rotation_pattern(data, g_animation_phase);
    else if (pattern == 3)
        animate_plasma_pattern(data, g_animation_phase);
}

void	update_dynamic_map_test(t_cub_data *data)
{
    uint64_t	current_time;
    int			pattern;

    if (!g_animation_enabled || !data)
        return ;
    current_time = get_time_ms();
    if (current_time - g_last_animation_time >= 16)
    {
        g_last_animation_time = current_time;
        g_animation_phase++;
        pattern = (g_animation_phase / 120) % 4;
        run_animation_pattern(data, pattern);
        if (g_animation_phase > 900)
        {
            g_animation_enabled = false;
            ft_putstr_fd("FAST dynamic map test completed!\n", 1);
        }
    }
}

void	stop_dynamic_map_test(void)
{
    g_animation_enabled = false;
    ft_putstr_fd("Dynamic map test stopped.\n", 1);
}