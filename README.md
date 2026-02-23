# Cub3D

> **🎮 Playable Demo Available**  
> Core gameplay features implemented: raycasting, enemies, doors, ammo boxes, combat system.

---
![purgetheheretics](assets/textures/splash_art.png)
---

![42](https://img.shields.io/badge/42-joamiran-blue.svg?style=flat-square) ![42 Lisboa](https://img.shields.io/badge/42-Lisboa-blue.svg?style=flat-square) ![GitHub](https://img.shields.io/github/license/joamiran/cub3d?style=flat-square) ![GitHub last commit](https://img.shields.io/github/last-commit/joamiran/cub3d?style=flat-square)
![C-lang](https://img.shields.io/badge/language-C-blue.svg?style=flat-square) ![Status](https://img.shields.io/badge/status-combat%20system%20complete-brightgreen.svg?style=flat-square)
![Version](https://img.shields.io/badge/version-0.4.1-blue.svg?style=flat-square)
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
11. [Architecture Documentation](#architecture-documentation)  👈 Add this
12. [Debugging Tools](#debugging-tools)
13. [Resources](#resources)
14. [Contributing](#contributing)
15. [License](#license)
16. [Contact Information](#contact-information)

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
- [x] Makefile configuration with automatic dependency management
- [x] Basic compilation system
- [x] Library integration (libft, poormanfixedpoint, minilibx)
- [x] **Complete map file parsing** (.cub format)
- [x] **Map validation and error handling**
- [x] **Memory management** for map data
- [x] **Debug visualization tools**

### 🚧 In Development

- [x] Player initialization and positioning
- [x] Raycasting engine
- [ ] Texture loading and mapping
- [x] Wall rendering
- [x] Player movement system
- [x] Collision detection
- [x] Input handling (WASD, mouse)

### 🔮 Planned

- [ ] Floor and ceiling rendering
- [ ] Sprite support
- [?] Minimap display
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

2. **Compile the project** (automatically downloads dependencies):

   ```bash
   make
   ```

   The Makefile will automatically:
   - Clone libft if missing
   - Clone poormanfixedpoint if missing  
   - Clone minilibx-linux if missing
   - Compile all dependencies
   - Build the final executable

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
NO ./path_to_the_north_texture
SO ./path_to_the_south_texture
WE ./path_to_the_west_texture
EA ./path_to_the_east_texture

F 220,100,0
C 225,30,0

1111111111111111111111111
1000000000110000000000001
1011000001110000000000001
1001000000000000000000001
111111111011000001110000000000001
100000000011000001110111111111111
11110111111111011100000010001
11110111111111011101010010001
11000000110101011100000010001
10000000000000001100000010001
10000000000000001101010010001
11000001110101011111011110N0111
11110111 1110101 101111010001
11111111 1111111 111111111111
```

### Map Elements

- **NO/SO/WE/EA**: Texture paths for north, south, west, east walls
- **F**: Floor color (R,G,B format)
- **C**: Ceiling color (R,G,B format)
- **1**: Wall
- **0**: Empty space
- **D**: Door (interactive, animated)
- **M**: Ammo pickup (ammunition sprite)
- **X**: Enemy spawn point
- **N/S/E/W**: Player starting position and orientation (facing North/South/East/West)
- **Space**: Valid empty space within the map

### Map Validation

The parser performs comprehensive validation:
- ✅ **File format validation**
- ✅ **Texture path verification**
- ✅ **Color range checking** (0-255)
- ✅ **Map boundary validation**
- ✅ **Player position detection**
- ✅ **Character validity checking**
- ✅ **Memory safety guarantees**

## Directory Structure

```
cub3d/
├── assets/                    # Textures and sprites
├── includes/                  # Header files
│   ├── cub3d.h               # Main project header
│   ├── ft_debug.h            # Debug utilities
│   ├── ft_validation.h       # Validation functions
│   ├── includes.h            # System includes
│   ├── inits.h               # Initialization functions
│   ├── mlx_init.h            # MLX initialization
│   └── typedefs.h            # Type definitions
├── srcs/                     # Source code
│   ├── main.c                # Entry point
│   ├── init.c                # Game initialization
│   ├── map/                  # Map-related functions ✅
│   │   ├── map_parse.c       # Map file parsing
│   │   ├── map_validate.c    # Map validation
│   │   └── map_utils.c       # Map utilities
│   ├── debugging/            # Debug tools ✅
│   │   └── print_map.c       # Map visualization
│   ├── render/               # Rendering engine ✅
│   │   ├── render.c          # Main render loop
│   │   ├── raycast.c         # DDA raycasting
│   │   ├── sprites.c         # Billboard sprite rendering
│   │   └── draw_*.c          # Drawing utilities
│   ├── player/               # Player management ✅
│   │   ├── movement.c        # Movement and collision
│   │   └── shooting.c        # Combat mechanics
│   ├── gamelogic/            # Game systems ✅
│   │   ├── enemies_init.c    # Enemy initialization
│   │   ├── doors.c           # Door mechanics
│   │   └── ammo.c            # Pickup system
│   ├── mlx/                  # MLX integration ✅
│   │   └── mylx_utils.c      # MLX utilities
│   └── cleanup/              # Memory management ✅
│       └── cleanup.c         # Resource cleanup
├── extLibs/                  # External libraries (auto-managed)
│   ├── libft/                # Custom C library
│   ├── poormanfixedpoint/    # Fixed-point math library
│   └── minilibx-linux/       # Graphics library
├── obj/                      # Compiled object files
├── Makefile                  # Build configuration
├── LICENSE                   # MIT License
├── map.cub                   # Sample map file
└── README.md                 # This file
```

## Compilation

### Makefile Targets

| Target               | Description                        |
| -------------------- | ---------------------------------- |
| `make` or `make all` | Build the complete project         |
| `make clean`         | Remove object files                |
| `make fclean`        | Remove object files and executable |
| `make re`            | Clean and rebuild everything       |
| `make check_libft`   | Check/clone libft if missing       |
| `make check_pmfp`    | Check/clone poormanfixedpoint      |
| `make check_mlx`     | Check/clone minilibx if missing    |
| `make libft_clean`   | Remove libft completely            |
| `make pmfp_clean`    | Remove poormanfixedpoint           |
| `make mlx_clean`     | Remove minilibx completely         |

### 🚀 Automatic Dependency Management

The Makefile includes intelligent dependency management for all external libraries:

- **Auto-detection**: Checks if libraries exist before building
- **Auto-clone**: Downloads libraries from GitHub if missing
- **Error handling**: Provides clear feedback if cloning fails
- **Version control**: Uses official repositories

**Example workflow:**

```bash
# First time building (clean system)
make
# 🔍 libft not found, cloning...
# ✅ libft cloned successfully.
# 🔍 poormanfixedpoint not found, cloning...
# ✅ poormanfixedpoint cloned successfully.
# 🔍 minilibx-linux not found, cloning...
# ✅ minilibx-linux cloned successfully.
# [compilation continues...]

# Subsequent builds
make
# ✅ libft already exists.
# ✅ poormanfixedpoint already exists.
# ✅ minilibx-linux already exists.
# [compilation continues...]
```

## Controls

### Implemented Controls

| Key         | Action                    |
| ----------- | ------------------------- |
| `W`         | Move forward              |
| `S`         | Move backward             |
| `A`         | Strafe left               |
| `D`         | Strafe right              |
| `←`         | Turn left                 |
| `→`         | Turn right                |
| `Space`     | Jump                      |
| `E`         | Interact (open doors)     |
| `LMB/Click` | Shoot                     |
| `ESC`       | Exit game                 |

## Development Status

### Current Phase: **Combat System & Enemies** ✅

- [x] Project structure organized
- [x] Makefile configured with auto-dependency management
- [x] Libraries integrated (libft, poormanfixedpoint, minilibx)
- [x] **Complete map file parsing system**
- [x] **Comprehensive map validation**
- [x] **Memory management and cleanup**
- [x] **Debug visualization tools**
- [x] **Error handling and reporting**

### Raycasting Engine ✅

- [x] DDA raycasting algorithm implementation
- [x] Player initialization and positioning system
- [x] Texture loading (XPM format)
- [x] Textured wall rendering with proper orientation
- [x] Floor and ceiling rendering

### Player & Movement ✅

- [x] Player movement (WASD + arrow keys)
- [x] Collision detection with walls
- [x] Enemy collision blocking
- [x] View bobbing and head movement
- [x] Jump mechanics with view offset

### Interactive Elements ✅

- [x] **Doors**: Animated opening/closing with `D` marker
- [x] **Ammo pickups**: Collectible sprites with `M` marker
- [x] **HUD system**: Health, ammo, and crosshair display

### Sprite & Enemy System ✅

- [x] Billboard sprite rendering for items and enemies
- [x] Enemy initialization from map (`X` marker)
- [x] Enemy collision detection
- [x] Per-scanline door occlusion (sprites properly render behind doors)
- [x] Sprite sorting by distance (painter's algorithm)
- [x] View offset compensation for jumping/bobbing

### Combat System ✅

- [x] Shooting mechanics with click/key input
- [x] Ammo management and pickup system
- [x] Enemy health tracking
- [x] Combat constants (damage, health values)

### Future Phases: **Polish & Optimization** 🔮

- [ ] Enemy AI and pathfinding
- [ ] Enemy attack behavior
- [ ] Sound effects and music
- [ ] Additional weapon types
- [ ] Performance profiling and optimization

### Bonus: **Portal System** 🌀 (Experimental)

- [ ] Portal placement (aim + click on walls)
- [ ] Portal pair linking (blue/orange)
- [ ] View-through rendering (see other side)
- [ ] Teleportation mechanics
- [ ] Momentum preservation

> See [THINKING_IN_PORTALS.md](THINKING_IN_PORTALS.md) for design documentation.

### Map Parsing & Rendering Architecture

- **Two-pass parsing**: First pass counts dimensions, second pass populates data
- **Memory-safe allocation**: Proper cleanup on all error paths
- **Validation pipeline**: Multiple validation stages with detailed error reporting
- **Line normalization**: All map lines padded to consistent width

### Raycasting Engine

- **DDA Algorithm**: Digital Differential Analyzer for efficient ray-wall intersection
- **Fixed-Point Arithmetic**: 16.16 format for deterministic calculations
- **Per-Column Rendering**: Each screen column casts one ray
- **Texture Mapping**: Wall textures sampled based on ray hit position

### Sprite Rendering System

- **Billboard Rendering**: Sprites always face the player
- **Distance Sorting**: Painter's algorithm renders far sprites first
- **Door Occlusion**: Per-scanline calculation ensures sprites render correctly behind doors
- **View Offset Compensation**: Sprites account for jump/bob effects

### Graphics Library

- **MinilibX**: Lightweight graphics library for X11
- **Double Buffering**: Smooth frame rendering
- **XPM Textures**: Support for XPM image format

### Mathematical Libraries

- **Poor Man's Fixed Point**: Custom fixed-point arithmetic library
- **Vector mathematics**: For ray calculations (planned)
- **Trigonometric functions**: For angle computations (planned)

### Memory Management

- **Dynamic allocation** for map data with proper cleanup
- **Resource management** with structured cleanup functions
- **Error handling** with memory leak prevention

## Architecture Documentation

For detailed technical information about the coordinate system, fixed-point arithmetic, and raycasting implementation:

📖 **[Technical Architecture Guide](ARCHITECTURE.md)** - Comprehensive explanation of:
- Coordinate system design and conversion logic
- Fixed-point arithmetic implementation
- Trigonometric lookup table optimization
- Raycasting algorithm architecture
- Memory management strategies
- Performance considerations

This document provides in-depth technical reasoning behind design decisions and mathematical foundations.

### Directory Structure

```
cub3d/
├── assets/                    # Textures and sprites
├── includes/                  # Header files
│   ├── cub3d.h               # Main project header
│   ├── ft_debug.h            # Debug utilities
│   ├── ft_validation.h       # Validation functions
│   ├── includes.h            # System includes
│   ├── inits.h               # Initialization functions
│   ├── mlx_init.h            # MLX initialization
│   └── typedefs.h            # Type definitions
├── srcs/                     # Source code
│   ├── main.c                # Entry point
│   ├── init.c                # Game initialization
│   ├── map/                  # Map-related functions ✅
│   │   ├── map_parse.c       # Map file parsing
│   │   ├── map_validate.c    # Map validation
│   │   └── map_utils.c       # Map utilities
│   ├── debugging/            # Debug tools ✅
│   │   └── print_map.c       # Map visualization
│   ├── render/               # Rendering engine ✅
│   │   ├── render.c          # Main render loop
│   │   ├── raycast.c         # DDA raycasting
│   │   ├── sprites.c         # Billboard sprite rendering
│   │   └── draw_*.c          # Drawing utilities
│   ├── player/               # Player management ✅
│   │   ├── movement.c        # Movement and collision
│   │   └── shooting.c        # Combat mechanics
│   ├── gamelogic/            # Game systems ✅
│   │   ├── enemies_init.c    # Enemy initialization
│   │   ├── doors.c           # Door mechanics
│   │   └── ammo.c            # Pickup system
│   ├── mlx/                  # MLX integration ✅
│   │   └── mylx_utils.c      # MLX utilities
│   └── cleanup/              # Memory management ✅
│       └── cleanup.c         # Resource cleanup
├── extLibs/                  # External libraries (auto-managed)
│   ├── libft/                # Custom C library
│   ├── poormanfixedpoint/    # Fixed-point math library
│   └── minilibx-linux/       # Graphics library
├── obj/                      # Compiled object files
├── Makefile                  # Build configuration
├── LICENSE                   # MIT License
├── map.cub                   # Sample map file
└── README.md                 # This file
```

### File Descriptions

- **assets/**: Contains all texture and sprite files used in the game.
- **includes/**: Header files for the project, containing function declarations and macros.
- **srcs/**: Source code for the project, divided into subdirectories by functionality.
- **extLibs/**: External libraries used in the project, managed by the Makefile.
- **obj/**: Directory for compiled object files.
- **Makefile**: Build configuration file for compiling the project.
- **LICENSE**: License file for the project.
- **map.cub**: Sample map file for testing the project.
- **README.md**: This README file.

## Debugging Tools

### Map Visualization

The project includes comprehensive debugging tools:

**Console Output:**
```c
print_map_array(map);    // Detailed character-by-character display
print_map_grid(map);     // Visual grid representation
```

**Visual Debug Grid:**
```c
draw_map_grid(data);     // In-window map visualization
```

**Debug Features:**
- ✅ Character-by-character map inspection
- ✅ Visual grid with color coding
- ✅ Memory allocation tracking
- ✅ Parse step-by-step debugging
- ✅ Validation error reporting

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

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

Created for educational purposes as part of the 42 school curriculum. Please respect academic integrity guidelines.

## Contact Information

**Developer**: João Miranda  
**Email**: joamiran@student.42lisboa.com  
**42 Login**: joamiran  
**Project**: Cub3D  
**School**: 42 Lisboa

---

**Last Updated**: February 23, 2026  
**Version**: 0.4.1  
**Status**: ✅ Combat System Complete - Enemy AI Next
