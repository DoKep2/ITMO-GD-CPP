// Copyright 2024 Sergo Elizbarashvili

#include <format>
#include <fstream>
#include <iostream>
#include <algorithm>
#include "Game.h"
#include "Utils.h"
#include "../file_system/file_manager.h" //fixme todo

constexpr int kWheatPerPerson = 20;
constexpr int kMaxYears = 10;
constexpr double kMaxStarvationDeaths = 0.45;
constexpr double kRatsWheatConsumptionFraction = 0.07;

Game::Game(const GameState& game_state): starvation_deaths_(0), new_citizens_(0),
              plague_(false), wheat_per_acre_(0), rats_ate_(0), land_price_(0),
              total_starvation_deaths_(0), game_state_(game_state) {
}

Game::Game(): Game(GameState()) {}

void Game::StartGame() {
    GenerateRandomParams();
    while (!IsGameOver()) {
        AskSaveAndExit();
        UserInput();
        NextYear();
    }
}

void Game::AskSaveAndExit() const {
    std::cout << "Желаете сохранить игру и выйти? (y/n): ";
    char choice;
    std::cin >> choice;
    if (choice == 'y') {
        FileManager::SaveGame(game_state_);
    }
}

void Game::NextYear() {
    game_state_.year_++;

    const int totalWheat = game_state_.acres_ * wheat_per_acre_ - rats_ate_;
    game_state_.wheat_ += totalWheat;

    if (plague_) {
        game_state_.population_ /= 2;
    }

    game_state_.population_ -= starvation_deaths_;

    total_starvation_deaths_ += starvation_deaths_;
    new_citizens_ = (starvation_deaths_ / 2 + (5 - wheat_per_acre_) * totalWheat / 600 + 1);
    new_citizens_ = std::max(0, std::min(50, new_citizens_));

    game_state_.population_ += new_citizens_;

    if (IsGameOver()) {
        CalculateResults();
    } else {
        GenerateRandomParams();
        PrintStatus();
    }
}

bool Game::IsGameOver() const {
    return starvation_deaths_ > kMaxStarvationDeaths * game_state_.population_ ||
        game_state_.population_ == 0 || game_state_.year_ >= kMaxYears;
}


void Game::GenerateRandomParams() {
    plague_ = RandomInRange(0, 100) < 15;
    wheat_per_acre_ = RandomInRange(1, 6);
    rats_ate_ = RandomInRange(0, static_cast<int>(game_state_.wheat_ * kRatsWheatConsumptionFraction));
    land_price_ = RandomInRange(17, 26);
}

void Game::UpdateCityState(const int acres_to_buy, const int acres_to_sell,
                           const int wheat_to_plant, const int wheat_to_eat) {
    game_state_.acres_ += acres_to_buy - acres_to_sell;
    game_state_.wheat_ -= wheat_to_eat + wheat_to_plant;
    game_state_.wheat_ += acres_to_sell * wheat_per_acre_;
    starvation_deaths_ = std::max(0, game_state_.population_ - wheat_to_eat / kWheatPerPerson);
}

void Game::UserInput() {
    std::cout << "Что пожелаешь, повелитель? ";
    std::cout << "Сколько акров земли повелеваешь купить?\n";
    const int max_acres_to_buy = game_state_.wheat_ / land_price_;
    const int acres_to_buy = ValidateInput(0, max_acres_to_buy, "Введите число от 0 до " + std::to_string(max_acres_to_buy) + ": ");
    std::cout << "Сколько акров земли повелеваешь продать?\n";
    const int acres_to_sell = ValidateInput(0, game_state_.acres_, "Введите число от 0 до " + std::to_string(game_state_.acres_) + ": ");
    std::cout << "Сколько акров земли повелеваешь засеять?\n";
    const int max_wheat_to_plant = std::min(game_state_.wheat_, game_state_.acres_);
    const int wheat_to_plant = ValidateInput(0, std::max(game_state_.wheat_, max_wheat_to_plant), "Введите число от 0 до " + std::to_string(max_wheat_to_plant) + ": ");
    std::cout << "Сколько бушелей пшеницы повелеваешь съесть?\n";
    const int wheat_remaining = game_state_.wheat_ - acres_to_buy * land_price_ + acres_to_sell * land_price_ - wheat_to_plant;
    const int wheat_to_eat = ValidateInput(0, wheat_remaining, "Введите число от 0 до " + std::to_string(wheat_remaining) + ": ");

    UpdateCityState(acres_to_buy, acres_to_sell, wheat_to_plant, wheat_to_eat);
}

void Game::PrintStatus() const {
    std::cout << std::format("Мой повелитель, соизволь поведать тебе в году {}"
                             " твоего высочайшего правления {} человек умерло от голода."
                             " {} новых граждан прибыло в город. ",
                             game_state_.year_, starvation_deaths_, new_citizens_);
    if (plague_) {
        std::cout << " Чума уничтожила половину населения. ";
    }
    std::cout << std::format(
        "Население города сейчас составляет {} человек. "
        "Мы собрали {} бушелей пшеницы, по {} бушелей с акра. "
        "Крысы уничтожили {} бушелей пшеницы. У нас есть {} бушелей пшеницы и {} акров земли. "
        "Цена акра земли составляет {} бушелей.\n",
        game_state_.population_, game_state_.acres_ * wheat_per_acre_, wheat_per_acre_,
        rats_ate_, game_state_.wheat_, game_state_.acres_, land_price_);
}

void Game::CalculateResults() const {
    if (game_state_.population_ == 0) {
        std::cout << "Ваш город разорен. Игра окончена.\n";
    } else if (starvation_deaths_ > kMaxStarvationDeaths * game_state_.population_) {
        std::cout << "Слишком много смертей от голода. Игра окончена.\n";
    } else {
        const double acres_per_person = static_cast<double>(game_state_.acres_) / game_state_.population_;
        const double average_starvation_deaths = static_cast<double>(total_starvation_deaths_) / game_state_.year_;

        if (average_starvation_deaths > 0.33 && acres_per_person < 7) {
            std::cout << "Из-за вашей некомпетентности в управлении, народ устроил бунт, и изгнал вас их города. "
                    "Теперь вы вынуждены влачить жалкое существование в изгнании";
        } else if (average_starvation_deaths > 0.1 && acres_per_person < 9) {
            std::cout << "Вы правили железной рукой, подобно Нерону и Ивану Грозному. "
                    "Народ вздохнул с облегчением, и никто больше не желает видеть вас правителем";
        } else if (average_starvation_deaths > 0.03 && acres_per_person < 10) {
            std::cout <<
                    "Вы справились вполне неплохо, у вас, конечно, есть недоброжелатели, но многие хотели бы увидеть вас во главе города снова";
        } else {
            std::cout << "Фантастика! Карл Великий, Дизраэли и Джефферсон вместе не справились бы лучше";
        }
    }
}
