/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 20:04:40 by joamiran          #+#    #+#             */
/*   Updated: 2026/02/23 02:30:10 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/gamelogic.h"
#include "../../includes/utils.h"
#include <math.h>

void index_to_coords(int index, int width, int *x, int *y) {
  *x = index % width;
  *y = index / width;
}

int coords_to_index(int x, int y, int width) { return (y * width + x); }

char get_map_char_at(t_cub_data *data, t_fixed32 world_x, t_fixed32 world_y) {
  int map_x;
  int map_y;
  int index;

  map_x = from_fixed32(world_x);
  map_y = from_fixed32(world_y);
  if (map_x < 0 || map_x >= data->map->width || map_y < 0 ||
      map_y >= data->map->height)
    return ('1');
  index = coords_to_index(map_x, map_y, data->map->width);
  return (data->map->map_array[index]);
}

bool is_wall_at(t_cub_data *data, t_fixed32 world_x, t_fixed32 world_y) {
  char c;
  int x;
  int y;
  t_door *door;

  c = get_map_char_at(data, world_x, world_y);
  if (c == '1')
    return (true);
  if (c == 'D') {
    x = from_fixed32(world_x);
    y = from_fixed32(world_y);
    if (!data || !data->game)
      return (true);
    door = get_door_at(data, x, y);
    if (door && door->open_amount < 1.0f)
      return (true);
  }
  return (false);
}

void calc_trig_table(t_trig *trig) {
  int i;
  double angle_degrees;
  double rad;

  i = 0;
  while (i < TRIG_TABLE_SIZE) {
    angle_degrees = (double)i * 0.01;
    rad = angle_degrees * (M_PI / 180.0);
    trig->sin[i] = to_fixed32((float)sin(rad));
    trig->cos[i] = to_fixed32((float)cos(rad));
    i++;
  }
}

int init_trig_table(t_cub_data *data) {
  if (data->trig.sin != NULL || data->trig.cos != NULL)
    return (1);
  data->trig.sin = (t_fixed32 *)ft_calloc(TRIG_TABLE_SIZE, sizeof(t_fixed32));
  if (!data->trig.sin)
    return (0);
  data->trig.cos = (t_fixed32 *)ft_calloc(TRIG_TABLE_SIZE, sizeof(t_fixed32));
  if (!data->trig.cos) {
    free(data->trig.sin);
    data->trig.sin = NULL;
    return (0);
  }
  calc_trig_table(&data->trig);
  return (1);
}

t_fixed32 normalize_angle_degrees(t_fixed32 angle) {
  while (angle < 0)
    angle = fixed32_add(angle, to_fixed32(360.0f));
  while (angle >= to_fixed32(360.0f))
    angle = fixed32_sub(angle, to_fixed32(360.0f));
  return (angle);
}

static int angle_to_index(t_fixed32 angle_degrees) {
  t_fixed32 index_fixed;
  int index;

  index_fixed = fixed32_mul(angle_degrees, to_fixed32(100.0f));
  index = from_fixed32(index_fixed);
  if (index < 0)
    index = 0;
  if (index >= TRIG_TABLE_SIZE)
    index = TRIG_TABLE_SIZE - 1;
  return (index);
}

static t_fixed32 handle_sin_quadrant(t_trig *trig, t_fixed32 degrees) {
  int index;
  t_fixed32 angle_in_quadrant;

  if (degrees <= to_fixed32(90.0f))
    return (trig->sin[angle_to_index(degrees)]);
  else if (degrees <= to_fixed32(180.0f)) {
    angle_in_quadrant = to_fixed32(180.0f) - degrees;
    index = angle_to_index(angle_in_quadrant);
    return (trig->sin[index]);
  } else if (degrees <= to_fixed32(270.0f)) {
    angle_in_quadrant = degrees - to_fixed32(180.0f);
    index = angle_to_index(angle_in_quadrant);
    return (-trig->sin[index]);
  } else {
    angle_in_quadrant = to_fixed32(360.0f) - degrees;
    index = angle_to_index(angle_in_quadrant);
    return (-trig->sin[index]);
  }
}

static t_fixed32 handle_cos_quadrant(t_trig *trig, t_fixed32 degrees) {
  int index;
  t_fixed32 angle_in_quadrant;

  if (degrees <= to_fixed32(90.0f))
    return (trig->cos[angle_to_index(degrees)]);
  else if (degrees <= to_fixed32(180.0f)) {
    angle_in_quadrant = to_fixed32(180.0f) - degrees;
    index = angle_to_index(angle_in_quadrant);
    return (-trig->cos[index]);
  } else if (degrees <= to_fixed32(270.0f)) {
    angle_in_quadrant = degrees - to_fixed32(180.0f);
    index = angle_to_index(angle_in_quadrant);
    return (-trig->cos[index]);
  } else {
    angle_in_quadrant = to_fixed32(360.0f) - degrees;
    index = angle_to_index(angle_in_quadrant);
    return (trig->cos[index]);
  }
}

t_fixed32 fast_sin(t_trig *trig, t_fixed32 degrees) {
  if (!trig || !trig->sin)
    return (0);
  degrees = normalize_angle_degrees(degrees);
  return (handle_sin_quadrant(trig, degrees));
}

t_fixed32 fast_cos(t_trig *trig, t_fixed32 degrees) {
  if (!trig || !trig->cos)
    return (to_fixed32(1.0f));
  degrees = normalize_angle_degrees(degrees);
  return (handle_cos_quadrant(trig, degrees));
}

t_fixed32 fixed_radians_to_degrees(t_fixed32 radians) {
  return (fixed32_mul(radians, to_fixed32(180.0f / M_PI)));
}

t_fixed32 fixed_sin(t_trig *trig, t_fixed32 radians) {
  t_fixed32 degrees;

  degrees = fixed_radians_to_degrees(radians);
  return (fast_sin(trig, degrees));
}

t_fixed32 fixed_cos(t_trig *trig, t_fixed32 radians) {
  t_fixed32 degrees;

  degrees = fixed_radians_to_degrees(radians);
  return (fast_cos(trig, degrees));
}

void cleanup_trig_table(t_trig *trig) {
  if (trig->sin) {
    free(trig->sin);
    trig->sin = NULL;
  }
  if (trig->cos) {
    free(trig->cos);
    trig->cos = NULL;
  }
}