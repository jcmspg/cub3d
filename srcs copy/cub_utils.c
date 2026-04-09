/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malourei <malourei@student.42lisboa.pt>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 00:52:34 by malourei          #+#    #+#             */
/*   Updated: 2025/05/24 20:50:06 by malourei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

int	len_strs(char **strs)
{
	int	i;

	i = 0;
	while (strs[i] != NULL && strs[i][0] != '\n')
		i++;
	return (i);
}

static long	ft_atol_help(const char *nptr)
{
	int		i;
	int		sign;
	long	result;

	i = 0;
	sign = 1;
	result = 0;
	while (nptr[i] == 32 || (nptr[i] >= 9 && nptr[i] <= 13))
		i++;
	if (nptr[i] == '-')
	{
		sign *= -1;
		i++;
	}
	else if (nptr[i] == '+')
		i++;
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		result = (result * 10) + (nptr[i] - '0');
		i++;
	}
	return (result * sign);
}

long	ft_atol(const char *nptr)
{
	long	number;

	number = ft_atol_help(nptr);
	if (number > 255 || number < 0)
	{
		write(1, "ERROR\nRGB out of [0 - 255]\n", 27);
		return (-1);
	}
	return (number);
}
