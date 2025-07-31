/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gamelogic.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 16:54:04 by joamiran          #+#    #+#             */
/*   Updated: 2025/07/31 21:57:24 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/gamelogic.h"

void update_game_logic(t_cub_data *data)
{
    // Update dynamic map test animations
    update_dynamic_map_test(data);
    
    // Future: Add player movement logic here
    // Future: Add physics updates here
}