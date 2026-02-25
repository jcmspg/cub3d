/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 19:46:00 by joamiran          #+#    #+#             */
/*   Updated: 2025/09/05 19:47:27 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RENDER_H
# define RENDER_H


#include "cub3d.h"

int		init_textures(t_cub_data *data);
void	draw_walls_textured(t_cub_data *data);


#endif /* RENDER_H */