/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malourei <malourei@student.42lisboa.pt>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 23:38:53 by malourei          #+#    #+#             */
/*   Updated: 2025/06/08 01:59:32 by malourei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

static void	init_values(t_map *map)
{
	map->coor_c = NULL;
	map->coor_f = NULL;
	map->textures.tex_ea = NULL;
	map->textures.tex_so = NULL;
	map->textures.tex_we = NULL;
	map->textures.tex_no = NULL;
	map->val_map.strs = NULL;
	map->map = NULL;
	map->val_map.count_c = 0;
	map->val_map.count_f = 0;
	map->val_map.count_we = 0;
	map->val_map.count_ea = 0;
	map->val_map.count_no = 0;
	map->val_map.count_so = 0;
	map->altura = 0;
	map->largura = 0;
	map->val_map.count_p = 0;
	map->player.coor_x = 0;
	map->player.coor_y = 0;
	map->player.dir = 0;
}

int	main(int argc, char *argv[])
{
	t_map	map;
	int		i;

	if (validate_args(argc, argv) != 1)
		return (1);
	map.fd = open(argv[1], O_RDONLY);
	if (map.fd < 0)
	{
		write(1, "ERROR\nCannot open the map!\n", 27);
		return (1);
	}
	init_values(&map);
	printf("EU sei\n");
	validate_textures_rgb(map.fd, &map);
	ft_close(map.fd);
	if (map.val_map.count_p != 1)
	{
		write(1, "ERROR\nNumber of players is wrong!\n", 34);
		clean(&map);
		return (1);
	}
	printf("Texture WE: %s\n", map.textures.tex_we);
	printf("Texture SO: %s\n", map.textures.tex_so);
	printf("Texture EA: %s\n", map.textures.tex_ea);
	printf("Texture NO: %s\n", map.textures.tex_no);
	printf("C: %s\n", map.coor_c[0]);
	printf("F: %s\n", map.coor_f[0]);
	printf("ARGC: %d\nARGV: %s\n", argc, argv[1]);
	printf("Altura mapa: %ld\n", map.altura);
	printf("largura mapa: %ld\n", map.largura);
	validate_map(&map, argv[1]);
	i = 0;
	while (map.map[i] != NULL)
	{
		if (i < 10)
			printf("Mapa linha %d  -> %s\n", i, map.map[i]);
		else
			printf("Mapa linha %d -> %s\n", i, map.map[i]);
		i++;
	}
	clean(&map);
	return (0);
}
