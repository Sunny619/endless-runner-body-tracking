#include "Renderer.hpp"
#include "Player.hpp"
#include <cmath>

namespace Engine {

Renderer::Renderer()
    : m_spectatorBouncePhase(0.0f)
    , m_fontHandle(GLUT_BITMAP_TIMES_ROMAN_24)
{
}

void Renderer::InitializeOpenGL() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Setup scene diffuse and ambient lights
    glLightfv(GL_LIGHT0, GL_DIFFUSE, GameConfig::SceneLight0.diffuse);
    glLightfv(GL_LIGHT0, GL_AMBIENT, GameConfig::SceneLight0.ambient);
    glLightfv(GL_LIGHT0, GL_POSITION, GameConfig::SceneLight0.position);

    glClearColor(0.05f, 0.05f, 0.08f, 1.0f);
}

void Renderer::SetMaterial(const MaterialProperties& material) {
    glMaterialfv(GL_FRONT, GL_AMBIENT, material.ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, material.diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, material.specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, material.shininess);
}

void Renderer::RenderTrack(float length, float thickness) {
    SetMaterial(GameConfig::FloorMaterial);
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);
    glScalef(5.0f, thickness, length);
    glutSolidCube(1.0);
    glPopMatrix();
}

static void DrawSingleStair(float x, float y, float mapLen) {
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glScalef(6.0f, 1.0f, mapLen);
    glutSolidCube(1.0);
    glPopMatrix();
}

static void DrawSingleSpectator(float x, float y, float z) {
    // Torso
    glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(0.3f, 1.0f, 0.3f);
    glutSolidCube(1.0);
    glPopMatrix();

    // Right arm
    glPushMatrix();
    glTranslatef(x, y + 0.76f, z + 0.45f);
    glRotatef(30.0f, 1.0f, 0.0f, 0.0f);
    glScalef(0.1f, 0.3f, 0.1f);
    glutSolidCube(1.0);
    glPopMatrix();

    // Left arm
    glPushMatrix();
    glTranslatef(-x, y + 0.76f, z - 0.45f);
    glRotatef(-30.0f, 1.0f, 0.0f, 0.0f);
    glScalef(0.1f, 0.3f, 0.1f);
    glutSolidCube(1.0);
    glPopMatrix();

    // Head sphere
    glPushMatrix();
    glTranslatef(x, y + 0.8f, z);
    glutSolidSphere(0.25, 16, 16);
    glPopMatrix();
}

void Renderer::RenderStairs(float mapLength) {
    SetMaterial(GameConfig::FloorMaterial);
    DrawSingleStair(6.0f, 0.0f, mapLength);
    DrawSingleStair(-6.0f, 0.0f, mapLength);
    DrawSingleStair(8.0f, 0.5f, mapLength);
    DrawSingleStair(-8.0f, 0.5f, mapLength);
    DrawSingleStair(10.0f, 1.0f, mapLength);
    DrawSingleStair(-10.0f, 1.0f, mapLength);
}

void Renderer::RenderSpectators(float mapLength) {
    SetMaterial(GameConfig::FloorMaterial);
    float bounce = std::sin(m_spectatorBouncePhase) * 0.15f;

    for (int i = -3; i > -mapLength / 2; i -= 2) {
        float zi = static_cast<float>(i);
        DrawSingleSpectator(4.5f, 0.5f + bounce, zi);
        DrawSingleSpectator(-4.5f, 0.5f + bounce, zi);
        DrawSingleSpectator(6.0f, 1.0f + bounce, zi + 1.0f);
        DrawSingleSpectator(-6.0f, 1.0f + bounce, zi + 1.0f);
        DrawSingleSpectator(7.5f, 1.5f + bounce, zi);
        DrawSingleSpectator(-7.5f, 1.5f + bounce, zi);
    }
}

void Renderer::RenderPlayer(const Player& player) {
    SetMaterial(GameConfig::PlayerMaterial);
    glPushMatrix();
    glTranslatef(player.GetX(), player.GetY(), player.GetZ());
    glScalef(1.0f, 0.7f, 1.0f);
    glutSolidCube(1.0);
    glPopMatrix();
}

void Renderer::RenderHUDText(const std::string& text, int x, int y, float r, float g, float b) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, GameConfig::SCREEN_WIDTH, 0, GameConfig::SCREEN_HEIGHT, 0, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glColor3f(r, g, b);
    glRasterPos2i(x, y);
    for (char c : text) {
        glutBitmapCharacter(m_fontHandle, static_cast<int>(c));
    }

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void Renderer::RenderGameOverlay(bool started, bool gameOver, int score, int lives) {
    glDisable(GL_LIGHTING);

    if (!started && !gameOver) {
        RenderHUDText("Press 's' or Raise your right hand to start", 150, 600, 1.0f, 1.0f, 0.2f);
    }

    if (gameOver) {
        RenderHUDText("GAME OVER - Press 's' to Restart", 230, 600, 1.0f, 0.2f, 0.2f);
    }

    // Render Score HUD
    std::string scoreStr = "Score: " + std::to_string(score);
    RenderHUDText(scoreStr, 325, 700, 1.0f, 1.0f, 1.0f);

    // Render Lives HUD
    std::string livesStr = "Lives: ";
    for (int i = 0; i < lives; i++) {
        livesStr += "[#] ";
    }
    RenderHUDText(livesStr, 50, 700, 0.2f, 1.0f, 0.4f);

    glEnable(GL_LIGHTING);
}

void Renderer::UpdateSpectatorAnimation(float delta) {
    m_spectatorBouncePhase += delta;
    if (m_spectatorBouncePhase > 6.2831853f) {
        m_spectatorBouncePhase -= 6.2831853f;
    }
}

} // namespace Engine
