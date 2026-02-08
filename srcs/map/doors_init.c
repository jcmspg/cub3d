/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doors_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 21:00:00 by joamiran          #+#    #+#             */
/*   Updated: 2026/01/24 21:00:00 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static int count_doors(t_map *map) {
  int i;
  int count;

  i = 0;
  count = 0;
  while (i < map->width * map->height) {
    if (map->map_array[i] == 'D')
      count++;
    i++;
  }
  return (count);
}

static void init_single_door(t_door *door, int x, int y) {
  door->x = x;
  door->y = y;
  door->is_open = false;
  door->is_opening = false;
  door->is_closing = false;
  door->open_amount = 0.0f;
}

static void fill_doors_array(t_cub_data *data) {
  int i;
  int d;
  int x;
  int y;

  i = 0;
  d = 0;
  while (i < data->map->width * data->map->height) {
    if (data->map->map_array[i] == 'D') {
      index_to_coords(i, data->map->width, &x, &y);
      init_single_door(&data->game->doors[d], x, y);
      d++;
    }
    i++;
  }
}

bool init_doors(t_cub_data *data) {
  data->game->door_count = count_doors(data->map);
  if (data->game->door_count == 0) {
    data->game->doors = NULL;
    return (true);
  }
  data->game->doors = malloc(sizeof(t_door) * data->game->door_count);
  if (!data->game->doors)
    return (false);
  fill_doors_array(data);
  return (true);
}
