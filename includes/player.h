/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 18:41:20 by joamiran          #+#    #+#             */
/*   Updated: 2026/04/08 01:49:23 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PLAYER_H
# define PLAYER_H

# include "cub3d.h"

t_player	*init_player(t_cub_data *data);
void		draw_player(t_cub_data *data);
void		print_player_coords(t_cub_data *data);
void		calc_player_dirs(t_cub_data *data);

#endif /* PLAYER_H */