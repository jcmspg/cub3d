/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 20:39:30 by joamiran          #+#    #+#             */
/*   Updated: 2025/07/20 21:31:41 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "inits.h"

int	main(int argc, char **argv)
{
	t_cub_data	data;

	if (argc != 2)
	{
		ft_putstr_fd("Usage: ./cub3d <map_file>\n", 2);
		return (ERR_INVALID_ARG);
	}
	// if (!validate_map(argv[1]))
	// 	return (ERR_INVALID_MAP);
	ft_putstr_fd("Game initialized with map: ", 1);
	ft_putstr_fd(argv[1], 1);
	ft_putchar_fd('\n', 1);
	init_game(&data);
	// Initialize game data
	return (ERR_NO_ERROR);
}
