# Cub3D - TODO List (Updated)

> **Project Status**: Foundation Complete ✅ → Mathematics Complete ✅ → Player System 70% ✅ → Next: Input & 2D Rendering 🚧

---

## 🎯 Current Priority: Input System & 2D Visualization

### **IMMEDIATE NEXT STEPS** (This Week)
- [ ] **MLX Input Hooks Setup**
  - [ ] Key press/release event registration
  - [ ] WASD movement key bindings
  - [ ] ESC key for exit
  - [ ] Window close event handling

- [ ] **Basic 2D Map Rendering**
  - [ ] MLX window creation and image buffer setup
  - [ ] Grid-based map visualization (walls vs empty)
  - [ ] Player position indicator on 2D map
  - [ ] Real-time position updates

---

## ✅ **COMPLETED FEATURES**

### 🏗️ **Foundation (100% Complete)**
- [x] **Project Structure**
  - [x] Directory organization (`srcs/`, `includes/`, `extLibs/`)
  - [x] Makefile with proper compilation
  - [x] External libraries integration (libft, poormanfixedpoint, minilibx)
  - [x] Header organization (`typedefs.h`, `cub3d.h`, `utils.h`)

- [x] **Map Parsing System**
  - [x] `.cub` file parsing and validation
  - [x] Map conversion to 1D array (`char *map_array`)
  - [x] Memory management and error handling
  - [x] Map dimensions tracking (`width`, `height`)

### 🔢 **Fixed-Point Mathematics (95% Complete)**
- [x] **poormanfixedpoint Integration**
  - [x] Library inclusion in build system
  - [x] Type aliases (`t_fixed32`) 
  - [x] Conversion functions (`to_fixed32`, `from_fixed32`)
  - [x] Fixed-point constants (`FIXED_PI`, `FIXED_TWO_PI`, etc.)

- [x] **Trigonometric Optimization**
  - [x] 90-degree lookup tables (`t_trig` with dynamic allocation)
  - [x] Quadrant transformation (`fast_sin`, `fast_cos`)
  - [x] Memory management (`init_trig_table`, `cleanup_trig_table`)
  - [x] Fixed-point trig functions (`fixed_sin`, `fixed_cos`)

- [x] **Coordinate System**
  - [x] Array-to-grid conversions (`index_to_coords`, `coords_to_index`)
  - [x] World coordinate mapping (1 grid cell = 1.0 world unit)
  - [x] Collision detection (`is_wall_at`, `get_map_char_at`)
  - [x] World-to-screen scaling framework

### 🎮 **Player System (70% Complete)**
- [x] **Player Initialization**
  - [x] Automatic spawn detection from map (`find_player_spawn`)
  - [x] Grid-to-world coordinate conversion
  - [x] Player position in fixed-point world space
  - [x] Map character replacement ('N'→'0')

- [x] **Movement Functions**
  - [x] Basic movement (`move_player_x`, `move_player_y`)
  - [x] Position queries (`get_player_position`, `get_player_grid_position`)
  - [x] Collision checking (`can_player_move_to`)
  - [x] Fixed-point arithmetic integration (`fixed32_add`, `fixed32_mul`)

- [ ] **Missing: Input Integration** (Next Priority)
  - [ ] Key hook integration for movement
  - [ ] Smooth movement with delta time
  - [ ] Input state management

---

## 🚧 **IN PROGRESS / TODO**

### **PHASE 3A: Input System (Priority: URGENT)**

#### 🎮 **MLX Event Handling**
```c
// Need to implement these functions:
int key_press_hook(int keycode, t_cub_data *data);
int key_release_hook(int keycode, t_cub_data *data);
int close_window_hook(t_cub_data *data);
void setup_mlx_hooks(t_cub_data *data);
```

- [ ] **Key Event Setup**
  - [ ] Register MLX key press/release hooks
  - [ ] Define key codes for WASD movement
  - [ ] ESC key (65307) for graceful exit
  - [ ] Window close button handling

- [ ] **Input State Management**
  - [ ] Update `t_input` structure implementation
  - [ ] Track key states (pressed/released/held)
  - [ ] Prevent key repeat issues
  - [ ] Smooth movement implementation

#### 📋 **t_input Structure Implementation**
```c
typedef struct s_input
{
    bool move_forward;    // W key
    bool move_backward;   // S key  
    bool move_left;       // A key
    bool move_right;      // D key
    bool rotate_left;     // Left arrow (future)
    bool rotate_right;    // Right arrow (future)
    bool exit_game;       // ESC key
} t_input;
```

### **PHASE 3B: 2D Rendering (Priority: HIGH)**

#### 🖼️ **MLX Graphics Setup**
- [ ] **Window Management**
  - [ ] MLX window creation (`mlx_new_window`)
  - [ ] Window size configuration (800x600 default)
  - [ ] Window title setup
  - [ ] Proper window cleanup

