#include <fstream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <iomanip>

#include "helpers.h"

double stoneCount(std::vector<double> stones, int blinks) {
    std::unordered_map<double, double> map;
    std::unordered_set<double> currentStones;

    for (auto stone: stones) {
        map[stone] = 1;
        currentStones.insert(stone);
    }

    for (int i = 0; i < blinks; i++) {
        std::unordered_set<double> newStones;
        std::unordered_map<double, double> initial = map;

        auto size = static_cast<long long>(currentStones.size());
        auto setIterator = currentStones.begin();

        for (auto j = 0; j < size; j++) {
            double stone = *setIterator;

            if (map[stone] <= 0) {
                ++setIterator;
                continue;
            }

            int digits = static_cast<int>(log10(stone)) + 1;

            if (stone == 0) {
                if (!map.contains(1)) {
                    map.insert_or_assign(1, 0);
                }
                map[1] += initial[0];
                newStones.insert(1);
            } else if (digits % 2 == 0) {
                std::string num = std::to_string(stone);
                double firstHalf = std::stod(num.substr(0, digits / 2));
                double secondHalf = std::stod(num.substr(digits / 2));

                if (!map.contains(firstHalf)) {
                    map.insert_or_assign(firstHalf, 0);
                }

                if (!map.contains(secondHalf)) {
                    map.insert_or_assign(secondHalf, 0);
                }

                map[firstHalf] += initial[stone];
                map[secondHalf] += initial[stone];

                newStones.insert(firstHalf);
                newStones.insert(secondHalf);
            } else {
                double bigBoi = stone * 2024.0;

                if (!map.contains(bigBoi)) {
                    map.insert_or_assign(bigBoi, 0);
                }

                map[bigBoi] += initial[stone];
                newStones.insert(bigBoi);
            }

            map[stone] -= initial[stone];

            ++setIterator;
        }
        currentStones = newStones;
    }

   double answer = 0;

    for (auto &pair: map) {
        if (pair.second > 0) {
            answer += pair.second;
        }
    }

    return answer;
}

int main() {
    std::ifstream file = get_input("11.txt");
    std::string line((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    std::stringstream ss(line);
    std::string string;
    std::vector<double> stones;

    while (getline(ss, string, ' ')) {
        stones.push_back(std::stod(string));
    }

    double answer[] = {0, 0};

    answer[0] = stoneCount(stones, 25);
    answer[1] = stoneCount(stones, 75);

    std::cout << std::format("{}", answer);

    return 0;
}
