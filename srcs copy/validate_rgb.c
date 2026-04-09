/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_rgb.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malourei <malourei@student.42lisboa.pt>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 22:05:10 by malourei          #+#    #+#             */
/*   Updated: 2025/06/03 14:39:32 by malourei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

static int	is_all_digit(char *str)
{
	int	i;

	if (!str || str[0] == '\0')
		return (0);
	i = 0;
	while (str[i] != '\0' && str[i] != '\n')
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

static int	val_rgb_digits(char **strs, t_map *map)
{
	int		i;
	long	nb;

	i = 0;
	while (strs[i] != NULL)
	{
		if (!is_all_digit(strs[i]))
		{
			return (0);
		}
		nb = ft_atol(strs[i]);
		if (nb == -1)
		{
			clean(map);
			ft_close(map->fd);
			exit (1);
		}
		i++;
	}
	return (1);
}

int	validate_rgb(t_map *map)
{
	int	i;

	i = 1;
	if (map->coor_c == 0)
	{
		write(1, "ERROR\nMissing C color definition\n", 33);
		i = 0;
	}
	else if (len_strs(map->coor_c) != 3 || !val_rgb_digits(map->coor_c, map))
	{
		write(1, "ERROR\nRGB C length missing!\n", 28);
		i = 0;
	}
	else if (map->coor_f == 0)
	{
		write(1, "ERROR\nMissing F color definition\n", 33);
		i = 0;
	}
	else if (len_strs(map->coor_f) != 3 || !val_rgb_digits(map->coor_f, map))
	{
		write(1, "ERROR\nRGB F length missing!\n", 28);
		i = 0;
	}
	return (i);
}
