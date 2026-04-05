/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hud.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 21:00:00 by joamiran          #+#    #+#             */
/*   Updated: 2026/01/24 20:35:38 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/hud.h"

/**
 * Initialize HUD structure and calculate positions
 */
int	init_hud(t_cub_data *data)
{
	if (!data)
		return (0);
	data->hud = ft_calloc(1, sizeof(t_hud));
	if (!data->hud)
		return (0);
	data->hud->x = 0;
	data->hud->y = data->mlx->height - HUD_HEIGHT;
	data->hud->width = data->mlx->width;
	data->hud->height = HUD_HEIGHT;
	data->hud->bg_color = HUD_BG_COLOR;
	data->hud->bg_img = NULL;
	data->hud->enabled = true;
	return (1);
}

/**
 * Cleanup HUD resources
 */
int	cleanup_hud(t_hud *hud)
{
	if (!hud)
		return (ERR_NO_ERROR);
	if (hud->bg_img)
		hud->bg_img = NULL;
	free(hud);
	return (ERR_NO_ERROR);
}

/**
 * Draw the HUD background (solid color or texture)
 */
void	draw_hud_background(t_cub_data *data)
{
	int	x;
	int	y;
	int	color;

	if (!data || !data->hud || !data->hud->enabled)
		return ;
	y = data->hud->y - 1;
	while (++y < data->mlx->height)
	{
		x = -1;
		while (++x < data->hud->width)
		{
			if (y < data->hud->y + HUD_BORDER_WIDTH)
				color = HUD_BORDER_COLOR;
			else
				color = data->hud->bg_color;
			mylx_pixel_put(data, x, y, color);
		}
	}
}

/**
 * Draw minimap inside the HUD - player-centered scrolling view
 * Fixed square on bottom-right, shows area around player
 * Smooth scrolling: map moves with player's fractional position
 */
void	draw_hud_minimap(t_cub_data *data)
{
	int		minimap_size;
	int		cell_size;
	int		view_cells;
	int		minimap_x;
	int		minimap_y;
	float	player_x;
	float	player_y;
	float	frac_x;
	float	frac_y;
	int		offset_x;
	int		offset_y;
	int		start_cell_x;
	int		start_cell_y;
	int		x;
	int		y;
	int		draw_x;
	int		draw_y;

	if (!data || !data->hud || !data->hud->enabled)
		return ;
	if (!data->map || !data->map->map_array)
		return ;
	minimap_size = data->hud->height - (HUD_MINIMAP_PADDING * 2)
		- HUD_BORDER_WIDTH;
	cell_size = HUD_MINIMAP_CELL;
	view_cells = minimap_size / cell_size; 
	minimap_x = data->mlx->width - minimap_size - HUD_MINIMAP_PADDING;
	minimap_y = data->hud->y + HUD_BORDER_WIDTH + HUD_MINIMAP_PADDING;
	player_x = from_fixed32(data->player->x);
	player_y = from_fixed32(data->player->y);
	frac_x = player_x - (int)player_x;
	frac_y = player_y - (int)player_y;
	offset_x = (int)(frac_x * cell_size);
	offset_y = (int)(frac_y * cell_size);
	start_cell_x = (int)player_x - view_cells / 2 - 1;
	start_cell_y = (int)player_y - view_cells / 2 - 1;
	y = -1;
	while (++y < view_cells + 2)
	{
		x = -1;
		while (++x < view_cells + 2)
		{
			draw_x = minimap_x + x * cell_size - offset_x;
			draw_y = minimap_y + y * cell_size - offset_y;
			if (draw_x < minimap_x + minimap_size && draw_x
				+ cell_size > minimap_x && draw_y < minimap_y + minimap_size
				&& draw_y + cell_size > minimap_y)
				draw_minimap_cell_at(data, start_cell_x + x, start_cell_y + y,
					draw_x, draw_y, cell_size);
		}
	}
	draw_hud_rays(data, minimap_x, minimap_y, minimap_size, cell_size);
	draw_minimap_player_centered(data, minimap_x, minimap_y, minimap_size,
		cell_size);
}

/**
 * Draw a single cell at specific screen position
 * Handles out-of-bounds map coordinates (draws dark for void)
 */
void	draw_minimap_cell_at(t_cub_data *data, int map_x, int map_y,
		int screen_x, int screen_y, int cell_size)
{
	int		px;
	int		py;
	int		color;
	char	c;

	if (map_x < 0 || map_x >= data->map->width || map_y < 0
		|| map_y >= data->map->height)
		color = 0x111111; 
	else
	{
		c = data->map->map_array[map_y * data->map->width + map_x];
		if (c == '1')
			color = 0xFFFFFF; 
		else if (c == 'D')
			color = 0x8B4513; 
		else if (c == 'M')
			color = 0xFFD700; 
		else if (c == '0' || c == 'N' || c == 'S' || c == 'E' || c == 'W')
			color = 0x444444; 
		else
			color = 0x111111; 
	}
	py = -1;
	while (++py < cell_size)
	{
		px = -1;
		while (++px < cell_size)
		{
			if (px == 0 || py == 0)
				mylx_pixel_put(data, screen_x + px, screen_y + py, 0x222222);
			else
				mylx_pixel_put(data, screen_x + px, screen_y + py, color);
			px++;
		}
	}
}

