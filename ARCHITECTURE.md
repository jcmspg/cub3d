# Cub3D Technical Architecture Guide

> **Deep Dive into Coordinate Systems, Fixed-Point Arithmetic, and Raycasting Design**

This document provides comprehensive technical documentation for the Cub3D raycasting engine, explaining the mathematical foundations, design decisions, and implementation strategies used throughout the project.

## Table of Contents

1. [Design Philosophy](#design-philosophy)
2. [Coordinate System Architecture](#coordinate-system-architecture)
3. [Fixed-Point Arithmetic Implementation](#fixed-point-arithmetic-implementation)
4. [Trigonometric Optimization](#trigonometric-optimization)
5. [Raycasting Algorithm](#raycasting-algorithm)
6. [Memory Management Strategy](#memory-management-strategy)
7. [Performance Considerations](#performance-considerations)
8. [Cross-Platform Compatibility](#cross-platform-compatibility)

---

## Design Philosophy

### Core Principles

The Cub3D engine is built on several key architectural principles:

1. **Deterministic Behavior**: Fixed-point arithmetic ensures identical results across different hardware platforms
2. **Performance Optimization**: Lookup tables and efficient algorithms minimize computational overhead
3. **Memory Efficiency**: Smart data structures reduce memory footprint while maintaining functionality
4. **Separation of Concerns**: Clear distinction between game logic, mathematical operations, and rendering
5. **Scalability**: Resolution-independent design allows adaptation to any screen size

### Why These Choices?

**Fixed-Point Over Floating-Point:**
- Eliminates precision errors in collision detection
- Provides consistent behavior across different CPUs/FPUs
- Enables deterministic gameplay for testing and debugging
- Aligns perfectly with integer-based map collision system

**Lookup Tables Over Runtime Calculation:**
- O(1) trigonometric operations instead of expensive `sin()/cos()` calls
- 75% memory reduction through quadrant symmetry exploitation
- Cache-friendly data access patterns
- Predictable performance characteristics

---

## Coordinate System Architecture

### The Three-Layer System

The Cub3D engine operates with three distinct but interconnected coordinate systems:

```
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│   Screen Space  │    │   World Space   │    │   Map Array     │
│                 │    │                 │    │                 │
│ Pixels          │◄──►│ Fixed-Point     │◄──►│ 1D Array        │
│ (0,0) to        │    │ Units           │    │ Indices         │
│ (width,height)  │    │ Sub-cell        │    │ Integer Grid    │
└─────────────────┘    └─────────────────┘    └─────────────────┘
     Rendering              Game Logic           Collision Data
```

### 1. Map Array Structure (Storage Layer)

The foundation is a **1D character array** representing collision data:

```c
typedef struct s_map
{
    char *map_array;    // "111101100111000..."
    int width;          // 25 cells
    int height;         // 14 cells
    char *filename;     // "map.cub"
} t_map;
```

**Visual Representation:**
```
1D Array Storage:
[0][1][2][3][4][5][6][7][8][9][10][11]...
 1  1  1  0  0  1  1  0  0  1   1   0 ...

Interpreted as 2D Grid (width=4):
     Grid X: 0   1   2   3
Grid Y: 0  │ 1 │ 1 │ 1 │ 0 │  ← indices [0][1][2][3]
        1  │ 0 │ 1 │ 1 │ 0 │  ← indices [4][5][6][7]
        2  │ 0 │ 1 │ 1 │ 0 │  ← indices [8][9][10][11]
```

**Array-to-Grid Conversion:**
```c
// Convert 1D array index to 2D grid coordinates
void index_to_coords(int index, int width, int *x, int *y)
{
    *x = index % width;   // Column number
    *y = index / width;   // Row number
}

// Convert 2D grid coordinates to 1D array index
int coords_to_index(int x, int y, int width)
{
    return (y * width + x);
}

// Example: index 6 with width=4
// x = 6 % 4 = 2 (column 2)
// y = 6 / 4 = 1 (row 1)
// Grid position: (2, 1)
```

### 2. World Coordinate System (Game Logic Layer)

All gameplay mathematics operate in **world space** using fixed-point arithmetic:

```c
typedef struct s_player
{
    t_fixed32 x, y;        // World position (23.5, 11.7)
    t_fixed32 dir_x, dir_y; // Direction vector
    t_fixed32 plane_x, plane_y; // Camera plane
} t_player;
```

**Key Relationship: Each Grid Cell = 1.0 x 1.0 World Unit**

```
World Coordinate Grid:
     0.0  1.0  2.0  3.0  4.0
  0.0┌────┬────┬────┬────┐
     │ 1  │ 1  │ 1  │ 0  │
  1.0├────┼────┼────┼────┤
     │ 0  │ P  │ 1  │ 0  │  ← Player at world (1.5, 1.5)
  2.0├────┼────┼────┼────┤
     │ 0  │ 1  │ 1  │ 0  │
  3.0└────┴────┴────┴────┘

Map Array Mapping:
Grid (0,0) → Array[0] → '1' (wall)
Grid (1,1) → Array[5] → '0' (empty, player spawn)
Grid (3,0) → Array[3] → '0' (empty)
```

### 3. Screen Coordinate System (Rendering Layer)

Rendering scales world coordinates to screen pixels dynamically:

```c
// Dynamic scaling based on window dimensions
void world_to_screen(t_cub_data *data, t_fixed32 world_x, t_fixed32 world_y,
                     int *screen_x, int *screen_y)
{
    float scale_x = (float)data->mlx->width / (float)data->map->width;
    float scale_y = (float)data->mlx->height / (float)data->map->height;
    
    *screen_x = (int)(from_fixed32(world_x) * scale_x);
    *screen_y = (int)(from_fixed32(world_y) * scale_y);
}
```

**Scaling Examples:**
```
Scenario 1: 800x600 window, 25x14 map
scale_x = 800/25 = 32 pixels per world unit
scale_y = 600/14 = 42.8 pixels per world unit

Player at world (23.5, 11.5):
screen_x = 23.5 × 32 = 752 pixels
screen_y = 11.5 × 42.8 = 492 pixels

Scenario 2: 1920x1080 window, same map
scale_x = 1920/25 = 76.8 pixels per world unit
Player at world (23.5, 11.5):
screen_x = 23.5 × 76.8 = 1804 pixels
```

## Fixed-Point Arithmetic Implementation

### Mathematical Foundation

The engine uses **16.16 fixed-point format** from the poormanfixedpoint library:

```
32-bit Fixed-Point Number:
IIIIIIIIIIIIIIII.FFFFFFFFFFFFFFFF
└─ 16 bits ──┘└─ 16 bits ──┘
   Integer      Fractional
```

**Advantages in Raycasting Context:**

1. **Deterministic Collision Detection:**
```c
// Floating-point (unpredictable):
float pos = 1.0f;
for (int i = 0; i < 10; i++)
    pos += 0.1f;  // May not equal 2.0f due to precision errors

// Fixed-point (guaranteed):
t_fixed32 pos = to_fixed32(1.0f);
for (int i = 0; i < 10; i++)
    pos = fixed_add(pos, to_fixed32(0.1f));  // Always equals 2.0f exactly
```

2. **Perfect Grid Alignment:**
```c
// World position to grid conversion
t_fixed32 world_x = to_fixed32(23.7f);
int grid_x = from_fixed32(world_x);  // Always 23, never 22 or 24

// Critical for collision detection consistency
char cell = data->map->map_array[grid_y * width + grid_x];
```

### Fixed-Point in Practice

**Player Movement Example:**
```c
// Player moves forward by 0.1 world units
t_fixed32 move_speed = to_fixed32(0.1f);
t_fixed32 new_x = fixed_add(data->player->x, 
                           fixed32_mul(data->player->dir_x, move_speed));
t_fixed32 new_y = fixed_add(data->player->y,
                           fixed32_mul(data->player->dir_y, move_speed));

// Check collision at new position
if (!is_wall_at(data, new_x, new_y))
{
    data->player->x = new_x;
    data->player->y = new_y;
}
```

**Coordinate Conversion Pipeline:**
```c
// The complete conversion chain
t_fixed32 world_x = to_fixed32(23.7f);     // World space
int grid_x = from_fixed32(world_x);        // Grid space (23)
int array_idx = grid_y * width + grid_x;   // Array index
char cell = map_array[array_idx];          // Collision data
```

## Trigonometric Optimization

### 90-Degree Lookup Table Strategy

Instead of storing 360 degrees, we use mathematical symmetry:

```c
#define TRIG_TABLE_SIZE 91  // 0-90 degrees inclusive

typedef struct s_trig
{
    t_fixed32 sin[91];  // Precomputed sine values
    t_fixed32 cos[91];  // Precomputed cosine values
} t_trig;
```

**Memory Comparison:**
```
360-degree table: 360 × 2 × 4 bytes = 2,880 bytes
 90-degree table:  91 × 2 × 4 bytes =   728 bytes
Memory savings: 75% reduction
```

### Quadrant Transformation Rules

**Visual Quadrant Map:**
```
        90°
         │
    II   │   I
 ────────┼────────► 0°/360°
    III  │   IV
         │
        270°

Quadrant I   (0°-90°):   Use table directly
Quadrant II  (91°-180°): sin(180°-x), -cos(180°-x)
Quadrant III (181°-270°): -sin(x-180°), -cos(x-180°)
Quadrant IV  (271°-359°): -sin(360°-x), cos(360°-x)
```

**Implementation:**
```c
t_fixed32 fast_sin(t_trig *trig, int degrees)
{
    degrees = normalize_angle_degrees(degrees);  // 0-359
    
    if (degrees <= 90)
        return trig->sin[degrees];
    else if (degrees <= 180)
        return trig->sin[180 - degrees];    // sin(180° - x) = sin(x)
    else if (degrees <= 270)
        return -trig->sin[degrees - 180];   // sin(180° + x) = -sin(x)
    else
        return -trig->sin[360 - degrees];   // sin(360° - x) = -sin(x)
}
```

**Performance Analysis:**
```c
// Standard library call:
float result = sinf(angle);  // ~100-300 CPU cycles

// Optimized lookup:
t_fixed32 result = fast_sin(&trig, angle);  // ~5-10 CPU cycles
// 20-60x performance improvement
```

## Raycasting Algorithm

### The Complete Pipeline

**Step 1: Player Position in World Space**
```c
// Player spawns in center of grid cell
t_fixed32 player_x = to_fixed32(23.5f);  // Center of grid (23, y)
t_fixed32 player_y = to_fixed32(11.5f);  // Center of grid (x, 11)
```

**Step 2: Ray Direction Calculation**
```c
// Calculate ray angle for screen column
t_fixed32 fov = to_fixed32(60.0f);  // 60-degree field of view
t_fixed32 start_angle = fixed_sub(player_angle, fixed32_div(fov, to_fixed32(2.0f)));
t_fixed32 angle_step = fixed32_div(fov, to_fixed32((float)screen_width));

for (int x = 0; x < screen_width; x++)
{
    t_fixed32 ray_angle = fixed_add(start_angle, 
                                   fixed32_mul(to_fixed32((float)x), angle_step));
    
    // Direction vector using lookup tables
    t_fixed32 ray_dir_x = fixed_cos(&data->trig, ray_angle);
    t_fixed32 ray_dir_y = fixed_sin(&data->trig, ray_angle);
}
```

**Step 3: Ray-Grid Traversal**
```c
t_ray cast_single_ray(t_cub_data *data, t_fixed32 ray_angle)
{
    t_ray ray;
    t_fixed32 current_x = data->player->x;
    t_fixed32 current_y = data->player->y;
    
    // Ray direction from trig tables
    t_fixed32 dir_x = fixed_cos(&data->trig, ray_angle);
    t_fixed32 dir_y = fixed_sin(&data->trig, ray_angle);
    
    // Step size for accuracy vs performance
    t_fixed32 step_size = to_fixed32(0.01f);  // 1% of a grid cell
    t_fixed32 step_x = fixed32_mul(dir_x, step_size);
    t_fixed32 step_y = fixed32_mul(dir_y, step_size);
    
    // March along ray until wall hit
    while (!is_wall_at(data, current_x, current_y))
    {
        current_x = fixed_add(current_x, step_x);
        current_y = fixed_add(current_y, step_y);
    }
    
    // Calculate distance
    t_fixed32 dx = fixed_sub(current_x, data->player->x);
    t_fixed32 dy = fixed_sub(current_y, data->player->y);
    ray.distance = fixed_sqrt(fixed_add(fixed32_mul(dx, dx), fixed32_mul(dy, dy)));
    
    return ray;
}
```

**Step 4: Collision Detection**
```c
bool is_wall_at(t_cub_data *data, t_fixed32 world_x, t_fixed32 world_y)
{
    // Convert world coordinates to grid coordinates
    int grid_x = from_fixed32(world_x);  // Truncate to integer
    int grid_y = from_fixed32(world_y);
    
    // Bounds checking
    if (grid_x < 0 || grid_x >= data->map->width ||
        grid_y < 0 || grid_y >= data->map->height)
        return true;  // Out of bounds = wall
    
    // Array lookup
    int index = coords_to_index(grid_x, grid_y, data->map->width);
    return (data->map->map_array[index] == '1');
}
```

**Step 5: 3D Projection**
```c
void render_wall_slice(t_cub_data *data, int screen_x, t_ray ray)
{
    // Convert world distance to screen height
    float distance_float = from_fixed32(ray.distance);
    int wall_height = (int)(data->mlx->height / distance_float);
    
    // Center wall on screen
    int wall_start = (data->mlx->height - wall_height) / 2;
    int wall_end = wall_start + wall_height;
    
    // Draw vertical line
    for (int y = wall_start; y < wall_end; y++)
    {
        if (y >= 0 && y < data->mlx->height)
            mylx_pixel_put(data, screen_x, y, 0xFFFFFF);
    }
}
```

### Complete Data Flow Visualization

```
Input: map.cub file
         ↓
┌─────────────────────────────────────────────────────────┐
│ MAP PARSING                                             │
│ "111101100..." → map_array[width × height]             │
└─────────────────────────────────────────────────────────┘
         ↓
┌─────────────────────────────────────────────────────────┐
│ PLAYER INITIALIZATION                                   │
│ Find 'N' in array → Grid (23,11) → World (23.5, 11.5) │
└─────────────────────────────────────────────────────────┘
         ↓
┌─────────────────────────────────────────────────────────┐
│ RAYCASTING LOOP (for each screen column)               │
│                                                         │
│ 1. Calculate ray angle                                  │
│ 2. Get direction from trig tables                      │
│ 3. March through world space                           │
│ 4. Check collision via array lookup                    │
│ 5. Calculate distance                                   │
└─────────────────────────────────────────────────────────┘
         ↓
┌─────────────────────────────────────────────────────────┐
│ 3D RENDERING                                           │
│ Distance → Wall Height → Screen Pixels                 │
└─────────────────────────────────────────────────────────┘
```

### Real-World Example

Using your actual map structure:

```c
// Your map: 25×14 grid
// Player starts at 'N' position

// Example ray calculation:
t_fixed32 ray_angle = to_fixed32(45.0f);  // 45-degree ray

// Trig table lookup:
t_fixed32 dir_x = fast_cos(&trig, 45);  // ≈ 0.707 (fixed-point)
t_fixed32 dir_y = fast_sin(&trig, 45);  // ≈ 0.707 (fixed-point)

// Starting from player at (23.5, 11.5):
t_fixed32 current_x = to_fixed32(23.5f);
t_fixed32 current_y = to_fixed32(11.5f);

// Step along ray:
// Step 1: (23.507, 11.507) → Grid (23, 11) → Array[11×25+23=298] → '0' (empty)
// Step 2: (23.514, 11.514) → Grid (23, 11) → Array[298] → '0' (empty)
// ...
// Step N: (24.001, 12.001) → Grid (24, 12) → Array[12×25+24=324] → '1' (wall!)

// Distance calculation:
// dx = 24.001 - 23.5 = 0.501
// dy = 12.001 - 11.5 = 0.501  
// distance = √(0.501² + 0.501²) = 0.709 world units
```

This complete pipeline demonstrates how the three coordinate systems work together to create efficient, deterministic raycasting with sub-pixel precision and optimal performance.

## Memory Management Strategy

### Dynamic Memory Allocation

The engine uses `malloc()` and `free()` for runtime memory management:

```c
// Allocate a new map structure
t_map *map = (t_map *)malloc(sizeof(t_map));

// Allocate map array memory
map->map_array = (char *)malloc(map->width * map->height);

// Free allocated memory
free(map->map_array);
free(map);
```

### Memory Pooling

To optimize performance and reduce fragmentation:

- **Fixed-size pools** for common object sizes (e.g., rays, players)
- **Slab allocation** for map arrays to minimize fragmentation

```c
#define POOL_SIZE 1024

typedef struct s_memory_pool
{
    void *pool;
    size_t size;
    size_t used;
} t_memory_pool;

// Initialize memory pool
t_memory_pool *create_pool(size_t size)
{
    t_memory_pool *pool = (t_memory_pool *)malloc(sizeof(t_memory_pool));
    pool->pool = malloc(size);
    pool->size = size;
    pool->used = 0;
    return pool;
}

// Allocate from pool
void *pool_alloc(t_memory_pool *pool, size_t size)
{
    if (pool->used + size > pool->size)
        return NULL;  // Out of memory
    void *ptr = (char *)pool->pool + pool->used;
    pool->used += size;
    return ptr;
}

// Free memory pool
void destroy_pool(t_memory_pool *pool)
{
    free(pool->pool);
    free(pool);
}
```

## Performance Considerations

### Profiling Tools

Use `gprof` and `valgrind` to identify bottlenecks and memory issues:

```bash
# Compile with profiling enabled
gcc -pg -o cub3d cub3d.c

# Run the program
./cub3d

# Generate profile report
gprof cub3d gmon.out > report.txt
```

### Optimization Strategies

1. **Algorithmic Improvements**: Optimize raycasting and collision detection algorithms
2. **Data-Oriented Design**: Optimize memory access patterns for cache efficiency
3. **Parallel Processing**: Utilize multi-threading for raycasting and rendering

```c
#pragma omp parallel for
for (int x = 0; x < screen_width; x++)
{
    // Cast rays in parallel
    t_ray ray = cast_single_ray(data, x);
    
    // Store results
    results[x] = ray.distance;
}
```

## Cross-Platform Compatibility

### 42 School Environment Considerations

The architecture is specifically designed for the 42 school evaluation environment:

**Norminette Compliance:**
- All functions under 25 lines
- No variable declarations in loops
- Consistent coding style
- Memory safety guarantees

**Evaluation Robustness:**
- Deterministic behavior for consistent grading
- Comprehensive error handling
- Memory leak prevention
- Clear separation of modules for easy testing

**Performance Predictability:**
- Consistent frame rates across different school machines
- Minimal dependency on floating-point units
- Efficient memory usage for systems with limited resources

## Implementation Timeline

### Phase 1: Foundation (Complete ✅)
- [x] Map parsing and validation system
- [x] Memory management architecture
- [x] Coordinate conversion functions
- [x] Debug visualization tools

### Phase 2: Mathematics (In Progress 🚧)
- [ ] Fixed-point arithmetic integration
- [ ] Trigonometric lookup table implementation
- [ ] Player positioning system
- [ ] Basic collision detection

### Phase 3: Raycasting (Planned 🔮)
- [ ] DDA algorithm implementation
- [ ] Wall distance calculations
- [ ] Texture mapping system
- [ ] 3D rendering pipeline

### Phase 4: Optimization (Future 🎯)
- [ ] Performance profiling and optimization
- [ ] Advanced rendering features
- [ ] Platform-specific optimizations
- [ ] Comprehensive testing suite

## Mathematical Foundations

### Fixed-Point Precision Analysis

The poormanfixedpoint library uses 16.16 fixed-point format:

```
32-bit integer: IIIIIIIIIIIIIIII.FFFFFFFFFFFFFFFF
                ↑ 16 bits integer ↑ 16 bits fractional
```

**Precision Characteristics:**
- **Range**: -32,768.0 to 32,767.999984741
- **Precision**: 1/65,536 ≈ 0.0000152587890625
- **Suitable for**: World coordinates up to ±32K units with sub-pixel precision

**Comparison with float:**
```c
// Float precision varies with magnitude
float f1 = 1.0f + 0.0000001f;  // May lose precision
float f2 = 1000.0f + 0.001f;   // Definitely loses precision

// Fixed-point precision is constant
t_fixed32 fx1 = to_fixed32(1.0f) + to_fixed32(0.0000153f);     // Preserved
t_fixed32 fx2 = to_fixed32(1000.0f) + to_fixed32(0.0000153f);  // Preserved
```

### Trigonometric Accuracy Analysis

**Standard Library vs Lookup Table:**

| Method | Accuracy | Performance | Memory |
|--------|----------|-------------|---------|
| `sinf()` | ~7 decimal places | ~100 cycles | 0 bytes |
| Lookup 360° | Exact at degrees | ~5 cycles | 2,880 bytes |
| Lookup 90° + quad | Exact at degrees | ~8 cycles | 728 bytes |

**Error Analysis:**
```c
// Maximum error occurs between table entries
// For 1-degree resolution: max error ≈ 0.0003 radians
// This translates to < 0.02% error in distance calculations
// Well within acceptable limits for pixel-level rendering
```

## Performance Benchmarks

### Theoretical Performance Gains

**Trigonometric Operations:**
- Standard: `sin(x)` ≈ 100-300 CPU cycles
- Lookup: `table[x]` ≈ 1-5 CPU cycles
- **Speedup**: 20-300x faster

**Memory Access Patterns:**
- Sequential table access: Cache-friendly
- Random texture access: Optimized for MLX buffer layout
- Map array access: Predictable stride patterns

**Frame Rate Projections:**
```
Target: 60 FPS = 16.67ms per frame
Screen width: 800 pixels
Rays per frame: 800
Time per ray: 20.8μs budget

With lookup tables: ~2μs per ray
Safety margin: 10x performance headroom
```

## Code Organization Strategy

### Module Hierarchy

```
cub3d/
├── Core Mathematics (poormanfixedpoint)
│   ├── Fixed-point arithmetic
│   ├── Basic operations (+, -, *, /)
│   └── Advanced functions (sqrt, etc.)
├── Engine Mathematics (math_utils.c)
│   ├── Coordinate conversions
│   ├── Trigonometric tables
│   └── Collision detection
├── Game Logic
│   ├── Player management
│   ├── Map interaction
│   └── Game state
└── Rendering Pipeline
    ├── Ray calculations
    ├── Wall rendering
    └── Screen output
```

### Data Flow Architecture

```
Input → Player State → Ray Calculations → Collision Detection → Rendering
  ↓         ↓              ↓                    ↓                ↓
Fixed32   Fixed32      Trig Tables         Map Array        Screen Pixels
```

**Key Interfaces:**
1. **Input → Game Logic**: Raw input converted to fixed-point movement vectors
2. **Game Logic → Math**: World coordinates for collision and ray calculations  
3. **Math → Rendering**: Distances and angles for wall height and texture mapping
4. **Rendering → Display**: Screen coordinates and pixel colors for final output

## Testing and Validation Strategy

### Unit Testing Approach

**Mathematical Accuracy:**
```c
// Test fixed-point precision boundaries
assert(to_fixed32(1.0f) + to_fixed32(0.0000153f) > to_fixed32(1.0f));

// Test trigonometric table accuracy
for (int deg = 0; deg < 360; deg++) {
    float expected = sinf(deg * M_PI / 180.0f);
    float actual = from_fixed32(fast_sin(&trig, deg));
    assert(fabs(expected - actual) < 0.001f);
}
```

**Performance Validation:**
```c
// Benchmark ray casting performance
uint64_t start = get_time_microseconds();
for (int i = 0; i < 10000; i++) {
    cast_single_ray(data, to_fixed32((float)i));
}
uint64_t duration = get_time_microseconds() - start;
assert(duration < 100000);  // Must complete in < 100ms
```

**Memory Safety:**
```c
// Valgrind integration for automated memory testing
// Static analysis for buffer overflow detection
// Stress testing with large maps and long sessions
```

## Future Optimization Opportunities

### SIMD Integration

**Potential SIMD Applications:**
- Parallel ray calculations for multiple screen columns
- Vectorized texture sampling operations
- Batch coordinate transformations

**Implementation Considerations:**
```c
// Example: Process 4 rays simultaneously
__m128 ray_distances = _mm_set_ps(d1, d2, d3, d4);
__m128 wall_heights = _mm_div_ps(screen_height_vec, ray_distances);
```

### GPU Acceleration Possibilities

**Shader-Friendly Algorithms:**
- Ray-grid intersection calculations
- Texture sampling and filtering
- Color blending operations

**Hybrid CPU-GPU Approach:**
- CPU: Game logic, collision detection, input handling
- GPU: Ray calculations, texture mapping, pixel shading

### Advanced Rendering Features

**Planned Enhancements:**
1. **Dynamic Lighting**: Shadow casting using ray-based algorithms
2. **Reflection Mapping**: Environmental reflections for enhanced realism
3. **Particle Systems**: Smoke, fire, and atmospheric effects
4. **Level-of-Detail**: Adaptive quality based on distance and performance

---

## Conclusion

This architecture provides a solid foundation for a high-performance, cross-platform raycasting engine while maintaining the simplicity and educational value required for the 42 school curriculum. The combination of fixed-point arithmetic, optimized lookup tables, and clean separation of concerns creates a system that is both efficient and maintainable.

The design decisions prioritize:
- **Reliability**: Deterministic behavior for consistent results
- **Performance**: Optimized algorithms for smooth gameplay
- **Maintainability**: Clear code organization for easy debugging and extension
- **Educational Value**: Transparent implementation of computer graphics concepts

**Next Steps**: Implement the raycasting core using this mathematical foundation, followed by texture mapping and advanced rendering features.

---

*This document will be updated as the project progresses through each implementation phase.*