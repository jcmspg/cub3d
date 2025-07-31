# Cub3D - TODO List (Fixed-Point Implementation)

> **Project Status**: Map Parsing Complete ✅ → Next: Fixed-Point Coordinate System 🚧

---

## 🎯 Current Priority

### Phase 2A: Fixed-Point Coordinate System

#### 🔧 Core Systems with Fixed-Point Math
- [ ] **Fixed-Point Integration**
  - [ ] Include poormanfixedpoint library in build system
  - [ ] Create fixed-point type aliases for project
  - [ ] Implement conversion helpers (int ↔ fixed, float ↔ fixed)
  - [ ] Set up fixed-point constants (MAP_SCALE, WORLD_SCALE, etc.)

- [ ] **Coordinate System Setup (Fixed-Point)**
  - [ ] Implement 1D array to 2D coordinate translation using fixed-point
  - [ ] Create world-to-screen coordinate transformation with fixed precision
  - [ ] Set up player position coordinate system using pmfp_t
  - [ ] Define map-to-world space conversion with fixed-point math

---

## 📋 Detailed Implementation Plan

### 🔢 Fixed-Point Architecture

#### **Type Definitions**
```c
// Fixed-point coordinate system
typedef pmfp_t t_fixed;
typedef struct s_fixed_point {
    t_fixed x;
    t_fixed y;
} t_fixed_point;

typedef struct s_player_fixed {
    t_fixed_point position;     // World coordinates (fixed-point)
    t_fixed angle;              // Direction in radians (fixed-point)
    t_fixed fov;               // Field of view (fixed-point)
} t_player_fixed;
```

#### **Constants Setup**
- [ ] Define `FIXED_MAP_UNIT` - size of one map cell in fixed-point world coords
- [ ] Define `FIXED_PI`, `FIXED_2PI`, `FIXED_PI_2` - trigonometric constants
- [ ] Define `FIXED_PLAYER_SPEED` - movement speed in fixed-point units
- [ ] Define `FIXED_ROTATION_SPEED` - rotation speed in fixed-point radians

### 🗺️ Coordinate Transformation Functions

#### **Core Coordinate Functions (Fixed-Point)**
- [ ] **Map ↔ World Conversions**
  ```c
  t_fixed map_to_world_x_fixed(int map_x);
  t_fixed map_to_world_y_fixed(int map_y);
  int world_to_map_x_fixed(t_fixed world_x);
  int world_to_map_y_fixed(t_fixed world_y);
  ```

- [ ] **World ↔ Screen Conversions**
  ```c
  int world_to_screen_x_fixed(t_fixed world_x, t_player_fixed *player);
  int world_to_screen_y_fixed(t_fixed world_y, t_fixed distance);
  t_fixed_point screen_to_world_fixed(int screen_x, int screen_y, t_player_fixed *player);
  ```

- [ ] **1D Array ↔ 2D Coordinate Translation**
  ```c
  int coords_to_index(int x, int y, int width);
  t_point index_to_coords(int index, int width);
  char get_map_at_fixed(t_fixed world_x, t_fixed world_y, t_map *map);
  ```

### 🎮 Player System (Fixed-Point)

#### **Player Initialization**
- [ ] **`find_player_position_fixed()`** - locate player and convert to fixed-point
- [ ] **`init_player_fixed()`** - initialize player with fixed-point coordinates
- [ ] **`set_player_direction_fixed()`** - convert N/S/E/W to fixed-point angles

#### **Player Utilities**
- [ ] **`get_player_map_position()`** - get current map cell of player
- [ ] **`is_valid_player_position()`** - validate player position for movement
- [ ] **`player_facing_direction()`** - get cardinal direction player is facing

### 🎨 Raycasting (Fixed-Point Implementation)

#### **Ray Mathematics**
- [ ] **Ray Casting with Fixed-Point**
  ```c
  typedef struct s_ray_fixed {
      t_fixed_point origin;       // Ray start position
      t_fixed_point direction;    // Ray direction (normalized)
      t_fixed length;            // Distance to wall hit
      int map_x, map_y;          // Map cell hit
      bool hit_vertical;         // Wall orientation
  } t_ray_fixed;
  ```

- [ ] **Ray Functions**
  ```c
  t_ray_fixed cast_ray_fixed(t_fixed_point origin, t_fixed angle);
  t_fixed calculate_wall_distance_fixed(t_ray_fixed ray, t_fixed player_angle);
  int calculate_wall_height_fixed(t_fixed distance, int screen_height);
  ```

