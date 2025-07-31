/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_tests.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 23:10:00 by joamiran          #+#    #+#             */
/*   Updated: 2025/07/31 21:56:13 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_debug.h"
#include <math.h>

/**
 * Test fixed-point arithmetic accuracy
 */
void test_fixed_point_math(void)
{
    printf("\n=== FIXED-POINT MATH TESTS ===\n");
    
    // Test basic conversions
    printf("Testing conversions:\n");
    t_fixed32 test1 = to_fixed32(3.14159f);
    printf("  3.14159 -> fixed -> float: %.6f\n", from_fixed32(test1));
    
    t_fixed32 test2 = to_fixed32(-2.5f);
    printf("  -2.5 -> fixed -> float: %.6f\n", from_fixed32(test2));
    
    // Test arithmetic operations
    printf("\nTesting arithmetic:\n");
    t_fixed32 a = to_fixed32(5.25f);
    t_fixed32 b = to_fixed32(3.75f);
    
    t_fixed32 sum = fixed32_add(a, b);
    printf("  5.25 + 3.75 = %.6f (expected: 9.0)\n", from_fixed32(sum));
    
    t_fixed32 diff = fixed32_sub(a, b);
    printf("  5.25 - 3.75 = %.6f (expected: 1.5)\n", from_fixed32(diff));
    
    t_fixed32 product = fixed32_mul(a, b);
    printf("  5.25 * 3.75 = %.6f (expected: 19.6875)\n", from_fixed32(product));
    
    // Test precision limits
    printf("\nTesting precision:\n");
    t_fixed32 small = to_fixed32(0.0000153f);  // Close to precision limit
    printf("  Smallest representable: %.10f\n", from_fixed32(small));
    
    printf("=== FIXED-POINT TESTS COMPLETE ===\n\n");
}

/**
 * Test trigonometric lookup tables
 */
void test_trig_tables(t_cub_data *data)
{
    printf("\n=== TRIGONOMETRIC LOOKUP TABLE TESTS ===\n");
    
    if (!data || !data->trig.sin || !data->trig.cos)
    {
        printf("ERROR: Trig tables not initialized!\n");
        return;
    }
    
    printf("Testing specific angles:\n");
    
    // Test known angles
    int test_angles[] = {0, 30, 45, 60, 90, 120, 135, 180, 270, 360};
    float expected_sin[] = {0.0f, 0.5f, 0.707107f, 0.866025f, 1.0f, 0.866025f, 0.707107f, 0.0f, -1.0f, 0.0f};
    float expected_cos[] = {1.0f, 0.866025f, 0.707107f, 0.5f, 0.0f, -0.5f, -0.707107f, -1.0f, 0.0f, 1.0f};
    
    for (int i = 0; i < 10; i++)
    {
        int angle = test_angles[i];
        t_fixed32 sin_result = fast_sin(&data->trig, angle);
        t_fixed32 cos_result = fast_cos(&data->trig, angle);
        
        float sin_val = from_fixed32(sin_result);
        float cos_val = from_fixed32(cos_result);
        
        float sin_error = fabsf(sin_val - expected_sin[i]);
        float cos_error = fabsf(cos_val - expected_cos[i]);
        
        printf("  %3d°: sin=%.6f (exp=%.6f, err=%.6f) cos=%.6f (exp=%.6f, err=%.6f)\n",
               angle, sin_val, expected_sin[i], sin_error, cos_val, expected_cos[i], cos_error);
        
        if (sin_error > 0.001f || cos_error > 0.001f)
            printf("       WARNING: Error exceeds tolerance!\n");
    }
    
    // Test quadrant symmetry
    printf("\nTesting quadrant symmetry:\n");
    for (int base_angle = 15; base_angle <= 75; base_angle += 30)
    {
        t_fixed32 q1_sin = fast_sin(&data->trig, base_angle);
        t_fixed32 q2_sin = fast_sin(&data->trig, 180 - base_angle);
        t_fixed32 q3_sin = fast_sin(&data->trig, 180 + base_angle);
        t_fixed32 q4_sin = fast_sin(&data->trig, 360 - base_angle);
        
        printf("  %d°: Q1=%.4f Q2=%.4f Q3=%.4f Q4=%.4f\n", base_angle,
               from_fixed32(q1_sin), from_fixed32(q2_sin), 
               from_fixed32(q3_sin), from_fixed32(q4_sin));
    }
    
    printf("=== TRIG TABLE TESTS COMPLETE ===\n\n");
}

