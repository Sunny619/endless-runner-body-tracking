#include "GameConfig.hpp"

namespace Engine {

const char* GameConfig::WINDOW_TITLE = "CyberRunner 3D - AI Vision Endless Runner";

MaterialProperties GameConfig::FloorMaterial = {
    { 0.6f, 0.6f, 0.6f, 1.0f },
    { 0.6f, 0.6f, 0.6f, 1.0f },
    { 0.7f, 0.7f, 0.7f, 1.0f },
    { 50.0f }
};

MaterialProperties GameConfig::PlayerMaterial = {
    { 1.0f, 1.0f, 1.0f, 1.0f },
    { 0.9f, 0.9f, 0.9f, 1.0f },
    { 0.9f, 0.9f, 0.9f, 1.0f },
    { 80.0f }
};

MaterialProperties GameConfig::EnemyMaterial = {
    { 0.8f, 0.1f, 0.1f, 1.0f },
    { 0.8f, 0.1f, 0.1f, 1.0f },
    { 0.6f, 0.1f, 0.1f, 1.0f },
    { 80.0f }
};

LightSource GameConfig::SceneLight0 = {
    { 0.0f, 2.0f, -5.0f, 1.0f },
    { 0.0f, 0.0f, 1.0f, 1.0f },
    { 0.2f, 0.2f, 0.3f, 1.0f },
    { 0.5f, 0.5f, 0.5f, 1.0f },
    { 0.0f, 0.0f, -1.0f },
    { 180.0f },
    { 0.0f }
};

LightSource GameConfig::SpotLight1 = {
    { 0.0f, 0.0f, 2.0f, 0.0f },
    { 0.0f, 0.9f, 0.9f, 1.0f },
    { 0.1f, 0.1f, 0.1f, 1.0f },
    { 0.8f, 0.8f, 0.8f, 1.0f },
    { 0.0f, 0.0f, -5.0f },
    { 30.0f },
    { 100.0f }
};

void GameConfig::InitializeDefaults() {
    // Config runtime initialization hook
}

} // namespace Engine
