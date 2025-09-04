/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gamelogic.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 16:54:04 by joamiran          #+#    #+#             */
/*   Updated: 2025/09/04 18:45:19 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/gamelogic.h"

static void process_player_input(t_cub_data *data)
{
    if (!data || !data->player)
        return;
    if (data->input->forward)
        move_player(data, data->player->move_speed);
    if (data->input->backward)
        move_player(data, -data->player->move_speed);
    if (data->input->left)
        strafe_player(data, -data->player->move_speed);
    if (data->input->right)
        strafe_player(data, data->player->move_speed);
}

static void process_player_rotation(t_cub_data *data)
{
    if (!data || !data->player)
        return;
    if (data->input->turn_left)
    {
        data->player->dir_angle -= data->player->rotate_speed;
        if (data->player->dir_angle < 0)
            data->player->dir_angle += to_fixed32(360);
    }
    if (data->input->turn_right)
    {
        data->player->dir_angle += data->player->rotate_speed;
        if (data->player->dir_angle >= to_fixed32(360))
            data->player->dir_angle -= to_fixed32(360);
    }
    // Recalculate direction and plane vectors after rotation
    calc_player_dirs(data);
}

void update_game_logic(t_cub_data *data)
{
    float frame_multiplier;

    frame_multiplier = data->fps.delta_time / 16.6f;
    if (frame_multiplier > 3.0f)
        frame_multiplier = 3.0f;
    if (frame_multiplier < 0.1f)
        frame_multiplier = 0.1f;
    data->player->move_speed = to_fixed32(PLAYER_SPEED * (data->fps.delta_time / 16.67f));
    data->player->rotate_speed = to_fixed32(ROTATE_SPEED * (data->fps.delta_time / 16.67f));

    process_player_input(data);
    process_player_rotation(data);
    // Future: Add physics updates here
}