/**
 * Test coordinate conversion functions
 */
void test_coordinate_conversions(t_cub_data *data)
{
    printf("\n=== COORDINATE CONVERSION TESTS ===\n");
    
    if (!data || !data->map)
    {
        printf("ERROR: Map not initialized!\n");
        return;
    }
    
    printf("Map dimensions: %d x %d\n", data->map->width, data->map->height);
    
    // Test array index to grid coordinates
    printf("\nTesting index_to_coords:\n");
    int test_indices[] = {0, 1, data->map->width, data->map->width + 1, 
                         data->map->width * data->map->height - 1};
    
    for (int i = 0; i < 5; i++)
    {
        int index = test_indices[i];
        if (index >= data->map->width * data->map->height)
            continue;
            
        int x, y;
        index_to_coords(index, data->map->width, &x, &y);
        int back_to_index = coords_to_index(x, y, data->map->width);
        
        printf("  Index %d -> Grid (%d, %d) -> Index %d", index, x, y, back_to_index);
        if (back_to_index == index)
            printf(" ✓\n");
        else
            printf(" ✗ ERROR!\n");
    }
    
    // Test world coordinate collision detection
    printf("\nTesting collision detection:\n");
    
    // Find a wall and empty space for testing
    char wall_char = '1';
    char empty_char = '0';
    int wall_x = -1, wall_y = -1, empty_x = -1, empty_y = -1;
    
    for (int y = 0; y < data->map->height && (wall_x == -1 || empty_x == -1); y++)
    {
        for (int x = 0; x < data->map->width && (wall_x == -1 || empty_x == -1); x++)
        {
            char c = data->map->map_array[y * data->map->width + x];
            if (c == wall_char && wall_x == -1)
            {
                wall_x = x;
                wall_y = y;
            }
            if (c == empty_char && empty_x == -1)
            {
                empty_x = x;
                empty_y = y;
            }
        }
    }
    
    if (wall_x != -1)
    {
        t_fixed32 world_x = to_fixed32((float)wall_x + 0.5f);
        t_fixed32 world_y = to_fixed32((float)wall_y + 0.5f);
        bool is_wall = is_wall_at(data, world_x, world_y);
        printf("  Wall at grid (%d, %d) -> world (%.1f, %.1f): %s",
               wall_x, wall_y, from_fixed32(world_x), from_fixed32(world_y),
               is_wall ? "WALL ✓" : "NOT WALL ✗");
        printf("\n");
    }
    
    if (empty_x != -1)
    {
        t_fixed32 world_x = to_fixed32((float)empty_x + 0.5f);
        t_fixed32 world_y = to_fixed32((float)empty_y + 0.5f);
        bool is_wall = is_wall_at(data, world_x, world_y);
        printf("  Empty at grid (%d, %d) -> world (%.1f, %.1f): %s",
               empty_x, empty_y, from_fixed32(world_x), from_fixed32(world_y),
               is_wall ? "WALL ✗" : "NOT WALL ✓");
        printf("\n");
    }
    
    printf("=== COORDINATE TESTS COMPLETE ===\n\n");
}

/**
 * Run all math tests
 */
void run_all_math_tests(t_cub_data *data)
{
    printf("\n" 
           "████████████████████████████████████████████████████████████████\n"
           "███                    MATH SYSTEM TESTS                    ███\n"
           "████████████████████████████████████████████████████████████████\n");
    
    test_fixed_point_math();
    test_trig_tables(data);
    test_coordinate_conversions(data);
    
    printf("████████████████████████████████████████████████████████████████\n"
           "███                   ALL TESTS COMPLETE                    ███\n"
           "████████████████████████████████████████████████████████████████\n\n");
}