#### **DDA Algorithm (Fixed-Point)**
- [ ] Implement DDA traversal using fixed-point arithmetic
- [ ] Calculate step sizes and initial positions in fixed-point
- [ ] Handle wall intersection detection with precision
- [ ] Determine wall hit side (N/S/E/W) for texture selection

### 🔧 Fixed-Point Utility Functions

#### **Math Helpers**
- [ ] **Trigonometry**
  ```c
  t_fixed sin_fixed(t_fixed angle);          // Using pmfp sin functions
  t_fixed cos_fixed(t_fixed angle);          // Using pmfp cos functions
  t_fixed atan2_fixed(t_fixed y, t_fixed x); // Angle from vector
  t_fixed normalize_angle_fixed(t_fixed angle); // Keep in 0-2π
  ```

- [ ] **Vector Operations**
  ```c
  t_fixed_point vector_add_fixed(t_fixed_point a, t_fixed_point b);
  t_fixed_point vector_subtract_fixed(t_fixed_point a, t_fixed_point b);
  t_fixed vector_dot_fixed(t_fixed_point a, t_fixed_point b);
  t_fixed vector_length_fixed(t_fixed_point v);
  t_fixed_point vector_normalize_fixed(t_fixed_point v);
  ```

- [ ] **Distance and Rotation**
  ```c
  t_fixed distance_fixed(t_fixed_point a, t_fixed_point b);
  t_fixed_point rotate_point_fixed(t_fixed_point point, t_fixed angle);
  ```

### 🏗️ Build System Integration

#### **Makefile Updates**
- [ ] Add poormanfixedpoint to library dependencies
- [ ] Update include paths for pmfp headers
- [ ] Add pmfp compilation flags if needed
- [ ] Ensure pmfp library is built before main project

#### **Header Integration**
- [ ] Include `poormansfixed.h` in main project headers
- [ ] Create pmfp wrapper functions for common operations
- [ ] Add fixed-point type definitions to project headers

---

## 🧮 Fixed-Point Implementation Strategy

### **Precision Planning**
```c
// Example fixed-point setup
#define WORLD_SCALE_BITS 16        // 16.16 fixed-point for world coordinates
#define ANGLE_SCALE_BITS 20        // Higher precision for angles
#define DISTANCE_SCALE_BITS 12     // Sufficient for wall distances

// Convert between different precisions as needed
t_fixed world_coord = pmfp_from_int(map_x);
t_fixed angle_precise = pmfp_from_float(angle_degrees * PI / 180.0f);
```

### **Performance Considerations**
- [ ] Use integer operations where possible
- [ ] Minimize conversions between fixed and float
- [ ] Cache frequently used trigonometric values
- [ ] Profile critical path functions

### **Testing Strategy**
- [ ] Unit tests for coordinate conversions
- [ ] Precision validation (compare with float calculations)
- [ ] Edge case testing (boundaries, overflow)
- [ ] Performance benchmarks vs float implementation

---

## 📋 Implementation Phases

### Phase 1: Foundation (Current) 🎯
- [ ] Set up fixed-point type system
- [ ] Implement basic coordinate transformations
- [ ] Create player initialization with fixed-point
- [ ] Test 1D↔2D array conversions

### Phase 2: Ray Casting 🚧
- [ ] Implement fixed-point DDA algorithm
- [ ] Add wall intersection detection
- [ ] Calculate distances and wall heights
- [ ] Basic wall rendering

### Phase 3: Movement & Controls 🔮
- [ ] Player movement with fixed-point precision
- [ ] Smooth rotation using fixed-point angles
- [ ] Collision detection with sub-pixel accuracy
- [ ] Input handling and response

### Phase 4: Optimization & Polish ⭐
- [ ] Performance tuning of fixed-point operations
- [ ] Memory usage optimization
- [ ] Visual debugging improvements
- [ ] Documentation and cleanup

---

## 🔬 Debug & Validation Tools

### **Fixed-Point Debugging**
- [ ] `print_fixed_point()` - display fixed-point values in readable format
- [ ] `validate_coordinate_precision()` - check conversion accuracy
- [ ] `debug_player_position()` - show player position in multiple formats
- [ ] `visualize_rays_fixed()` - draw ray casting with fixed-point precision

### **Coordinate System Validation**
- [ ] Test round-trip conversions (map→world→map)
- [ ] Verify screen projection accuracy
- [ ] Check boundary conditions and edge cases
- [ ] Compare fixed-point vs float results

---

**Last Updated**: July 31, 2025  
**Focus**: Fixed-point coordinate system implementation  
**Library**: poormanfixedpoint integration  
**Next**: Ray casting with fixed-point precision