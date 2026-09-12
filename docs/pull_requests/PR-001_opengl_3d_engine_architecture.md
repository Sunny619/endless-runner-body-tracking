# PR #001: Core 3D OpenGL Engine Architecture & Modularization

- **Author**: Graphics & Engine Team
- **Type**: Feature / Architecture
- **Status**: Merged into `develop`

---

## 📌 Description
Refactors the initial monolithic OpenGL rendering loop into a decoupled, object-oriented C++17 architecture:
- `include/GameConfig.hpp` & `src/GameConfig.cpp`: Centralizes lane metrics (`-1.4, 0.0, 1.4`), lighting presets, and physics speeds.
- `include/Player.hpp` & `src/Player.cpp`: Encapsulates 3-lane interpolation, sinusoidal jump arc calculations ($y(t) = y_0 + \sin(\theta)$), and immunity counters.
- `include/Camera.hpp` & `src/Camera.cpp`: Implements 3rd-person follow-camera with dynamic view matrix update and screen shake dampening on impact.
- `include/ObstacleManager.hpp` & `src/ObstacleManager.cpp`: Implements procedural multi-lane obstacle hazard generation across 3 intervals.
- `include/Renderer.hpp` & `src/Renderer.cpp`: Manages OpenGL materials, Phong lighting, stadium architecture, spectator crowd animation, and 2D text HUD rasterization.
- `include/GameEngine.hpp` & `src/GameEngine.cpp`: Coordinates the master game loop, GLUT event wrappers, scoring, and state lifecycle.

---

## 🧪 Validation & Testing
- Verified 60 FPS rendering under OpenGL double-buffering.
- Verified smooth lane transition clamping and ground collision clearance during jumps.
