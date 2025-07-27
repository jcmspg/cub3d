/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mylx_hooks.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 17:56:09 by joamiran          #+#    #+#             */
/*   Updated: 2025/07/27 18:05:27 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MYLX_HOOKS_H
# define MYLX_HOOKS_H

# include "cub3d.h"

int	handle_key_press(int keycode, t_cub_data *data);
int	handle_close(t_cub_data *data);

#endif /* MYLX_HOOKS_H*/