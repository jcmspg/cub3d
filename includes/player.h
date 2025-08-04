/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 18:41:20 by joamiran          #+#    #+#             */
/*   Updated: 2025/08/04 19:12:11 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PLAYER_H
# define PLAYER_H

# include "cub3d.h"

// Initialize player with default values
t_player	*init_player(t_cub_data *data);

// // draw the player on the screen
void draw_player(t_cub_data *data);

void print_player_coords(t_cub_data *data);


#endif /* PLAYER_H */ 