/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   includes.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 21:07:31 by joamiran          #+#    #+#             */
/*   Updated: 2025/07/27 18:34:27 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INCLUDES_H
# define INCLUDES_H

// This file includes all necessary headers for the project.

// C language standard version
# include <fcntl.h>
# include <limits.h>
# include <math.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

// C lib for time functions
# include <sys/stat.h>
# include <sys/time.h>
# include <sys/types.h>
# include <time.h>

// External Libraries
# include "../extLibs/libft/libft.h"
# include "../extLibs/minilibx-linux/mlx.h"

// Project-specific headers
# include "typedefs.h"

// Internal headers
# include "fps_gen.h"
# include "ft_debug.h"
# include "ft_validation.h"
# include "inits.h"
# include "map_parse.h"
# include "mylx_hooks.h"
# include "mylx_init.h"
# include "mylx_utils.h"

// Cleanups
# include "cleanup.h"

// Keys
# include "key_values.h"

#endif /* INCLUDES_H */