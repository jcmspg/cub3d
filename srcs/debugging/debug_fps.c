/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_fps.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 17:54:52 by joamiran          #+#    #+#             */
/*   Updated: 2025/07/26 21:39:03 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_debug.h"

void	display_fps(t_cub_data *data, float fps)
{
	char	*fps_str;
	char	*label;
	char	*full_str;

	fps_str = ft_itoa((int)fps);
	if (!fps_str)
		return ;
	label = "FPS: ";
	full_str = ft_strjoin(label, fps_str);
	free(fps_str);
	if (!full_str)
		return ;
	mlx_string_put(data->mlx->mlx_ptr, data->mlx->win_ptr, 10, 10, 0xFFFFFF,
		full_str);
	free(full_str);
}
