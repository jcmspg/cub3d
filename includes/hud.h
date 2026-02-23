/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hud.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 21:00:00 by joamiran          #+#    #+#             */
/*   Updated: 2026/02/08 16:03:00 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HUD_H
# define HUD_H

# include "cub3d.h"

int			init_hud(t_cub_data *data);
void		render_hud(t_cub_data *data);
void		render_hud_text(t_cub_data *data);
void		draw_hud_background(t_cub_data *data);
void		draw_hud_minimap(t_cub_data *data);
void		draw_minimap_cell(t_cub_data *data, t_point p, int size);
void		draw_minimap_cell_at(t_cub_data *data, t_point map, t_rect screen);
void		draw_minimap_player(t_cub_data *data, int cell_size);
void		draw_minimap_player_centered(t_cub_data *data, t_rect minimap);
void		draw_hud_rays(t_cub_data *data, t_rect minimap);
void		draw_hud_ray_line(t_cub_data *data, t_line line);

#endif
