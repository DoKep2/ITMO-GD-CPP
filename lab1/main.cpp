#include <iostream>

#include "src/game/Game.h"

#ifdef _WIN32
    #include <windows.h>
#endif

int main() {

    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
    #endif

    Game game;
    char choice;
    std::cout << "Хотите загрузить сохраненную игру? (y/n): ";
    std::cin >> choice;
    if (choice == 'y') {
        game.loadGame();
    } else {
        game.startGame();
    }
}
