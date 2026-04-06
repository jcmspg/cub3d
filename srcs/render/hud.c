/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hud.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 21:00:00 by joamiran          #+#    #+#             */
/*   Updated: 2026/04/06 18:10:00 by copilot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/hud.h"

struct		s_minimap_view
{
	int		minimap_size;
	int		cell_size;
	int		view_cells;
	int		minimap_x;
	int		minimap_y;
	int		offset_x;
	int		offset_y;
	int		start_cell_x;
	int		start_cell_y;
};

struct		s_minimap_ray_ctx
{
	int		center_x;
	int		center_y;
	float	ray_len;
};

struct		s_hud_ray_ctx
{
	int		minimap_x;
	int		minimap_y;
	int		minimap_size;
	int		center_x;
	int		center_y;
	int		cell_size;
};

struct		s_cell_draw
{
	int		x;
	int		y;
	int		size;
	int		color;
};

struct		s_bresenham
{
	int		x0;
	int		y0;
	int		x1;
	int		y1;
	int		dx;
	int		dy;
	int		sx;
	int		sy;
	int		err;
	int		e2;
	int		color;
};

static void	init_minimap_view(t_cub_data *data, struct s_minimap_view *view)
{
	float	player_x;
	float	player_y;
	float	frac_x;
	float	frac_y;

	view->minimap_size = data->hud->height - (HUD_MINIMAP_PADDING * 2)
		- HUD_BORDER_WIDTH;
	view->cell_size = HUD_MINIMAP_CELL;
	view->view_cells = view->minimap_size / view->cell_size;
	view->minimap_x = data->mlx->width - view->minimap_size
		- HUD_MINIMAP_PADDING;
	view->minimap_y = data->hud->y + HUD_BORDER_WIDTH + HUD_MINIMAP_PADDING;
	player_x = from_fixed32(data->player->x);
	player_y = from_fixed32(data->player->y);
	frac_x = player_x - (int)player_x;
	frac_y = player_y - (int)player_y;
	view->offset_x = (int)(frac_x * view->cell_size);
	view->offset_y = (int)(frac_y * view->cell_size);
	view->start_cell_x = (int)player_x - view->view_cells / 2 - 1;
	view->start_cell_y = (int)player_y - view->view_cells / 2 - 1;
}

static int	get_minimap_cell_color(t_cub_data *data, int map_x, int map_y)
{
	char	c;

	if (map_x < 0 || map_x >= data->map->width || map_y < 0
		|| map_y >= data->map->height)
		return (0x111111);
	c = data->map->map_array[map_y * data->map->width + map_x];
	if (c == '1')
		return (0xFFFFFF);
	if (c == 'D')
		return (0x8B4513);
	if (c == 'M')
		return (0xFFD700);
	if (c == '0' || c == 'N' || c == 'S' || c == 'E' || c == 'W')
		return (0x444444);
	return (0x111111);
}

static void	fill_minimap_cell(t_cub_data *data, struct s_cell_draw *cell)
{
	int	px;
	int	py;

	py = -1;
	while (++py < cell->size)
	{
		px = -1;
		while (++px < cell->size)
		{
			if (px == 0 || py == 0)
				mylx_pixel_put(data, cell->x + px, cell->y + py, 0x222222);
			else
				mylx_pixel_put(data, cell->x + px, cell->y + py, cell->color);
		}
	}
}

static void	draw_minimap_cell_screen(t_cub_data *data, int map_x, int map_y,
		struct s_cell_draw *cell)
{
	cell->color = get_minimap_cell_color(data, map_x, map_y);
	fill_minimap_cell(data, cell);
}

static void	draw_minimap_visible_cell(t_cub_data *data,
		struct s_minimap_view *view, int x, int y)
{
	int				map_x;
	int				map_y;
	struct s_cell_draw	cell;

	cell.x = view->minimap_x + x * view->cell_size - view->offset_x;
	cell.y = view->minimap_y + y * view->cell_size - view->offset_y;
	cell.size = view->cell_size;
	if (cell.x < view->minimap_x + view->minimap_size && cell.x
		+ view->cell_size > view->minimap_x && cell.y < view->minimap_y
		+ view->minimap_size && cell.y + view->cell_size > view->minimap_y)
	{
		map_x = view->start_cell_x + x;
		map_y = view->start_cell_y + y;
		draw_minimap_cell_screen(data, map_x, map_y, &cell);
	}
}

static void	draw_visible_minimap_cells(t_cub_data *data,
		struct s_minimap_view *view)
{
	int	x;
	int	y;

	y = -1;
	while (++y < view->view_cells + 2)
	{
		x = -1;
		while (++x < view->view_cells + 2)
			draw_minimap_visible_cell(data, view, x, y);
	}
}

static void	draw_minimap_ray(t_cub_data *data, struct s_minimap_ray_ctx *ray,
		float angle, int color)
{
	int	i;
	int	px;
	int	py;

	i = 0;
	while (i < (int)ray->ray_len)
	{
		px = ray->center_x + (int)(cosf(angle) * i);
		py = ray->center_y + (int)(sinf(angle) * i);
		mylx_pixel_put(data, px, py, color);
		i++;
	}
}

static void	draw_minimap_center_dot(t_cub_data *data, int center_x,
		int center_y)
{
	int	px;
	int	py;

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

static void	draw_minimap_player_centered_view(t_cub_data *data,
		struct s_minimap_view *view)
{
	struct s_minimap_ray_ctx	ray;
	float						dir_angle;
	float						left_angle;
	float						right_angle;

	ray.center_x = view->minimap_x + view->minimap_size / 2;
	ray.center_y = view->minimap_y + view->minimap_size / 2;
	ray.ray_len = view->minimap_size / 2.5f;
	dir_angle = from_fixed32(data->player->dir_angle);
	left_angle = (dir_angle - START_FOV / 2.0f) * M_PI / 180.0f;
	right_angle = (dir_angle + START_FOV / 2.0f) * M_PI / 180.0f;
	draw_minimap_ray(data, &ray, left_angle, 0x00AA00);
	draw_minimap_ray(data, &ray, right_angle, 0x00AA00);
	draw_minimap_ray(data, &ray, dir_angle * M_PI / 180.0f, 0xFFFF00);
	draw_minimap_center_dot(data, ray.center_x, ray.center_y);
}

static void	clamp_ray_end(int *x, int *y, struct s_hud_ray_ctx *ctx)
{
	if (*x < ctx->minimap_x)
		*x = ctx->minimap_x;
	if (*x >= ctx->minimap_x + ctx->minimap_size)
		*x = ctx->minimap_x + ctx->minimap_size - 1;
	if (*y < ctx->minimap_y)
		*y = ctx->minimap_y;
	if (*y >= ctx->minimap_y + ctx->minimap_size)
		*y = ctx->minimap_y + ctx->minimap_size - 1;
}

static void	setup_bresenham(struct s_bresenham *line)
{
	line->dx = abs(line->x1 - line->x0);
	line->dy = abs(line->y1 - line->y0);
	line->sx = -1;
	line->sy = -1;
	if (line->x0 < line->x1)
		line->sx = 1;
	if (line->y0 < line->y1)
		line->sy = 1;
	line->err = line->dx - line->dy;
}

static void	step_bresenham(struct s_bresenham *line)
{
	line->e2 = 2 * line->err;
	if (line->e2 > -line->dy)
	{
		line->err -= line->dy;
		line->x0 += line->sx;
	}
	if (line->e2 < line->dx)
	{
		line->err += line->dx;
		line->y0 += line->sy;
	}
}

static void	draw_bresenham_line(t_cub_data *data, struct s_bresenham *line)
{
	while (1)
	{
		mylx_pixel_put(data, line->x0, line->y0, line->color);
		if (line->x0 == line->x1 && line->y0 == line->y1)
			break ;
		step_bresenham(line);
	}
}

static void	draw_single_hud_ray(t_cub_data *data, struct s_hud_ray_ctx *ctx,
		t_ray *ray)
{
	float				hit_x;
	float				hit_y;
	struct s_bresenham	line;

	if (!ray->hit)
		return ;
	hit_x = from_fixed32(ray->dir_x) * from_fixed32(ray->perp_dist);
	hit_y = from_fixed32(ray->dir_y) * from_fixed32(ray->perp_dist);
	line.x0 = ctx->center_x;
	line.y0 = ctx->center_y;
	line.x1 = ctx->center_x + (int)(hit_x * ctx->cell_size);
	line.y1 = ctx->center_y + (int)(hit_y * ctx->cell_size);
	clamp_ray_end(&line.x1, &line.y1, ctx);
	line.color = 0x00AA00;
	setup_bresenham(&line);
	draw_bresenham_line(data, &line);
}

static void	draw_hud_rays_view(t_cub_data *data, struct s_minimap_view *view)
{
	int						i;
	struct s_hud_ray_ctx	ctx;

	if (!data->raycasting || !data->raycasting->rays)
		return ;
	ctx.minimap_x = view->minimap_x;
	ctx.minimap_y = view->minimap_y;
	ctx.minimap_size = view->minimap_size;
	ctx.center_x = view->minimap_x + view->minimap_size / 2;
	ctx.center_y = view->minimap_y + view->minimap_size / 2;
	ctx.cell_size = view->cell_size;
	i = 0;
	while (i < data->raycasting->num_rays)
	{
		draw_single_hud_ray(data, &ctx, &data->raycasting->rays[i]);
		i += 20;
	}
}

static int	get_health_color(int health)
{
	if (health <= 25)
		return (0xFF0000);
	if (health <= 50)
		return (0xFFFF00);
	return (0x00FF00);
}

static int	get_ammo_color(int ammo)
{
	if (ammo <= 5)
		return (0xFF0000);
	if (ammo <= 10)
		return (0xFFFF00);
	return (0x00FFFF);
}

static char	*build_stat_string(char *prefix, int value)
{
	char	*tmp;
	char	*str;

	tmp = ft_itoa(value);
	if (!tmp)
		return (NULL);
	str = ft_strjoin(prefix, tmp);
	free(tmp);
	return (str);
}

static void	draw_hud_stat_text(t_cub_data *data, char *text, int y, int color)
{
	mlx_string_put(data->mlx->mlx_ptr, data->mlx->win_ptr, 20, y, color, text);
}

static int	get_legacy_minimap_cell_color(char c)
{
	if (c == '1')
		return (0xFFFFFF);
	if (c == '0' || c == 'N' || c == 'S' || c == 'E' || c == 'W')
		return (0x333333);
	return (HUD_BG_COLOR);
}

static void	draw_minimap_player_square(t_cub_data *data, int player_x,
		int player_y, int size)
{
	int	px;
	int	py;

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
}

static void	draw_minimap_player_direction(t_cub_data *data, int player_x,
		int player_y, int cell_size)
{
	float	dir_x;
	float	dir_y;
	int		i;
	int		px;
	int		py;

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
	struct s_minimap_view	view;

	if (!data || !data->hud || !data->hud->enabled)
		return ;
	if (!data->map || !data->map->map_array)
		return ;
	init_minimap_view(data, &view);
	draw_visible_minimap_cells(data, &view);
	draw_hud_rays_view(data, &view);
	draw_minimap_player_centered_view(data, &view);
}

/**
 * Draw a single cell on the HUD minimap (legacy - kept for compatibility)
 */
