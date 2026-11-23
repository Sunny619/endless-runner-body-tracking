@echo off
REM ==============================================================================
REM CyberRunner 3D - Windows Automated Build Script
REM Builds C++ 3D Game Engine using CMake or MSVC/MinGW
REM ==============================================================================

echo [INFO] Starting CyberRunner 3D Build Process...

where cmake >nul 2>nul
if %ERRORLEVEL% EQU 0 (
    echo [INFO] CMake detected. Configuring build system...
    if not exist build mkdir build
    cd build
    cmake ..
    if %ERRORLEVEL% NEQ 0 (
        echo [ERROR] CMake configuration failed.
        cd ..
        exit /b 1
    )
    cmake --build . --config Release
    if %ERRORLEVEL% NEQ 0 (
        echo [ERROR] CMake compilation failed.
        cd ..
        exit /b 1
    )
    cd ..
    echo [SUCCESS] Build completed successfully via CMake.
    exit /b 0
)

where g++ >nul 2>nul
if %ERRORLEVEL% EQU 0 (
    echo [INFO] MinGW g++ detected. Compiling directly...
    if not exist bin mkdir bin
    g++ -std=c++17 -O2 -Iinclude src\GameConfig.cpp src\Camera.cpp src\Player.cpp src\ObstacleManager.cpp src\Renderer.cpp src\InputHandler.cpp src\GameEngine.cpp src\main.cpp -lopengl32 -lglu32 -lfreeglut -o bin\cyber_runner.exe
    if %ERRORLEVEL% NEQ 0 (
        echo [ERROR] Compilation failed.
        exit /b 1
    )
    echo [SUCCESS] Binary generated at bin\cyber_runner.exe
    exit /b 0
)

echo [WARNING] Neither CMake nor g++ was found in PATH.
echo [INFO] Please install CMake, Visual Studio Build Tools, or MinGW to compile the C++ engine.
exit /b 1
