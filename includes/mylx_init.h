/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mylx_init.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 21:07:31 by joamiran          #+#    #+#             */
/*   Updated: 2025/11/01 17:55:31 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MYLX_INIT_H
# define MYLX_INIT_H

# include "cub3d.h"

void		mylx_init(t_cub_data *data);
void		mylx_create_window(t_cub_data *data);
void		mylx_create_image(t_cub_data *data);
void		mylx_make_image(t_cub_data *data);

#endif
