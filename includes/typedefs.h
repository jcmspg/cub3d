/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   typedefs.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 21:09:26 by joamiran          #+#    #+#             */
/*   Updated: 2026/01/24 20:24:02 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#define CYAN 0x00FFFF
#define RED 0xFF0000
#define GREEN 0x00FF00
#define BLUE 0x0000FF

#define START_WIDTH 1280
#define START_HEIGHT 720
#define START_FOV 66.0f

#define WALL_WIDTH 32
#define WALL_HEIGHT 32

#define PLAYER_SPEED 0.02f
#define STRAFE_SPEED 0.02f
#define ROTATE_SPEED 1.5f // degrees per frame
#define SPRINT_MULTIPLIER 3.5f
#define JUMP_HEIGHT 100    // pixels to offset view
#define JUMP_DURATION 500  // ms for full jump arc
#define BOB_AMPLITUDE 6    // pixels of vertical bounce
#define BOB_FREQUENCY 2.0f // oscillations per second
#define HEAD_BOB_ENABLED 1 // 1 = on, 0 = off

// HUD settings
#define HUD_HEIGHT 100            // height of HUD bar at bottom
#define HUD_BG_COLOR 0x2A2A2A     // dark gray background
#define HUD_BORDER_COLOR 0x555555 // lighter gray border
#define HUD_BORDER_WIDTH 2        // border thickness
#define HUD_MINIMAP_CELL 8        // pixels per cell on minimap
#define HUD_MINIMAP_PADDING 4     // padding around minimap

#define PIXELS_TO_TEST 1500 // debug value for fps sync testing

#define MLX_COLOR(r, g, b) (0xFF000000 | ((r) << 16) | ((g) << 8) | (b))

#define VOID_SYMBOL ' '

#define TRIG_TABLE_SIZE 9001 // [from 0 to 90] degrees evrery 0.01 degree

// Trigonometry and fixed points
#define FIXED_PI to_fixed32(3.14159265f)
#define FIXED_HALF_PI to_fixed32(3.14159265f / 2.0f)
#define FIXED_TWO_PI to_fixed32(6.28318530f)

#define M_PI 3.14159265358979323846

enum e_error {
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

typedef struct s_trig {
  t_fixed32 *sin; // 0-90 degrees inclusive
  t_fixed32 *cos; // same
} t_trig;

typedef struct s_image_data {
  void *img;
  void *address;
  int bits_per_pixel;
  int line_length;
  int endian;
} t_img_data;

typedef struct s_mlx {
  int width;
  int height;
  char *title;
  char *map_file;
  void *mlx_ptr;
  void *win_ptr;
  t_img_data *img;
} t_mlx;

typedef struct s_map {
  char **map_lines; // temp buffer so i wont do 2nd passover
  char *map_array;  // 2D array of map data
  int width;        // Width of the map in tiles
  int height;       // Height of the map in tiles
  int fd;           // File descriptor for the map file
  char *filename;   // Name of the map file
} t_map;

typedef struct s_fps_data {
  uint64_t last_frame_time;
  uint64_t delta_time;
  uint64_t target_frame_duration;
  uint64_t accumulator;
  int frame_count;

} t_fps_data;

/*
** =============================================================================
** TEXTURE/SURFACE TYPES
** For now using solid colors, later will support XPM textures
** =============================================================================
*/

// Texture/surface indices
#define TEX_NORTH 0
#define TEX_SOUTH 1
#define TEX_EAST 2
#define TEX_WEST 3
#define TEX_FLOOR 4
#define TEX_CEILING 5
#define TEX_COUNT 6

// Default colors for each surface (used until real textures are loaded)
#define COLOR_NORTH 0x8B0000   // Dark red
#define COLOR_SOUTH 0x006400   // Dark green
#define COLOR_EAST 0x00008B    // Dark blue
#define COLOR_WEST 0x8B8B00    // Dark yellow
#define COLOR_FLOOR 0x404040   // Dark gray
#define COLOR_CEILING 0x87CEEB // Sky blue

// HUD structure
typedef struct s_hud {
  int x;            // HUD x position (0 = left edge)
  int y;            // HUD y position (screen_height - HUD_HEIGHT)
  int width;        // HUD width (screen width)
  int height;       // HUD height
  int minimap_x;    // Minimap x position within HUD
  int minimap_y;    // Minimap y position within HUD
  int minimap_size; // Size of minimap
  int bg_color;     // Background color
  void *bg_img;     // Background texture (NULL = use color)
  bool enabled;     // Toggle HUD on/off
} t_hud;

