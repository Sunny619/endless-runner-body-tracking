# CyberRunner 3D — Build, Run & Execution Instructions (`run_instruction.md`)

> **Repository**: [Sunny619/endless-runner-body-tracking](https://github.com/Sunny619/endless-runner-body-tracking)  
> **Application Type**: Real-Time Human-Computer Interaction (HCI) Gaming Platform (C++17 OpenGL 3D Game + Python MediaPipe AI Vision Controller)  
> **Target Audience**: Code Evaluators, Systems Engineers, and HCI Researchers  

This document provides definitive, reproducible step-by-step instructions to compile, configure, run, and verify the **CyberRunner 3D** end-to-end system from scratch across Windows, Linux, and macOS.

---

## 📋 System Prerequisites

Ensure the following runtimes and compilers are available on your development workstation:

| Component | Minimum Version | Recommended Version | Verification Command | Notes |
| :--- | :--- | :--- | :--- | :--- |
| **C++ Compiler** | C++17 compliant | GCC 9+, Clang 11+, or MSVC 2019+ | `g++ --version` / `clang++ --version` | Standard C++17 support |
| **CMake** | `>= 3.16.0` | `3.22.x` or newer | `cmake --version` | Optional if using direct script/Make |
| **Python** | `>= 3.9.0` | `3.10.x` – `3.12.x` | `python --version` | Required for AI Vision Controller |
| **OpenGL** | Core 2.1+ | 3.3+ Compatible Driver | OS Graphics Driver | Native on Windows/Linux/macOS |
| **GLUT / FreeGLUT** | FreeGLUT 3.0+ | Native GLUT (macOS) / FreeGLUT (Win/Linux) | Package manager / Library | Header `<GL/glut.h>` or `<GLUT/glut.h>` |
| **Webcam** | USB / Integrated | 720p @ 30 FPS | OS Device Manager | Required for optical gesture control |

### Platform-Specific Dependency Installation

#### Windows (MinGW or MSVC)
```powershell
# If using MSYS2 / MinGW:
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-freeglut mingw-w64-x86_64-cmake

# Or install Visual Studio with "Desktop development with C++" and vcpkg:
# vcpkg install freeglut
```

#### Linux (Ubuntu / Debian)
```bash
sudo apt update
sudo apt install -y build-essential cmake freeglut3-dev libgl1-mesa-dev libglu1-mesa-dev python3-pip python3-venv
```

#### macOS
```bash
# macOS includes OpenGL and GLUT frameworks out-of-the-box in the Xcode command line tools:
xcode-select --install
```

---

## ⚡ Option A: Quickstart in Under 2 Minutes

### 1. Build the C++ 3D Game Engine
```bash
# On Windows:
.\build.bat

# On Linux / macOS:
chmod +x build.sh
./build.sh
```

### 2. Set Up Python Environment & Dependencies
```bash
# Create isolated virtual environment
python -m venv .venv

# Activate virtual environment
# Windows:
.\.venv\Scripts\activate
# Linux / macOS:
source .venv/bin/activate

# Install required packages
pip install -r requirements.txt
```

---

## 🛠️ Option B: Step-by-Step Manual Compilation

### Step 1: Compile the C++ 3D Game Engine via CMake

```bash
# 1. Create and enter build directory
mkdir build
cd build

# 2. Configure build with CMake
cmake .. -DCMAKE_BUILD_TYPE=Release

# 3. Build executable
cmake --build . --config Release

# The compiled binary will be located in:
# Windows: bin/cyber_runner.exe or build/Release/CyberRunner.exe
# Linux / macOS: bin/CyberRunner or build/CyberRunner
cd ..
```

*(Alternatively, on Linux/macOS you can directly run `make all` from the project root).*

---

## 🚀 Running the Dual-Process Application

CyberRunner 3D operates as a synchronized dual-process system:
- **Process 1**: The C++ OpenGL 3D Game rendering the gameplay at high framerates.
- **Process 2**: The Python AI Vision Controller capturing webcam input, analyzing body poses, and injecting low-latency virtual movement commands into the game window.

```
+---------------------------+                +---------------------------+
|  Process 2: AI Vision     |  Virtual Keys  |  Process 1: 3D Game       |
|  Camera -> MediaPipe Pose | -------------> |  OpenGL Renderer          |
|  Gesture State Machine    |  ('s', Arrows) |  60 FPS Game Simulation   |
+---------------------------+                +---------------------------+
```

### Step 1: Launch the C++ 3D Game

Open Terminal 1 and execute the compiled binary:

```bash
# Windows
.\bin\cyber_runner.exe

# Linux / macOS
./bin/CyberRunner
```

The game window will open titled **"CyberRunner 3D - AI Vision Endless Runner"**, displaying the 3D track, stadium stairs, spectators, and the initial prompt:
> `"Press 's' or Raise your right hand to start"`

*(Note: The game can also be played entirely with keyboard controls: `Left/Right Arrows` or `A/D` to switch lanes, `Up Arrow` or `W` or `Space` to jump, `P` to pause, `Q` or `ESC` to quit).*

---

### Step 2: Launch the AI Vision Motion Controller

Open Terminal 2 (with your virtual environment active) and run:

```bash
python -m ai_controller.main
```

#### Optional CLI Arguments:
- `--camera-id 0`: Specifies webcam device index (default: `0`).
- `--width 640 --height 480`: Sets capture resolution (default: `640x480`).
- `--no-gui`: Runs the controller in headless mode without rendering the camera preview window.
- `--mock-input`: Runs gesture recognition and logs actions without dispatching real OS keypresses (useful for automated testing).

---

## 🧘 Gesture Calibration & Gameplay Mechanics

Once both processes are running, position yourself centered in front of the webcam approximately **1.5 to 2.5 meters (5 to 8 feet)** away, ensuring your torso and upper body are clearly visible.

```
       [Top Boundary Line (Jump / Hand Raise)]
  ------------------------------------------------
   LEFT ZONE  |       CENTER DEADZONE      | RIGHT ZONE
              |                            |
  ------------------------------------------------
       [Bottom Boundary Line (Crouch / Duck)]
```

| In-Game Maneuver | Biomechanical Action | Optical Landmark Trigger |
| :--- | :--- | :--- |
| **Start / Restart** | **Raise Right Hand** | Elevate right wrist (Landmark 16) above the top guide line. Dispatches `'s'`. |
| **Shift Left Lane** | **Lean Torso to Left** | Shift body laterally to your left. Crosses the left guide line. Dispatches `Left Arrow`. |
| **Shift Right Lane** | **Lean Torso to Right**| Shift body laterally to your right. Crosses the right guide line. Dispatches `Right Arrow`. |
| **Center Recovery** | **Return to Center** | Return body into the central deadzone box. Dispatches compensatory recenter key. |
| **Jump Obstacle** | **Physical Jump / Crouch Extension** | Rapidly elevate shoulders/torso above top guide line. Dispatches `Up Arrow`. |
| **Duck / Crouch** | **Squat / Crouch** | Lower torso below bottom guide line. Dispatches `Down Arrow`. |

---

## 🧪 Automated Verification & Unit Test Suite

The repository includes comprehensive automated tests covering gesture classification math, boundary hysteresis, landmark normalization, and physics trajectories:

```bash
# Run all unit tests
python -m unittest discover -s tests -p "test_*.py" -v
```

### Test Suite Coverage:
1. **`tests/test_gesture_classifier.py`**:
   - `test_initial_state_is_idle`: Verifies default state machine initialization.
   - `test_hand_raise_starts_game`: Verifies hand elevation trigger mechanics.
   - `test_lean_left_detection`: Validates mirrored coordinate translation for left lane changes.
   - `test_lean_right_detection`: Validates mirrored coordinate translation for right lane changes.
   - `test_return_to_center_from_left`: Verifies hysteresis return-to-center dispatch.
   - `test_jump_elevation_detection`: Verifies vertical jump threshold triggers.
   - `test_crouch_detection`: Verifies squat/duck detection.
2. **`tests/test_pose_detector.py`**:
   - `test_keypoints_namedtuple_instantiation`: Validates keypoint data structures.
   - `test_ema_smoothing_convergence`: Verifies Exponential Moving Average filter jitter reduction.
3. **`tests/test_game_physics.py`**:
   - `test_jump_trajectory_kinematics`: Verifies sinusoidal jump trajectory ($y = y_0 + \sin(\theta)$).
   - `test_discrete_lane_boundaries`: Verifies symmetric 3-lane metric offsets (`-1.4, 0.0, 1.4`).
   - `test_aabb_collision_clearance`: Verifies AABB collision queries and jump clearance math.

---

## 🔧 Troubleshooting Matrix

| Issue | Root Cause | Solution |
| :--- | :--- | :--- |
| **"Cannot open camera index 0"** | Webcam in use by another app or device index mismatch | Close applications using the camera (Zoom, Teams, Browser) or specify `--camera-id 1`. |
| **"fatal error: GL/glut.h: No such file"** | Missing FreeGLUT development headers on Linux | Run `sudo apt install freeglut3-dev libgl1-mesa-dev libglu1-mesa-dev`. |
| **"undefined reference to `glutInit`"** | Linker cannot find FreeGLUT library | Ensure `-lglut -lGL -lGLU` are passed to compiler, or build via `CMakeLists.txt`. |
| **Virtual keypresses not registering in game** | Game window does not have OS window focus | Click inside the **CyberRunner 3D** game window to ensure it is the active focused window. |
| **PyAutoGUI fails on Linux (Wayland)** | Wayland restricts synthetic input injection | Switch session to **X11** (Xorg) at login screen, or run with `xdotool` permissions. |
| **macOS Accessibility Prompt** | macOS requires Accessibility permission for synthetic input | Go to **System Preferences -> Security & Privacy -> Accessibility** and grant permission to Terminal / Python. |
