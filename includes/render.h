/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 18:41:20 by joamiran          #+#    #+#             */
/*   Updated: 2026/01/24 21:10:15 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RENDER_H
# define RENDER_H

# include "cub3d.h"

int				main_render_loop(t_cub_data *data);
void			render_frame(t_cub_data *data);
void			render_walls(t_cub_data *data);
void			render_sprites(t_cub_data *data);
void			render_enemies(t_cub_data *data);
void			render_billboard(t_cub_data *data, float sx, float sy, t_sprite_render *sr);
int				init_sprites(t_cub_data *data);
void			render_weapon(t_cub_data *data);
int				render_raycasting(t_cub_data *data);
void			draw_textures(t_cub_data *data, int x);
t_raycasting	*init_raycasting(int num_rays);
int				init_textures(t_cub_data *data);
void			cast_rays(t_cub_data *data);
void			calculate_wall_rendering(t_cub_data *data, int x);
void			reset_ray(t_ray *ray);
void			reset_all_rays(t_raycasting *rc);
void			free_raycasting(t_raycasting *rc);
void			calculate_ray_dir(t_cub_data *data, t_ray *ray, int x);
void			init_dda(t_cub_data *data, t_ray *ray);
int				perform_dda(t_cub_data *data, t_ray *ray);
void			calculate_perp_distance(t_cub_data *data, t_ray *ray);
void			cast_single_ray(t_cub_data *data, t_ray *ray, int x);
void			cast_all_rays(t_cub_data *data);
int				get_wall_color(t_ray *ray, t_textures *textures);
void			free_textures(t_textures *textures);
void			render_door_overlays(t_cub_data *data);

#endif
