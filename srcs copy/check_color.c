/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_color.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malourei <malourei@student.42lisboa.pt>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 22:25:21 by malourei          #+#    #+#             */
/*   Updated: 2025/06/02 22:26:22 by malourei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

static void	free_color(char **strs, int i)
{
	int	j;

	j = 0;
	while (j < i)
	{
		free(strs[j]);
		j++;
	}
	free(strs);
}

static char	**copy_args(char **strs)
{
	int		i;
	int		j;
	char	**color;

	color = malloc(sizeof(char *) * 4);
	if (!color)
	{
		write(1, "ERROR\nMALLOC FAIL: COLOR!\n", 26);
		return (NULL);
	}
	i = 0;
	j = 0;
	while (strs[++i] != NULL)
	{
		color[j] = ft_strdup(strs[i]);
		if (!color[j])
		{
			write(1, "ERROR\nFT_STRDUP() color!\n", 25);
			free_color(color, j);
			return (NULL);
		}
		j++;
	}
	color[j] = NULL;
	return (color);
}

static void	copy_values(t_map *map, char **strs)
{
	if (strs[0][0] == 'C' && ++map->val_map.count_c)
	{
		if (map->coor_c)
			clear_strs(map->coor_c);
		map->coor_c = copy_args(strs);
		if (!map->coor_c)
		{
			write(1, "ERRO\nCopy color c!\n", 19);
			exit(1);
		}
	}
	else if (strs[0][0] == 'F' && ++map->val_map.count_f)
	{
		if (map->coor_f)
			clear_strs(map->coor_f);
		map->coor_f = copy_args(strs);
		if (!map->coor_f)
		{
			write(1, "ERRO\nCopy color f!\n", 19);
			exit(1);
		}
	}
}

int	check_color_c_f(t_map *map)
{
	if (len_strs(map->val_map.strs) != 4)
	{
		write(1, "ERROR\nInvalid len C or F!\n", 26);
		return (1);
	}
	copy_values(map, map->val_map.strs);
	if (map->val_map.count_c > 1 || map->val_map.count_f > 1)
	{
		write(1, "ERROR\nSome C or F is duplicate!\n", 32);
		return (1);
	}
	return (0);
}
