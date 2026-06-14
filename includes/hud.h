/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hud.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 21:00:00 by joamiran          #+#    #+#             */
/*   Updated: 2026/04/08 01:55:19 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HUD_H
# define HUD_H

# include "cub3d.h"

int		init_hud(t_cub_data *data);
void	render_hud(t_cub_data *data);
void	render_hud_text(t_cub_data *data);
void	draw_hud_background(t_cub_data *data);
void	draw_hud_minimap(t_cub_data *data);
void	draw_minimap_cell(t_cub_data *data, int map_x, int map_y,
			int cell_size);
void	draw_minimap_player(t_cub_data *data, int cell_size);

#endif /* HUD_H */
