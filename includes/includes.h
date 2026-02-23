/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   includes.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 21:07:31 by joamiran          #+#    #+#             */
/*   Updated: 2025/11/01 17:55:31 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INCLUDES_H
# define INCLUDES_H

# include <fcntl.h>
# include <limits.h>
# include <math.h>
# include <stdbool.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/stat.h>
# include <sys/time.h>
# include <sys/types.h>
# include <time.h>
# include "../extLibs/libft/libft.h"
# if defined(__APPLE__) && defined(__MACH__)
#  include "../extLibs/mlx/mlx.h"
# else
#  include "../extLibs/minilibx-linux/mlx.h"
# endif
# include "../extLibs/poormanfixedpoint/includes/poormansfixed.h"
# include "typedefs.h"
# include "fps_gen.h"
# include "ft_debug.h"
# include "ft_validation.h"
# include "inits.h"
# include "map_parse.h"
# include "mylx_hooks.h"
# include "mylx_init.h"
# include "mylx_utils.h"
# include "cleanup.h"
# include "gamelogic.h"
# include "key_values.h"
# include "utils.h"
# include "movements.h"
# include "player.h"
# include "render.h"
# include "hud.h"

#endif
