//
// Created by sergo on 22.10.2024.
//

#pragma once

#include <iostream>
#include <string>
#include <format>

#include "game_state.h"

class UserInputData {
 public:
    int acres_to_buy;
    int acres_to_sell;
    int wheat_to_plant;
    int wheat_to_eat;

    UserInputData(int acres_to_buy, int acres_to_sell, int wheat_to_plant, int wheat_to_eat);
};

class GameIO {
 public:
    static void PrintMessage(const std::string& message);
    static void PrintStatus(const GameState &game_state, int starvation_deaths, int land_price,
                            int new_citizens, bool plague, int rats_ate, int wheat_per_acre);
    static UserInputData UserInput(const GameState& game_state_, int land_price_);

    [[nodiscard]] static bool AskSaveAndExit(const GameState& game_state);
    [[nodiscard]] static int GetUserInput(const std::string& prompt, int min, int max);
};
