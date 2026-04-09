/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malourei <malourei@student.42lisboa.pt>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 23:39:05 by malourei          #+#    #+#             */
/*   Updated: 2025/06/08 01:55:53 by malourei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB_H
# define CUB_H

# define PI 3.141592653589793

# include "libft/libft.h"
# include "get_next_line/get_next_line.h"
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <fcntl.h>

typedef char	*t_string;

typedef struct s_player
{
	int		coor_x;
	int		coor_y;
	double	dir;
}	t_player;

typedef struct s_validate_map
{
	int		count_f;
	int		count_c;
	int		count_we;
	int		count_ea;
	int		count_so;
	int		count_no;
	char	**strs;
	int		count_p;
}	t_validate_map;

typedef struct s_texture
{
	char	*tex_we;
	char	*tex_ea;
	char	*tex_so;
	char	*tex_no;
}	t_texture;

typedef struct s_map
{
	char			**coor_c;
	char			**coor_f;
	int				fd;
	size_t			altura;
	size_t			largura;
	char			**map;
	t_validate_map	val_map;
	t_texture		textures;
	t_player		player;
}	t_map;

int		validate_args(int argc, char **argv);
void	validate_textures(t_map *map, char *str, char *fre);
void	clear_strs(char **strs);
void	clean(t_map *map);
void	validate_textures_rgb(int fd, t_map *map);
int		len_strs(char **strs);
int		validate_rgb(t_map *map);
long	ft_atol(const char *nptr);
void	ft_close(int fd);
void	validate_c_f(t_map *map, char *tmp);
void	is_comma(t_map *map, char *str);
int		check_color_c_f(t_map *map);
int		texture_select(t_map *map, char **strs);
void	validate_map(t_map *map, char *path_map);
void	init_map_value(t_map *map, char *str);
void	create_map(t_map *map);
void	player_values(t_map *map);
void	validate_map_playability(t_map *map);

#endif
