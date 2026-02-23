
#include "../includes/cub3d.h"
#include <math.h>
#include <stdio.h>

// Mock data structures for testing
t_cub_data data;
t_map map;
t_player player;
t_trig trig;

// int main(int argc, char **argv) {
//   (void)argc;
//   (void)argv;
//   printf("=== Fisheye Verification Test ===\n");

//   // Setup trig tables
//   data.trig.sin = malloc(sizeof(t_fixed32) * TRIG_TABLE_SIZE);
//   data.trig.cos = malloc(sizeof(t_fixed32) * TRIG_TABLE_SIZE);
//   calc_trig_table(&data.trig);

//   // Setup player and map
//   data.map = &map;
//   data.map->width = 10;
//   data.map->height = 10;
//   data.map->map_array = malloc(100);
//   memset(data.map->map_array, '0', 100);
//   // Add wall at x=4
//   for (int y = 0; y < 10; y++)
//     data.map->map_array[y * 10 + 4] = '1';

//   data.player = &player;
//   data.player->x = to_fixed32(2.5f); // Player at 2.5
//   data.player->y = to_fixed32(2.5f);
//   // Player facing East (1, 0)
//   data.player->dir_x = to_fixed32(1.0f);
//   data.player->dir_y = to_fixed32(0.0f);
//   data.player->dir_angle = to_fixed32(0.0f);

//   // TEST 1: Straight ray (0 degrees)
//   t_ray ray_straight = {0};
//   ray_straight.dir_x = to_fixed32(1.0f); // cos(0)
//   ray_straight.dir_y = to_fixed32(0.0f); // sin(0)

//   init_dda(&data, &ray_straight);
//   perform_dda(&data, &ray_straight);
//   calculate_perp_distance(&data, &ray_straight);

//   float dist_straight = from_fixed32(ray_straight.perp_dist);
//   printf("Ray 0 deg (Straight): Dist = %.4f (Expected ~1.5)\n",
//   dist_straight);

//   // TEST 2: Angled ray (45 degrees)
//   // Wall is at x=4. Player at x=2.5. Distance should be 1.5 perpendicular.
//   // Euclidean distance at 45 deg should be 1.5 / cos(45) = 1.5 / 0.707
//   = 2.12 t_ray ray_angled = {0}; ray_angled.dir_x = to_fixed32(0.7071f);
//   ray_angled.dir_y = to_fixed32(0.7071f);

//   init_dda(&data, &ray_angled);
//   perform_dda(&data, &ray_angled);
//   calculate_perp_distance(&data, &ray_angled);

//   float dist_angled = from_fixed32(ray_angled.perp_dist);
//   printf("Ray 45 deg (Angled):   Dist = %.4f\n", dist_angled);

//   printf("\nAnalysis:\n");
//   if (fabs(dist_angled - dist_straight) > 0.1) {
//     printf("FAIL: Distances differ significantly. Fisheye effect
//     DETECTED.\n"); printf("      Straight: %.4f, Angled: %.4f, Diff: %.4f\n",
//     dist_straight,
//            dist_angled, dist_angled - dist_straight);
//   } else {
//     printf("PASS: Distances are similar. Fisheye correction present.\n");
//   }

//   free(data.trig.sin);
//   free(data.trig.cos);
//   free(data.map->map_array);
//   return 0;
// }
