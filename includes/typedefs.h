/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   typedefs.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 21:09:26 by joamiran          #+#    #+#             */
/*   Updated: 2026/02/23 01:23:48 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPEDEFS_H
# define TYPEDEFS_H

# include <stdint.h>
# include <stdbool.h>

# define CYAN				0x00FFFF
# define RED				0xFF0000
# define GREEN				0x00FF00
# define BLUE				0x0000FF
# define START_WIDTH		1280
# define START_HEIGHT		720
# define START_FOV		66.0f
# define WALL_WIDTH		32
# define WALL_HEIGHT		32
# define PLAYER_SPEED		0.02f
# define STRAFE_SPEED		0.02f
# define ROTATE_SPEED		1.5f
# define SPRINT_MULTIPLIER	3.5f
# define JUMP_HEIGHT		100
# define JUMP_DURATION		500
# define AMMO_PICKUP_AMOUNT	42
# define ENEMY_MAX_HEALTH	50
# define ENEMY_DAMAGE		10
# define ENEMY_AMMO		0
# define HUD_HEIGHT		100
# define HUD_BG_COLOR		0x2A2A2A
# define HUD_BORDER_COLOR	0x555555
# define HUD_BORDER_WIDTH	2
# define HUD_MINIMAP_CELL	8
# define HUD_MINIMAP_PADDING	4
# define PIXELS_TO_TEST		1500
# define VOID_SYMBOL		' '
# define TRIG_TABLE_SIZE	9001
# define M_PI				3.14159265358979323846

# define TEX_NORTH			0
# define TEX_SOUTH			1
# define TEX_EAST			2
# define TEX_WEST			3

# define COLOR_NORTH		0xCC0000
# define COLOR_SOUTH		0x990000
# define COLOR_EAST		0x00CC00
# define COLOR_WEST		0x009900
# define COLOR_FLOOR		0x444444
# define COLOR_CEILING		0x222222

# define HEAD_BOB_ENABLED	1
# define BOB_FREQUENCY		3.5f
# define BOB_AMPLITUDE		4

# define MIN_FRAME_TIME		16
# define MAX_FPS			60
# define FIXED_STEP_MS		16

typedef struct s_map			t_map;
typedef struct s_sprite			t_sprite;

enum e_error
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
	ERR_CLEAN_UP
};

enum e_enemy_state
{
	ENEMY_IDLE,
	ENEMY_CHASE,
	ENEMY_ATTACK,
	ENEMY_DEAD
};

typedef struct s_point
{
	int					x;
	int					y;
}	t_point;

typedef struct s_line
{
	t_point				p0;
	t_point				p1;
	int					color;
}	t_line;

typedef struct s_rect
{
	int					x;
	int					y;
	int					width;
	int					height;
	int					color;
}	t_rect;

typedef struct s_trig
{
	t_fixed32			*sin;
	t_fixed32			*cos;
}	t_trig;

typedef struct s_img_data
{
	void				*img;
	void				*address;
	int					bits_per_pixel;
	int					line_length;
	int					endian;
}	t_img_data;

typedef struct s_mlx
{
	int					width;
	int					height;
	char				*title;
	char				*map_file;
	void				*mlx_ptr;
	void				*win_ptr;
	t_img_data			*img;
}	t_mlx;

typedef struct s_map
{
	char				**map_lines;
	char				*map_array;
	int					width;
	int					height;
	int					fd;
	char				*filename;
}	t_map;

typedef struct s_fps_data
{
	uint64_t			last_frame_time;
	uint64_t			delta_time;
	uint64_t			target_frame_duration;
	uint64_t			accumulator;
	int					frame_count;
}	t_fps_data;

typedef struct s_hud
{
	int					x;
	int					y;
	int					width;
	int					height;
	int					minimap_x;
	int					minimap_y;
	int					minimap_size;
	int					bg_color;
	void				*bg_img;
	bool				enabled;
}	t_hud;

typedef struct s_texture
{
	int					color;
	void				*img;
	int					*pixels;
	int					width;
	int					height;
	char				*path;
	bool				loaded;
}	t_texture;

typedef struct s_textures
{
	t_texture			walls[4];
	t_texture			floor;
	t_texture			ceiling;
	int					floor_color;
	int					ceiling_color;
}	t_textures;

typedef struct s_door
{
	int					x;
	int					y;
	bool				is_open;
	bool				is_opening;
	bool				is_closing;
	float				open_amount;
}	t_door;

typedef struct s_sprite_render
{
	float				trans[2];
	int					spr_screen_x;
	int					spr_h;
	int					d_start[2];
	int					d_end[2];
	int					stripe;
	int					color;
	int					scale_div;
}	t_sprite_render;

typedef struct s_character
{
	int					health;
	int					max_health;
	int					ammo;
	int					max_ammo;
	int					damage;
}	t_character;

typedef struct s_enemy
{
	t_fixed32			x;
	t_fixed32			y;
	t_fixed32			dir_x;
	t_fixed32			dir_y;
	t_character			stats;
	int					state;
	int					id;
}	t_enemy;

typedef struct s_input
{
	bool				forward;
	bool				backward;
	bool				left;
	bool				right;
	bool				turn_left;
	bool				turn_right;
	bool				shoot;
	bool				use;
	bool				exit;
	bool				sprint;
	bool				jumping;
	uint64_t			jump_start_time;
	bool				fire_held;
}	t_input;

typedef struct s_player
{
	t_fixed32			x;
	t_fixed32			y;
	t_character			stats;
	t_fixed32			dir_angle;
	t_fixed32			dir_x;
	t_fixed32			dir_y;
	t_fixed32			plane_x;
	t_fixed32			plane_y;
	t_fixed32			move_speed;
	t_fixed32			rotate_speed;
	int					view_offset;
	float				bob_phase;
	int					bob_offset;
}	t_player;

typedef struct s_bres
{
	int					x;
	int					y;
	int					dx;
	int					dy;
	int					step_x;
	int					step_y;
	int					err;
}	t_bres;

typedef struct s_graphics
{
	int					x;
	int					y;
	int					color;
	int					*pixels;
}	t_graphics;

typedef struct s_ray
{
	t_fixed32			dir_x;
	t_fixed32			dir_y;
	int					map_x;
	int					map_y;
	int					step_x;
	int					step_y;
	t_fixed32			side_dist_x;
	t_fixed32			side_dist_y;
	t_fixed32			delta_dist_x;
	t_fixed32			delta_dist_y;
	t_fixed32			perp_dist;
	int					side;
	int					hit;
	int					door_hit;
	t_fixed32			door_dist;
	int					door_side;
	int					door_map_x;
	int					door_map_y;
	char				hit_content;
}	t_ray;

typedef struct s_raycasting
{
	struct s_ray		*rays;
	int					num_rays;
}	t_raycasting;

typedef struct s_game
{
	t_fixed32			fov;
	struct s_ray		*rays;
	t_door				*doors;
	int					door_count;
	t_enemy				*enemies;
	int					enemy_count;
}	t_game;

typedef struct s_cub_data
{
	t_trig				trig;
	t_fps_data			fps;
	t_game				*game;
	t_map				*map;
	t_player			*player;
	t_graphics			*graphics;
	t_input				*input;
	t_textures			*textures;
	t_hud				*hud;
	t_sprite			*sprites;
	t_raycasting		*raycasting;
	t_mlx				*mlx;
}	t_cub_data;

#endif
