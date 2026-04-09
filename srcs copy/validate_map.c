/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_map.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malourei <malourei@student.42lisboa.pt>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 20:05:55 by malourei          #+#    #+#             */
/*   Updated: 2025/06/08 01:50:22 by malourei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

static int	is_valid_char_map(char c)
{
	return (c == '0' || c == '1' || c == ' ' || c == 'N' || c == 'S'
		|| c == 'W' || c == 'E');
}

static void	validate_char_map(t_map *map)
{
	int	i;
	int	j;

	i = 0;
	while (map->map[i])
	{
		j = 0;
		while (map->map[i][j])
		{
			if (is_valid_char_map(map->map[i][j]) == 0)
			{
				write(1, "ERROR\nFound a forbidden char!\n", 30);
				clean(map);
				exit (1);
			}
			j++;
		}
		i++;
	}
}

static int	count_char_2(char *str)
{
	int	i;

	i = 0;
	while (str[i] == 32 && str[i + 1] != '\0')
		i++;
	if (str[i] == 48 || str[i] == 49)
		return (1);
	else
		return (0);
}

static void	validate_map_help(t_map *map, char *tmp, int fd)
{
	int	flag;
	int	j;

	flag = 0;
	j = 0;
	while (tmp != NULL)
	{
		tmp[ft_strlen(tmp) - 1] = '\0';
		if (count_char_2(tmp) == 1 || flag == 1)
		{
			flag = 1;
			ft_memcpy(map->map[j], tmp, ft_strlen(tmp));
			j++;
		}
		free(tmp);
		tmp = get_next_line(fd);
	}
}

void	validate_map(t_map *map, char *path_map)
{
	char	*tmp;
	int		fd;

	create_map(map);
	fd = open(path_map, O_RDONLY);
	if (fd < 0)
	{
		write(1, "ERROR\nFailed to open map for 2 nd time!\n", 40);
		exit(1);
	}
	tmp = get_next_line(fd);
	validate_map_help(map, tmp, fd);
	ft_close(fd);
	validate_char_map(map);
	player_values(map);
	validate_map_playability(map);
}
