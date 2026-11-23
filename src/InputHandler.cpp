#include "InputHandler.hpp"

namespace Engine {

GameAction InputHandler::ProcessNormalKey(unsigned char key, int x, int y) {
    switch (key) {
        case 's':
        case 'S':
            return GameAction::START_GAME;
        case 'r':
        case 'R':
            return GameAction::RESTART;
        case 'p':
        case 'P':
            return GameAction::TOGGLE_PAUSE;
        case 27: // ESC key
        case 'q':
        case 'Q':
            return GameAction::QUIT;
        case 'a':
        case 'A':
            return GameAction::MOVE_LEFT;
        case 'd':
        case 'D':
            return GameAction::MOVE_RIGHT;
        case 'w':
        case 'W':
        case ' ': // Spacebar to jump
            return GameAction::JUMP;
        default:
            return GameAction::NONE;
    }
}

GameAction InputHandler::ProcessSpecialKey(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_LEFT:
            return GameAction::MOVE_LEFT;
        case GLUT_KEY_RIGHT:
            return GameAction::MOVE_RIGHT;
        case GLUT_KEY_UP:
            return GameAction::JUMP;
        default:
            return GameAction::NONE;
    }
}

} // namespace Engine
