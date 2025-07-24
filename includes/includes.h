/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   includes.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 21:07:31 by joamiran          #+#    #+#             */
/*   Updated: 2025/07/24 20:17:01 by joamiran         ###   ########.fr       */
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

// External Libraries
# include "../extLibs/libft/libft.h"
# include "../extLibs/minilibx-linux/mlx.h"

// Project-specific headers
# include "typedefs.h"

// Internal headers
# include "ft_validation.h"
# include "inits.h"
# include "map_parse.h"
# include "mylx_init.h"
# include "mylx_utils.h"

#endif /* INCLUDES_H */