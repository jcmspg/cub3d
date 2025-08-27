#ifndef MOVEMENTS_H
#define MOVEMENTS_H

# include "cub3d.h"


void	move_player_x(t_cub_data *data, t_fixed32 speed);
void	move_player_y(t_cub_data *data, t_fixed32 speed);
void	look_left(t_cub_data *data, t_fixed32 speed);
void	look_right(t_cub_data *data, t_fixed32 speed);

void	move_player(t_cub_data *data, t_fixed32 move_speed);
void	rotate_player(t_cub_data *data, t_fixed32 rotation_angle);

#endif /* MOVEMENTS_H */
