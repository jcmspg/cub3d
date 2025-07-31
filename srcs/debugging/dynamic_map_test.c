/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dynamic_map_test.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 23:20:00 by joamiran          #+#    #+#             */
/*   Updated: 2025/07/31 21:59:35 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_debug.h"

// Global variables for animation state
static uint64_t last_animation_time = 0;
static int animation_phase = 0;
static bool animation_enabled = false;

/**
 * Initialize dynamic map testing
 */
void init_dynamic_map_test(t_cub_data *data)
{
    if (!data || !data->map)
        return;
        
    animation_enabled = true;
    last_animation_time = get_time_ms();
    animation_phase = 0;
    
    printf("🚀 FAST Dynamic map test initialized! Buckle up for rapid animations!\n");
}

/**
 * Create a FAST moving wall pattern
 */
void animate_moving_walls(t_cub_data *data, int phase)
{
    if (!data || !data->map || !data->map->map_array)
        return;
    
    // Find some empty spaces to animate with FASTER wave
    for (int y = 1; y < data->map->height - 1; y++)
    {
        for (int x = 1; x < data->map->width - 1; x++)
        {
            int index = y * data->map->width + x;
            char current = data->map->map_array[index];
            
            // Only animate originally empty spaces
            if (current == '0' || current == '2')  // '2' will be our animated marker
            {
                // Create a MUCH FASTER wave pattern (increased frequency)
                int wave = (int)(sin((x + y + phase) * 1.5) * 3);  // Was 0.5, now 1.5!
                if (wave > 0)
                    data->map->map_array[index] = '1';  // Temporary wall
                else
                    data->map->map_array[index] = '2';  // Animated empty space
            }
        }
    }
}

/**
 * Create FAST blinking borders
 */
void animate_blinking_borders(t_cub_data *data, int phase)
{
    if (!data || !data->map || !data->map->map_array)
        return;
    
    // MUCH FASTER blinking - every 10 frames instead of 60
    bool blink_on = (phase % 10 < 5);  // Was (phase % 60 < 30)
    
    // Animate border walls
    for (int x = 0; x < data->map->width; x++)
    {
        // Top border
        if (data->map->map_array[x] == '1')
            data->map->map_array[x] = blink_on ? '1' : '0';
            
        // Bottom border
        int bottom_index = (data->map->height - 1) * data->map->width + x;
        if (data->map->map_array[bottom_index] == '1')
            data->map->map_array[bottom_index] = blink_on ? '1' : '0';
    }
    
    for (int y = 0; y < data->map->height; y++)
    {
        // Left border
        int left_index = y * data->map->width;
        if (data->map->map_array[left_index] == '1')
            data->map->map_array[left_index] = blink_on ? '1' : '0';
            
        // Right border
        int right_index = y * data->map->width + (data->map->width - 1);
        if (data->map->map_array[right_index] == '1')
            data->map->map_array[right_index] = blink_on ? '1' : '0';
    }
}

/**
 * Create a FAST rotating pattern
 */
void animate_rotating_pattern(t_cub_data *data, int phase)
{
    if (!data || !data->map || !data->map->map_array)
        return;
    
    int center_x = data->map->width / 2;
    int center_y = data->map->height / 2;
    
    // Clear previous pattern
    for (int y = center_y - 4; y <= center_y + 4; y++)  // Bigger pattern area
    {
        for (int x = center_x - 4; x <= center_x + 4; x++)
        {
            if (x >= 0 && x < data->map->width && y >= 0 && y < data->map->height)
            {
                int index = y * data->map->width + x;
                if (data->map->map_array[index] != '1')  // Don't overwrite permanent walls
                    data->map->map_array[index] = '0';
            }
        }
    }
    
    // Create FAST rotating arms (5x faster rotation)
    float angle = (phase * 0.5f);  // Was 0.1f, now 0.5f!
    
    // More arms for cooler effect
    for (int arm = 0; arm < 6; arm++)  // Was 4, now 6 arms
    {
        float arm_angle = angle + (arm * M_PI / 3);  // 60-degree spacing
        for (int dist = 1; dist <= 4; dist++)  // Longer arms
        {
            int x = center_x + (int)(cos(arm_angle) * dist);
            int y = center_y + (int)(sin(arm_angle) * dist);
            
            if (x >= 0 && x < data->map->width && y >= 0 && y < data->map->height)
            {
                int index = y * data->map->width + x;
                data->map->map_array[index] = '1';
            }
        }
    }
}

/**
 * Create a CRAZY FAST plasma pattern
 */
void animate_plasma_pattern(t_cub_data *data, int phase)
{
    if (!data || !data->map || !data->map->map_array)
        return;
    
    // Plasma effect over entire map
    for (int y = 0; y < data->map->height; y++)
    {
        for (int x = 0; x < data->map->width; x++)
        {
            int index = y * data->map->width + x;
            
            // Skip permanent walls (border)
            if (x == 0 || x == data->map->width - 1 || 
                y == 0 || y == data->map->height - 1)
                continue;
            
            // Create plasma effect
            float plasma = sin(x * 0.5f + phase * 0.3f) + 
                          sin(y * 0.3f + phase * 0.2f) + 
                          sin((x + y) * 0.25f + phase * 0.4f);
            
            if (plasma > 0)
                data->map->map_array[index] = '1';
            else
                data->map->map_array[index] = '0';
        }
    }
}

/**
 * Update dynamic map animations - NOW MUCH FASTER!
 */
void update_dynamic_map_test(t_cub_data *data)
{
    if (!animation_enabled || !data)
        return;
    
    uint64_t current_time = get_time_ms();
    
    // Update animation every 16ms (60fps animation!) - Was 50ms
    if (current_time - last_animation_time >= 16)
    {
        last_animation_time = current_time;
        animation_phase++;
        
        // Switch between different animation patterns every 2 seconds (was 5)
        int pattern = (animation_phase / 120) % 4;  // Now 4 patterns
        
        switch (pattern)
        {
            case 0:
                animate_moving_walls(data, animation_phase);
                break;
            case 1:
                animate_blinking_borders(data, animation_phase);
                break;
            case 2:
                animate_rotating_pattern(data, animation_phase);
                break;
            case 3:
                animate_plasma_pattern(data, animation_phase);  // NEW!
                break;
        }
        
        // Stop animation after 15 seconds (was 20)
        if (animation_phase > 900)  // 15 seconds at 60fps
        {
            animation_enabled = false;
            printf("🎯 FAST dynamic map test completed! Hope you enjoyed the show!\n");
        }
    }
}

/**
 * Stop dynamic map testing
 */
void stop_dynamic_map_test(void)
{
    animation_enabled = false;
    printf("⚡ FAST dynamic map test stopped.\n");
}