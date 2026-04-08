/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mylx_hooks.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 17:56:09 by joamiran          #+#    #+#             */
/*   Updated: 2026/04/08 01:49:10 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MYLX_HOOKS_H
# define MYLX_HOOKS_H

# include "cub3d.h"

int	handle_key_press(int keycode, t_cub_data *data);
int	handle_key_release(int keycode, t_cub_data *data);
int	handle_mouse_move(int x, int y, t_cub_data *data);
int	handle_mouse_button(int button, int x, int y, t_cub_data *data);
int	handle_mouse_release(int button, int x, int y, t_cub_data *data);
int	handle_close(t_cub_data *data);

#endif /* MYLX_HOOKS_H */
