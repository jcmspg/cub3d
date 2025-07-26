/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mylx_utils.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 19:44:03 by joamiran          #+#    #+#             */
/*   Updated: 2025/07/26 22:10:16 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MYLX_UTILS_H
# define MYLX_UTILS_H

# include "cub3d.h"

// Initialize MLX
int		mylx_init(t_cub_data *data);

// Create a new window
int		mylx_create_window(t_cub_data *data);

// Create a new image
int		mylx_create_image(t_cub_data *data);

// Make the image visible in the window
int		mylx_make_image(t_cub_data *data);

// Destroy the image
int		mylx_destroy_image(t_cub_data *data);

// Destroy the window
int		mylx_destroy_window(t_cub_data *data);

// Destroy the MLX instance
int		mylx_destroy_mlx(t_cub_data *data);

// Put a pixel in the image
void	mylx_pixel_put(t_cub_data *data, int x, int y, int color);

// Clear the image
int		mylx_clear_image(t_cub_data *data);

int		mylx_update_scene(t_cub_data *data);

#endif /* MYLX_UTILS_H */