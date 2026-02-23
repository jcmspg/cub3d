/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mylx_hooks.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 21:07:31 by joamiran          #+#    #+#             */
/*   Updated: 2025/11/01 17:55:31 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MYLX_HOOKS_H
# define MYLX_HOOKS_H

# include "cub3d.h"

int			handle_key_press(int keycode, t_cub_data *data);
int			handle_key_release(int keycode, t_cub_data *data);
int			handle_mouse_move(int x, int y, t_cub_data *data);
int			handle_mouse_button(int button, int x, int y, t_cub_data *data);
int			handle_mouse_release(int button, int x, int y, t_cub_data *data);
int			handle_close(t_cub_data *data);
int			main_loop(t_cub_data *data);

#endif