- [ ] **Image Buffer System**
  - [ ] Image creation (`mlx_new_image`)
  - [ ] Buffer address retrieval (`mlx_get_data_addr`)
  - [ ] Pixel manipulation functions
  - [ ] Double buffering for smooth rendering

#### 🗺️ **2D Map Visualization**
```c
// Functions to implement:
void render_2d_map(t_cub_data *data);
void draw_map_grid(t_cub_data *data);
void draw_player_position(t_cub_data *data);
void update_display(t_cub_data *data);
```

- [ ] **Map Rendering**
  - [ ] Grid-based wall/empty space visualization
  - [ ] Color coding (walls=white, empty=black)
  - [ ] Dynamic scaling based on window size
  - [ ] Efficient pixel manipulation

- [ ] **Player Visualization**
  - [ ] Player position indicator (small colored square)
  - [ ] Real-time position updates
  - [ ] Sub-pixel position accuracy
  - [ ] Movement trail (optional debug feature)

#### 🔄 **Game Loop Integration**
- [ ] **Main Loop Structure**
  - [ ] Input processing
  - [ ] Player update logic
  - [ ] Rendering pipeline
  - [ ] Frame rate limiting

### **PHASE 4: 3D Raycasting (Future)**

#### 🎯 **Ray Casting Engine**
- [ ] **Ray Calculation**
  - [ ] Ray direction from player angle
  - [ ] DDA algorithm implementation
  - [ ] Wall intersection detection
  - [ ] Distance calculations

- [ ] **3D Projection**
  - [ ] Wall height from distance
  - [ ] Screen column rendering  
  - [ ] Perspective correction
  - [ ] Field of view management

#### 🎨 **Rendering Pipeline**
- [ ] **Wall Rendering**
  - [ ] Vertical line drawing
  - [ ] Distance-based shading
  - [ ] Wall texture application
  - [ ] Floor/ceiling rendering

### **PHASE 5: Polish & Advanced Features**

#### 🔧 **Performance Optimization**
- [ ] **Frame Rate Management**
  - [ ] Target 60 FPS implementation
  - [ ] Delta time calculations
  - [ ] Performance monitoring
  - [ ] Optimization profiling

- [ ] **Memory Optimization**
  - [ ] Memory pool implementation
  - [ ] Efficient buffer management
  - [ ] Leak detection and fixes

#### 🎨 **Visual Enhancements**
- [ ] **Textures & Colors**
  - [ ] Wall texture loading
  - [ ] Floor/ceiling colors
  - [ ] Lighting effects
  - [ ] Atmospheric perspective

#### 🧪 **Testing & Validation**
- [ ] **Quality Assurance**
  - [ ] Unit tests for critical functions
  - [ ] Memory leak testing (Valgrind)
  - [ ] Performance benchmarks
  - [ ] Cross-platform testing

---

## 📊 **Current Status Overview**

| Component | Status | Progress |
|-----------|--------|----------|
| **Foundation** | ✅ Complete | 100% |
| **Map Parsing** | ✅ Complete | 100% |
| **Fixed-Point Math** | ✅ Complete | 95% |
| **Coordinate System** | ✅ Complete | 100% |
| **Player System** | 🚧 Partial | 70% |
| **Input System** | ❌ Not Started | 0% |
| **2D Rendering** | ❌ Not Started | 0% |
| **3D Raycasting** | ❌ Not Started | 0% |

---

## 🎯 **Next Week's Goals**

### **Day 1-2: Input System**
- [ ] Implement MLX key hooks
- [ ] Add WASD movement controls
- [ ] Test player movement with keyboard

### **Day 3-4: 2D Rendering**
- [ ] Create MLX window and image buffer
- [ ] Implement basic 2D map rendering
- [ ] Add player position visualization

### **Day 5-7: Integration & Testing**
- [ ] Integrate input with rendering
- [ ] Test real-time movement
- [ ] Debug and optimize performance
- [ ] Prepare for 3D raycasting phase

---

## 💡 **Implementation Notes**

### **Key Design Decisions Made:**
1. **Fixed-Point Arithmetic**: Using poormanfixedpoint for deterministic behavior
2. **1D Map Array**: Efficient memory layout with coordinate conversion functions  
3. **Dynamic Trig Tables**: 90-degree tables with quadrant transformations (75% memory savings)
4. **Coordinate System**: 1 grid cell = 1.0 world unit with sub-cell precision
5. **Modular Architecture**: Clear separation between parsing, math, player, and rendering

### **Current Architecture Strengths:**
- ✅ **Deterministic behavior** across platforms
- ✅ **Memory efficient** trigonometric calculations
- ✅ **Precise collision detection** with fixed-point math
- ✅ **Scalable coordinate system** for any screen size
- ✅ **Clean separation of concerns** between modules

**Ready for next phase: Input handling and 2D visualization! 🚀**

---

**Last Updated**: July 31, 2025  
**Current Phase**: Input System Implementation  
**Next Milestone**: Playable 2D demo with WASD movement