/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flood_fill.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malourei <malourei@student.42lisboa.pt>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 01:46:58 by malourei          #+#    #+#             */
/*   Updated: 2025/06/11 17:33:19 by malourei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

static int	char_valid(t_map *map, int x, int y)
{
	int		x2;
	int		y2;
	char	**c;
	int		largura;

	c = map->map;
	largura = (int)ft_strlen(c[y]);
	if (y < 0 || y >= (int)map->altura || x < 0 || x >= largura)
		return (1);
	x2 = x - 1;
	y2 = y - 1;
	if (y2 < 0 || y2 >= (int)map->altura || x2 < 0 || x2 >= largura)
		return (1);
	x2 = x + 1;
	y2 = y + 1;
	if (y2 < 0 || y2 >= (int)map->altura || x2 < 0 || x2 >= largura)
		return (1);
	if (c[y + 1][x - 1] == ' ' || c[y - 1][x - 1] == ' '
		|| c[y + 1][x + 1] == ' ' || c[y - 1][x + 1] == ' ')
		return (1);
	return (0);
}

static char	**copy_map_for_fill(char **original, int altura)
{
	int		i;
	char	**copy;

	copy = malloc(sizeof(char *) * (altura + 1));
	if (!copy)
		return (NULL);
	i = 0;
	while (i < altura)
	{
		copy[i] = ft_strdup(original[i]);
		if (!copy[i])
		{
			while (--i >= 0)
				free(copy[i]);
			free(copy);
			return (NULL);
		}
		i++;
	}
	copy[i] = NULL;
	return (copy);
}

static void	exit_help(t_map *map, char **copy, char *str)
{
	write (1, str, ft_strlen(str));
	clear_strs(copy);
	clean(map);
	exit(1);
}

static void	flood_fill(char **copy, int x, int y, t_map *map)
{
	char	c;

	c = copy[y][x];
	if (c == '0' && char_valid(map, x, y) == 1)
		exit_help(map, copy, "ERROR\nMap is open!\n");
	if (c == '1' || c == 'F')
		return ;
	copy[y][x] = 'F';
	flood_fill(copy, x + 1, y, map);
	flood_fill(copy, x - 1, y, map);
	flood_fill(copy, x, y + 1, map);
	flood_fill(copy, x, y - 1, map);
	flood_fill(copy, x + 1, y - 1, map);
	flood_fill(copy, x + 1, y + 1, map);
	flood_fill(copy, x - 1, y + 1, map);
	flood_fill(copy, x - 1, y - 1, map);
}

void	validate_map_playability(t_map *map)
{
	char	**copy;

	copy = copy_map_for_fill(map->map, map->altura);
	if (!copy)
	{
		write(1, "ERROR\nCan't Duplicate map!\n", 27);
		exit(1);
	}
	flood_fill(copy, map->player.coor_x, map->player.coor_y, map);
	clear_strs(copy);
}
