#include <iostream>
#include <sstream>
#include <map>
#include <ranges>
#include <set>

#include "helpers.h"

typedef std::map<std::pair<long, int>, std::vector<std::pair<std::string, std::pair<long, int> > > > graph;

bool dfs(const std::pair<long, int> &node, const graph &graph, const long long &target, long long currentValue = 0, const std::optional<std::string>& operand = std::nullopt) {
    if (!operand) {
        currentValue = node.first;
    } else {
        if (operand == "*") {
            currentValue *= node.first;
        }
        if (operand == "+") {
            currentValue += node.first;
        }
        if (operand == "||") {
            currentValue = std::stoll(std::to_string(currentValue) + std::to_string(node.first));
        }
    }

    if (graph.at(node).empty()) {
        return currentValue == target;
    }

    for (const auto& neighbor: graph.at(node)) {
        if (currentValue > target) {
            continue;
        }
        if (dfs(neighbor.second, graph, target, currentValue, neighbor.first)) {
            return true;
        }
    }

    return false;
}

bool isValid(const std::vector<long> &numbers, const long long &target, std::vector<std::string> operators) {
    graph graph;
    std::pair<long, int> first;

    for (int i = 0; i < numbers.size(); i++) {
        if (i + 1 >= numbers.size()) {
            std::vector<std::pair<std::string, std::pair<long, int> > > vec;
            graph.insert_or_assign(std::make_pair(numbers[i], i), vec);
            break;
        };

        for (auto &operand : operators) {
            graph[std::make_pair(numbers[i], i)].emplace_back(operand, std::make_pair(numbers[i + 1], i + 1));
        }

        if (i == 0) {
            first = std::make_pair(numbers[i], i);
        }
    }
    return dfs(first, graph, target);
}

// 17: (*, 8), (+, 8)
int main() {
    std::ifstream file = get_input("7.txt");
    std::string line;
    long long answer[2] = {0, 0};

    while (getline(file, line)) {
        graph graph;
        auto split = line.find(": ");
        long long equals = stoll(line.substr(0, split));
        auto numbers = line.substr(split + 2);

        std::stringstream ss(numbers);
        std::string number;
        std::vector<long> numberVec;

        while (getline(ss, number, ' ')) {
            numberVec.push_back(stol(number));
        }

        if (isValid(numberVec, equals, std::vector<std::string>{"+", "*"})) {
            answer[0] += equals;
        }

        if (isValid(numberVec, equals, std::vector<std::string>{"+", "*", "||"})) {
            answer[1] += equals;
        }
    }

    std::println("{}", answer);

    return 0;
}
