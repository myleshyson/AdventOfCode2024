#include <regex>
#include <sstream>
#include <iostream>
#include <functional>
#include "helpers.h"

long long getTokens
(std::pair<long long, long long> A, std::pair<long long, long long> B, std::pair<long long, long long> prize) {
    long long tokens = 0;
    long long numeratorX = prize.first * B.second - prize.second * B.first;
    long long denominatorX = A.first * B.second - A.second * B.first;

    long long numeratorY = prize.first * A.second - prize.second * A.first;
    long long denominatorY = A.second * B.first - A.first * B.second;

    if (numeratorX % denominatorX == 0 && numeratorY % denominatorY == 0) {
        long long a = numeratorX / denominatorX;
        long long b = numeratorY / denominatorY;
        tokens += 3 * a + b;
    }

    return tokens;
}

int main() {
    std::ifstream input = get_input("13.txt");
    std::string content((std::istreambuf_iterator(input)), std::istreambuf_iterator<char>());
    std::stringstream ss(content);
    std::string line;
    std::string block;
    std::vector<std::string> blocks;

    while (getline(ss, line, '\n')) {
        if (line.empty()) {
            blocks.push_back(block);
            block = "";
            continue;
        }

        block += line + "\n";
    }

    blocks.push_back(block);

    long long answer[] = {0, 0};

    for (auto instructions: blocks) {
        std::regex re(
            R"(Button A: X\+([0-9]+), Y\+([0-9]+)\nButton B: X\+([0-9]+), Y\+([0-9]+)\nPrize: X\=([0-9]+), Y\=([0-9]+)\n)"
        );

        auto matchesBegin = std::sregex_iterator(instructions.begin(), instructions.end(), re);
        auto matchesEnd = std::sregex_iterator();

        for (auto it = matchesBegin; it != matchesEnd; ++it) {
            std::smatch match = *it;

            auto ax = match[1];
            auto ay = match[2];
            auto bx = match[3];
            auto by = match[4];
            auto px = match[5];
            auto py = match[6];

            std::pair A(std::stoll(ax), std::stoll(ay));
            std::pair B(std::stoll(bx), std::stoll(by));
            std::pair destination(std::stoll(px), std::stoll(py));

            answer[0] += getTokens(A, B, destination);
            answer[1] += getTokens(A, B, {destination.first + 10000000000000, destination.second + 10000000000000});
        }
    }

    std::println("{}", answer);
}
