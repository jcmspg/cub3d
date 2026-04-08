/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_validate.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 20:47:37 by joamiran          #+#    #+#             */
/*   Updated: 2025/11/04 20:02:05 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

typedef struct s_point
{
	int	x;
	int	y;
}t_point;

static bool	is_walkable_tile(char c)
{
	return (c == '0' || c == '2' || c == 'N' || c == 'S' || c == 'E'
		|| c == 'W' || c == 'D' || c == 'M' || c == 'X');
}

static bool	has_open_neighbor(const t_map *map, int x, int y)
{
	int		dx;
	int		dy;
	int		nx;
	int		ny;
	char	neighbor;

	dy = -1;
	while (dy <= 1)
	{
		dx = -1;
		while (dx <= 1)
		{
			if (!(dx == 0 && dy == 0))
			{
				nx = x + dx;
				ny = y + dy;
				if (nx < 0 || ny < 0 || nx >= map->width || ny >= map->height)
					return (true);
				neighbor = map->map_array[ny * map->width + nx];
				if (neighbor == ' ')
					return (true);
			}
			dx++;
		}
		dy++;
	}
	return (false);
}

static bool	flood_fill_closed_check(const t_map *map, int spawn_x, int spawn_y)
{
	char	*visited;
	t_point	*stack;
	int		top;
	int		total;
	t_point	p;
	t_point	n;
	int		i;

	total = map->width * map->height;
	visited = ft_calloc(total, sizeof(char));
	stack = ft_calloc(total, sizeof(t_point));
	if (!visited || !stack)
		return (free(visited), free(stack), false);
	top = 0;
	stack[top++] = (t_point){spawn_x, spawn_y};
	while (top > 0)
	{
		p = stack[--top];
		i = p.y * map->width + p.x;
		if (visited[i])
			continue ;
		visited[i] = 1;
		if (has_open_neighbor(map, p.x, p.y))
			return (free(visited), free(stack), false);
		n = (t_point){p.x + 1, p.y};
		if (n.x >= 0 && n.y >= 0 && n.x < map->width && n.y < map->height
			&& !visited[n.y * map->width + n.x]
			&& is_walkable_tile(map->map_array[n.y * map->width + n.x]))
			stack[top++] = n;
		n = (t_point){p.x - 1, p.y};
		if (n.x >= 0 && n.y >= 0 && n.x < map->width && n.y < map->height
			&& !visited[n.y * map->width + n.x]
			&& is_walkable_tile(map->map_array[n.y * map->width + n.x]))
			stack[top++] = n;
		n = (t_point){p.x, p.y + 1};
		if (n.x >= 0 && n.y >= 0 && n.x < map->width && n.y < map->height
			&& !visited[n.y * map->width + n.x]
			&& is_walkable_tile(map->map_array[n.y * map->width + n.x]))
			stack[top++] = n;
		n = (t_point){p.x, p.y - 1};
		if (n.x >= 0 && n.y >= 0 && n.x < map->width && n.y < map->height
			&& !visited[n.y * map->width + n.x]
			&& is_walkable_tile(map->map_array[n.y * map->width + n.x]))
			stack[top++] = n;
	}
	free(visited);
	free(stack);
	return (true);
}

static bool	validate_symbols_and_spawn(const t_map *map, int *spawn_x,
		int *spawn_y)
{
	int		i;
	int		total;
	int		spawn_count;
	char	c;

	total = map->width * map->height;
	spawn_count = 0;
	i = 0;
	while (i < total)
	{
		c = map->map_array[i];
		if (!is_valid_map_char(c))
			return (false);
		if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
		{
			spawn_count++;
			*spawn_x = i % map->width;
			*spawn_y = i / map->width;
		}
		i++;
	}
	return (spawn_count == 1);
}

static bool	validate_all_walkables_closed(const t_map *map)
{
	int		x;
	int		y;
	char	c;

	y = 0;
	while (y < map->height)
	{
		x = 0;
		while (x < map->width)
		{
			c = map->map_array[y * map->width + x];
			if (is_walkable_tile(c) && has_open_neighbor(map, x, y))
				return (false);
			x++;
		}
		y++;
	}
	return (true);
}

bool	validate_map_name(const char *map_name)
{
	size_t	len;

	if (!map_name || ft_strlen(map_name) == 0)
		return (false);
	len = ft_strlen(map_name);
	return (len > 4 && ft_strcmp(map_name + len - 4, ".cub") == 0);
}

bool	validate_map_shape(const char *map)
{
	int	fd;

	fd = open(map, O_RDONLY);
	if (fd < 0)
		return (false);
	return (close(fd), true);
}

bool	validate_map(const char *map_file)
{
	if (!map_file || ft_strlen(map_file) == 0)
		return (false);
	if (!validate_map_name(map_file))
		return (false);
	if (!validate_map_shape(map_file))
		return (false);
	return (true);
}

bool	validate_loaded_map(t_map *map)
{
	int	spawn_x;
	int	spawn_y;

	if (!map || !map->map_array || map->width <= 0 || map->height <= 0)
		return (false);
	spawn_x = -1;
	spawn_y = -1;
	if (!validate_symbols_and_spawn(map, &spawn_x, &spawn_y))
		return (false);
	if (!validate_all_walkables_closed(map))
		return (false);
	if (!flood_fill_closed_check(map, spawn_x, spawn_y))
		return (false);
	return (true);
}
