/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamiran <joamiran@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 20:39:30 by joamiran          #+#    #+#             */
/*   Updated: 2025/08/04 17:09:42 by joamiran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"
#include "../includes/inits.h"

int main(int argc, char **argv)
{
  t_cub_data data;

  ft_memset(&data, 0, sizeof(t_cub_data));
  srand(time(NULL));
  if (argc != 2) {
    ft_putstr_fd("Usage: ./cub3d <map_file>\n", 2);
    return (ERR_INVALID_ARG);
  }

  ft_putstr_fd("Game initialized with map: ", 1);
  ft_putstr_fd(argv[1], 1);
  ft_putchar_fd('\n', 1);

  if (parse_cub_file(argv[1], &data) != ERR_NO_ERROR) {
    ft_putstr_fd("Error parsing .cub file.\n", STDERR_FILENO);
    return (ERR_FILE_NOT_FOUND);
  }

  print_map_array(data.map);

  // === INITIALIZE TRIG TABLES FIRST ===
  printf("Initializing trigonometric lookup tables...\n");
  if (!init_trig_table(&data)) {
    ft_putstr_fd("Error: Failed to initialize trig tables\n", STDERR_FILENO);
    return (ERR_MEMORY_ALLOCATION);
  }
  printf("Trig tables initialized successfully!\n");

  // === NOW RUN MATH TESTS ===
  //run_all_math_tests(&data);

  // === ENABLE DYNAMIC MAP TESTING ===
  //init_dynamic_map_test(&data);

  // start a window
  init_game_window(&data);

  // allocate pixel buffer
  graphics_init(&data);

  // init the fps sync
  init_fps_sync(&data.fps);


  // register the renderer loop within the mlx_loop
  mlx_loop_hook(data.mlx->mlx_ptr, (int (*)(void *))main_render_loop, &data);

  // In main() function, update your hook registration:
  mlx_hook(data.mlx->win_ptr, 2, 1L << 0, handle_key_press, &data); // Key press
  mlx_hook(data.mlx->win_ptr, 3, 1L << 1, handle_key_release,&data); // Key release
  mlx_hook(data.mlx->win_ptr, 17, 0L, handle_close, &data);


  mlx_loop(data.mlx->mlx_ptr);

  
  cleanup(&data);
  return (ERR_NO_ERROR);
}
