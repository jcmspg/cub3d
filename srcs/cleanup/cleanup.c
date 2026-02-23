/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 17:32:32 by joamiran          #+#    #+#             */
/*   Updated: 2026/01/24 20:36:50 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cleanup.h"
#include <unistd.h>

int cleanup_mylx(t_cub_data *data) {
  ft_printf_fd(STDERR_FILENO, "destroying img\n");
  mylx_destroy_image(data);
  ft_printf_fd(STDERR_FILENO, "destroying window\n");
  mylx_destroy_window(data);
  ft_printf_fd(STDERR_FILENO, "destroying mlx\n");
  mylx_destroy_mlx(data);
  return (ERR_NO_ERROR);
}

int cleanup_doors(t_door *doors) {
  ft_printf_fd(STDERR_FILENO, "cleaning doors\n");
  if (doors)
    free(doors);
  return (ERR_NO_ERROR);
}

int cleanup_enemies(t_enemy *enemies) {
  ft_printf_fd(STDERR_FILENO, "cleaning enemies\n");
  if (enemies)
    free(enemies);
  return (ERR_NO_ERROR);
}

int cleanup_game(t_game *game) {
  ft_printf_fd(STDERR_FILENO, "cleaning t_game\n");
  if (game)
    free(game);
  return (ERR_NO_ERROR);
}

int cleanup_map(t_map *map) {
  int i;

  ft_printf_fd(STDERR_FILENO, "cleaning t_map\n");
  if (map) {
    if (map->map_array)
      free(map->map_array);
    if (map->filename)
      free(map->filename);
    if (map->map_lines) {
      i = 0;
      while (i < map->height) {
        if (map->map_lines[i])
          free(map->map_lines[i]);
        i++;
      }
      free(map->map_lines);
    }
    // Free any other allocated fields in t_map
    free(map);
  }
  return (ERR_NO_ERROR);
}

int cleanup_player(t_player *player) {
  ft_printf_fd(STDERR_FILENO, "cleaning t_player\n");
  if (!player)
    return (ERR_PLAYER_INIT);
  free(player);
  return (ERR_NO_ERROR);
}

int cleanup_graphics(t_graphics *graphics) {
  ft_printf_fd(STDERR_FILENO, "cleaning t_graphics\n");
  if (graphics->pixels)
    free(graphics->pixels);
  free(graphics);
  return (ERR_NO_ERROR);
}

int cleanup_textures(t_textures *textures) {
  ft_printf_fd(STDERR_FILENO, "cleaning t_textures\n");
  if (textures)
    free_textures(textures);
  return (ERR_NO_ERROR);
}

int cleanup_sprites(t_sprite *sprites) {
  ft_printf_fd(STDERR_FILENO, "cleaning t_sprites\n");
  // * to do * //
  (void)sprites;
  return (ERR_NO_ERROR);
}

int cleanup_fps(t_fps_data fps) {
  ft_printf_fd(STDERR_FILENO, "cleaning t_fps_data\n");
  (void)fps;
  return (ERR_NO_ERROR);
}

int cleanup_raycasting(t_raycasting *s_raycasting) {
  ft_printf_fd(STDERR_FILENO, "cleaning t_raycasting\n");
  if (s_raycasting) {
    if (s_raycasting->rays)
      free(s_raycasting->rays);
    free(s_raycasting);
  }
  return (ERR_NO_ERROR);
}

int cleanup_mlx_struct(t_mlx *mlx) {
  ft_printf_fd(STDERR_FILENO, "cleaning t_mlx struct\n");
  if (mlx)
    free(mlx);
  return (ERR_NO_ERROR);
}

int cleanup_input(t_input *input) {
  if (!input)
    return ERR_NO_ERROR;
  ft_printf_fd(STDERR_FILENO, "cleaning t_input\n");
  free(input);
  return (ERR_NO_ERROR);
}

int cleanup(t_cub_data *data) {
  if (!data)
    return (ERR_UNKNOWN);
  cleanup_trig_table(&data->trig);
  if (data->mlx)
    cleanup_mylx(data);
  if (data->game->doors)
    cleanup_doors(data->game->doors);
  if (data->game->enemies)
    cleanup_enemies(data->game->enemies);
  if (data->game)
    cleanup_game(data->game);
  if (data->map)
    cleanup_map(data->map);
  if (data->player)
    cleanup_player(data->player);
  if (data->graphics)
    cleanup_graphics(data->graphics);
  if (data->input)
    cleanup_input(data->input);
  if (data->textures)
    cleanup_textures(data->textures);
  if (data->sprites)
    cleanup_sprites(data->sprites);
  if (data->raycasting)
    cleanup_raycasting(data->raycasting);
  if (data->hud)
    cleanup_hud(data->hud);
  cleanup_fps(data->fps);
  // free(data);
  return (ERR_NO_ERROR);
}

int cleanup_and_exit(t_cub_data *data) {
  if (!data)
    return (ERR_UNKNOWN);
  if (cleanup(data) != 0)
    return (ERR_CLEAN_UP);
  return (ERR_NO_ERROR);
}
