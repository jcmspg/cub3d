/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_map.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malourei <malourei@student.42lisboa.pt>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 13:49:59 by malourei          #+#    #+#             */
/*   Updated: 2025/06/06 18:59:41 by malourei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

static void	clear_memory(char **strs, size_t size)
{
	size_t	i;

	i = 0;
	while (i < size)
	{
		free(strs[i]);
		i++;
	}
	free(strs);
}

void	create_map(t_map *map)
{
	size_t	i;

	//map->map = ft_calloc(map->altura + 1, sizeof(char *));
	map->map = malloc((map->altura + 1) * sizeof(char *));
	if (!map->map)
	{
		write(1, "ERROR\nMap ft_calloc() fail!\n", 28);
		clean(map);
		exit(1);
	}
	i = 0;
	while (i < map->altura)
	{
		//map->map[i] = ft_calloc(map->largura + 1, sizeof(char));
		map->map[i] = malloc((map->largura + 1) * sizeof(char));
		if (!map->map[i])
		{
			write(1, "ERROR\nFailed ft_calloc() map->map!\n", 35);
			clear_memory(map->map, i);
			clean(map);
			exit (1);
		}
		ft_memset(map->map[i], 32, map->largura);
		// se for calloc esta não é preciso
		map->map[i][map->largura] = '\0';
		i++;
	}
	map->map[i] = NULL;
}
