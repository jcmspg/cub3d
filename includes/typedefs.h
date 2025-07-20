/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   typedefs.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 21:09:26 by joamiran          #+#    #+#             */
/*   Updated: 2025/07/20 21:11:04 by joamiran         ###   ########.fr       */
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

enum				e_error
{
	ERR_NO_ERROR,
	ERR_INVALID_ARG,
	ERR_FILE_NOT_FOUND,
	ERR_INVALID_MAP,
	ERR_MEMORY_ALLOCATION,
	ERR_TEXTURE_LOAD,
	ERR_SPRITE_LOAD,
	ERR_RAYCASTING,
	ERR_PLAYER_INIT,
	ERR_GRAPHICS_INIT,
	ERR_UNKNOWN,
};

typedef struct s_mlx
{
	void			*mlx_ptr;
	void			*win_ptr;
	int				width;
	int				height;
	char			*map_file;
}					t_mlx;

// Forward declarations for types that will be defined later
typedef struct s_game		t_game;
typedef struct s_map		t_map;
typedef struct s_player		t_player;
typedef struct s_graphics	t_graphics;
typedef struct s_input		t_input;
typedef struct s_texture	t_texture;
typedef struct s_sprite		t_sprite;
typedef struct s_raycasting	t_raycasting;

typedef struct s_cub_data
{
	t_game			*game;
	t_map			*map;
	t_player		*player;
	t_graphics		*graphics;
	t_input			*input;
	t_texture		*textures;
	t_sprite		*sprites;
	t_raycasting	*raycasting;
	t_mlx			mlx;
}					t_cub_data;

#endif /* TYPEDEFS_H */
