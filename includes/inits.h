/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inits.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 21:18:33 by joamiran          #+#    #+#             */
/*   Updated: 2025/07/26 17:44:12 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INITS_H
# define INITS_H

# include "cub3d.h"

// start the fps generator struct
void	init_fps_sync(t_fps_data *fps);

// start the game window
void	init_game_window(t_cub_data *data);

#endif /* INITS_H */