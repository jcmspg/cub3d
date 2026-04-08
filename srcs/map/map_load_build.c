/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_load_build.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 00:53:12 by hladeiro          #+#    #+#             */
/*   Updated: 2026/04/08 00:57:05 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map_load_internal.h"

static bool	line_starts_map_block(const char *line)
{
	int	i;

	i = 0;
	while (line[i] == ' ' || line[i] == '\t')
		i++;
	return (line[i] == '0' || line[i] == '1');
}

static char	*normalize_line(const char *line, int width)
{
	char	*clean;
	char	*norm;
	int		len;

	clean = ft_strdup(line);
	if (!clean)
		return (NULL);
	len = ft_strlen(clean);
	if (len > 0 && clean[len - 1] == '\n')
		clean[len - 1] = '\0';
	len = ft_strlen(clean);
	norm = malloc(width + 1);
	if (!norm)
	{
		free(clean);
		return (NULL);
	}
	ft_memcpy(norm, clean, len);
	ft_memset(norm + len, ' ', width - len);
	norm[width] = '\0';
	free(clean);
	return (norm);
}

static bool	process_pre_map_line(char *line, bool *map_started)
{
	if (is_texture_or_color(line) || is_empty_line(line))
		return ((true));
	if (!is_valid_map_line(line))
		return ((false));
	if (!line_starts_map_block(line))
		return ((true));
	*map_started = true;
	return ((true));
}

static bool	populate_line(char *line, t_map *map, char **dest)
{
	if (is_empty_line(line))
	{
		*dest = normalize_line("", map->width);
		if (!*dest)
			return ((false));
		return ((true));
	}
	if (!is_valid_map_line(line))
		return ((false));
	*dest = normalize_line(line, map->width);
	if (!*dest)
		return ((false));
	return ((true));
}

static bool	handle_second_pass_line(t_map *map, char *line, bool *map_started,
		char **clean)
{
	if (!*map_started)
	{
		if (!process_pre_map_line(line, map_started))
			return ((false));
		if (*map_started && !populate_line(line, map, clean))
			return ((false));
		return ((true));
	}
	return (populate_line(line, map, clean));
}

bool	map_second_pass(t_map *map)
{
	char	*line;
	bool	map_started;
	char	*clean;
	int		i;

	map_started = false;
	i = 0;
	while (1)
	{
		line = get_next_line(map->fd);
		if (!line)
			break ;
		clean = NULL;
		if (!handle_second_pass_line(map, line, &map_started, &clean))
			return (free(line), (false));
		free(line);
		if (clean)
		{
			if (i >= map->height)
				return (free(clean), (false));
			map->map_lines[i++] = clean;
		}
	}
	return ((true));
}
