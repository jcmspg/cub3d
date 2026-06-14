/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 00:48:04 by hladeiro          #+#    #+#             */
/*   Updated: 2026/04/08 00:52:11 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

bool	open_map(char *filename, t_cub_data *data)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (false);
	data->map = ft_calloc(1, sizeof(t_map));
	if (!data->map)
		return (close(fd), (false));
	data->map->fd = fd;
	data->map->filename = ft_strdup(filename);
	if (!data->map->filename)
		return (close(fd), (false));
	if (!create_map_array(data))
		return (close(fd), (false));
	return (true);
}
