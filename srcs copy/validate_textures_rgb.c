/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_textures_rgb.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malourei <malourei@student.42lisboa.pt>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 20:44:16 by malourei          #+#    #+#             */
/*   Updated: 2025/06/06 19:24:23 by malourei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

static void	check_tabs(t_map *map, int fd, char *tmp)
{
	int	i;

	i = 0;
	while (tmp[i] != '\0')
	{
		if (tmp[i] == '\t')
		{
			write(1, "ERROR\nTAB FOUND!\n", 17);
			clean(map);
			free(tmp);
			ft_close(fd);
			exit(1);
		}
		i++;
	}
}

static int	check_texture_count(t_map *map)
{
	if (!map->val_map.count_ea || !map->val_map.count_no
		|| !map->val_map.count_so || !map->val_map.count_we)
	{
		write(1, "ERROR\nMissing some texture!\n", 28);
		return (0);
	}
	return (1);
}

static int	count_char(char *str)
{
	if (*str == 48 || *str == 49)
		return (1);
	else
		return (0);
}

static void	validate_texts_help(t_map *map, char *str, int fd)
{
	int		flag;
	char	*tmp;

	flag = 0;
	while (str != NULL)
	{
		check_tabs(map, fd, str);
		tmp = str;
		tmp[ft_strlen(str) - 1] = '\0';
		while (*tmp == ' ' && *(tmp + 1) != '\0')
			tmp++;
		if ((*tmp == 'C' || *tmp == 'F') && *(tmp + 1) == ' ')
			validate_c_f(map, tmp);
		if (!ft_strncmp(tmp, "NO ", 3) || !ft_strncmp(tmp, "WE ", 3)
			|| !ft_strncmp(tmp, "SO ", 3) || !ft_strncmp(tmp, "EA ", 3))
			validate_textures(map, tmp, str);
		if (count_char(tmp) == 1 || flag == 1)
		{
			flag = 1;
			init_map_value(map, str);
		}
		free(str);
		str = get_next_line(fd);
	}
}

void	validate_textures_rgb(int fd, t_map *map)
{
	char	*tmp2;

	tmp2 = get_next_line(fd);
	validate_texts_help(map, tmp2, fd);
	if (!validate_rgb(map) || !check_texture_count(map))
	{
		clean(map);
		ft_close(map->fd);
		exit (1);
	}
}
