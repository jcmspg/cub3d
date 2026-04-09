/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_map_values.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malourei <malourei@student.42lisboa.pt>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 12:26:56 by malourei          #+#    #+#             */
/*   Updated: 2025/06/06 12:27:56 by malourei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

static int	valid_char_palyer(char c)
{
	return (c == 'N' || c == 'S' || c == 'W' || c == 'E');
}

void	init_map_value(t_map *map, char *str)
{
	int		i;
	size_t	tamanho;

	i = 0;
	tamanho = ft_strlen(str);
	while (str[i])
	{
		if (valid_char_palyer(str[i]) == 1)
			map->val_map.count_p++;
		i++;
	}
	if (tamanho > map->largura)
		map->largura = tamanho;
	map->altura++;
}
