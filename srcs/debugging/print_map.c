/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 19:07:59 by joamiran          #+#    #+#             */
/*   Updated: 2025/08/27 18:25:25 by joamiran         ###   ########.fr       */
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
            
            
            if (c == '\0')
                printf("?");  
            else if (c == '\n')
                printf("\\n"); 
            else if (c == '\r')
                printf("\\r"); 
            else if (c == '\t')
                printf("\\t"); 
            else if (c == ' ')
                printf("·");   
            else
                printf("%c", c);
        }
        printf("|\n");
    }
    printf("=== END MAP ===\n");
}


void draw_filled_tri(t_cub_data *data, int x, int y, int width, int height, int color)
{
    if (!data || !data->mlx || !data->mlx->img)
        return;

    int start_x = (x);
    int start_y = (y);

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i <= (width * j) / height; i++)
        {
            int pixel_x = start_x + i;
            int pixel_y = start_y + j;

            
            if (pixel_x >= 0 && pixel_x < data->mlx->width && 
                pixel_y >= 0 && pixel_y < data->mlx->height)
            {
                mylx_pixel_put(data, pixel_x, pixel_y, color);
            }
        }
    }
}


void draw_filled_rect(t_cub_data *data, int x, int y, int width, int height, int color)
{
    if (!data || !data->mlx || !data->mlx->img)
        return;

    int start_x = (x);
    int start_y = (y);

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            int pixel_x = start_x + i;
            int pixel_y = start_y + j;

            
            if (pixel_x >= 0 && pixel_x < data->mlx->width && 
                pixel_y >= 0 && pixel_y < data->mlx->height)
            {
                mylx_pixel_put(data, pixel_x, pixel_y, color);
            }
        }
    }
}

void draw_map_grid(t_cub_data *data)
{
    if (!data || !data->map || !data->map->map_array || !data->mlx)
        return;

    int cell_size = 12;  
    int start_x = 10;    
    int start_y = 10;    
    int border_color = 0x404040;  

    for (int y = 0; y < data->map->height; y++)
    {
        for (int x = 0; x < data->map->width; x++)
        {
            char c = data->map->map_array[y * data->map->width + x];
            int fill_color;

            
            if (c == '1')
                fill_color = 0xFFFFFF;  
            else if (c == '0')
                fill_color = 0x000000;  
            else if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
                fill_color = 0xFF0000;  
            else if (c == '2')
                fill_color = 0x00FF00;  
            else
                fill_color = 0x808080;  

            int base_x = start_x + x * cell_size;
            int base_y = start_y + y * cell_size;

            
            for (int py = 0; py < cell_size; py++)
            {
                for (int px = 0; px < cell_size; px++)
                {
                    int pixel_x = base_x + px;
                    int pixel_y = base_y + py;
                    
                    
                    if (pixel_x >= 0 && pixel_x < data->mlx->width && 
                        pixel_y >= 0 && pixel_y < data->mlx->height)
                    {
                        int color;
                        
                        
                        if (px == 0 || py == 0 || px == cell_size - 1 || py == cell_size - 1)
                            color = border_color;
                        else
                            color = fill_color;
                        
                        mylx_pixel_put(data, pixel_x, pixel_y, color);
                    }
                }
            }
        }
    }
}

void draw_map_grid_enhanced(t_cub_data *data)
{
    if (!data || !data->map || !data->map->map_array || !data->mlx)
        return;

    int cell_size = 20;   
    int start_x = 20;     
    int start_y = 50;     
    int border_color = 0x333333;  

    
    
    

    for (int y = 0; y < data->map->height; y++)
    {
        for (int x = 0; x < data->map->width; x++)
        {
            char c = data->map->map_array[y * data->map->width + x];
            int fill_color;

            
            switch (c) {
                case '1':
                    fill_color = 0xDDDDDD;  
                    break;
                case '0':
                    fill_color = 0x2a2a2a;  
                    break;
                case 'N':
                    fill_color = 0xFF4444;  
                    break;
                case 'S':
                    fill_color = 0xFF6666;  
                    break;
                case 'E':
                    fill_color = 0xFF8888;  
                    break;
                case 'W':
                    fill_color = 0xFFAAAA;  
                    break;
                case '2':
                    fill_color = 0x44FF44;  
                    break;
                case ' ':
                    fill_color = 0x404040;  
                    break;
                default:
                    fill_color = 0xFF00FF;  
                    break;
            }

            int base_x = start_x + x * cell_size;
            int base_y = start_y + y * cell_size;

            
            for (int py = 0; py < cell_size; py++)
            {
                for (int px = 0; px < cell_size; px++)
                {
                    int pixel_x = base_x + px;
                    int pixel_y = base_y + py;
                    
                    if (pixel_x >= 0 && pixel_x < data->mlx->width && 
                        pixel_y >= 0 && pixel_y < data->mlx->height)
                    {
                        int color;
                        
                        
                        if (px == 0 || py == 0 || px == cell_size - 1 || py == cell_size - 1)
                            color = border_color;
                        else
                            color = fill_color;
                        
                        mylx_pixel_put(data, pixel_x, pixel_y, color);
                    }
                }
            }
        }
    }

    
    
}

void draw_map_grid_thick_borders(t_cub_data *data)
{
    if (!data || !data->map || !data->map->map_array || !data->mlx)
        return;

    int cell_size = 15;   
    int border_width = 2; 
    int start_x = 10;
    int start_y = 10;
    int border_color = 0x606060;  

    for (int y = 0; y < data->map->height; y++)
    {
        for (int x = 0; x < data->map->width; x++)
        {
            char c = data->map->map_array[y * data->map->width + x];
            int fill_color;

            
            if (c == '1')
                fill_color = 0xFFFFFF;  
            else if (c == '0')
                fill_color = 0x1a1a1a;  
            else if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
                fill_color = 0xFF0000;  
            else if (c == '2')
                fill_color = 0x00FF00;  
            else
                fill_color = 0x808080;  

            int base_x = start_x + x * cell_size;
            int base_y = start_y + y * cell_size;

            
            for (int py = 0; py < cell_size; py++)
            {
                for (int px = 0; px < cell_size; px++)
                {
                    int pixel_x = base_x + px;
                    int pixel_y = base_y + py;
                    
                    
                    if (pixel_x >= 0 && pixel_x < data->mlx->width && 
                        pixel_y >= 0 && pixel_y < data->mlx->height)
                    {
                        int color;
                        
                        
                        if (px < border_width || py < border_width || 
                            px >= cell_size - border_width || py >= cell_size - border_width)
                            color = border_color;
                        else
                            color = fill_color;
                        
                        mylx_pixel_put(data, pixel_x, pixel_y, color);
                    }
                }
            }
        }
    }
}