/**
 * Draw player dot and FOV cone in center of minimap
 */
void	draw_minimap_player_centered(t_cub_data *data, int minimap_x,
		int minimap_y, int minimap_size, int cell_size)
{
	int		center_x;
	int		center_y;
	int		px;
	int		py;
	float	dir_angle;
	float	fov;
	float	left_angle;
	float	right_angle;
	float	ray_len;
	int		i;

	// Player is always in the center of the minimap
	(void)cell_size;
	center_x = minimap_x + minimap_size / 2;
	center_y = minimap_y + minimap_size / 2;
	// Get direction angle and FOV
	dir_angle = from_fixed32(data->player->dir_angle);
	fov = START_FOV;
	left_angle = (dir_angle - fov / 2.0f) * M_PI / 180.0f;
	right_angle = (dir_angle + fov / 2.0f) * M_PI / 180.0f;
	ray_len = minimap_size / 2.5f;
	// Draw FOV cone (left edge)
	i = 0;
	while (i < (int)ray_len)
	{
		px = center_x + (int)(cosf(left_angle) * i);
		py = center_y + (int)(sinf(left_angle) * i);
		mylx_pixel_put(data, px, py, 0x00AA00);
		i++;
	}
	// Draw FOV cone (right edge)
	i = 0;
	while (i < (int)ray_len)
	{
		px = center_x + (int)(cosf(right_angle) * i);
		py = center_y + (int)(sinf(right_angle) * i);
		mylx_pixel_put(data, px, py, 0x00AA00);
		i++;
	}
	// Draw center direction line
	i = 0;
	while (i < (int)ray_len)
	{
		px = center_x + (int)(cosf(dir_angle * M_PI / 180.0f) * i);
		py = center_y + (int)(sinf(dir_angle * M_PI / 180.0f) * i);
		mylx_pixel_put(data, px, py, 0xFFFF00);
		i++;
	}
	// Draw player dot (small square)
	py = -2;
	while (py <= 2)
	{
		px = -2;
		while (px <= 2)
		{
			mylx_pixel_put(data, center_x + px, center_y + py, 0xFF0000);
			px++;
		}
		py++;
	}
}

/**
 * Draw a single cell on the HUD minimap (legacy - kept for compatibility)
 */
void	draw_minimap_cell(t_cub_data *data, int map_x, int map_y, int cell_size)
{
	int		px;
	int		py;
	int		base_x;
	int		base_y;
	int		color;
	char	c;

	c = data->map->map_array[map_y * data->map->width + map_x];
	// Choose color based on cell type
	if (c == '1')
		color = 0xFFFFFF;
	else if (c == '0' || c == 'N' || c == 'S' || c == 'E' || c == 'W')
		color = 0x333333;
	else
		color = HUD_BG_COLOR;
	base_x = data->hud->minimap_x + (map_x * cell_size);
	base_y = data->hud->minimap_y + (map_y * cell_size);
	py = 0;
	while (py < cell_size)
	{
		px = 0;
		while (px < cell_size)
		{
			// Draw border on edges
			if (px == 0 || py == 0)
				mylx_pixel_put(data, base_x + px, base_y + py, 0x222222);
			else
				mylx_pixel_put(data, base_x + px, base_y + py, color);
			px++;
		}
		py++;
	}
}

/**
 * Draw player position and direction on HUD minimap
 */
void	draw_minimap_player(t_cub_data *data, int cell_size)
{
	int		player_x;
	int		player_y;
	int		px;
	int		py;
	int		size;
	float	dir_x;
	float	dir_y;
	int		i;

	// Convert player world position to minimap position
	player_x = data->hud->minimap_x + (int)(from_fixed32(data->player->x)
			* cell_size);
	player_y = data->hud->minimap_y + (int)(from_fixed32(data->player->y)
			* cell_size);
	// Draw player dot (small square)
	size = cell_size / 2;
	if (size < 3)
		size = 3;
	py = -size / 2;
	while (py <= size / 2)
	{
		px = -size / 2;
		while (px <= size / 2)
		{
			mylx_pixel_put(data, player_x + px, player_y + py, 0xFF0000);
			px++;
		}
		py++;
	}
	// Draw direction line
	dir_x = from_fixed32(data->player->dir_x);
	dir_y = from_fixed32(data->player->dir_y);
	i = 0;
	while (i < cell_size * 2)
	{
		px = player_x + (int)(dir_x * i);
		py = player_y + (int)(dir_y * i);
		mylx_pixel_put(data, px, py, 0xFFFF00);
		i++;
	}
}

/**
 * Draw rays from player to hit points on HUD minimap
 * Shows the actual raycasting visualization
 */
