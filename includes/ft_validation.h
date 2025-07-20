/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_validation.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 20:48:02 by joamiran          #+#    #+#             */
/*   Updated: 2025/07/20 21:13:56 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_VALIDATION_H
# define FT_VALIDATION_H

# include "cub3d.h"

bool	validate_map_name(const char *map_name);
bool	validate_map_shape(void);
bool	validate_map(const char *map_file);

#endif /* FT_VALIDATION_H */