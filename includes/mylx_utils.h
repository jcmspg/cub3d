/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mylx_utils.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 21:07:31 by joamiran          #+#    #+#             */
/*   Updated: 2025/11/01 17:55:31 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MYLX_UTILS_H
# define MYLX_UTILS_H

# include "cub3d.h"

void		mylx_pixel_put(t_cub_data *data, int x, int y, int color);
int			mylx_get_pixel(t_img_data *img, int x, int y);
void		mylx_clear_image(t_img_data *img, int color);
void		mylx_destroy_image(t_cub_data *data);
void		mylx_destroy_window(t_cub_data *data);
void		mylx_destroy_mlx(t_cub_data *data);

#endif
