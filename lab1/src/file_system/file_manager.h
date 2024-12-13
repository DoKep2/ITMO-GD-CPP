//
// Created by sergo on 22.10.2024.
//

#pragma once
#include "../game/game_state.h" //fixme todo

class FileManager {
 public:
    static bool IsSaveFileExists();
    static GameState TryLoadGame();
    static void SaveGame(const GameState& gameState);
};
