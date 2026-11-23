#include "GameEngine.hpp"
#include <iostream>

int main(int argc, char** argv) {
    std::cout << "=======================================================\n";
    std::cout << "  CyberRunner 3D - AI Vision Endless Runner Engine      \n";
    std::cout << "  Proprietary Real-Time Human-Computer Interaction Demo \n";
    std::cout << "=======================================================\n";
    std::cout << "Controls:\n";
    std::cout << "  [S]          : Start Game / Restart\n";
    std::cout << "  [Left Arrow] : Move Left Lane (or Lean Left)\n";
    std::cout << "  [Right Arrow]: Move Right Lane (or Lean Right)\n";
    std::cout << "  [Up Arrow]   : Jump (or Physical Jump / Hand Raise)\n";
    std::cout << "  [P]          : Pause / Resume\n";
    std::cout << "  [Q / ESC]    : Quit Game\n";
    std::cout << "=======================================================\n";

    try {
        Engine::GameEngine& engine = Engine::GameEngine::Instance();
        engine.Initialize(argc, argv);
        engine.Run();
    } catch (const std::exception& ex) {
        std::cerr << "Engine fatal error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
