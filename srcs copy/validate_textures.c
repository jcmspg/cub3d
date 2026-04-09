/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_textures.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malourei <malourei@student.42lisboa.pt>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:54:01 by malourei          #+#    #+#             */
/*   Updated: 2025/06/03 14:21:54 by malourei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

static void	clean_same(t_map *map, char **strs, char *fre, int fd)
{
	clear_strs(strs);
	free(fre);
	clean(map);
	ft_close(fd);
	ft_close(map->fd);
	exit(1);
}

static char	**validations(t_map *map, char *str, char *fre, int fd)
{
	char	**strs;

	strs = ft_split(str, ' ');
	if (len_strs(strs) == 1)
	{
		clear_strs(strs);
		strs = ft_split(str, 9);
	}
	if (!strs)
	{
		write(1, "ERROR\nSplit texture...\n", 23);
		exit(1);
	}
	if (strs[0][2])
	{
		write(1, "ERROR\nInvalid texture identifier name!\n", 39);
		clean_same(map, strs, fre, fd);
	}
	if (len_strs(strs) != 2)
	{
		write(1, "ERROR\nWrong texture path!\n", 26);
		clean_same(map, strs, fre, fd);
	}
	return (strs);
}

static int	valid_name(char *s)
{
	if (ft_strncmp_reverse(s, "/.xpm", 5) == 0)
		return (1);
	if (ft_strncmp_reverse(s, ".xpm", 4) == 1)
		return (1);
	return (0);
}

void	validate_textures(t_map *map, char *str, char *fre)
{
	char	**strs;
	int		fd;

	strs = NULL;
	fd = -1;
	if (valid_name(str) == 1)
	{
		write(1, "ERROR\nTexture name is wrong!\n", 29);
		clean_same(map, strs, fre, fd);
	}
	strs = validations(map, str, fre, fd);
	fd = open(strs[1], O_RDONLY);
	if (fd < 0)
	{
		write(1, "ERROR\nFailed to open texture!\n", 30);
		clean_same(map, strs, fre, fd);
	}
	ft_close(fd);
	if (texture_select(map, strs) == 1)
		clean_same(map, strs, fre, fd);
	clear_strs(strs);
}
