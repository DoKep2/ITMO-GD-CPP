#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <random>
#include <map>

int roll_die(int sides) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, sides);
    return dis(gen);
}

struct Dice {
    int count;
    int sides;
    int modifier;
};

Dice parse_spec(const std::string& spec) {
    Dice result{0, 0, 0};
    char d;
    std::stringstream ss(spec);

    ss >> result.count >> d >> result.sides;
    if (ss.peek() == '+' || ss.peek() == '-') {
        ss >> result.modifier;
    }

    return result;
}

int dice(const std::string& input) {
    std::stringstream ss(input);
    std::string part;
    int total = 0;

    while (std::getline(ss, part, ',')) {
        Dice spec = parse_spec(part);
        for (int i = 0; i < spec.count; ++i) {
            total += roll_die(spec.sides);
        }
        total += spec.modifier;
    }

    return total;
}

std::map<int, int> simulate(const std::string& input, int trials) {
    std::map<int, int> frequency;

    for (int i = 0; i < trials; ++i) {
        int result = dice(input);
        frequency[result]++;
    }

    return frequency;
}

int main() {
    std::string spec = "2d6+2,3d10";
    int result = dice(spec);
    std::cout << "Result: " << result << std::endl;

    std::vector<std::string> tests = {"1d6", "2d6", "3d6", "1d10", "2d10", "3d10"};
    int trials = 10000;

    for (const auto& test : tests) {
        std::cout << "\nDistribution for " << test << ":\n";
        auto freq = simulate(test, trials);
        for (const auto& [value, count] : freq) {
            std::cout << value << ": " << count << std::endl;
        }
    }

    return 0;
}