void	draw_hud_rays(t_cub_data *data, int minimap_x, int minimap_y,
		int minimap_size, int cell_size)
{
	int		i;
	int		num_rays;
	int		center_x;
	int		center_y;
	int		end_x;
	int		end_y;
	float	hit_x;
	float	hit_y;
	t_ray	*ray;

	if (!data->raycasting || !data->raycasting->rays)
		return ;
	// Player screen position (center of minimap)
	center_x = minimap_x + minimap_size / 2;
	center_y = minimap_y + minimap_size / 2;
	num_rays = data->raycasting->num_rays;
	// Draw every 20th ray to avoid clutter
	i = 0;
	while (i < num_rays)
	{
		ray = &data->raycasting->rays[i];
		if (ray->hit)
		{
			// Calculate hit point relative to player
			hit_x = from_fixed32(ray->dir_x) * from_fixed32(ray->perp_dist);
			hit_y = from_fixed32(ray->dir_y) * from_fixed32(ray->perp_dist);
			// Convert to screen coordinates (relative to player center)
			end_x = center_x + (int)(hit_x * cell_size);
			end_y = center_y + (int)(hit_y * cell_size);
			// Clamp to minimap bounds
			if (end_x < minimap_x)
				end_x = minimap_x;
			if (end_x >= minimap_x + minimap_size)
				end_x = minimap_x + minimap_size - 1;
			if (end_y < minimap_y)
				end_y = minimap_y;
			if (end_y >= minimap_y + minimap_size)
				end_y = minimap_y + minimap_size - 1;
			// Draw the ray line
			draw_hud_ray_line(data, center_x, center_y, end_x, end_y, 0x00AA00);
		}
		i += 20; // Skip rays for performance
	}
}

/**
 * Draw a line for ray visualization using Bresenham
 */
void	draw_hud_ray_line(t_cub_data *data, int x0, int y0, int x1, int y1,
		int color)
{
	int	dx;
	int	dy;
	int	sx;
	int	sy;
	int	err;
	int	e2;

	dx = abs(x1 - x0);
	dy = abs(y1 - y0);
	sx = (x0 < x1) ? 1 : -1;
	sy = (y0 < y1) ? 1 : -1;
	err = dx - dy;
	while (1)
	{
		mylx_pixel_put(data, x0, y0, color);
		if (x0 == x1 && y0 == y1)
			break ;
		e2 = 2 * err;
		if (e2 > -dy)
		{
			err -= dy;
			x0 += sx;
		}
		if (e2 < dx)
		{
			err += dx;
			y0 += sy;
		}
	}
}

/**
 * Draw player stats (health, ammo) on HUD
 */
void	draw_hud_stats(t_cub_data *data)
{
	char	*health_str;
	char	*ammo_str;
	char	*temp;
	int		text_y;
	int		health_x;
	int		ammo_x;

	if (!data || !data->hud || !data->hud->enabled)
		return ;
	// Position text in the HUD area (left side, avoiding minimap on right)
	text_y = data->hud->y + HUD_BORDER_WIDTH + 15;
	health_x = 20;
	ammo_x = 20;
	// Create health string
	temp = ft_itoa(data->player->stats.health);
	health_str = ft_strjoin("HP: ", temp);
	free(temp);
	// Create ammo string
	temp = ft_itoa(data->player->stats.ammo);
	ammo_str = ft_strjoin("AMMO: ", temp);
	free(temp);
	// Draw health (green if >50, yellow if >25, red otherwise)
	int health_color = 0x00FF00; // Green
	if (data->player->stats.health <= 50)
		health_color = 0xFFFF00; // Yellow
	if (data->player->stats.health <= 25)
		health_color = 0xFF0000; // Red
	mlx_string_put(data->mlx->mlx_ptr, data->mlx->win_ptr, health_x, text_y,
		health_color, health_str);
	// Draw ammo (cyan if >10, yellow if >5, red otherwise)
	int ammo_color = 0x00FFFF; // Cyan
	if (data->player->stats.ammo <= 10)
		ammo_color = 0xFFFF00; // Yellow
	if (data->player->stats.ammo <= 5)
		ammo_color = 0xFF0000; // Red
	mlx_string_put(data->mlx->mlx_ptr, data->mlx->win_ptr, ammo_x, text_y + 20,
		ammo_color, ammo_str);
	free(health_str);
	free(ammo_str);
}

/**
 * Main HUD render function - call this from update_scene
 */
void	render_hud(t_cub_data *data)
{
	if (!data || !data->hud || !data->hud->enabled)
		return ;
	draw_hud_background(data);
	draw_hud_minimap(data);
	// Note: text stats are rendered separately after image display
}

/**
 * Render HUD text overlays - MUST be called after mlx_put_image_to_window
 * because mlx_string_put draws directly to the window
 */
void	render_hud_text(t_cub_data *data)
{
	if (!data || !data->hud || !data->hud->enabled)
		return ;
	draw_hud_stats(data);
}
