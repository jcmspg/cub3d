/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_textures_help.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malourei <malourei@student.42lisboa.pt>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 22:39:38 by malourei          #+#    #+#             */
/*   Updated: 2025/06/03 11:49:48 by malourei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

static int	check_duplicates(t_map *map, char **strs)
{
	char	c;

	c = strs[0][0];
	if ((c == 'E' && map->val_map.count_ea > 0)
		|| (c == 'W' && map->val_map.count_we > 0)
		|| (c == 'S' && map->val_map.count_so > 0)
		|| (c == 'N' && map->val_map.count_no > 0))
	{
		write(1, "ERROR\nDuplicate texture!\n", 25);
		return (1);
	}
	return (0);
}

static void	select_texture(t_map *map, char **strs)
{
	if (strs[0][0] == 'N' && ++map->val_map.count_no)
		map->textures.tex_no = ft_strdup(strs[1]);
	else if (strs[0][0] == 'W' && ++map->val_map.count_we)
		map->textures.tex_we = ft_strdup(strs[1]);
	else if (strs[0][0] == 'E' && ++map->val_map.count_ea)
		map->textures.tex_ea = ft_strdup(strs[1]);
	else if (strs[0][0] == 'S' && ++map->val_map.count_so)
		map->textures.tex_so = ft_strdup(strs[1]);
}

int	texture_select(t_map *map, char **strs)
{
	if (check_duplicates(map, strs) == 1)
		return (1);
	select_texture(map, strs);
	return (0);
}
