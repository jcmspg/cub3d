/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_c_f.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malourei <malourei@student.42lisboa.pt>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 21:45:09 by malourei          #+#    #+#             */
/*   Updated: 2025/06/03 14:31:31 by malourei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

static int	count_virgula(t_string str)
{
	int	v;

	v = 0;
	while (*str)
	{
		if (*str == ',')
			v++;
		str++;
	}
	return (v);
}

static void	clean_val(t_map *map, char *tmp)
{
	clean(map);
	clear_strs(map->val_map.strs);
	free(tmp);
	ft_close(map->fd);
	exit(1);
}

void	validate_c_f(t_map *map, char *tmp)
{
	int	i;

	is_comma(map, tmp);
	i = count_virgula(tmp);
	map->val_map.strs = NULL;
	if (i != 2)
	{
		write(1, "ERROR\nNumber of , is wrong!\n", 28);
		clean_val(map, tmp);
	}
	i = 0;
	map->val_map.strs = ft_split_new(tmp);
	if (!map->val_map.strs)
	{
		write(1, "ERROR\nMalloc from split Color!\n", 31);
		exit(1);
	}
	if (check_color_c_f(map))
		clean_val(map, tmp);
	clear_strs(map->val_map.strs);
}
