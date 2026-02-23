/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 18:41:20 by joamiran          #+#    #+#             */
/*   Updated: 2026/01/24 20:10:15 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLEANUP_H
# define CLEANUP_H

# include "cub3d.h"

void			cleanup_and_exit(t_cub_data *data);
int				cleanup(t_cub_data *data);
int				cleanup_mylx(t_cub_data *data);
int				cleanup_doors(t_door *doors);
int				cleanup_enemies(t_enemy *enemies);
int				cleanup_game(t_game *game);
int				cleanup_map(t_map *map);
int				cleanup_player(t_player *player);
int				cleanup_graphics(t_graphics *graphics);
int				cleanup_textures(t_textures *textures);
int				cleanup_sprites(t_sprite *sprites);
int				cleanup_fps(t_fps_data fps);
int				cleanup_raycasting(t_raycasting *s_raycasting);
int				cleanup_mlx_struct(t_mlx *mlx);
int				cleanup_input(t_input *input);
void			cleanup_hud(t_hud *hud);

#endif
