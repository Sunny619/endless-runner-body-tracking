#ifndef GAME_ENGINE_HPP
#define GAME_ENGINE_HPP

#include "GameConfig.hpp"
#include "Player.hpp"
#include "Camera.hpp"
#include "ObstacleManager.hpp"
#include "Renderer.hpp"
#include "InputHandler.hpp"

namespace Engine {

enum class EngineState {
    MENU,
    RUNNING,
    PAUSED,
    GAME_OVER
};

class GameEngine {
private:
    static GameEngine* s_instance;

    EngineState m_state;
    Player m_player;
    Camera m_camera;
    ObstacleManager m_obstacleManager;
    Renderer m_renderer;
    InputHandler m_inputHandler;

    int m_score;
    int m_highScore;

public:
    GameEngine();
    ~GameEngine();

    static GameEngine& Instance();

    void Initialize(int argc, char** argv);
    void Run();

    void OnDisplay();
    void OnIdle();
    void OnKeyboard(unsigned char key, int x, int y);
    void OnSpecialKey(int key, int x, int y);
    void OnReshape(int width, int height);

    void StartGame();
    void RestartGame();
    void TriggerGameOver();

    EngineState GetState() const { return m_state; }
    int GetScore() const { return m_score; }
};

} // namespace Engine

#endif // GAME_ENGINE_HPP