void	draw_minimap_cell(t_cub_data *data, int map_x, int map_y, int cell_size)
{
	char				c;
	struct s_cell_draw	cell;

	c = data->map->map_array[map_y * data->map->width + map_x];
	cell.color = get_legacy_minimap_cell_color(c);
	cell.x = data->hud->minimap_x + (map_x * cell_size);
	cell.y = data->hud->minimap_y + (map_y * cell_size);
	cell.size = cell_size;
	fill_minimap_cell(data, &cell);
}

/**
 * Draw player position and direction on HUD minimap
 */
void	draw_minimap_player(t_cub_data *data, int cell_size)
{
	int	player_x;
	int	player_y;
	int	size;

	player_x = data->hud->minimap_x + (int)(from_fixed32(data->player->x)
			* cell_size);
	player_y = data->hud->minimap_y + (int)(from_fixed32(data->player->y)
			* cell_size);
	size = cell_size / 2;
	if (size < 3)
		size = 3;
	draw_minimap_player_square(data, player_x, player_y, size);
	draw_minimap_player_direction(data, player_x, player_y, cell_size);
}

/**
 * Draw player stats (health, ammo) on HUD
 */
void	draw_hud_stats(t_cub_data *data)
{
	char	*health_str;
	char	*ammo_str;
	int		text_y;
	int		health_color;
	int		ammo_color;

	if (!data || !data->hud || !data->hud->enabled)
		return ;
	text_y = data->hud->y + HUD_BORDER_WIDTH + 15;
	health_str = build_stat_string("HP: ", data->player->stats.health);
	ammo_str = build_stat_string("AMMO: ", data->player->stats.ammo);
	if (!health_str || !ammo_str)
	{
		free(health_str);
		free(ammo_str);
		return ;
	}
	health_color = get_health_color(data->player->stats.health);
	ammo_color = get_ammo_color(data->player->stats.ammo);
	draw_hud_stat_text(data, health_str, text_y, health_color);
	draw_hud_stat_text(data, ammo_str, text_y + 20, ammo_color);
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
