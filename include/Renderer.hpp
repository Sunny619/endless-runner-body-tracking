#ifndef RENDERER_HPP
#define RENDERER_HPP

#if defined(__APPLE__)
    #define GL_SILENCE_DEPRECATION
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif

#include "GameConfig.hpp"
#include <string>

namespace Engine {

class Player;

class Renderer {
private:
    float m_spectatorBouncePhase;
    void* m_fontHandle;

public:
    Renderer();

    void InitializeOpenGL();
    void SetMaterial(const MaterialProperties& material);

    void RenderTrack(float length, float thickness);
    void RenderStairs(float mapLength);
    void RenderSpectators(float mapLength);
    void RenderPlayer(const Player& player);

    void RenderHUDText(const std::string& text, int x, int y, float r = 1.0f, float g = 1.0f, float b = 1.0f);
    void RenderGameOverlay(bool started, bool gameOver, int score, int lives);

    void UpdateSpectatorAnimation(float delta = 0.05f);
};

} // namespace Engine

#endif // RENDERER_HPP
