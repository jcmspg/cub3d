/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 00:48:04 by hladeiro          #+#    #+#             */
/*   Updated: 2026/04/09 02:22:04 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static bool	open_map_fail(t_cub_data *data, int fd)
{
	if (fd >= 0)
		close(fd);
	if (data && data->map)
	{
		cleanup_map(data->map);
		data->map = NULL;
	}
	return (false);
}

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
	data->map->height = 0;
	data->map->width = 0;
	data->map->filename = ft_strdup(filename);
	if (!data->map->filename)
		return (open_map_fail(data, fd));
	if (!create_map_array(data))
		return (open_map_fail(data, data->map->fd));
	if (!validate_loaded_map(data->map))
		return (open_map_fail(data, data->map->fd));
	return (true);
}
