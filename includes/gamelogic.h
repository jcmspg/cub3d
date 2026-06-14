/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gamelogic.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 16:55:26 by joamiran          #+#    #+#             */
/*   Updated: 2026/04/08 01:49:10 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAMELOGIC_H
# define GAMELOGIC_H

# include "cub3d.h"
# include "fps_gen.h"

void	update_game_logic(t_cub_data *data);
void	update_doors(t_cub_data *data);
void	interact_doors(t_cub_data *data);
t_door	*get_door_at(t_cub_data *data, int x, int y);
void	player_shoot(t_cub_data *data);
void	check_ammo_pickups(t_cub_data *data);
int		init_enemies(t_cub_data *data);
t_enemy	*get_enemy_at(t_cub_data *data, int x, int y);

#endif /* GAMELOGIC_H */