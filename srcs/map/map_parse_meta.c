/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parse_meta.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 00:47:42 by hladeiro          #+#    #+#             */
/*   Updated: 2026/04/08 00:57:05 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static int	parse_single_texture(char *line, t_texture *tex)
{
	char	*path_start;
	char	*path_end;
	int		len;

	path_start = line + 3;
	while (*path_start == ' ' || *path_start == '\t')
		path_start++;
	path_end = path_start;
	while (*path_end && *path_end != '\n' && *path_end != '\r')
		path_end++;
	len = path_end - path_start;
	if (len <= 0)
		return (-1);
	tex->path = ft_calloc(len + 1, sizeof(char));
	if (!tex->path)
		return (-1);
	ft_memcpy(tex->path, path_start, len);
	tex->path[len] = '\0';
	return (0);
}

static int	parse_rgb_value(char **str)
{
	int	value;

	value = ft_atoi(*str);
	if (value < 0 || value > 255)
		return (-1);
	while (**str && ft_isdigit(**str))
		(*str)++;
	return (value);
}

static int	parse_rgb_triplet(char *line, int *color)
{
	char	*ptr;
	int		r;
	int		g;
	int		b;

	ptr = line + 2;
	while (*ptr == ' ' || *ptr == '\t')
		ptr++;
	r = parse_rgb_value(&ptr);
	if (r < 0 || *ptr != ',')
		return (-1);
	ptr++;
	g = parse_rgb_value(&ptr);
	if (g < 0 || *ptr != ',')
		return (-1);
	ptr++;
	b = parse_rgb_value(&ptr);
	if (b < 0)
		return (-1);
	*color = (r << 16) | (g << 8) | b;
	return (0);
}

int	parse_textures(char *line, t_cub_data *data)
{
	if (!line || !data || !data->textures)
		return (-1);
	if (ft_strncmp(line, "NO ", 3) == 0)
		return (parse_single_texture(line, &data->textures->walls[TEX_NORTH]));
	if (ft_strncmp(line, "SO ", 3) == 0)
		return (parse_single_texture(line, &data->textures->walls[TEX_SOUTH]));
	if (ft_strncmp(line, "WE ", 3) == 0)
		return (parse_single_texture(line, &data->textures->walls[TEX_WEST]));
	if (ft_strncmp(line, "EA ", 3) == 0)
		return (parse_single_texture(line, &data->textures->walls[TEX_EAST]));
	return (0);
}

int	parse_colors(char *line, t_cub_data *data)
{
	int		color;

	if (!line || !data || !data->textures)
		return (-1);
	if (parse_rgb_triplet(line, &color) != 0)
		return (-1);
	if (ft_strncmp(line, "F ", 2) == 0)
		data->textures->floor_color = color;
	else if (ft_strncmp(line, "C ", 2) == 0)
		data->textures->ceiling_color = color;
	return (0);
}
