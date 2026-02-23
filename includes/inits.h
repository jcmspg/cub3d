/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inits.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 19:18:12 by joamiran          #+#    #+#             */
/*   Updated: 2026/01/24 20:10:15 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INITS_H
# define INITS_H

# include "cub3d.h"

void		init_game_window(t_cub_data *data);
int			init_textures(t_cub_data *data);
int			init_sprites(t_cub_data *data);
void		init_fps(t_cub_data *data);
void		init_fps_sync(t_cub_data *data);
int			init_trig_table(t_cub_data *data);
int			init_doors(t_cub_data *data);
int			init_enemies(t_cub_data *data);
int			init_hud(t_cub_data *data);

#endif
