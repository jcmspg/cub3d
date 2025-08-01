/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_debug.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 17:55:35 by joamiran          #+#    #+#             */
/*   Updated: 2025/07/26 22:15:21 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_DEBUG_H
# define FT_DEBUG_H

# include "cub3d.h"

void	display_fps(t_cub_data *data, float fps);

int		test_sync(t_cub_data *data);

int		graphics_init(t_cub_data *data);

#endif /* FT_DEBUG_H */