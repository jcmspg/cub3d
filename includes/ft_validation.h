/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_validation.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 20:48:02 by joamiran          #+#    #+#             */
/*   Updated: 2025/11/04 20:24:42 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_VALIDATION_H
# define FT_VALIDATION_H

# include "cub3d.h"

typedef struct s_mapstruct{
char *so;
char *we;
char *no;
char *ea;
int c[3];
int f[3];
char **map;
double pos;

}		t_mapstruct;

bool	validate_map_name(const char *map_name);
bool	validate_map_shape(const char *map);
bool	validate_map(const char *map_file);

#endif /* FT_VALIDATION_H */
