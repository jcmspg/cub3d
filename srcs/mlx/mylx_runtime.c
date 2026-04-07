/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mylx_runtime.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 21:59:50 by hladeiro          #+#    #+#             */
/*   Updated: 2026/04/06 21:59:53 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/hud.h"
#include "../../includes/mylx_utils.h"

int	mylx_destroy_window(t_cub_data *data)
{
	if (!data || !data->mlx || !data->mlx->win_ptr)
		return (ERR_WINDOW_DESTROY);
	if (data && data->mlx && data->mlx->win_ptr)
	{
		mlx_destroy_window(data->mlx->mlx_ptr, data->mlx->win_ptr);
		free(data->mlx->win_ptr);
		data->mlx->win_ptr = NULL;
		return (ERR_NO_ERROR);
	}
	return (ft_printf_fd(STDERR_FILENO, "Error destroying window\n"),
		ERR_WINDOW_DESTROY);
}

int	mylx_destroy_mlx(t_cub_data *data)
{
	if (data && data->mlx)
	{
		if (data->mlx->img != NULL)
			mylx_destroy_image(data);
		if (data->mlx->win_ptr != NULL)
			mylx_destroy_window(data);
		free(data->mlx->mlx_ptr);
		data->mlx->mlx_ptr = NULL;
		free(data->mlx);
		data->mlx = NULL;
		return (ERR_NO_ERROR);
	}
	return (ft_printf_fd(STDERR_FILENO, "Error destroying MLX\n"),
		ERR_MLX_DESTROY);
}

void	mylx_pixel_put(t_cub_data *data, int x, int y, int color)
{
	char	*distance;

	if (x < 0 || x >= data->mlx->width || y < 0 || y >= data->mlx->height)
		return ;
	distance = data->mlx->img->address + (y * data->mlx->img->line_length + x
			* (data->mlx->img->bits_per_pixel / 8));
	*(unsigned int *)distance = color;
}

int	mylx_clear_image(t_cub_data *data)
{
	int	total_bytes;

	if (!data || !data->mlx || !data->mlx->img || !data->mlx->img->address)
		return (ft_printf_fd(STDERR_FILENO, "Error clearing image\n"),
			ERR_IMAGE_CLEAR);
	total_bytes = data->mlx->img->line_length * data->mlx->height;
	ft_memset(data->mlx->img->address, 0, total_bytes);
	return (ERR_NO_ERROR);
}

int	mylx_update_scene(t_cub_data *data)
{
	mylx_clear_image(data);
	start_rays(data);
	render_hud(data);
	mylx_make_image(data);
	render_hud_text(data);
	return (ERR_NO_ERROR);
}
