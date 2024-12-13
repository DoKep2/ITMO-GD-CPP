//
// Created by sergo on 22.10.2024.
//

#include "file_manager.h"

#include <iostream>
#include <fstream>

bool FileManager::IsSaveFileExists() {
    return std::ifstream("../save/savegame.txt").good();
}

GameState FileManager::TryLoadGame() {
    GameState gameState;
    if (std::ifstream saveFile("../save/savegame.txt"); saveFile.is_open()) {
        saveFile >> gameState.year_ >> gameState.population_ >> gameState.acres_ >> gameState.wheat_;
        saveFile.close();
    }
    return gameState;
}

void FileManager::SaveGame(const GameState& gameState) {
    if (std::ofstream saveFile("../../save/savegame.txt"); saveFile.is_open()) {
        saveFile << gameState.year_ << " " << gameState.population_ << " " << gameState.acres_ << " " << gameState.wheat_ << std::endl;
        saveFile.close();
    }
}
