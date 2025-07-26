/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mylx_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 18:30:16 by joamiran          #+#    #+#             */
/*   Updated: 2025/07/26 22:13:14 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mylx_utils.h"

// Initialize MLX
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

// create a new Window
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

// Create a new image
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

// Make the image visible in the window
int	mylx_make_image(t_cub_data *data)
{
	if (!data || !data->mlx || !data->mlx->img || !data->mlx->img->img)
		return (ft_printf_fd(STDERR_FILENO, "Error: No image to display\n"),
			ERR_IMAGE_CREATE);
	mlx_put_image_to_window(data->mlx->mlx_ptr, data->mlx->win_ptr,
		data->mlx->img->img, 0, 0);
	return (ERR_NO_ERROR);
}

// Destroy the image
int	mylx_destroy_image(t_cub_data *data)
{
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

// Destroy the window
int	mylx_destroy_window(t_cub_data *data)
{
	if (data && data->mlx && data->mlx->win_ptr)
	{
		mlx_destroy_window(data->mlx->mlx_ptr, data->mlx->win_ptr);
		data->mlx->win_ptr = NULL;
		return (ERR_NO_ERROR);
	}
	return (ft_printf_fd(STDERR_FILENO, "Error destroying window\n"),
		ERR_WINDOW_DESTROY);
}

// Destroy the MLX instance
int	mylx_destroy_mlx(t_cub_data *data)
{
	if (data && data->mlx)
	{
		if (data->mlx->img)
			mylx_destroy_image(data);
		mylx_destroy_window(data);
		mlx_destroy_display(data->mlx->mlx_ptr);
		free(data->mlx->mlx_ptr);
		data->mlx->mlx_ptr = NULL;
		free(data->mlx);
		data->mlx = NULL;
		return (ERR_NO_ERROR);
	}
	return (ft_printf_fd(STDERR_FILENO, "Error destroying MLX\n"),
		ERR_MLX_DESTROY);
}

// Put a pixel in the image
void	mylx_pixel_put(t_cub_data *data, int x, int y, int color)
{
	char	*distance;

	if (x < 0 || x >= data->mlx->width || y < 0 || y >= data->mlx->height)
		return ;
	distance = data->mlx->img->address + (y * data->mlx->img->line_length + x
			* (data->mlx->img->bits_per_pixel / 8));
	*(unsigned int *)distance = color;
}

// Clear the image
int	mylx_clear_image(t_cub_data *data)
{
	int	x;
	int	y;

	if (!data || !data->mlx || !data->mlx->img || !data->mlx->img->address)
		return (ft_printf_fd(STDERR_FILENO, "Error clearing image\n"),
			ERR_IMAGE_CLEAR);
	y = 0;
	while (y < data->mlx->height)
	{
		x = 0;
		while (x < data->mlx->width)
		{
			mylx_pixel_put(data, x, y, MLX_COLOR(255, 0, 0));
				// Clear with black color
			x++;
		}
		y++;
	}
	return (ERR_NO_ERROR);
}

int	mylx_update_scene(t_cub_data *data)
{
	// cleans the image
	mylx_clear_image(data);
	// draws scene
	test_sync(data);
	// places new scene on the image
	mylx_make_image(data);
	return (ERR_NO_ERROR);
}
// End of mylx_utils.c
// ************************************************************************** */