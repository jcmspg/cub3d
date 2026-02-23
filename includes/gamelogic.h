/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gamelogic.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 17:32:58 by joamiran          #+#    #+#             */
/*   Updated: 2026/01/24 20:30:15 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAMELOGIC_H
# define GAMELOGIC_H

# include "cub3d.h"

void		update_game_logic(t_cub_data *data);
void		update_enemies(t_cub_data *data, float delta_time);
void		update_head_bob(t_cub_data *data);
void		update_jump(t_cub_data *data);
void		player_shoot(t_cub_data *data);
void		check_ammo_pickups(t_cub_data *data);
void		handle_player_combat(t_cub_data *data);
void		interact_doors(t_cub_data *data);
void		update_doors(t_cub_data *data, float delta_time);
t_door		*get_door_at(t_cub_data *data, int x, int y);
t_enemy		*get_enemy_at(t_cub_data *data, int x, int y);

#endif
