//
// Created by sergo on 22.10.2024.
//

#include "game_io.h"

#include <Game.h>
#include <utils.h>

#include "file_manager.h"
#include "game_state.h"

UserInputData::UserInputData(int acres_to_buy, int acres_to_sell, int wheat_to_plant, int wheat_to_eat) {
    return {acres_to_buy, acres_to_sell, wheat_to_plant, wheat_to_eat};
}

void GameIO::PrintMessage(const std::string& message) {
    std::cout << message << std::endl;
}

void GameIO::PrintStatus(const GameState &game_state, const int starvation_deaths, const int land_price,
                         const int new_citizens, const bool plague, const int rats_ate, const int wheat_per_acre) {
    PrintMessage(std::format("Мой повелитель, соизволь поведать тебе в году {}"
                             " твоего высочайшего правления {} человек умерло от голода."
                             " {} новых граждан прибыло в город. ",
                             game_state.year_, starvation_deaths, new_citizens));
    if (plague) {
        PrintMessage(" Чума уничтожила половину населения. ");
    }
    PrintMessage(std::format(
        "Население города сейчас составляет {} человек. "
        "Мы собрали {} бушелей пшеницы, по {} бушелей с акра. "
        "Крысы уничтожили {} бушелей пшеницы. У нас есть {} бушелей пшеницы и {} акров земли. "
        "Цена акра земли составляет {} бушелей.\n",
        game_state.population_, game_state.acres_ * wheat_per_acre, wheat_per_acre,
        rats_ate, game_state.wheat_, game_state.acres_, land_price));
}

bool GameIO::AskSaveAndExit(const GameState& game_state) {
    std::cout << "Желаете сохранить игру и выйти? (y/n): ";
    char choice;
    std::cin >> choice;
    if (choice == 'y') {
        FileManager::SaveGame(game_state);
        return true;
    }
    return false;
}

UserInputData GameIO::UserInput(const GameState& game_state, int land_price) {
    std::cout << "Что пожелаешь, повелитель? ";
    std::cout << "Сколько акров земли повелеваешь купить?\n";
    const int max_acres_to_buy = game_state.wheat_ / land_price;
    const int acres_to_buy = ValidateInput(0, max_acres_to_buy, "Введите число от 0 до " + std::to_string(max_acres_to_buy) + ": ");
    std::cout << "Сколько акров земли повелеваешь продать?\n";
    const int acres_to_sell = ValidateInput(0, game_state.acres_, "Введите число от 0 до " + std::to_string(game_state.acres_) + ": ");
    std::cout << "Сколько акров земли повелеваешь засеять?\n";
    const int max_wheat_to_plant = std::min(game_state.wheat_, game_state.acres_);
    const int wheat_to_plant = ValidateInput(0, std::max(game_state.wheat_, max_wheat_to_plant), "Введите число от 0 до " + std::to_string(max_wheat_to_plant) + ": ");
    std::cout << "Сколько бушелей пшеницы повелеваешь съесть?\n";
    const int wheat_remaining = game_state.wheat_ - acres_to_buy * land_price + acres_to_sell * land_price - wheat_to_plant;
    const int wheat_to_eat = ValidateInput(0, wheat_remaining, "Введите число от 0 до " + std::to_string(wheat_remaining) + ": ");

    return UserInputData{acres_to_buy, acres_to_sell, wheat_to_plant, wheat_to_eat};
}

int GameIO::GetUserInput(const std::string& prompt, int min, int max) {
    int input;
    do {
        std::cout << prompt;
        std::cin >> input;
    } while (input < min || input > max);
    return input;
}
