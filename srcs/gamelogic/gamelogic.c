/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gamelogic.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 16:54:04 by joamiran          #+#    #+#             */
/*   Updated: 2025/08/27 21:20:51 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/gamelogic.h"

static void process_player_input(t_cub_data *data)
{
    if (!data || !data->player)
        return;

    t_fixed32 move_speed = data->player->move_speed;
    t_fixed32 rotate_speed = data->player->rotate_speed;

    if (data->input->forward)
        move_player(data, move_speed);
    if (data->input->backward)
        move_player(data, -move_speed);
    if (data->input->left)
        move_player_x(data, -move_speed);
    if (data->input->right)
        move_player_x(data, move_speed);
    if (data->input->turn_left)
        rotate_player(data, -rotate_speed); // Rotate left
    if (data->input->turn_right)
        rotate_player(data, rotate_speed); // Rotate right

    // Reset turn flags after processing
    data->input->turn_left = false;
    data->input->turn_right = false;
    
}

void update_game_logic(t_cub_data *data)
{
    //Update dynamic map test animations
    //update_dynamic_map_test(data);
    
    // Future: Add player movement logic here
    process_player_input(data);
    // print_player_coords(data);
    // Future: Add physics updates here
    //print_player_coords(data);
}
