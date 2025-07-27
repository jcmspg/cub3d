# Cub3D

> **🚧 Work in Progress**  
> This project is currently under development. Some features may be incomplete or subject to change.

---

![42](https://img.shields.io/badge/42-joamiran-blue.svg?style=flat-square) ![42 Lisboa](https://img.shields.io/badge/42-Lisboa-blue.svg?style=flat-square) ![GitHub](https://img.shields.io/github/license/joamiran/cub3d?style=flat-square) ![GitHub last commit](https://img.shields.io/github/last-commit/joamiran/cub3d?style=flat-square)
![C-lang](https://img.shields.io/badge/language-C-blue.svg?style=flat-square) ![Status](https://img.shields.io/badge/status-under%20development-orange.svg?style=flat-square)
![Version](https://img.shields.io/badge/version-0.1.0-blue.svg?style=flat-square)
![Project](https://img.shields.io/badge/project-Cub3D-blue.svg?style=flat-square)

---

Welcome to the Cub3D project, a simple 3D game engine inspired by Wolfenstein 3D. This project is part of the 42 school curriculum and aims to help students learn computer graphics and game development concepts.

## Table of Contents

1. [Project Overview](#project-overview)
2. [Features](#features)
3. [Installation](#installation)
4. [Usage](#usage)
5. [Map File Format](#map-file-format)
6. [Directory Structure](#directory-structure)
7. [Compilation](#compilation)
8. [Controls](#controls)
9. [Development Status](#development-status)
10. [Technical Details](#technical-details)
11. [Resources](#resources)
12. [Contributing](#contributing)
13. [License](#license)
14. [Contact Information](#contact-information)

---

## Project Overview

Cub3D is a basic raycasting game engine project that covers key concepts in computer graphics, including:

- **Raycasting algorithm** for 3D rendering
- **Texture mapping** for wall surfaces
- **Player movement** and collision detection
- **Map parsing** from `.cub` configuration files
- **Event handling** for user inputs

The project provides hands-on experience with low-level graphics programming and mathematical concepts used in game development.

## Features

### ✅ Implemented

- [x] Project structure setup
- [x] Makefile configuration
- [x] Basic compilation system
- [x] Library integration (libft, minilibx)

### 🚧 In Development

- [ ] Map file parsing (.cub format)
- [ ] Player initialization and positioning
- [ ] Raycasting engine
- [ ] Texture loading and mapping
- [ ] Wall rendering
- [ ] Player movement system
- [ ] Collision detection
- [ ] Input handling (WASD, mouse)

### 🔮 Planned

- [ ] Floor and ceiling rendering
- [ ] Sprite support
- [ ] Minimap display
- [ ] Sound effects
- [ ] Performance optimizations

## Installation

### Prerequisites

- **Operating System**: Linux (Ubuntu recommended)
- **Compiler**: GCC with C99 support
- **Build System**: GNU Make
- **Libraries**: X11 development libraries

### Install Dependencies (Ubuntu)

```bash
sudo apt-get update
sudo apt-get install gcc make xorg libxext-dev libbsd-dev
```

### Build Instructions

1. **Clone the repository**:

   ```bash
   git clone <repository-url>
   cd cub3d
   ```

2. **Compile the project**:

   ```bash
   make
   ```

3. **Clean build files** (optional):
   ```bash
   make clean    # Remove object files
   make fclean   # Remove object files and executable
   make re       # Clean and rebuild
   ```

## Usage

### Basic Execution

```bash
./cuboid <map_file.cub>
```

### Example

```bash
./cuboid map.cub
```

### Command Line Arguments

- `map_file.cub`: Path to a valid map configuration file

## Map File Format

The `.cub` file format defines the game world and textures:

```
NO ./assets/wall_north.xpm
SO ./assets/wall_south.xpm
WE ./assets/wall_west.xpm
EA ./assets/wall_east.xpm

F 220,100,0
C 225,30,0

        1111111111111111111111111
        1000000000110000000000001
        1011000001110000000000001
        1001000000000000000000001
111111111011000001110000000000001
100000000011000001110111111111111
11110111111111011100000010001
11000001110101011111011110N0111
11111111 1111111 111111111111
```

### Map Elements

- **NO/SO/WE/EA**: Texture paths for north, south, west, east walls
- **F**: Floor color (R,G,B format)
- **C**: Ceiling color (R,G,B format)
- **1**: Wall
- **0**: Empty space
- **N/S/E/W**: Player starting position and orientation

## Directory Structure

```
cub3d/
├── assets/                  # Textures and sprites
│   ├── wall_north.xpm      # North wall texture
│   ├── wall_south.xpm      # South wall texture
│   ├── wall_east.xpm       # East wall texture
│   └── wall_west.xpm       # West wall texture
├── includes/                # Header files
│   ├── cub3d.h             # Main project header
│   ├── ft_validation.h     # Validation functions
│   ├── includes.h          # System includes
│   ├── inits.h             # Initialization functions
│   ├── mlx_init.h          # MLX initialization
│   └── typedefs.h          # Type definitions
├── srcs/                   # Source code
│   ├── main.c              # Entry point
│   ├── init.c              # Game initialization
│   ├── map/                # Map-related functions
│   │   ├── map_parse.c     # Map file parsing
│   │   ├── map_validate.c  # Map validation
│   │   └── map_utils.c     # Map utilities
│   ├── render/             # Rendering engine
│   │   ├── raycasting.c    # Raycasting algorithm
│   │   ├── draw_walls.c    # Wall rendering
│   │   └── textures.c      # Texture management
│   ├── player/             # Player management
│   │   ├── player.c        # Player state
│   │   └── movement.c      # Movement handling
│   └── mlx/                # MLX integration
│       └── hooks.c         # Event handlers
├── extLibs/                # External libraries
│   ├── libft/              # Custom C library
│   └── minilibx-linux/     # Graphics library
├── obj/                    # Compiled object files
├── Makefile               # Build configuration
├── map.cub                # Sample map file
└── README.md              # This file
```

## Compilation

### Makefile Targets

| Target               | Description                        |
| -------------------- | ---------------------------------- |
| `make` or `make all` | Build the complete project         |
| `make clean`         | Remove object files                |
| `make fclean`        | Remove object files and executable |
| `make re`            | Clean and rebuild everything       |
| `make check_mlx`     | Check/clone minilibx if missing    |
| `make mlx_clean`     | Remove minilibx completely         |

### Compilation Flags

- **-Wall -Wextra -Werror**: Strict error checking
- **-I./includes**: Include project headers
- **-I./extLibs/libft**: Include libft headers
- **-I./extLibs/minilibx-linux**: Include MLX headers

### Linking

- **-lft**: Link with libft
- **-lmlx**: Link with minilibx
- **-lm**: Math library
- **-lXext -lX11**: X11 libraries

### 🚀 Automatic MinilibX Management

The Makefile includes intelligent dependency management for MinilibX:

- **Auto-detection**: Checks if MinilibX exists before building
- **Auto-clone**: Downloads MinilibX from GitHub if missing
- **Error handling**: Provides clear feedback if cloning fails
- **Version control**: Uses official 42Paris MinilibX repository

**Example workflow:**

```bash
# First time building (no MinilibX)
make
# ✅ minilibx-linux cloned successfully.
# [compilation continues...]

# Subsequent builds
make
# ✅ minilibx-linux found at ./extLibs/minilibx-linux
# [compilation continues...]
```

**Manual MinilibX management:**

```bash
make check_mlx     # Check/clone MinilibX
make mlx_clean     # Remove MinilibX completely
```

## Controls

### Planned Controls

| Key   | Action        |
| ----- | ------------- |
| `W`   | Move forward  |
| `S`   | Move backward |
| `A`   | Strafe left   |
| `D`   | Strafe right  |
| `←`   | Turn left     |
| `→`   | Turn right    |
| `ESC` | Exit game     |

## Development Status

### Current Phase: **Setup & Architecture** ✅

- [x] Project structure organized
- [x] Makefile configured and working
- [x] Libraries integrated (libft, minilibx)
- [x] Basic compilation successful
- [x] Template functions created

### Next Phase: **Core Implementation** 🚧

- [ ] Implement map parsing logic
- [ ] Create player initialization system
- [ ] Develop raycasting algorithm
- [ ] Add texture loading functionality

### Future Phases: **Enhancement & Polish** 🔮

- [ ] Performance optimizations
- [ ] Advanced features (sprites, effects)
- [ ] Error handling improvements
- [ ] Documentation completion

## Technical Details

### Graphics Library

- **MinilibX**: Lightweight graphics library for X11
- **Raycasting**: Primary rendering technique
- **Texture Mapping**: XPM format support

### Mathematical Concepts

- Vector mathematics for ray calculations
- Trigonometric functions for angle computations
- DDA algorithm for grid traversal
- Perspective projection for 3D effect

### Memory Management

- Dynamic allocation for map data
- Texture buffer management
- Resource cleanup and error handling

## Resources

### Documentation

- [42 Intranet - Cub3D Subject](https://cdn.intra.42.fr/pdf/pdf/960/cub3d.en.pdf)
- [MinilibX Documentation](https://harm-smits.github.io/42docs/libs/minilibx)
- [Raycasting Tutorial](https://lodev.org/cgtutor/raycasting.html)

### References

- [Wolfenstein 3D Source Code](https://github.com/id-Software/wolf3d)
- [42 School Resources](https://github.com/42School)
- [Graphics Programming Tutorials](https://learnopengl.com/)

## Contributing

This is a 42 school project and follows academic guidelines. External contributions are not accepted, but feedback and suggestions are welcome for learning purposes.

### Code Style

- Follow 42's Norm coding standard
- Use consistent indentation (tabs)
- Comment complex algorithms
- Keep functions under 25 lines when possible

## License

This project is created for educational purposes as part of the 42 school curriculum. Please respect academic integrity guidelines.

## Contact Information

**Developer**: João Miranda  
**Email**: joamiran@student.42lisboa.com  
**42 Login**: joamiran  
**Project**: Cub3D  
**School**: 42 Lisboa

---

**Last Updated**: July 20, 2025  
**Version**: 0.1.0 (WIP)  
**Status**: 🚧 Under Development
