/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 21:53:26 by hladeiro          #+#    #+#             */
/*   Updated: 2026/04/06 21:53:28 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/gamelogic.h"
#include "../../includes/movements.h"

static bool	check_collision(t_cub_data *data, int x, int y)
{
	char	cell;
	t_door	*door;
	t_enemy	*enemy;

	if (x < 0 || x >= data->map->width || y < 0 || y >= data->map->height)
		return (true);
	cell = data->map->map_array[y * data->map->width + x];
	if (cell == '1')
		return (true);
	if (cell == 'D')
	{
		door = get_door_at(data, x, y);
		if (door && door->open_amount < 0.7f)
			return (true);
	}
	if (data->game && data->game->enemies)
	{
		enemy = get_enemy_at(data, x, y);
		if (enemy)
			return (true);
	}
	return (false);
}

static bool	check_move_buffer(t_cub_data *data, t_fixed32 x,
		t_fixed32 y, t_fixed32 buffer)
{
	if (check_collision(data, (int)from_fixed32(fixed32_sub(x, buffer)),
			(int)from_fixed32(y)))
		return (true);
	if (check_collision(data, (int)from_fixed32(fixed32_add(x, buffer)),
			(int)from_fixed32(y)))
		return (true);
	if (check_collision(data, (int)from_fixed32(x),
			(int)from_fixed32(fixed32_sub(y, buffer))))
		return (true);
	if (check_collision(data, (int)from_fixed32(x),
			(int)from_fixed32(fixed32_add(y, buffer))))
		return (true);
	return (false);
}

bool	is_valid_move(t_cub_data *data, t_fixed32 new_x, t_fixed32 new_y)
{
	t_fixed32		pos_x;
	t_fixed32		pos_y;
	int				map_x;
	int				map_y;
	const t_fixed32	buffer = to_fixed32(0.2f);

	if (!data || !data->map || !data->map->map_array)
		return (false);
	pos_x = new_x;
	pos_y = new_y;
	map_x = (int)from_fixed32(pos_x);
	map_y = (int)from_fixed32(pos_y);
	if (map_x < 0 || map_x >= data->map->width || map_y < 0
		|| map_y >= data->map->height)
		return (false);
	if (check_collision(data, map_x, map_y))
		return (false);
	if (check_move_buffer(data, pos_x, pos_y, buffer))
		return (false);
	return (true);
}

void	apply_strafe(t_cub_data *data, t_fixed32 strafe_speed,
		t_fixed32 perp_x, t_fixed32 perp_y)
{
	t_fixed32	new_x;
	t_fixed32	new_y;
	t_fixed32	new_pos_x;
	t_fixed32	new_pos_y;

	new_x = fixed32_mul(strafe_speed, perp_x);
	new_y = fixed32_mul(strafe_speed, perp_y);
	new_pos_x = fixed32_add(data->player->x, new_x);
	new_pos_y = fixed32_add(data->player->y, new_y);
	if (!is_valid_move(data, new_pos_x, new_pos_y))
		return ;
	data->player->x = new_pos_x;
	data->player->y = new_pos_y;
}
