#ifndef GAME_CONFIG_HPP
#define GAME_CONFIG_HPP

#if defined(__APPLE__)
    #define GL_SILENCE_DEPRECATION
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif

#include <string>

namespace Engine {

struct MaterialProperties {
    GLfloat ambient[4];
    GLfloat diffuse[4];
    GLfloat specular[4];
    GLfloat shininess[1];
};

struct LightSource {
    GLfloat position[4];
    GLfloat diffuse[4];
    GLfloat ambient[4];
    GLfloat specular[4];
    GLfloat spotDirection[3];
    GLfloat spotCutoff;
    GLfloat spotExponent;
};

class GameConfig {
public:
    // Window settings
    static constexpr unsigned int SCREEN_WIDTH = 750;
    static constexpr unsigned int SCREEN_HEIGHT = 750;
    static const char* WINDOW_TITLE;

    // Track and layout metrics
    static constexpr float MAP_LENGTH = 400.0f;
    static constexpr float LANE_WIDTH = 1.4f;
    static constexpr float LANE_POS_LEFT = -1.4f;
    static constexpr float LANE_POS_CENTER = 0.0f;
    static constexpr float LANE_POS_RIGHT = 1.4f;

    // Physics parameters
    static constexpr float PLAYER_DEFAULT_Y = 0.35f;
    static constexpr float FORWARD_VELOCITY = 0.1f;
    static constexpr float JUMP_STEP_DEG = 3.0f;
    static constexpr float COLLISION_RADIUS = 0.55f;
    static constexpr int DEFAULT_IMMUNITY_TICKS = 7;
    static constexpr int MAX_LIVES = 3;

    // Obstacle arrays
    static constexpr int MAX_OBSTACLES = 100;

    // Material definitions
    static MaterialProperties FloorMaterial;
    static MaterialProperties PlayerMaterial;
    static MaterialProperties EnemyMaterial;

    // Lighting configurations
    static LightSource SceneLight0;
    static LightSource SpotLight1;

    static void InitializeDefaults();
};

} // namespace Engine

#endif // GAME_CONFIG_HPP
