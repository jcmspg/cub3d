/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_validate.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 20:47:37 by joamiran          #+#    #+#             */
/*   Updated: 2025/11/04 20:02:05 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

bool	validate_map_name(const char *map_name)
{
	size_t	len;

	if (!map_name || ft_strlen(map_name) == 0)
		return (false);
	len = ft_strlen(map_name);
	return (len > 4 && ft_strcmp(map_name + len - 4, ".cub") == 0);
}

bool	validate_map_shape(const char *map)
{
	int	fd;

	fd = open(map, O_RDONLY);
	if (fd < 0)
		return (false);
	return (close(fd), true);
}

bool	validate_map(const char *map_file)
{
	if (!map_file || ft_strlen(map_file) == 0)
		return (false);
	if (!validate_map_name(map_file))
		return (false);
	if (!validate_map_shape(map_file))
		return (false);
	return (true);
}
