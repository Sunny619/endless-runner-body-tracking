#!/usr/bin/env bash
# ==============================================================================
# CyberRunner 3D - Unix/macOS/Linux Automated Build Script
# ==============================================================================

set -e

echo "[INFO] Detecting build environment..."

if command -v cmake &> /dev/null; then
    echo "[INFO] CMake detected. Building modular engine..."
    mkdir -p build
    cd build
    cmake ..
    cmake --build . --config Release
    cd ..
    echo "[SUCCESS] Build finished via CMake."
    exit 0
fi

if command -v make &> /dev/null; then
    echo "[INFO] Make detected. Executing Makefile..."
    make all
    echo "[SUCCESS] Build finished via Make."
    exit 0
fi

echo "[ERROR] Neither CMake nor Make was found in PATH."
exit 1
