/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 22:30:00 by hladeiro          #+#    #+#             */
/*   Updated: 2026/04/09 02:01:56 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/player.h"

static t_fixed32	direction_to_angle(char direction)
{
	t_fixed32	angle;

	if (direction == 'N')
		angle = to_fixed32(270.0f);
	else if (direction == 'S')
		angle = to_fixed32(90.0f);
	else if (direction == 'E')
		angle = to_fixed32(0.0f);
	else if (direction == 'W')
		angle = to_fixed32(180.0f);
	else
		angle = to_fixed32(0.0f);
	printf("DIRECTION TO ANGLE: '%c' -> %.1f degrees\n", direction,
		from_fixed32(angle));
	return (angle);
}

static int	find_and_validate_spawn(t_cub_data *data, int *x, int *y, char *dir)
{
	printf("PLAYER INIT: Searching for spawn position...\n");
	if (!look_for_spawn(data->map, x, y, dir))
		return (0);
	if (*x == -1 || *y == -1)
		return (0);
	return (1);
}

static void	set_player_spawn(t_player *player, int x, int y, char direction)
{
	player->x = to_fixed32(x + 0.5f);
	player->y = to_fixed32(y + 0.5f);
	player->dir_angle = direction_to_angle(direction);
	player->move_speed = to_fixed32(PLAYER_SPEED);
	player->rotate_speed = to_fixed32(ROTATE_SPEED);
}

static void	exit_player_init_err(char *msg, int err, t_player *player,
		t_cub_data *data)
{
	if (msg)
		ft_putstr_fd(msg, STDERR_FILENO);
	if (player)
		free(player);
	cleanup(data);
	exit(err);
}

t_player	*init_player(t_cub_data *data)
{
	t_player	*player;
	int			spawn_x;
	int			spawn_y;
	char		direction;

	if (!data || !data->map)
		exit_player_init_err("Error: Invalid data or map\n", ERR_PLAYER_INIT,
			NULL, data);
	player = ft_calloc(sizeof(t_player), 1);
	if (!player)
		exit_player_init_err("Error: Memory allocation failed for player\n",
			ERR_MEMORY_ALLOCATION, NULL, data);
	spawn_x = -1;
	spawn_y = -1;
	direction = 'X';
	if (!find_and_validate_spawn(data, &spawn_x, &spawn_y, &direction))
		exit_player_init_err("Error: Failed to find player spawn position\n",
			ERR_PLAYER_INIT, player, data);
	set_player_spawn(player, spawn_x, spawn_y, direction);
	calc_player_dirs(data);
	return (player);
}
