/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 21:07:31 by joamiran          #+#    #+#             */
/*   Updated: 2026/02/23 03:00:10 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int handle_key_press(int keycode, t_cub_data *data) {
  if (keycode == KEY_ESC)
    data->input->exit = true;
  else if (keycode == KEY_W || keycode == KEY_UP)
    data->input->forward = true;
  else if (keycode == KEY_S || keycode == KEY_DOWN)
    data->input->backward = true;
  else if (keycode == KEY_A || keycode == KEY_LEFT)
    data->input->left = true;
  else if (keycode == KEY_D || keycode == KEY_RIGHT)
    data->input->right = true;
  else if (keycode == KEY_SPACE)
    handle_player_combat(data);
  else if (keycode == KEY_SHIFT)
    data->input->sprint = true;
  else if (keycode == 101)
    interact_doors(data);
  return (ERR_NO_ERROR);
}

int handle_key_release(int keycode, t_cub_data *data) {
  if (keycode == KEY_W || keycode == KEY_UP)
    data->input->forward = false;
  else if (keycode == KEY_S || keycode == KEY_DOWN)
    data->input->backward = false;
  else if (keycode == KEY_A || keycode == KEY_LEFT)
    data->input->left = false;
  else if (keycode == KEY_D || keycode == KEY_RIGHT)
    data->input->right = false;
  else if (keycode == KEY_SHIFT)
    data->input->sprint = false;
  return (ERR_NO_ERROR);
}

int handle_mouse_button(int button, int x, int y, t_cub_data *data) {
  (void)x;
  (void)y;
  if (button == 1)
    handle_player_combat(data);
  return (ERR_NO_ERROR);
}

int handle_mouse_release(int button, int x, int y, t_cub_data *data) {
  (void)button;
  (void)x;
  (void)y;
  (void)data;
  return (ERR_NO_ERROR);
}

int handle_mouse_move(int x, int y, t_cub_data *data) {
  static int center_x = -1;
  int delta_x;

  (void)y;
  if (center_x == -1)
    center_x = data->mlx->width / 2;
  delta_x = x - center_x;
  if (delta_x != 0) {
    rotate_player(data, to_fixed32(delta_x * 0.1f));
    mlx_mouse_move(data->mlx->mlx_ptr, data->mlx->win_ptr, center_x,
                   data->mlx->height / 2);
  }
  return (0);
}

int handle_close(t_cub_data *data) {
  if (data->mlx && data->mlx->mlx_ptr)
    mlx_loop_end(data->mlx->mlx_ptr);
  return (ERR_NO_ERROR);
}