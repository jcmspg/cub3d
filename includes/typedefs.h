/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   typedefs.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 21:09:26 by joamiran          #+#    #+#             */
/*   Updated: 2026/04/08 21:22:46 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPEDEFS_H
# define TYPEDEFS_H

# include "../extLibs/poormanfixedpoint/includes/poormansfixed.h"

# define CYAN 0x00FFFF
# define RED 0xFF0000
# define GREEN 0x00FF00
# define BLUE 0x0000FF

# define START_WIDTH 1280
# define START_HEIGHT 720
# define START_FOV 66.0f

# define WALL_WIDTH 32
# define WALL_HEIGHT 32

# define PLAYER_SPEED 0.02f
# define STRAFE_SPEED 0.02f
# define ROTATE_SPEED 1.5f
# define SPRINT_MULTIPLIER 3.5f
# define JUMP_HEIGHT 100
# define JUMP_DURATION 500
# define BOB_AMPLITUDE 6
# define BOB_FREQUENCY 1.0f
# define HEAD_BOB_ENABLED 1
# define AMMO_PICKUP_AMOUNT 42

# define ENEMY_MAX_HEALTH 50
# define ENEMY_DAMAGE 10
# define ENEMY_AMMO 0

# define BULLET_DMG 20

# define HUD_HEIGHT 100
# define HUD_BG_COLOR 0x2A2A2A
# define HUD_BORDER_COLOR 0x555555
# define HUD_BORDER_WIDTH 2
# define HUD_MINIMAP_CELL 8
# define HUD_MINIMAP_PADDING 4

# define PIXELS_TO_TEST 1500

# define VOID_SYMBOL ' '

# define TRIG_TABLE_SIZE 9001

# ifndef M_PI
#  define M_PI 3.14159265358979323846
# endif

enum					e_error
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

#define MINIMAP_CELL_SIZE 12
#define MINIMAP_OFFSET_X 10
#define MINIMAP_OFFSET_Y 10
#define RAY_COLOR 0xFFFF00
#define RAY_EDGE_COLOR 0x00FFFF
#define RAY_CENTER_COLOR 0xFFFFFF

typedef struct s_ray_line
{
	int	x0;
	int	y0;
	int	x1;
	int	y1;
	int	color;
}	t_ray_line;

typedef struct s_trig
{
	t_fixed32			*sin;
	t_fixed32			*cos;
}						t_trig;

typedef struct s_image_data
{
	void				*img;
	void				*address;
	int					bits_per_pixel;
	int					line_length;
	int					endian;
}						t_img_data;

typedef struct s_mlx
{
	int					width;
	int					height;
	char				*title;
	char				*map_file;
	void				*mlx_ptr;
	void				*win_ptr;
	t_img_data			*img;
}						t_mlx;

typedef struct s_map
{
	char				**map_lines;
	char				*map_array;
	int					width;
	int					height;
	int					fd;
	char				*filename;
}						t_map;

typedef struct s_fps_data
{
	uint64_t			last_frame_time;
	uint64_t			delta_time;
	uint64_t			target_frame_duration;
	uint64_t			accumulator;
	int					frame_count;
}						t_fps_data;

# define TEX_NORTH 0
# define TEX_SOUTH 1
# define TEX_EAST 2
# define TEX_WEST 3
# define TEX_FLOOR 4
# define TEX_CEILING 5
# define TEX_COUNT 6

# define COLOR_NORTH 0x8B0000
# define COLOR_SOUTH 0x006400
# define COLOR_EAST 0x00008B
# define COLOR_WEST 0x8B8B00
# define COLOR_FLOOR 0x404040
# define COLOR_CEILING 0x87CEEB

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
}						t_hud;

typedef struct s_texture
{
	int					color;
	void				*img;
	int					*pixels;
	int					width;
	int					height;
	char				*path;
	bool				loaded;
}						t_texture;

typedef struct s_textures
{
	t_texture			walls[4];
	t_texture			floor;
	t_texture			ceiling;
	t_texture			door;
	t_texture			ammo;
	t_texture			demon;
	t_texture			gun_pov;
	int					floor_color;
	int					ceiling_color;
}						t_textures;

typedef struct s_sprite	t_sprite;

typedef struct s_door
{
	int					x;
	int					y;
	bool				is_open;
	bool				is_opening;
	bool				is_closing;
	float				open_amount;
}						t_door;

typedef struct s_character
{
	int					health;
	int					max_health;
	int					ammo;
	int					max_ammo;
	int					damage;
}						t_character;

typedef enum e_enemy_state
{
	ENEMY_IDLE,
	ENEMY_CHASE,
	ENEMY_ATTACK,
	ENEMY_HIT,
	ENEMY_DEAD
}						t_enemy_state;

typedef struct s_enemy
{
	t_fixed32			x;
	t_fixed32			y;
	t_fixed32			dir_x;
	t_fixed32			dir_y;
	t_character			stats;
	t_enemy_state		state;
	int					id;
	uint64_t			hit_time;
	int					blink_count;
	int					blink_phase;
}						t_enemy;

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
}						t_input;

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
}						t_player;

typedef struct s_bres
{
	int					x;
	int					y;
	int					dx;
	int					dy;
	int					step_x;
	int					step_y;
	int					err;
}						t_bres;

typedef struct s_graphics
{
	int					x;
	int					y;
	int					color;
	int					*pixels;
}						t_graphics;

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
}						t_ray;

typedef struct s_raycasting
{
	struct s_ray		*rays;
	int					num_rays;
}						t_raycasting;

typedef struct s_game
{
	t_fixed32			fov;
	struct s_ray		*rays;
	t_door				*doors;
	int					door_count;
	t_enemy				*enemies;
	int					enemy_count;
}						t_game;

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
}						t_cub_data;

#endif /* TYPEDEFS_H */
