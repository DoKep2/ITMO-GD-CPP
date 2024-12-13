// Copyright 2024 Sergo Elizbarashvili

#include <string>
#include <iostream>
#include "Utils.h"

int RandomInRange(const int min, const int max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(min, max);

    return distrib(gen);
}

int ValidateInput(const int min, const int max, const std::string& prompt) {
    int input;
    while (true) {
        std::cout << prompt;
        std::cin >> input;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Ошибка: введите целое число.\n";
        } else if (input < min || input > max) {
            std::cout << "Ошибка: введите число в диапазоне от " << min << " до " << max << ".\n";
        } else {
            break;
        }
    }
    return input;
}

