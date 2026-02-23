/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 19:30:00 by joamiran          #+#    #+#             */
/*   Updated: 2026/02/23 02:55:10 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int render_raycasting(t_cub_data *data) {
  if (!data || !data->mlx || !data->player || !data->raycasting)
    return (1);
  cast_all_rays(data);
  render_walls(data);
  render_sprites(data);
  render_door_overlays(data);
  render_weapon(data);
  render_hud(data);
  mlx_put_image_to_window(data->mlx->mlx_ptr, data->mlx->win_ptr,
                          data->mlx->img->img, 0, 0);
  return (0);
}

void cast_rays(t_cub_data *data) {
  if (!data || !data->raycasting)
    return;
  cast_all_rays(data);
}
