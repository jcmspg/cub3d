/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fps_gen.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 16:37:03 by joamiran          #+#    #+#             */
/*   Updated: 2025/08/04 17:02:07 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FPS_GEN_H
# define FPS_GEN_H

# include "cub3d.h"

# define MAX_FPS 600
# define MIN_FPS 300
# define MIN_FRAME_TIME (1000 / MAX_FPS)
# define MAX_FRAME_TIME (1000 / MIN_FPS)
# define MAX_FRAME_SKIP 100
# define FIXED_STEP_MS 16

// function to get the time and return in an uint64
uint64_t	get_time_ms(void);

// function that handles the fps
int			main_render_loop(t_cub_data *data);

#endif /* FPS_GEN_H */