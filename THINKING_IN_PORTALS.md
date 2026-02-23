# Thinking in Portals

> **Design Document for Portal Implementation in Cub3D**  
> A deep dive into non-Euclidean rendering and teleportation mechanics

---

## Table of Contents

1. [Concept Overview](#concept-overview)
2. [Portal Mechanics Breakdown](#portal-mechanics-breakdown)
3. [Data Structures](#data-structures)
4. [Portal Placement System](#portal-placement-system)
5. [The Rendering Challenge](#the-rendering-challenge)
6. [Coordinate Transformations](#coordinate-transformations)
7. [Teleportation Physics](#teleportation-physics)
8. [Edge Cases & Gotchas](#edge-cases--gotchas)
9. [Implementation Phases](#implementation-phases)
10. [Visual Reference](#visual-reference)

---

## Concept Overview

### What We're Building

A portal system where:
1. **Player aims at a wall** and clicks to place a portal (blue or orange)
2. **Looking through a portal** shows the view from the linked portal's position
3. **Walking through a portal** teleports the player to the linked portal
4. **Objects maintain momentum** through portals (velocity is transformed)

### The Magic Illusion

The "magic" of Portal is that when you look through a portal, you see a **seamless continuation of space** that doesn't actually exist. Your brain is tricked because:

- The perspective is mathematically correct
- There's no visible seam or edge
- Movement through is instantaneous

```
Physical Reality:              What You See:
                              
  Room A        Room B          Seamless Space
┌────────┐    ┌────────┐      ┌────────────────┐
│   P    │    │    P   │      │                │
│   ║    │    │    ║   │  →   │   You ──────►  │
│  You   │    │        │      │                │
└────────┘    └────────┘      └────────────────┘

P = Portal    ║ = Wall (with portal)
```

---

## Portal Mechanics Breakdown

### The Three Core Systems

```
┌─────────────────┐     ┌─────────────────┐     ┌─────────────────┐
│   PLACEMENT     │     │    RENDERING    │     │  TELEPORTATION  │
│                 │     │                 │     │                 │
│ • Ray to wall   │     │ • View through  │     │ • Collision     │
│ • Store pos     │     │ • Transform     │     │ • Position      │
│ • Link pairs    │     │ • Recursion     │     │ • Velocity      │
└─────────────────┘     └─────────────────┘     └─────────────────┘
      EASY                    HARD                   MEDIUM
```

### How Portal (the game) Does It

1. **Stencil Buffer**: Draws portal frame, marks pixels
2. **Recursive Render**: Renders scene from other portal's view into marked area
3. **Depth Tricks**: Manages what's "in front" vs "through" the portal
4. **Physics Transform**: Rotates velocity vectors when teleporting

---

## Data Structures

### Portal Definition

```
Portal Structure:
┌────────────────────────────────────────────────────────┐
│  t_portal                                              │
├────────────────────────────────────────────────────────┤
│  Position:                                             │
│    • grid_x, grid_y    (which map cell)                │
│    • world_x, world_y  (exact position on wall)        │
│    • wall_side         (N=0, E=1, S=2, W=3)            │
│                                                        │
│  Properties:                                           │
│    • color             (BLUE=0, ORANGE=1)              │
│    • is_active         (has been placed?)              │
│    • linked            (pointer to partner portal)     │
│                                                        │
│  Rendering:                                            │
│    • screen_x_start    (left edge on screen)           │
│    • screen_x_end      (right edge on screen)          │
│    • distance          (for depth sorting)             │
└────────────────────────────────────────────────────────┘
```

### Portal Pair

```
Portal Pair Relationship:
                    
   BLUE PORTAL              ORANGE PORTAL
  ┌───────────┐            ┌───────────┐
  │ wall: N   │◄──linked──►│ wall: S   │
  │ pos: 5,3  │            │ pos: 12,8 │
  └───────────┘            └───────────┘
        │                        │
        └────────────────────────┘
             Always paired
```

### Game State Addition

```
Add to t_game:
  • t_portal  portals[2]     (blue=0, orange=1)
  • int       active_portal  (which one to place next)
  • int       portals_ready  (both placed = can teleport)
```

---

## Portal Placement System

### Step 1: Detect Where Player is Aiming

We already cast rays for rendering. For placement, cast a single ray from screen center:

```
Screen Center Ray:
                                    
        Player View                 
    ┌───────────────────┐           
    │                   │           
    │         +         │  ← Crosshair (screen center)
    │         │         │           
    └─────────│─────────┘           
              │                     
              ▼                     
         Cast ray here              
              │                     
              ▼                     
         ████████  ← Wall hit!      
         ████████                   
```

### Step 2: Determine Wall Surface

When ray hits a wall, we know:
- **Grid position** (x, y)
- **Which side** (the ray came from N/S/E/W)
- **Exact hit point** (for precise portal placement)

```
Wall Hit Analysis:

Ray traveling East (→) hits wall:
                        
    ════════════════    
         │    │         Wall side = WEST
    ─────│────│─────    (portal faces West)
         │ ██ │         
         │ ██ │◄─ Hit   
         │ ██ │         
    ─────│────│─────    
         │    │         
    ════════════════    
```

### Step 3: Validate Placement

Not all surfaces can hold portals:
- ✅ Regular walls (`'1'`)
- ❌ Doors (`'D'`) - they move!
- ❌ Thin walls - portal needs full surface
- ❌ Already has portal - replace it

### Step 4: Store Portal Data

```
On Right Click (Blue) or Middle Click (Orange):

1. Cast center ray
2. If hit valid wall:
   - Store grid position
   - Store exact world coordinates
   - Store wall orientation
   - Mark as active
   - If both portals placed → portals_ready = true
```

---

## The Rendering Challenge

### The Core Problem

When a ray hits a portal during normal rendering, instead of drawing a wall texture, we need to:

1. **Stop the ray** at the portal surface
2. **Transform the ray** to the linked portal's position
3. **Continue casting** from the new position
4. **Draw what that ray sees** in the portal's screen area

```
Normal Ray (hits wall):
                                    
Player ─────────────────► Wall      
                          ████      
       Distance = 5.0     ████      
       Draw: wall texture           


Portal Ray (hits portal):
                                    
Player ─────► Portal A ═══════► Continue from Portal B ────► Wall
              │                                               ████
              │ Transform ray position & direction            ████
              │                                               
              └── Don't draw here, draw what ray B sees!      
```

### Single-Depth vs Recursive

**Single-Depth (Easier)**:
- Ray hits portal → transform → continue → hit wall/object
- No portal-through-portal views
- Already impressive!

**Recursive (Full Portal Experience)**:
- Ray hits portal → transform → continue → hits ANOTHER portal → transform again...
- Need depth limit (or infinite loop!)
- Much more complex

```
Recursive Portal View:

Looking through Portal A at Portal B:

┌─────────────────────────────────┐
│  ┌───────────────────────────┐  │
│  │  ┌─────────────────────┐  │  │
│  │  │  ┌───────────────┐  │  │  │
│  │  │  │     ...       │  │  │  │
│  │  │  └───────────────┘  │  │  │
│  │  └─────────────────────┘  │  │
│  └───────────────────────────┘  │
└─────────────────────────────────┘
   Depth 1  Depth 2   Depth 3  ...
```

### The Screen-Space Approach

Instead of modifying raycasting, we can:

1. Render the full scene normally
2. Determine portal's screen rectangle
3. Render scene AGAIN from other portal's view
4. Copy that render INTO the portal rectangle

```
Two-Pass Rendering:

Pass 1: Normal render (portal = special texture/color)
┌────────────────────────┐
│████████████████████████│
│████████████████████████│
│████████[PORTAL]████████│  ← Mark this area
│████████████████████████│
│████████████████████████│
└────────────────────────┘

Pass 2: Render from Portal B's viewpoint
┌────────────────────────┐
│░░░░░░░░░░░░░░░░░░░░░░░░│
│░░░░░░ROOM B VIEW░░░░░░░│
│░░░░░░░░░░░░░░░░░░░░░░░░│
└────────────────────────┘

Final: Composite Pass 2 into marked area
┌────────────────────────┐
│████████████████████████│
│████████████████████████│
│████████[ROOM B]████████│  ← Seamless!
│████████████████████████│
│████████████████████████│
└────────────────────────┘
```

---

## Coordinate Transformations

### The Key Insight

When you step through a portal, your **position and direction** must transform to maintain the illusion of continuous space.

### Position Transform

```
Portal A (North wall)          Portal B (East wall)
     at (5, 10)                    at (20, 15)

      N                                 N
      │                                 │
   ───Portal A───              ────┬────┬────
      │                            │Portal│
      │ ← You                      │ B    │
      S                            │    You→
                                   └────┘

You're 0.5 units south of Portal A
  → You appear 0.5 units west of Portal B
```

### Direction Transform

The rotation depends on portal orientations:

```
Portal Facing Directions:

Portal A faces: SOUTH (you enter from the south)
Portal B faces: WEST  (you exit to the west)

Rotation needed: 90° clockwise

Your direction (1, 0) "East"
  → Transforms to (0, 1) "South"
```

### The Rotation Matrix

```
Portal orientation difference = (B_facing - A_facing + 2) % 4

Rotations:
  0 (0°):   new_dir = (dir_x, dir_y)         // Same
  1 (90°):  new_dir = (-dir_y, dir_x)        // CW
  2 (180°): new_dir = (-dir_x, -dir_y)       // Flip
  3 (270°): new_dir = (dir_y, -dir_x)        // CCW
```

### Visual Example

```
Before Teleport:                After Teleport:

Portal A (faces South)          Portal B (faces West)
                                
    ▲ You (facing North)            You (facing East) ►
    │                               
════╪════ Portal A              ║
                                ║ Portal B
                                ║

You walked INTO Portal A (going North)
You come OUT of Portal B (going East)
Your facing direction rotated 90° CW
```

---

## Teleportation Physics

### When Does Teleport Trigger?

```
Teleportation Trigger:

1. Player position crosses portal plane
2. Player was on the "entry" side last frame
3. Player is on the "exit" side this frame

   Entry Side │ Portal │ Exit Side
              │  ████  │
         ○────│──████──│────●
      Last    │  ████  │   Current
      Frame   │        │   Frame
              │        │
        TRIGGER TELEPORT!
```

### Position Calculation

```
1. Calculate offset from portal center:
   offset = player_pos - portal_A_center

2. Rotate offset based on portal orientations:
   rotated_offset = rotate(offset, rotation_angle)

3. Apply to destination portal:
   new_pos = portal_B_center + rotated_offset
```

### Velocity Preservation (Momentum)

The "speedy thing goes in, speedy thing comes out" rule:

```
Velocity Transform:

Same rotation as direction!

vel_in = (5.0, 0.0)  // Moving East at 5 units/sec

If 90° rotation:
vel_out = (0.0, 5.0)  // Moving South at 5 units/sec

Speed is preserved, direction is transformed.
```

### The Falling-Into-Floor Portal

Portal's famous puzzle: portal on floor, portal on wall

```
Floor Portal → Wall Portal:

     │
     │ Fall down into floor portal
     ▼
  ═══════  Floor Portal
  
  
          Exit from Wall Portal
              ────►
          ════════ Wall
          
Falling velocity becomes horizontal velocity!
```

---

## Edge Cases & Gotchas

### 1. The Infinite Loop

If portals face each other directly:

```
    Portal A ←──────────────→ Portal B
    
Ray goes: A → B → A → B → A → B → ...
```

**Solution**: Depth limit (max 8-16 iterations)

### 2. The Z-Fighting Problem

When portal surface and view-through content are at same depth:

```
Flickering between:
  • Portal frame texture
  • Scene visible through portal
```

**Solution**: Render portal content FIRST, then frame ON TOP

### 3. The Close-Up Distortion

When player is very close to portal, the view angle gets extreme:

```
Normal view:        Close-up:
┌──────────┐       ┌──────────┐
│  Portal  │       │██Portal██│
│  ┌────┐  │       │██┌────┐██│
│  │view│  │       │██│FISH│██│
│  └────┘  │       │██│EYE!│██│
└──────────┘       │██└────┘██│
                   └──────────┘
```

**Solution**: Clamp near-plane or adjust FOV calculation

### 4. Objects Partially Through Portal

An enemy walking through a portal should be:
- Partially visible on BOTH sides
- Cut exactly at portal plane

**Solution**: This is VERY hard. Options:
- Ignore (object teleports instantly)
- Render object twice with clipping planes
- Accept visual pop when crossing

### 5. Sound Through Portals

If you hear something through a portal, it should come from:
- The portal's direction (not the object's true position)

**Solution**: Audio raycast through portals (future feature)

---

## Implementation Phases

### Phase 1: Data & Placement (Day 1-2)
```
□ Add t_portal struct to typedefs.h
□ Add portal array to t_game
□ Implement portal placement ray (screen center)
□ Detect right-click and middle-click events
□ Store portal position and orientation
□ Visual feedback (portal placed message)
```

### Phase 2: Basic Rendering (Day 2-3)
```
□ Detect portal during raycasting
□ Draw portal as colored rectangle (blue/orange)
□ Calculate portal's screen boundaries
□ Draw portal frame/border effect
```

### Phase 3: Teleportation (Day 3-4)
```
□ Detect player crossing portal plane
□ Calculate position transform
□ Calculate direction/angle transform
□ Handle camera rotation smoothly
□ Test with simple portal pairs
```

### Phase 4: View Through Portal (Day 5-7)
```
□ Implement view transformation matrix
□ Ray continuation through portal
□ Single-depth portal rendering
□ Proper depth sorting with portal views
□ Edge case handling
```

### Phase 5: Polish (Day 7+)
```
□ Portal particle effects
□ Recursive rendering (optional)
□ Velocity preservation
□ Sound considerations
□ Performance optimization
```

---

## Visual Reference

### Portal Rendering States

```
State 1: No Portals
┌────────────────────┐
│                    │
│   Normal Scene     │
│                    │
└────────────────────┘

State 2: One Portal (Blue placed)
┌────────────────────┐
│                    │
│  [BLUE]  ← Shows   │
│          static    │
│          pattern   │
└────────────────────┘

State 3: Both Portals (linked)
┌────────────────────┐
│                    │
│  [BLUE]  ← Shows   │
│          view from │
│          ORANGE    │
└────────────────────┘
```

### The Complete Pipeline

```
┌─────────────────────────────────────────────────────────────┐
│                     PORTAL RENDERING PIPELINE               │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│  1. RAYCAST                                                 │
│     │                                                       │
│     ├── Hit wall? → Draw texture (normal)                   │
│     │                                                       │
│     └── Hit portal? ─┬── Other portal not placed?           │
│                      │   → Draw static pattern              │
│                      │                                      │
│                      └── Both portals active?               │
│                          → Transform ray to other portal    │
│                          → Continue raycast                 │
│                          → Draw what transformed ray sees   │
│                                                             │
│  2. PLAYER UPDATE                                           │
│     │                                                       │
│     └── Crossed portal plane?                               │
│         → Transform position                                │
│         → Transform direction                               │
│         → Transform velocity                                │
│                                                             │
└─────────────────────────────────────────────────────────────┘
```

---

## Closing Thoughts

### Why This Is Worth It

Implementing portals demonstrates mastery of:
- **3D mathematics**: Transformations, rotations, coordinate systems
- **Rendering pipelines**: Understanding what happens per-pixel
- **Game physics**: Velocity, momentum, collision
- **Creative problem solving**: Non-Euclidean geometry in games

### The Payoff

A working portal system would be:
- **Visually stunning** in a raycaster
- **Unique** among 42 cub3d projects
- **Educational** for understanding graphics
- **Fun** to play with!

### Sleep On It

The key concepts to internalize:
1. Portals are **paired doorways** in space
2. Looking through transforms **viewpoint**
3. Walking through transforms **position + direction**
4. The math is rotations and translations

---

*"Now you're thinking with portals."* - GLaDOS

---

**Document Version**: 1.0  
**Created**: February 23, 2026  
**Status**: Design Phase - No Implementation Yet
