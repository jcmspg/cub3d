/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   typedefs.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 21:09:26 by joamiran          #+#    #+#             */
/*   Updated: 2025/07/27 18:02:05 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPEDEFS_H
# define TYPEDEFS_H

# define START_WIDTH 800
# define START_HEIGHT 600
# define START_FOV 60.0f

# define WALL_WIDTH 32
# define WALL_HEIGHT 32

# define PLAYER_SPEED 0.1f
# define PLAYER_ROTATION_SPEED 0.05f

# define PIXELS_TO_TEST 1500 // debug value for fps sync testing

# define MLX_COLOR(r, g, b) (0xFF000000 | ((r) << 16) | ((g) << 8) | (b))

enum						e_error
{
	ERR_NO_ERROR,
	ERR_INVALID_ARG,
	ERR_FILE_NOT_FOUND,
	ERR_INVALID_MAP,
	ERR_MEMORY_ALLOCATION,
	ERR_MLX_INIT,
	ERR_MLX_DESTROY,
	ERR_WINDOW_CREATE,
	ERR_IMAGE_CREATE,
	ERR_IMAGE_ADDRESS,
	ERR_IMAGE_DESTROY,
	ERR_IMAGE_PUT,
	ERR_IMAGE_CLEAR,
	ERR_IMAGE_LOAD,
	ERR_WINDOW_DESTROY,
	ERR_WINDOW_CLOSE,
	ERR_WINDOW_RESIZE,
	ERR_WINDOW_MOVE,
	ERR_WINDOW_TITLE,
	ERR_WINDOW_ICON,
	ERR_WINDOW_FULLSCREEN,
	ERR_WINDOW_BORDERLESS,
	ERR_WINDOW_MINIMIZE,
	ERR_TEXTURE_PARSE,
	ERR_TEXTURE_LOAD,
	ERR_SPRITE_LOAD,
	ERR_RAYCASTING,
	ERR_PLAYER_INIT,
	ERR_GRAPHICS_INIT,
	ERR_UNKNOWN,
	ERR_CLEAN_UP,
};

typedef struct s_image_data
{
	void					*img;
	void					*address;
	int						bits_per_pixel;
	int						line_length;
	int						endian;
}							t_img_data;

typedef struct s_mlx
{
	int						width;
	int						height;
	char					*title;
	char					*map_file;
	void					*mlx_ptr;
	void					*win_ptr;
	t_img_data				*img;
}							t_mlx;

typedef struct s_map
{
	char **map_array; // 2D array of map data
	int width;        // Width of the map in tiles
	int height;       // Height of the map in tiles
	int fd;           // File descriptor for the map file
	char *filename;   // Name of the map file
}							t_map;

typedef struct s_fps_data
{
	uint64_t				last_frame_time;
	uint64_t				delta_time;
	uint64_t				target_frame_duration;
	int						frame_count;
}							t_fps_data;

// Forward declarations for types that will be defined later
typedef struct s_game		t_game;
typedef struct s_map		t_map;
typedef struct s_player		t_player;
typedef struct s_input		t_input;
typedef struct s_texture	t_texture;
typedef struct s_sprite		t_sprite;
typedef struct s_raycasting	t_raycasting;

typedef struct s_graphics
{
	int						x;
	int						y;
	int						color;
	int *pixels; // 1d array of width * height size
}							t_graphics;

typedef struct s_cub_data
{
	// time struct
	t_fps_data				fps;

	t_game					*game;
	t_map					*map;
	t_player				*player;
	t_graphics				*graphics;
	t_input					*input;
	t_texture				*textures;
	t_sprite				*sprites;
	t_raycasting			*raycasting;
	t_mlx					*mlx;
}							t_cub_data;

#endif /* TYPEDEFS_H */
