#ifndef INPUT_HANDLER_HPP
#define INPUT_HANDLER_HPP

#if defined(__APPLE__)
    #define GL_SILENCE_DEPRECATION
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif

namespace Engine {

class GameEngine;

enum class GameAction {
    NONE,
    START_GAME,
    MOVE_LEFT,
    MOVE_RIGHT,
    JUMP,
    TOGGLE_PAUSE,
    RESTART,
    QUIT
};

class InputHandler {
public:
    InputHandler() = default;

    GameAction ProcessNormalKey(unsigned char key, int x, int y);
    GameAction ProcessSpecialKey(int key, int x, int y);
};

} // namespace Engine

#endif // INPUT_HANDLER_HPP
