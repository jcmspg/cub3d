/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hud.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 21:00:00 by joamiran          #+#    #+#             */
/*   Updated: 2026/02/08 16:03:00 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HUD_H
#define HUD_H

#include "cub3d.h"

// HUD initialization
int init_hud(t_cub_data *data);

// HUD rendering
void render_hud(t_cub_data *data);
void render_hud_text(t_cub_data *data);
void draw_hud_background(t_cub_data *data);
void draw_hud_minimap(t_cub_data *data);
void draw_minimap_cell(t_cub_data *data, int map_x, int map_y, int cell_size);
void draw_minimap_cell_at(t_cub_data *data, int map_x, int map_y, int screen_x,
                          int screen_y, int cell_size);
void draw_minimap_player(t_cub_data *data, int cell_size);
void draw_minimap_player_centered(t_cub_data *data, int minimap_x,
                                  int minimap_y, int minimap_size,
                                  int cell_size);
void draw_hud_rays(t_cub_data *data, int minimap_x, int minimap_y,
                   int minimap_size, int cell_size);
void draw_hud_ray_line(t_cub_data *data, int x0, int y0, int x1, int y1,
                       int color);

#endif /* HUD_H */
