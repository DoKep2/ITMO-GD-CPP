//
// Created by sergo on 22.10.2024.
//

#include "game_state.h"

constexpr int kInitPopulation = 100;
constexpr int kInitAcres = 1000;
constexpr int kInitWheat = 2800;

GameState::GameState(): year_(0), population_(kInitPopulation), acres_(kInitAcres), wheat_(kInitWheat) {}

