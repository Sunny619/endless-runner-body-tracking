#include "GameEngine.hpp"
#include <iostream>
#include <cstdlib>

namespace Engine {

GameEngine* GameEngine::s_instance = nullptr;

// Static C-style callback wrappers required by GLUT
static void DisplayCallbackWrapper() {
    GameEngine::Instance().OnDisplay();
}

static void IdleCallbackWrapper() {
    GameEngine::Instance().OnIdle();
}

static void KeyboardCallbackWrapper(unsigned char key, int x, int y) {
    GameEngine::Instance().OnKeyboard(key, x, y);
}

static void SpecialKeyCallbackWrapper(int key, int x, int y) {
    GameEngine::Instance().OnSpecialKey(key, x, y);
}

static void ReshapeCallbackWrapper(int width, int height) {
    GameEngine::Instance().OnReshape(width, height);
}

GameEngine::GameEngine()
    : m_state(EngineState::MENU)
    , m_score(0)
    , m_highScore(0)
{
    s_instance = this;
}

GameEngine::~GameEngine() {
    if (s_instance == this) {
        s_instance = nullptr;
    }
}

GameEngine& GameEngine::Instance() {
    if (!s_instance) {
        s_instance = new GameEngine();
    }
    return *s_instance;
}

void GameEngine::Initialize(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(GameConfig::SCREEN_WIDTH, GameConfig::SCREEN_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(GameConfig::WINDOW_TITLE);

    m_renderer.InitializeOpenGL();
    m_obstacleManager.Initialize();

    glutDisplayFunc(DisplayCallbackWrapper);
    glutIdleFunc(IdleCallbackWrapper);
    glutKeyboardFunc(KeyboardCallbackWrapper);
    glutSpecialFunc(SpecialKeyCallbackWrapper);
    glutReshapeFunc(ReshapeCallbackWrapper);
}

void GameEngine::Run() {
    glutMainLoop();
}

void GameEngine::StartGame() {
    if (m_state == EngineState::MENU || m_state == EngineState::GAME_OVER) {
        m_state = EngineState::RUNNING;
        m_player.Reset();
        m_score = 0;
        m_obstacleManager.Initialize();
    }
}

void GameEngine::RestartGame() {
    m_player.Reset();
    m_score = 0;
    m_obstacleManager.Initialize();
    m_state = EngineState::RUNNING;
}

void GameEngine::TriggerGameOver() {
    m_state = EngineState::GAME_OVER;
    if (m_score > m_highScore) {
        m_highScore = m_score;
    }
}

void GameEngine::OnDisplay() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Apply 3D Camera Follow Matrix
    m_camera.ApplyProjectionMatrix(GameConfig::SCREEN_WIDTH, GameConfig::SCREEN_HEIGHT);
    m_camera.ApplyViewMatrix();

    // Render 3D Environment Geometry
    m_renderer.RenderTrack(GameConfig::MAP_LENGTH, 0.02f);
    m_renderer.RenderStairs(GameConfig::MAP_LENGTH);
    m_renderer.RenderSpectators(GameConfig::MAP_LENGTH);

    // Render Dynamic Entities
    m_renderer.RenderPlayer(m_player);

    m_renderer.SetMaterial(GameConfig::EnemyMaterial);
    m_obstacleManager.RenderVisible(m_player.GetZ());

    // Render 2D Heads-Up Display (HUD)
    bool isStarted = (m_state == EngineState::RUNNING);
    bool isGameOver = (m_state == EngineState::GAME_OVER);
    m_renderer.RenderGameOverlay(isStarted, isGameOver, m_score, m_player.GetLives());

    glutSwapBuffers();
}

void GameEngine::OnIdle() {
    if (m_state == EngineState::RUNNING) {
        m_player.Update();
        m_camera.Update(m_player.GetX(), m_player.GetY(), m_player.GetZ());

        // Update score proportional to distance traveled
        m_score = static_cast<int>(-m_player.GetZ());

        // Collision detection and resolution
        if (m_obstacleManager.CheckCollisions(m_player)) {
            m_camera.TriggerShake(0.20f);
        }

        // Check for player life exhaustion
        if (m_player.GetLives() <= 0) {
            TriggerGameOver();
        }

        m_renderer.UpdateSpectatorAnimation(0.06f);
    }

    glutPostRedisplay();
}

void GameEngine::OnKeyboard(unsigned char key, int x, int y) {
    GameAction action = m_inputHandler.ProcessNormalKey(key, x, y);

    switch (action) {
        case GameAction::START_GAME:
            if (m_state == EngineState::GAME_OVER) {
                RestartGame();
            } else {
                StartGame();
            }
            break;
        case GameAction::RESTART:
            RestartGame();
            break;
        case GameAction::TOGGLE_PAUSE:
            if (m_state == EngineState::RUNNING) {
                m_state = EngineState::PAUSED;
            } else if (m_state == EngineState::PAUSED) {
                m_state = EngineState::RUNNING;
            }
            break;
        case GameAction::MOVE_LEFT:
            if (m_state == EngineState::RUNNING) m_player.MoveLeft();
            break;
        case GameAction::MOVE_RIGHT:
            if (m_state == EngineState::RUNNING) m_player.MoveRight();
            break;
        case GameAction::JUMP:
            if (m_state == EngineState::RUNNING) m_player.Jump();
            break;
        case GameAction::QUIT:
            std::exit(0);
            break;
        default:
            break;
    }
}

void GameEngine::OnSpecialKey(int key, int x, int y) {
    GameAction action = m_inputHandler.ProcessSpecialKey(key, x, y);

    if (m_state == EngineState::RUNNING) {
        switch (action) {
            case GameAction::MOVE_LEFT:
                m_player.MoveLeft();
                break;
            case GameAction::MOVE_RIGHT:
                m_player.MoveRight();
                break;
            case GameAction::JUMP:
                m_player.Jump();
                break;
            default:
                break;
        }
    }
}

void GameEngine::OnReshape(int width, int height) {
    if (height == 0) height = 1;
    glViewport(0, 0, width, height);
    m_camera.ApplyProjectionMatrix(width, height);
}

} // namespace Engine
