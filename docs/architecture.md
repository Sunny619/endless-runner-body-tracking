# CyberRunner 3D — System Architecture & Mathematical Specifications

## 1. Architectural Overview

CyberRunner 3D is designed as a decoupled, multi-threaded, real-time Human-Computer Interaction (HCI) software platform. It bridges optical computer vision landmark estimation with an interactive 3D graphics simulation pipeline.

```
+-------------------------------------------------------------------------------+
|                       CyberRunner 3D System Architecture                      |
+-------------------------------------------------------------------------------+
|  +-------------------------------------+   +-------------------------------+  |
|  |     AI Computer Vision Subsystem    |   |     3D OpenGL Game Engine     |  |
|  |  [RGB Webcam 640x480 @ 30-60 FPS]   |   |   [OpenGL Fixed/Prog Pipeline]|  |
|  |                 |                   |   |                 |             |  |
|  |  [MediaPipe Pose Inference (33 LM)] |   |   [GameEngine Master State]   |  |
|  |                 |                   |   |       |             |         |  |
|  |  [EMA Jitter Filter (alpha=0.65)]   |   |   [Player]      [Camera]      |  |
|  |                 |                   |   |   (Kinematics)  (Frustum 3D)  |  |
|  |  [Gesture Finite State Machine]     |   |       |             |         |  |
|  |  (Boundary Hysteresis Corridors)    |   |  [ObstacleMgr]  [Renderer]    |  |
|  |                 |                   |   |  (AABB Bounds)  (Lights/Mats) |  |
|  |  [Virtual Input Bridge (PyAutoGUI)] |   |       |             |         |  |
|  +-----------------|-------------------+   |   [InputRouter] [2D HUD Text] |  |
|                    |                       +-------------------------------+  |
|                    +====== OS Event Queue (Low-Latency IPC) ======>           |
+-------------------------------------------------------------------------------+
```

---

## 2. 3D Graphics Simulation Engine (C++17 / OpenGL)

### 2.1 Coordinate System & Camera Tracking
- **World Space Coordinates**: Right-handed Cartesian coordinate system:
  - $+X$: Right lateral corridor direction
  - $-X$: Left lateral corridor direction
  - $+Y$: Upwards vertical axis
  - $-Z$: Forward progress trajectory along the infinite track
- **Projection Matrix**: Symmetric perspective frustum projection defined via:
  $$\text{Frustum}(x_{\text{min}} = -2.0, x_{\text{max}} = 2.0, y_{\text{min}} = -2.0, y_{\text{max}} = 2.0, z_{\text{near}} = 1.0, z_{\text{far}} = 10.0)$$
- **Dynamic 3rd-Person Follow Camera**:
  $$\vec{E}(t) = \begin{bmatrix} X_{\text{player}}(t) + \delta_X^{\text{shake}}(t) \\ Y_{\text{player}}(t) + 1.0 + \delta_Y^{\text{shake}}(t) \\ Z_{\text{player}}(t) + 1.5 \end{bmatrix}, \quad \vec{T}(t) = \begin{bmatrix} X_{\text{player}}(t) \\ Y_{\text{player}}(t) \\ Z_{\text{player}}(t) \end{bmatrix}, \quad \vec{U} = \begin{bmatrix} 0 \\ 1 \\ 0 \end{bmatrix}$$
  The view matrix is computed via $\text{gluLookAt}(\vec{E}, \vec{T}, \vec{U})$.

### 2.2 Player Kinematics & Jump Mechanics
- **Lateral Lane Transitions**:
  Discrete 3-lane discretization:
  $$X_{\text{lane}} \in \{-1.4, 0.0, 1.4\}$$
  Smooth lateral interpolation prevents visual jarring:
  $$X_t = X_{t-1} + (X_{\text{target}} - X_{t-1}) \cdot \beta, \quad \beta = 0.35$$
- **Sinusoidal Jump Trajectory**:
  Vertical displacement is governed by angular phase $\theta_t \in [0, \pi]$:
  $$Y(t) = Y_{\text{ground}} + \sin(\theta_t), \quad \theta_t = \theta_{t-1} + \Delta\theta, \quad \Delta\theta = 3^\circ$$
  When $\theta_t \ge 180^\circ$, the player clamps back to $Y_{\text{ground}} = 0.35$ and resets the jump state.

### 2.3 Lighting & Material Models
The engine employs standard Phong illumination with ambient, diffuse, and specular components:
$$I = I_a k_a + I_d k_d (\vec{L} \cdot \vec{N}) + I_s k_s (\vec{R} \cdot \vec{V})^n$$
- **Floor Material**: Matte grey concrete ($k_a = 0.6, k_d = 0.6, k_s = 0.7, n = 50.0$).
- **Player Material**: Polished white avatar ($k_a = 1.0, k_d = 0.9, k_s = 0.9, n = 80.0$).
- **Hazard Material**: High-contrast hazard red ($k_a = 0.8, k_d = 0.8, k_s = 0.6, n = 80.0$).

---

## 3. Computer Vision Pipeline & Optical Tracking (Python)

### 3.1 MediaPipe 33-Landmark Extraction
The camera captures RGB frames $I \in \mathbb{R}^{H \times W \times 3}$. MediaPipe Pose predicts normalized 3D coordinates $(x_i, y_i, z_i) \in [0, 1]$ for 33 skeletal landmarks.
- **Torso Midpoint Computation**:
  $$X_{\text{torso}} = \frac{x_{11} + x_{12}}{2.0}$$
  $$Y_{\text{torso}} = \frac{x_{11} + x_{24}}{2.0}$$
  Where $11$ is Left Shoulder, $12$ is Right Shoulder, and $24$ is Right Hip.

### 3.2 Exponential Moving Average (EMA) Jitter Reduction
Sensor noise from optical webcams is eliminated using a single-pole IIR low-pass filter:
$$\hat{S}_t = \alpha S_t + (1 - \alpha) \hat{S}_{t-1}$$
With $\alpha = 0.65$, step responses settle within 3 frames while cutting high-frequency webcam sensor variance by $>80\%$.

---

## 4. Latency Budget & Dispatch Timing

| Pipeline Stage | Timing Budget | Target Hardware |
| :--- | :--- | :--- |
| **Optical Video Capture** | 8.0 ms – 16.6 ms | Standard USB / Integrated Webcam |
| **MediaPipe Pose Inference** | 8.0 ms – 14.0 ms | CPU / GPU acceleration |
| **EMA Filtering & FSM** | < 0.2 ms | Native Python |
| **Virtual Input Bridge (IPC)** | < 1.0 ms | OS Event Queue |
| **OpenGL Frame Render** | < 2.0 ms | Dedicated / Integrated GPU |
| **Total End-to-End Latency** | **< 35 ms** | **Sub-perceptual for 60 FPS gameplay** |
