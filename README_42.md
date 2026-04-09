*This project has been created as part of the 42 curriculum by joamiran and hladeiro.*

# Cub3D

Description
-----------
Cub3D is a small raycasting-based 3D engine (Wolfenstein‑style) implemented for the 42 curriculum.  
Goal: demonstrate core graphics and game-engine concepts — raycasting, texture mapping, map parsing, player movement, simple enemies, doors and pickups — with a focus on memory-safe C code and minimal dependencies.

![purgetheheretics](assets/textures/splash_art.png)

Quick highlights
- Raycasting renderer with textured walls
- Map parsing and validation (.cub)
- Player movement, collision, basic combat, doors and pickups
- MLX (minilibx) for rendering and X11 integration

Instructions
------------
Prerequisites (Linux)
- gcc, make, xorg development headers (libxext-dev), libbsd-dev

Build
1. From project root:
   make
   (Makefile will check / clone ext libs: libft, poormanfixedpoint, minilibx)
2. For debug builds (Valgrind / line numbers):
   make clean && make

Run
- ./cub3D <map_file.cub>
Example:
- ./cub3D maps/test_door.cub

Debugging & memory checks
- Valgrind:
  valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./cub3D maps/test_door.cub

Resources
---------
Primary references
- 42 subject: https://cdn.intra.42.fr/pdf/pdf/960/cub3d.en.pdf
- MinilibX docs: https://harm-smits.github.io/42docs/libs/minilibx
- Raycasting tutorial: https://lodev.org/cgtutor/raycasting.html

How AI was used
- Assisted development tasks only: generating and refactoring cleanup functions, diagnosing Valgrind output, scripting debug/run commands, and drafting documentation.
- Core algorithms (raycasting, texture sampling, game logic) were implemented by the author. AI suggestions were reviewed and adapted before integration.
- All assets (textures, charater sprites) were created by AI.

Project layout (short)
- includes/ : headers
- srcs/     : source (main, init, map, render, player, gamelogic, mlx, cleanup)
- assets/   : textures & sprites
- extLibs/  : auto-managed external libs (libft, pmfp, minilibx)
- maps/     : example maps

Controls
--------
- W/S/A/D                   : move
- ← / → (or mouse movement) : turn
- Space                     : jump
- E                         : interact (doors)
- LMB                       : shoot
- ESC                       : exit

Architecture overview
---------------------
This short note explains the two core maths/representation ideas used across the engine: the map grid (1D/2D) and the fixed‑point arithmetic used for world coordinates and ray steps. See the repository root README for links to more details.

## Map representation — 1D/2D grid
- The level is stored as a discrete tile grid: a sequence of rows (array of strings). Conceptually it's a 2D array map[row][col].
- For some subsystems it is convenient to flatten into a 1D buffer:
  - index = row * map_width + col
  - this makes iterations, memcpy and some allocations simpler and faster.
- Tiles are integer grid cells used for collision and raycasting (walls, floor, doors).
- World positions are continuous (sub‑tile) coordinates mapped to/from the grid:
  - tile at (col, row) covers world x in [col, col+1) and y in [row, row+1).
  - tile center = (col + 0.5, row + 0.5).

Use cases:
- Collision: sample the map at floor(world_y), floor(world_x).
- Raycasting DDA: step through grid cells by incrementing current cell indices using computed stepX/stepY.

## Fixed‑point arithmetic (16.16)
- To avoid floating point and keep deterministic, positions and some maths use a 16.16 fixed‑point format stored in signed 32‑bit integers:
  - representation: value = int32 where high 16 bits = integer part, low 16 bits = fractional part.
  - scale factor: FIXED_ONE = 1 << 16 (65536).
- Basic conversions and ops:
  - to fixed: fixed = int_val << 16
  - to int: int_val = fixed >> 16
  - add/sub: same as integers
  - multiply: (a * b) >> 16
  - divide: (a << 16) / b
- Advantages: fast integer ops, predictable behavior across platforms, easy bitwise conversion.

Example helper macros (C):
```c
/* convert integer to fixed and back */
static inline int32_t int_to_fixed(int32_t i) { return i << 16; }
static inline int32_t fixed_to_int(int32_t f) { return f >> 16; }

/* fixed-point multiply/divide using 64-bit intermediate */
static inline int32_t fixed_mul(int32_t a, int32_t b)
{ return (int32_t)(((int64_t)a * b) >> 16); }

static inline int32_t fixed_div(int32_t a, int32_t b)
{ return (int32_t)(((int64_t)a << 16) / b); }
```

## How they work together (overview)
- Player/world coordinates use fixed‑point; ray directions and step increments are computed in fixed‑point.
- Raycasting uses DDA: from player fixed position compute stepX/stepY and walk integer cells until a wall is hit.
- After a hit, convert fixed distance to screen projection (fixed-point division) to compute slice height and the texture sample coordinate.
- Trigonometry uses small lookup tables (symmetry exploited) to avoid expensive runtime sin/cos; values can be stored/returned in fixed‑point when needed.

## Notes

- Destroy MLX images/windows before freeing raw pixel buffers (MLX manages some internal allocations).
- This is a concise overview — consult the full architecture guide for diagrams and implementation details.

## Contributing / Academic note
----------------------------
This repository was created for the 42 curriculum. External contributions are not accepted; it is provided for learning and reference. Please respect academic integrity.

## License & Contact
-----------------
MIT License.  

Authors:
- João Miranda — joamiran@student.42lisboa.com 
- Hugo Ladeiro - hladeiro@student.42lisboa.com