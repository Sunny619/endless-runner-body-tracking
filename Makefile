# Universal Makefile for CyberRunner 3D Engine
# Compatible with Linux (GCC/Clang) and macOS

CXX ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -O2 -Iinclude
BIN_DIR = bin
BUILD_DIR = build

# Detect operating system
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S), Darwin)
    # macOS framework flags
    LDFLAGS = -framework OpenGL -framework GLUT
else
    # Linux / Unix flags
    LDFLAGS = -lGL -lGLU -lglut -lm
endif

SOURCES = src/GameConfig.cpp \
          src/Camera.cpp \
          src/Player.cpp \
          src/ObstacleManager.cpp \
          src/Renderer.cpp \
          src/InputHandler.cpp \
          src/GameEngine.cpp \
          src/main.cpp

OBJECTS = $(patsubst src/%.cpp, $(BUILD_DIR)/%.o, $(SOURCES))
TARGET = $(BIN_DIR)/cyber_runner

.PHONY: all clean legacy dirs

all: dirs $(TARGET)

dirs:
	@mkdir -p $(BIN_DIR) $(BUILD_DIR)

$(BUILD_DIR)/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $@
	@echo "Build successful: $(TARGET)"

legacy: dirs
	$(CXX) $(CXXFLAGS) main.cpp $(LDFLAGS) -o $(BIN_DIR)/cyber_runner_legacy
	@echo "Legacy target built: $(BIN_DIR)/cyber_runner_legacy"

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
	@echo "Cleaned build artifacts."
