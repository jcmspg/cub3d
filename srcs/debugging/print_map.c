/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 19:07:59 by joamiran          #+#    #+#             */
/*   Updated: 2025/07/29 19:45:51 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_debug.h"

void print_map_array(const t_map *map)
{
    if (!map || !map->map_array)
    {
        printf("Error: Map or map_array is NULL\n");
        return;
    }

    printf("=== MAP DISPLAY (Height: %d, Width: %d) ===\n", map->height, map->width);
    
    for (int y = 0; y < map->height; y++)
    {
        printf("Row %2d: |", y);
        for (int x = 0; x < map->width; x++)
        {
            char c = map->map_array[y * map->width + x];
            
            // Handle special characters for better visibility
            if (c == '\0')
                printf("?");  // Show null chars
            else if (c == '\n')
                printf("\\n"); // Show newlines
            else if (c == '\r')
                printf("\\r"); // Show carriage returns
            else if (c == '\t')
                printf("\\t"); // Show tabs
            else if (c == ' ')
                printf("·");   // Show spaces as dots for visibility
            else
                printf("%c", c);
        }
        printf("|\n");
    }
    printf("=== END MAP ===\n");
}

// Helper function to draw a filled rectangle
void draw_filled_rect(t_graphics *graphics, int x, int y, int width, int height, int color)
{
    // This function isn't actually needed - we'll draw directly
    (void)graphics;
    (void)x;
    (void)y;
    (void)width;
    (void)height;
    (void)color;
}

void draw_map_grid(t_cub_data *data)
{
    if (!data || !data->map || !data->map->map_array || !data->mlx)
        return;

    int cell_size = 10;  // Size of each grid cell in pixels
    int start_x = 10;    // Offset from left edge
    int start_y = 10;    // Offset from top edge

    for (int y = 0; y < data->map->height; y++)
    {
        for (int x = 0; x < data->map->width; x++)
        {
            char c = data->map->map_array[y * data->map->width + x];
            int color;

            // Choose color based on map character
            if (c == '1')
                color = 0xFFFFFF;  // White for walls
            else if (c == '0')
                color = 0x000000;  // Black for empty space
            else if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
                color = 0xFF0000;  // Red for player
            else if (c == '2')
                color = 0x00FF00;  // Green for sprites/items
            else
                color = 0x808080;  // Gray for unknown

            // Draw the cell as a filled rectangle using mylx_pixel_put
            for (int py = 0; py < cell_size; py++)
            {
                for (int px = 0; px < cell_size; px++)
                {
                    int pixel_x = start_x + x * cell_size + px;
                    int pixel_y = start_y + y * cell_size + py;
                    
                    // Check bounds and draw pixel
                    if (pixel_x >= 0 && pixel_x < data->mlx->width && 
                        pixel_y >= 0 && pixel_y < data->mlx->height)
                    {
                        mylx_pixel_put(data, pixel_x, pixel_y, color);
                    }
                }
            }
        }
    }
}


