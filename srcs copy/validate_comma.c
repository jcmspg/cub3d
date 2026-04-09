/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_comma.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malourei <malourei@student.42lisboa.pt>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 21:41:31 by malourei          #+#    #+#             */
/*   Updated: 2025/06/03 14:33:56 by malourei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

static void	clean_comma(t_map *map, char *str)
{
	clean(map);
	free(str);
	ft_close(map->fd);
	exit(1);
}

static int	check_comma(char *str)
{
	int	i;

	i = 1;
	while (str[i] == 32)
		i++;
	if (str[i] == 44)
	{
		write(1, "ERROR\nFound , after C or F!\n", 28);
		return (1);
	}
	while (str[i] != 44)
		i++;
	if (str[i] == 44)
	{
		i++;
		while (str[i] == 32)
			i++;
		if (str[i] == 44)
		{
			write(1, "ERROR\nFound two ,, ate same time!\n", 34);
			return (1);
		}
	}
	return (0);
}

static int	check_comma_reverse(char *str)
{
	size_t	size;

	size = ft_strlen(str) - 1;
	while (str[size] == 32)
		size--;
	if (str[size] == 44)
	{
		write (1, "ERROR\nFound , in end of C or F!\n", 32);
		return (1);
	}
	return (0);
}

void	is_comma(t_map *map, char *str)
{
	if (check_comma(str) || check_comma_reverse(str))
		clean_comma(map, str);
}
