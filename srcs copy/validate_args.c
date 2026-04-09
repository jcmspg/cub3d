/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_args.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malourei <malourei@student.42lisboa.pt>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 22:19:25 by malourei          #+#    #+#             */
/*   Updated: 2025/05/27 10:35:20 by malourei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

static int	valid_name(char *s, int l)
{
	return (s[--l] == 'b' && s[--l] == 'u' && s[--l] == 'c' && s[--l] == '.');
}

int	validate_args(int argc, char **argv)
{
	char	*str;
	int		len;

	if (argc != 2)
		return (write(1, "ERROR\nNumber of args invalid!\n", 30));
	str = argv[1];
	len = ft_strlen(str);
	if (len <= 4 || !valid_name(str, len))
		return (write(1, "ERROR\nName of map invalid!\n", 27));
	return (1);
}
