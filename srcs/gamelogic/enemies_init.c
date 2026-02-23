/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemies_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 00:50:00 by joamiran          #+#    #+#             */
/*   Updated: 2026/02/23 00:50:00 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/gamelogic.h"

static int count_enemies(t_map *map) {
  int i;
  int count;

  i = 0;
  count = 0;
  while (i < map->width * map->height) {
    if (map->map_array[i] == 'X')
      count++;
    i++;
  }
  return (count);
}

static void init_single_enemy(t_enemy *enemy, int x, int y, int id) {
  enemy->x = to_fixed32(x + 0.5f);
  enemy->y = to_fixed32(y + 0.5f);
  enemy->dir_x = to_fixed32(0.0f);
  enemy->dir_y = to_fixed32(0.0f);
  enemy->stats.health = ENEMY_MAX_HEALTH;
  enemy->stats.max_health = ENEMY_MAX_HEALTH;
  enemy->stats.ammo = ENEMY_AMMO;
  enemy->stats.max_ammo = ENEMY_AMMO;
  enemy->stats.damage = ENEMY_DAMAGE;
  enemy->state = ENEMY_IDLE;
  enemy->id = id;
}

static void fill_enemies_array(t_cub_data *data) {
  int i;
  int e;
  int x;
  int y;

  i = 0;
  e = 0;
  while (i < data->map->width * data->map->height) {
    if (data->map->map_array[i] == 'X') {
      index_to_coords(i, data->map->width, &x, &y);
      init_single_enemy(&data->game->enemies[e], x, y, e);
      printf("👾 Enemy %d spawned at grid (%d, %d)\n", e, x, y);
      e++;
    }
    i++;
  }
}

bool init_enemies(t_cub_data *data) {
  data->game->enemy_count = count_enemies(data->map);
  printf("👾 Found %d enemies in map\n", data->game->enemy_count);
  if (data->game->enemy_count == 0) {
    data->game->enemies = NULL;
    return (true);
  }
  data->game->enemies = malloc(sizeof(t_enemy) * data->game->enemy_count);
  if (!data->game->enemies)
    return (false);
  fill_enemies_array(data);
  return (true);
}

/**
 * Get the enemy at a given grid position (alive only)
 * Returns NULL if no living enemy is at (x, y)
 */
t_enemy *get_enemy_at(t_cub_data *data, int x, int y) {
  int i;

  if (!data->game->enemies)
    return (NULL);
  i = 0;
  while (i < data->game->enemy_count) {
    if (data->game->enemies[i].state != ENEMY_DEAD &&
        (int)from_fixed32(data->game->enemies[i].x) == x &&
        (int)from_fixed32(data->game->enemies[i].y) == y)
      return (&data->game->enemies[i]);
    i++;
  }
  return (NULL);
}
