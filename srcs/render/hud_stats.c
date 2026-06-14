/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hud_stats.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hladeiro <hladeiro@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 01:02:16 by hladeiro          #+#    #+#             */
/*   Updated: 2026/04/08 01:02:18 by hladeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/hud_internal.h"

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
