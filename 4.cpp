#include <map>
#include <set>
#include <iostream>
#include "helpers.h"

// xmas
bool dfs(
    std::pair<int, int> node,
    const std::map<std::pair<int, int>, std::vector<std::pair<int, int>>> &graph,
    const std::vector<std::vector<char>> &input,
    std::set<std::pair<int, int>> &seen,
    std::pair<int, int> direction,
    const char letter,
    const std::vector<char> &letters) {

    if (input[node.first][node.second] != letters[letter]) return false;
    if (letter == letters.size() - 1 && input[node.first][node.second] == letters[letter]) return true;

    for (auto &neighbor: graph.at(node)) {
        if (!seen.contains(neighbor) && std::make_pair(node.first + direction.first, node.second + direction.second) == neighbor) {
            seen.insert(neighbor);
            return dfs(neighbor, graph, input, seen, direction, letter + 1, letters);
        }
    }

    return false;
}

int first() {
    int answer = 0;
    std::ifstream file = get_input("4.txt");
    std::vector<std::vector<char>> input;
    std::map<std::pair<int, int>, std::vector<std::pair<int, int> > > graph;
    std::string line;
    std::vector directions = {
        std::make_pair(0, -1),
        std::make_pair(0, 1),
        std::make_pair(-1, 0),
        std::make_pair(1, 0),
        std::make_pair(-1, -1),
        std::make_pair(-1, 1),
        std::make_pair(1, -1),
        std::make_pair(1, 1)
    };

    while (getline(file, line)) {
        std::vector<char> row;

        for (char i: line) {
            row.push_back(i);
        }

        input.push_back(row);
    }

    for (int i = 0; i < input.size(); i++) {
        for (int j = 0; j < input[i].size(); j++) {
            std::vector<std::pair<int, int> > neighbors;

            for (auto direction: directions) {
                int newRow = i + direction.first;
                int newCol = j + direction.second;

                if (newRow < 0 || newRow >= input.size() || newCol < 0 || newCol >= input[i].size()) {
                    continue;
                }

                neighbors.emplace_back(newRow, newCol);
            }

            graph.emplace(std::make_pair(i, j), neighbors);
        }
    }

    std::vector letters = {'X', 'M', 'A', 'S'};

    for (const auto &pair: graph) {
        std::pair<int, int> node = pair.first;

        if (input[node.first][node.second] != 'X') continue;

        for (auto direction : directions) {
            std::set<std::pair<int, int>> seen;
            if (dfs(node, graph, input, seen, direction, 0, letters)) {
                answer++;
            }
        }

    }

    return answer;
}

int second() {
    int answer = 0;
    std::ifstream file = get_input("4.txt");
    std::vector<std::vector<char>> input;
    std::map<std::pair<int, int>, std::vector<std::pair<int, int> > > graph;
    std::string line;
    std::vector directions = {
        std::make_pair(-1, -1),
        std::make_pair(-1, 1),
        std::make_pair(1, -1),
        std::make_pair(1, 1)
    };

    while (getline(file, line)) {
        std::vector<char> row;

        for (char i: line) {
            row.push_back(i);
        }

        input.push_back(row);
    }

    for (int i = 0; i < input.size(); i++) {
        for (int j = 0; j < input[i].size(); j++) {
            std::vector<std::pair<int, int> > neighbors;

            for (auto direction: directions) {
                int newRow = i + direction.first;
                int newCol = j + direction.second;

                if (newRow < 0 || newRow >= input.size() || newCol < 0 || newCol >= input[i].size()) {
                    continue;
                }

                neighbors.emplace_back(newRow, newCol);
            }

            graph.emplace(std::make_pair(i, j), neighbors);
        }
    }

    std::vector letters = {'M', 'A', 'S'};
    std::set<std::pair<int, int>> intersectingPoints;

    for (const auto &pair: graph) {
        std::pair<int, int> node = pair.first;

        if (input[node.first][node.second] != 'M') continue;

        for (auto direction : directions) {
            std::set<std::pair<int, int>> seen;

            if (dfs(node, graph, input, seen, direction, 0, letters)) {
                for (auto point : seen) {
                    if (intersectingPoints.contains(point) && input[point.first][point.second] == 'A') answer++;

                    if (!intersectingPoints.contains(point)) {
                        intersectingPoints.insert(point);
                    }
                }
            }
        }

    }

    return answer;
}

int main() {
    std::cout << first() << std::endl;
    std::cout << second() << std::endl;
}
