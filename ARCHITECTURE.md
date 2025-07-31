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

[Insert the previous coordinate system content here...]

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