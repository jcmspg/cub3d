/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movements.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 18:41:20 by joamiran          #+#    #+#             */
/*   Updated: 2025/08/25 21:03:10 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MOVEMENTS_H
# define MOVEMENTS_H

# include "cub3d.h"

void		move_player(t_cub_data *data, t_fixed32 move_speed);
void		strafe_player(t_cub_data *data, t_fixed32 strafe_speed);
void		rotate_player(t_cub_data *data, t_fixed32 rotation_angle);
void		move_player_x(t_cub_data *data, t_fixed32 speed);
void		move_player_y(t_cub_data *data, t_fixed32 speed);

#endif
