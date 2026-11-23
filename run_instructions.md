# CyberRunner 3D — Build, Run & Execution Instructions (`run_instructions.md`)

> **Repository**: [Sunny619/endless-runner-body-tracking](https://github.com/Sunny619/endless-runner-body-tracking)  
> **Application**: Real-Time AI Body-Tracking Endless Runner (C++ OpenGL 3D Game + Python MediaPipe Pose Controller)  
> **Target Audience**: Evaluators, Developers, and Human-Computer Interaction (HCI) Researchers  

This document provides exhaustive, reproducible step-by-step instructions to compile, configure, execute, calibrate, and verify the **CyberRunner 3D** dual-process gaming system across Windows, Linux, and macOS.

---

## 📋 System Prerequisites

Before building and running the project, verify that your development environment satisfies the following requirements:

### 1. Hardware Requirements
| Component | Minimum Specification | Recommended Specification |
| :--- | :--- | :--- |
| **Webcam** | USB or integrated RGB webcam (640x480 @ 30 FPS) | 720p/1080p @ 30–60 FPS with good lighting |
| **Processor** | Dual-core x86_64 / Apple Silicon | Quad-core 2.4 GHz+ |
| **Graphics** | OpenGL 2.1 compatible GPU / integrated graphics | Dedicated GPU with OpenGL 3.3+ support |
| **RAM** | 4 GB | 8 GB or higher |

### 2. Software Requirements
| Software | Minimum Version | Verification Command | Notes |
| :--- | :--- | :--- | :--- |
| **C++ Compiler** | C++11 / C++17 compliant | `g++ --version` or `clang++ --version` | GCC 8+, Clang 10+, or MSVC 2019+ |
| **OpenGL & GLUT** | OpenGL 2.1+ & FreeGLUT 3.0+ | Check system packages | GLUT framework on macOS |
| **Python** | Python 3.8 – 3.12 | `python --version` / `python3 --version` | Required for AI pose tracker |
| **Pip** | Compatible with Python | `pip --version` | Python package manager |

---

## 📦 Platform-Specific Dependency Installation

### A. Windows (MinGW / MSYS2 or MSVC)

#### Option 1: MinGW via MSYS2 (Recommended for simple CLI builds)
1. Open the **MSYS2 MinGW 64-bit** shell and install GCC and FreeGLUT:
   ```bash
   pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-freeglut
   ```
2. Ensure `C:\msys64\mingw64\bin` is added to your system `PATH`.

#### Option 2: Precompiled FreeGLUT for MinGW / Visual Studio
If using standalone MinGW:
1. Download the FreeGLUT MinGW package from [freeglut.sourceforge.net](https://freeglut.sourceforge.net/) or Martin Payne's FreeGLUT Windows binaries.
2. Copy the `include/GL` headers to your compiler's include directory (e.g. `C:\mingw64\include\GL`).
3. Copy `libfreeglut.a` (or `freeglut.lib`) to your library directory (`C:\mingw64\lib`).
4. Place `freeglut.dll` in the same directory as the built executable or into `C:\Windows\System32`.

### B. Linux (Ubuntu / Debian / Linux Mint)

Install the standard build tools, OpenGL libraries, FreeGLUT development headers, and Python packages:
```bash
sudo apt update
sudo apt install -y build-essential freeglut3-dev libgl1-mesa-dev libglu1-mesa-dev python3-pip python3-venv
```

*(On Fedora / Red Hat: `sudo dnf install gcc-c++ freeglut-devel mesa-libGL-devel mesa-libGLU-devel python3-pip`)*  
*(On Arch Linux: `sudo pacman -S base-devel freeglut mesa glu python-pip`)*

### C. macOS

macOS includes native OpenGL and GLUT frameworks directly within Xcode Command Line Tools:
```bash
# Install Xcode Command Line Tools if not already present
xcode-select --install
```

> [!NOTE]
> On macOS, `main.cpp` includes `<GLUT/glut.h>`, which compiles cleanly against Apple's native `-framework OpenGL -framework GLUT`. On Linux and Windows, `<GL/glut.h>` is typically used. If your environment expects `<GL/glut.h>`, ensure your include path points to the FreeGLUT directory or update the include directive accordingly.

---

## 🛠️ Step-by-Step Compilation Guide

CyberRunner 3D consists of two standalone components:
- `main.cpp`: The 3D OpenGL endless runner simulation.
- `main.py`: The Python MediaPipe optical pose tracking controller.

### 1. Compiling `main.cpp` (The 3D Game Engine)

#### On Windows (MinGW GCC):
```powershell
# Open PowerShell or Command Prompt in the repository root:
g++ -std=c++17 -O2 main.cpp -lopengl32 -lglu32 -lfreeglut -o cyber_runner.exe
```

#### On Linux (GCC):
```bash
g++ -std=c++17 -O2 main.cpp -lGL -lGLU -lglut -o cyber_runner
```

#### On macOS (Clang with Native Frameworks):
```bash
clang++ -std=c++17 -O2 main.cpp -framework OpenGL -framework GLUT -o cyber_runner
```

Once compiled, you will have the standalone binary:
- Windows: `cyber_runner.exe`
- Linux / macOS: `cyber_runner`

---

## 🐍 Python Environment & Dependencies Setup

The AI Vision Controller requires Python dependencies: OpenCV for video capture and preview, Google MediaPipe for skeletal landmark inference, and PyAutoGUI for OS-level virtual key injection.

### 1. Create a Python Virtual Environment

It is strongly recommended to isolate dependencies inside a virtual environment:

```bash
# In the repository root:
python -m venv .venv
```

### 2. Activate the Virtual Environment

- **On Windows (PowerShell)**:
  ```powershell
  .\.venv\Scripts\Activate.ps1
  ```
  *(If execution of scripts is restricted in PowerShell, run `Set-ExecutionPolicy -Scope Process -ExecutionPolicy Bypass` first).*

- **On Windows (Command Prompt)**:
  ```cmd
  .\.venv\Scripts\activate.bat
  ```

- **On Linux / macOS (Bash / Zsh)**:
  ```bash
  source .venv/bin/activate
  ```

### 3. Install Required Packages

```bash
pip install --upgrade pip
pip install opencv-python mediapipe pyautogui
```

Verify that the packages import properly:
```bash
python -c "import cv2, mediapipe, pyautogui; print('All dependencies successfully verified!')"
```

---

## 🚀 Running the Dual-Process System

CyberRunner 3D is engineered as a coordinated dual-process architecture:

```
+---------------------------------+                +---------------------------------+
|   Process 2: AI Vision Tracker  |                |     Process 1: 3D Simulation    |
|        (python main.py)         |                |       (./cyber_runner)          |
|                                 |  Virtual Keys  |                                 |
|  - Webcam video capture         | -------------> |  - 3D Track & Obstacle Renderer |
|  - MediaPipe 33-pose inference  | ('s', Arrows)  |  - Kinematics & Jump Physics    |
|  - Boundary box classifier      |                |  - Dynamic Lights & Spectators  |
|  - Debounced key injection      |                |  - Real-Time Score & Lives HUD  |
+---------------------------------+                +---------------------------------+
```

### Step 1: Launch the 3D Game Engine (Terminal 1)

Open your first terminal window and launch the compiled binary:

```bash
# On Windows:
.\cyber_runner.exe

# On Linux / macOS:
./cyber_runner
```

The game window will appear with a 750x750 display showing the 3D track, stadium stairs, spectators, and the prompt:
> **"Press 's' or Raise your right hand to start"**

### Step 2: Launch the AI Optical Vision Controller (Terminal 2)

Open a second terminal window (with the virtual environment activated) and run:

```bash
python main.py
```

The webcam preview window titled **"MediaPipe Pose"** will open, showing your camera stream overlaid with green calibration guide boxes and skeletal landmarks.

> [!IMPORTANT]
> **Window Focus Requirement**: PyAutoGUI injects simulated keystrokes into whichever window currently holds active OS focus. After launching `main.py`, **click once inside the 3D Game window** so that it receives the keystrokes!

---

## 🧘 Optical Calibration & Gameplay Mechanics

### 1. Physical Camera Setup & Posture
- **Positioning**: Place your webcam at chest/head level.
- **Distance**: Stand approximately **1.5 to 2.5 meters (5 to 8 feet)** away from the camera.
- **Framing**: Ensure your head, shoulders, torso, hips, and arms are clearly visible within the camera frame.
- **Lighting**: Adequate, even front lighting is recommended for reliable skeletal keypoint tracking.

### 2. On-Screen Calibration Grid
When `main.py` is running, the webcam feed displays guide lines defined in normalized coordinates:

```
                  [ Top Guide: y = 0.4 (Jump & Start Zone) ]
   ------------------------------------------------------------------------
    LEFT ZONE (x > 0.6)  |    CENTER DEADZONE (0.4 <= x <= 0.6)    | RIGHT ZONE (x < 0.4)
                         |                                         |
   ------------------------------------------------------------------------
                 [ Bottom Guide: y = 0.6 (Crouch / Duck Zone) ]
```

*(Note: The camera preview is horizontally mirrored for an intuitive selfie-view).*

### 3. Biomechanical Controls Reference

| Maneuver | Biomechanical Action | Optical Landmark Trigger | Virtual Key Injected |
| :--- | :--- | :--- | :---: |
| **Start / Restart** | **Raise Right Hand** | Raise right wrist above upper guide line ($y < 0.4$) | `'s'` |
| **Shift Left Lane** | **Lean Torso to Left** | Lean your upper body to your left (camera $x > 0.6$) | `Left Arrow` |
| **Shift Right Lane** | **Lean Torso to Right** | Lean your upper body to your right (camera $x < 0.4$) | `Right Arrow` |
| **Return to Center** | **Stand Centered** | Return torso into the center box ($0.4 \le x \le 0.6$) | Compensatory key |
| **Jump Obstacle** | **Jump / Stand Up Tall** | Rapidly elevate torso above top guide line ($y < 0.4$) | `Up Arrow` |
| **Crouch / Duck** | **Squat / Crouch** | Drop torso below bottom guide line ($y > 0.6$) | `Down Arrow` |

### 4. Manual Keyboard Controls
If you wish to test the game without a webcam:
- `'s'`: Start game
- `Left Arrow` / `Right Arrow`: Shift lanes left and right
- `Up Arrow`: Jump over obstacles
- `ESC`: Exit the application

---

## 🔧 Comprehensive Troubleshooting & FAQ

| Problem / Error | Underlying Cause | Verified Solution |
| :--- | :--- | :--- |
| **`cv2.error: Cannot open camera 0`** | Webcam is unplugged, in use by another app, or indexed at a different ID | 1. Close apps using the webcam (Zoom, Teams, Discord, browser).<br>2. Edit `main.py` and change `cv2.VideoCapture(0)` to index `1` or `2`. |
| **`fatal error: GL/glut.h: No such file or directory`** | Missing FreeGLUT development packages on Linux/Windows | • On Linux: `sudo apt install freeglut3-dev`<br>• On Windows: Install FreeGLUT via MSYS2 `pacman -S mingw-w64-x86_64-freeglut`. |
| **`fatal error: GLUT/glut.h: No such file or directory`** | Building on Windows/Linux with macOS-style include directive | In `main.cpp`, change `#include<GLUT/glut.h>` to `#include<GL/glut.h>` for Windows/Linux. |
| **Virtual keypresses do not move the player** | 3D Game window does not have operating system focus | Click inside the **CyberRunner** 3D Game window to make it active so PyAutoGUI keys reach it. |
| **PyAutoGUI fails or crashes on Linux** | Wayland display server prevents virtual input injection | Log out and select **"Ubuntu on Xorg"** (X11 session) at the display login screen. |
| **macOS Accessibility Permission Dialog** | macOS blocks synthetic input injection without explicit permissions | Open **System Settings -> Privacy & Security -> Accessibility**, and add/enable Terminal (or iTerm2 / Python). |
| **In-game lane jumping feels erratic** | Camera positioning too close or lighting casting harsh shadows | Step back to 2 meters, ensure your entire torso is within view, and use front-facing light. |
| **Camera video preview lags** | High resolution capture or high CPU usage | Ensure Python runs with hardware acceleration enabled, and keep background heavy processes closed. |
