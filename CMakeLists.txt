cmake_minimum_required(VERSION 3.16)
project(CyberRunner3D VERSION 2.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable compiler warnings
if(MSVC)
    add_compile_options(/W4 /EHsc)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Find OpenGL
find_package(OpenGL REQUIRED)

# Include header directory
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Platform specific GLUT library linking
if(APPLE)
    find_library(GLUT_FRAMEWORK GLUT REQUIRED)
    set(GRAPHICS_LIBRARIES ${OPENGL_LIBRARIES} ${GLUT_FRAMEWORK})
elseif(WIN32)
    find_package(GLUT QUIET)
    if(GLUT_FOUND)
        set(GRAPHICS_LIBRARIES OpenGL::GL GLUT::GLUT)
    else()
        # Fallback to direct library names for MinGW / MSVC
        set(GRAPHICS_LIBRARIES opengl32 glu32 freeglut)
    endif()
else() # Linux / Unix
    find_package(GLUT REQUIRED)
    set(GRAPHICS_LIBRARIES OpenGL::GL GLUT::GLUT)
endif()

# 1. Main Modular Engine Target
file(GLOB_RECURSE ENGINE_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_executable(CyberRunner ${ENGINE_SOURCES})
target_include_directories(CyberRunner PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(CyberRunner PRIVATE ${GRAPHICS_LIBRARIES})

# 2. Legacy Standalone Target (Backwards Compatibility)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")
    add_executable(CyberRunnerLegacy "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")
    target_link_libraries(CyberRunnerLegacy PRIVATE ${GRAPHICS_LIBRARIES})
endif()

# Install targets
install(TARGETS CyberRunner DESTINATION bin)
