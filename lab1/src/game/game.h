// Copyright 2024 Sergo Elizbarashvili

#pragma once

#include "game_state.h"

class Game {
 private:
    int starvation_deaths_;
    int new_citizens_;
    bool plague_;
    int wheat_per_acre_;
    int rats_ate_;
    int land_price_;
    int total_starvation_deaths_;
    GameState game_state_;

 public:
    Game();
    explicit Game(const GameState&);

    void StartGame();
    void AskSaveAndExit() const;
    void NextYear();
    void GenerateRandomParams();
    void PrintStatus() const;
    void CalculateResults() const;
    void UpdateCityState(int acres_to_buy, int acres_to_sell, int wheat_to_plant, int wheat_to_eat);
    void UserInput();
    [[nodiscard]] bool IsGameOver() const;
};
