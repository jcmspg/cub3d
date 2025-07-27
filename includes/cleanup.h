/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 17:32:58 by joamiran          #+#    #+#             */
/*   Updated: 2025/07/27 18:26:04 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLEANUP_H
# define CLEANUP_H

# include "cub3d.h"

int	cleanup_mylx(t_cub_data *data);
int	cleanup_game(t_game *game);
int	cleanup_map(t_map *map);
int	cleanup_player(t_player *player);
int	cleanup_graphics(t_graphics *graphics);
int	cleanup_input(t_input *input);
int	cleanup_textures(t_texture *textures);
int	cleanup_sprites(t_sprite *sprites);
int	cleanup_fps(t_fps_data fps);
int	cleanup_raycasting(t_raycasting *s_raycasting);

int	cleanup(t_cub_data *data);

int	cleanup_and_exit(t_cub_data *data);

#endif /* CLEANUP_H */