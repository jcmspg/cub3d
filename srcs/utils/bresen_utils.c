/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bresen_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 16:24:33 by joamiran          #+#    #+#             */
/*   Updated: 2025/09/04 21:12:44 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils.h"


// /**
//  * Initialize Bresenham algorithm
//  */
// void	init_bresenham(t_bres *bres, t_fixed32 x0, t_fixed32 y0, 
//                       t_fixed32 x1, t_fixed32 y1)
// {
//     int	int_x0;
//     int	int_y0;
//     int	int_x1;
//     int	int_y1;

//     if (!bres)
//         return ;
//     int_x0 = (int)from_fixed32(x0);
//     int_y0 = (int)from_fixed32(y0);
//     int_x1 = (int)from_fixed32(x1);
//     int_y1 = (int)from_fixed32(y1);
//     bres->x = int_x0;
//     bres->y = int_y0;
//     bres->dx = abs(int_x1 - int_x0);
//     bres->dy = abs(int_y1 - int_y0);
//     bres->step_x = (int_x0 < int_x1) ? 1 : -1;
//     bres->step_y = (int_y0 < int_y1) ? 1 : -1;
//     bres->err = bres->dx - bres->dy;
// }

// /**
//  * Get next pixel in Bresenham line
//  * Returns: true if there's a next pixel, false if line is complete
//  */
// bool	bresenham_next_pixel(t_bres *bres, t_fixed32 x1, t_fixed32 y1)
// {
//     int	e2;
//     int	target_x;
//     int	target_y;

//     if (!bres)
//         return (false);
//     target_x = (int)from_fixed32(x1);
//     target_y = (int)from_fixed32(y1);
//     if (bres->x == target_x && bres->y == target_y)
//         return (false);
//     e2 = 2 * bres->err;
//     if (e2 > -bres->dy)
//     {
//         bres->err -= bres->dy;
//         bres->x += bres->step_x;
//     }
//     if (e2 < bres->dx)
//     {
//         bres->err += bres->dx;
//         bres->y += bres->step_y;
//     }
//     return (true);
// }

// /**
//  * Draw line between two fixed-point coordinates
//  */
// void	draw_line_fixed(t_cub_data *data, t_fixed32 x0, t_fixed32 y0,
//                         t_fixed32 x1, t_fixed32 y1, int color)
// {
//     t_bres	bres;

//     if (!data)
//         return ;
//     init_bresenham(&bres, x0, y0, x1, y1);
//     mylx_pixel_put(data, bres.x, bres.y, color);
//     while (bresenham_next_pixel(&bres, x1, y1))
//         mylx_pixel_put(data, bres.x, bres.y, color);
// }

// /**
//  * Draw line between two integer coordinates (for convenience)
//  */
// void	draw_line(t_cub_data *data, int x0, int y0, int x1, int y1, int color)
// {
//     draw_line_fixed(data, to_fixed32((float)x0), to_fixed32((float)y0),
//                     to_fixed32((float)x1), to_fixed32((float)y1), color);
// }

// /**
//  * Draw vertical line - optimized for raycasting wall columns
//  */
// void	draw_vertical_line(t_cub_data *data, int x, int y_start,
//                           int y_end, int color)
// {
//     int	y;

//     if (!data || x < 0 || x >= data->mlx->width)
//         return ;
//     if (y_start > y_end)
//     {
//         y = y_start;
//         y_start = y_end;
//         y_end = y;
//     }
//     if (y_start < 0)
//         y_start = 0;
//     if (y_end >= data->mlx->height)
//         y_end = data->mlx->height - 1;
//     y = y_start;
//     while (y <= y_end)
//     {
//         mylx_pixel_put(data, x, y, color);
//         y++;
//     }
// }

// /**
//  * Draw horizontal line - useful for debugging and UI
//  */
// void	draw_horizontal_line(t_cub_data *data, int y, int x_start,
//                             int x_end, int color)
// {
//     int	x;

//     if (!data || y < 0 || y >= data->mlx->height)
//         return ;
//     if (x_start > x_end)
//     {
//         x = x_start;
//         x_start = x_end;
//         x_end = x;
//     }
//     if (x_start < 0)
//         x_start = 0;
//     if (x_end >= data->mlx->width)
//         x_end = data->mlx->width - 1;
//     x = x_start;
//     while (x <= x_end)
//     {
//         mylx_pixel_put(data, x, y, color);
//         x++;
//     }
// }

// /**
//  * Draw filled rectangle - direct coordinate parameters
//  */
// void	draw_filled_rect(t_cub_data *data, int x, int y,
//                         int width, int height, int color)
// {
//     int	end_y;
//     int	current_y;

//     if (!data || width <= 0 || height <= 0)
//         return ;
//     if (x >= data->mlx->width || y >= data->mlx->height)
//         return ;
//     if (x + width < 0 || y + height < 0)
//         return ;
//     end_y = y + height;
//     current_y = y;
//     while (current_y < end_y)
//     {
//         draw_horizontal_line(data, current_y, x, x + width - 1, color);
//         current_y++;
//     }
// }

// /**
//  * Draw rectangle outline - direct coordinate parameters
//  */
// void	draw_rect_outline(t_cub_data *data, int x, int y,
//                          int width, int height, int color)
// {
//     if (!data || width <= 0 || height <= 0)
//         return ;
//     draw_horizontal_line(data, y, x, x + width - 1, color);
//     draw_horizontal_line(data, y + height - 1, x, x + width - 1, color);
//     draw_vertical_line(data, x, y, y + height - 1, color);
//     draw_vertical_line(data, x + width - 1, y, y + height - 1, color);
// }
