/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 21:45:00 by joao              #+#    #+#             */
/*   Updated: 2026/04/08 00:57:05 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

bool	is_texture_or_color(char *line)
{
	if (ft_strncmp(line, "NO ", 3) == 0 || ft_strncmp(line, "SO ", 3) == 0
		|| ft_strncmp(line, "WE ", 3) == 0 || ft_strncmp(line, "EA ", 3) == 0
		|| ft_strncmp(line, "F ", 2) == 0 || ft_strncmp(line, "C ", 2) == 0)
	{
		return (true);
	}
	return (false);
}

bool	is_empty_line(const char *line)
{
	char	*clean;
	int		len;
	int		i;

	if (!line)
		return (true);
	clean = ft_strdup(line);
	if (!clean)
		return (false);
	len = ft_strlen(clean);
	if (len > 0 && clean[len - 1] == '\n')
		clean[len - 1] = '\0';
	i = 0;
	while (clean[i])
	{
		if (clean[i] != ' ' && clean[i] != '\t')
		{
			free(clean);
			return (false);
		}
		i++;
	}
	free(clean);
	return (true);
}

bool	is_valid_map_char(char c)
{
	if (c == '0' || c == '1' || c == '2' || c == 'N' || c == 'S' || c == 'E'
		|| c == 'W' || c == ' ' || c == 'D' || c == 'M' || c == 'X')
		return (true);
	return (false);
}

bool	is_valid_map_line(const char *line)
{
	char	*clean;
	int		len;
	int		i;

	if (!line || !*line)
		return (false);
	clean = ft_strdup(line);
	if (!clean)
		return (false);
	len = ft_strlen(clean);
	if (len > 0 && clean[len - 1] == '\n')
		clean[len - 1] = '\0';
	if (ft_strlen(clean) == 0)
		return (free(clean), false);
	i = 0;
	while (clean[i])
	{
		if (!is_valid_map_char(clean[i]))
			return (free(clean), false);
		i++;
	}
	free(clean);
	return (true);
}

/**
 * Find player starting position
 */
int	find_player_position(t_game *game)
{
	(void)game;
	return (0);
}

/**
 * Get map dimensions
 */
