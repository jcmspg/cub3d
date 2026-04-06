/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 21:45:00 by joao              #+#    #+#             */
/*   Updated: 2026/01/24 19:48:31 by joamiran         ###   ########.fr       */
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
	printf("🧭 DIRECTION TO ANGLE: '%c' -> %.1f degrees\n", direction,
		from_fixed32(angle));
	return (angle);
}


static t_fixed32	cached_plane_length = 0;
static t_fixed32	last_angle = 999 << 16;


static void	init_cached_plane_length(t_cub_data *data)
{
	t_fixed32	sin_33;
	t_fixed32	cos_33;

	if (cached_plane_length != 0)
		return ;
	sin_33 = fast_sin(&data->trig, to_fixed32(33.0f));
	cos_33 = fast_cos(&data->trig, to_fixed32(33.0f));
	cached_plane_length = fixed32_div(sin_33, cos_33);
}


static void	check_spawn_result(t_player *player, t_map *map, int spawn_x,
		int spawn_y)
{
	if (spawn_x == -1 || spawn_y == -1)
	{
		free(player);
		(void)map;
		ft_putstr_fd("Error: No player spawn position found in map\n",
			STDERR_FILENO);
		exit(ERR_PLAYER_INIT);
	}
}

static void	set_player_spawn(t_player *player, int spawn_x, int spawn_y,
		char direction)
{
	player->x = to_fixed32(spawn_x + 0.5f);
	player->y = to_fixed32(spawn_y + 0.5f);
	player->dir_angle = direction_to_angle(direction);
	player->move_speed = to_fixed32(PLAYER_SPEED);
	player->rotate_speed = to_fixed32(ROTATE_SPEED);
}

static void	player_init_error(const char *msg, int err_code, t_player *player)
{
	if (msg)
		ft_putstr_fd((char *)msg, STDERR_FILENO);
	if (player)
		free(player);
	exit(err_code);
}

static void	find_player_spawn_or_exit(t_cub_data *data, t_player *player,
		int *spawn_x, int *spawn_y)
{
	char	direction;

	direction = 'X';
	printf("🔍 PLAYER INIT: Searching for spawn position...\n");
	if (!look_for_spawn(data->map, spawn_x, spawn_y, &direction))
		player_init_error("Error: Failed to find player spawn position\n",
			ERR_PLAYER_INIT, player);
	check_spawn_result(player, data->map, *spawn_x, *spawn_y);
	set_player_spawn(player, *spawn_x, *spawn_y, direction);
}

void	calc_player_dirs(t_cub_data *data)
{
	t_fixed32	angle_degrees;
	t_fixed32	angle_diff;

	if (!data || !data->player)
		return ;
	angle_degrees = data->player->dir_angle;
	angle_diff = angle_degrees - last_angle;
	if (angle_diff < to_fixed32(0.01f) && angle_diff > to_fixed32(-0.01f))
		return ;
	init_cached_plane_length(data);
	data->player->dir_x = fast_cos(&data->trig, angle_degrees);
	data->player->dir_y = fast_sin(&data->trig, angle_degrees);
	last_angle = angle_degrees;
	data->player->plane_x = fixed32_mul(-data->player->dir_y,
			cached_plane_length);
	data->player->plane_y = fixed32_mul(data->player->dir_x,
			cached_plane_length);
}

t_player	*init_player(t_cub_data *data)
{
	t_player	*player;
	int			spawn_x;
	int			spawn_y;

	if (!data || !data->map)
		player_init_error("Error: Invalid data or map\n", ERR_PLAYER_INIT,
			NULL);
	player = ft_calloc(sizeof(t_player), 1);
	if (!player)
		player_init_error("Error: Memory allocation failed for player\n",
			ERR_MEMORY_ALLOCATION, NULL);
	spawn_x = -1;
	spawn_y = -1;
	find_player_spawn_or_exit(data, player, &spawn_x, &spawn_y);
	calc_player_dirs(data);
	return (player);
}

void	draw_player(t_cub_data *data)
{
	int	player_x;
	int	player_y;
	int	dir_x;
	int	dir_y;

	if (!data || !data->player)
		return ;
	draw_filled_rect(data, from_fixed32(data->player->x) * 12 + 10 - 3,
		from_fixed32(data->player->y) * 12 + 10 - 3, 6, 6, 0xFF0000);
	player_x = from_fixed32(data->player->x) * 12 + 10;
	player_y = from_fixed32(data->player->y) * 12 + 10;
	dir_x = player_x + (int)(from_fixed32(data->player->dir_x) * 12);
	dir_y = player_y + (int)(from_fixed32(data->player->dir_y) * 12);
	draw_filled_rect(data, dir_x - 2, dir_y - 2, 4, 4, 0xFFFF00);
}

void	print_player_coords(t_cub_data *data)
{
	if (!data || !data->player)
		return ;
	printf("Player position: (%.2f, %.2f)\n", from_fixed32(data->player->x),
		from_fixed32(data->player->y));
}