typedef struct s_texture {
  int color;   // Solid color (used when no texture loaded)
  void *img;   // MLX image pointer (NULL if using color)
  int *pixels; // Pixel data array
  int width;   // Texture width
  int height;  // Texture height
  char *path;  // Path to texture file (NULL if using color)
  bool loaded; // True if texture is loaded from file
} t_texture;

typedef struct s_textures {
  t_texture walls[4]; // N, S, E, W
  t_texture floor;
  t_texture ceiling;
  int floor_color;   // Parsed from .cub file (F line)
  int ceiling_color; // Parsed from .cub file (C line)
} t_textures;

// Forward declarations for types that will be defined later
typedef struct s_map t_map;
typedef struct s_sprite t_sprite;

typedef struct s_door {
  int x;
  int y;
  bool is_open;
  bool is_opening;
  bool is_closing;
  float open_amount; // 0.0 (closed) -> 1.0 (fully open)
} t_door;

typedef struct s_character {
  int health;
  int max_health;
  int ammo;
  int max_ammo;
  int damage;
} t_character;

typedef struct s_enemy {
  t_fixed32 x;
  t_fixed32 y;
  t_fixed32 dir_x;
  t_fixed32 dir_y;
  t_character stats;
  int state; // 0=Idle, 1=Chase, 2=Attack, 3=Hit, 4=Dead
  int id;
} t_enemy;

typedef struct s_input {
  bool forward;
  bool backward;
  bool left;
  bool right;
  bool turn_left;
  bool turn_right;
  bool shoot;
  bool use;
  bool exit;
  bool sprint;
  bool jumping;
  uint64_t jump_start_time;
  bool fire_held; // To prevent machine-gunning single fire weapons
} t_input;

typedef struct s_player {
  t_fixed32 x;
  t_fixed32 y;
  t_character stats; // Gameplay stats

  // direction vector
  t_fixed32 dir_angle; // in degrees

  t_fixed32 dir_x;
  t_fixed32 dir_y;

  t_fixed32 plane_x;
  t_fixed32 plane_y;

  t_fixed32 move_speed;
  t_fixed32 rotate_speed;
  int view_offset; // vertical offset for jumping
  float bob_phase; // phase of head bob oscillation
  int bob_offset;  // current bob offset in pixels

} t_player;

typedef struct s_bres {
  int x;
  int y;
  int dx;
  int dy;
  int step_x;
  int step_y;
  int err;
} t_bres;

typedef struct s_graphics {
  int x;
  int y;
  int color;
  int *pixels; // 1d array of width * height size
} t_graphics;
typedef struct s_ray {
  // Input
  t_fixed32 dir_x; // Ray direction
  t_fixed32 dir_y;

  // DDA state
  int map_x; // Current grid cell
  int map_y;
  int step_x; // -1 or +1
  int step_y;
  t_fixed32 side_dist_x;  // Distance to next X gridline
  t_fixed32 side_dist_y;  // Distance to next Y gridline
  t_fixed32 delta_dist_x; // Distance between X gridlines
  t_fixed32 delta_dist_y; // Distance between Y gridlines

  // Output
  t_fixed32 perp_dist; // Perpendicular distance (no fisheye)
  int side;            // 0=vertical wall, 1=horizontal wall
  int hit;             // Did we hit something?

  // Door hit data (for transparent/animating doors)
  int door_hit;        // 1 if we passed through an animating door
  t_fixed32 door_dist; // Distance to the door
  int door_side;       // Side of the door hit
  int door_map_x;      // Map coordinates of the door
  int door_map_y;
  char hit_content; // The map character that was hit (e.g., '1', 'D')
} t_ray;

typedef struct s_raycasting {
  struct s_ray *rays; // array of rays, one per screen column
  int num_rays;       // typically equal to screen width

} t_raycasting;
typedef struct s_game {
  t_fixed32 fov;      // in degrees
  struct s_ray *rays; // array of rays, one per screen column
  t_door *doors;
  int door_count;

} t_game;

typedef struct s_cub_data {
  // data struct for the trigonometric values
  t_trig trig;

  // time struct
  t_fps_data fps;

  t_game *game;
  t_map *map;
  t_player *player;
  t_graphics *graphics;
  t_input *input;
  t_textures *textures;
  t_hud *hud;
  t_sprite *sprites;
  t_raycasting *raycasting;
  t_mlx *mlx;
} t_cub_data;

#endif /* TYPEDEFS_H */
