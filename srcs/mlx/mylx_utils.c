/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mylx_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 18:30:16 by joamiran          #+#    #+#             */
/*   Updated: 2026/04/06 21:58:50 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/hud.h"
#include "../../includes/mylx_utils.h"

int	mylx_init(t_cub_data *data)
{
	if (!data || !data->mlx)
		return (ft_printf_fd(STDERR_FILENO, "Error: no MLX\n"), ERR_MLX_INIT);
	data->mlx->mlx_ptr = mlx_init();
	if (!data->mlx->mlx_ptr)
	{
		ft_printf_fd(STDERR_FILENO, "Error initializing MLX\n");
		return (ERR_MLX_INIT);
	}
	return (ERR_NO_ERROR);
}

int	mylx_create_window(t_cub_data *data)
{
	data->mlx->win_ptr = mlx_new_window(data->mlx->mlx_ptr, data->mlx->width,
			data->mlx->height, data->mlx->title);
	if (!data || !data->mlx || !data->mlx->win_ptr)
		return (ft_printf_fd(STDERR_FILENO, "Error creating window\n"),
			ERR_MLX_INIT);
	if (data->mlx->win_ptr == NULL)
	{
		ft_printf_fd(STDERR_FILENO, "Error creating window: %s\n",
			data->mlx->title);
		return (ERR_WINDOW_CREATE);
	}
	return (ERR_NO_ERROR);
}

int	mylx_create_image(t_cub_data *data)
{
	if (!data || !data->mlx)
		return (ft_printf_fd(STDERR_FILENO, "Error: MLX not initialized\n"),
			ERR_MLX_INIT);
	data->mlx->img = ft_calloc(1, sizeof(t_img_data));
	if (!data->mlx->img)
		return (ft_printf_fd(STDERR_FILENO, "Error allocating image\n"),
			ERR_MEMORY_ALLOCATION);
	data->mlx->img->img = mlx_new_image(data->mlx->mlx_ptr, data->mlx->width,
			data->mlx->height);
	if (!data || !data->mlx || !data->mlx->img || !data->mlx->img->img)
		return (ft_printf_fd(STDERR_FILENO, "Error creating image\n"),
			ERR_IMAGE_CREATE);
	data->mlx->img->address = mlx_get_data_addr(data->mlx->img->img,
			&data->mlx->img->bits_per_pixel, &data->mlx->img->line_length,
			&data->mlx->img->endian);
	if (!data->mlx->img->address)
	{
		ft_printf_fd(STDERR_FILENO, "Error getting image address\n");
		mlx_destroy_image(data->mlx->mlx_ptr, data->mlx->img->img);
		free(data->mlx->img);
		return (ERR_IMAGE_ADDRESS);
	}
	return (ERR_NO_ERROR);
}

int	mylx_make_image(t_cub_data *data)
{
	if (!data || !data->mlx || !data->mlx->img || !data->mlx->img->img)
		return (ft_printf_fd(STDERR_FILENO, "Error: No image to display\n"),
			ERR_IMAGE_CREATE);
	mlx_put_image_to_window(data->mlx->mlx_ptr, data->mlx->win_ptr,
		data->mlx->img->img, 0, 0);
	return (ERR_NO_ERROR);
}

int	mylx_destroy_image(t_cub_data *data)
{
	if (!data)
		return (ERR_IMAGE_DESTROY);
	if (data && data->mlx && data->mlx->img && data->mlx->img->img)
	{
		mlx_destroy_image(data->mlx->mlx_ptr, data->mlx->img->img);
		free(data->mlx->img);
		data->mlx->img = NULL;
		return (ERR_NO_ERROR);
	}
	return (ft_printf_fd(STDERR_FILENO, "Error destroying image\n"),
		ERR_IMAGE_DESTROY);
}
