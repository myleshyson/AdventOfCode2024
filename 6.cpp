#include <sstream>
#include <map>
#include <set>
#include <vector>
#include <iostream>
#include <mutex>
#include <thread>
#include "helpers.h"

const std::pair<int, int> LEFT = std::make_pair(0, -1);
const std::pair<int, int> RIGHT = std::make_pair(0, 1);
const std::pair<int, int> UP = std::make_pair(-1, 0);
const std::pair<int, int> DOWN = std::make_pair(1, 0);
const std::pair<int, int> DIRECTIONS[] = {LEFT, RIGHT, UP, DOWN};

bool inBounds(const std::pair<int, int> position, const std::vector<std::vector<char> > &content) {
    return position.first >= 0 && position.first < content.size() && position.second >= 0 && position.second < content[0].size();
}

bool isBoundry(const std::pair<int, int> position, const std::vector<std::vector<char> > &content) {
    return content[position.first][position.second] == '#';
}

std::pair<int, int> getNewDirection(const std::pair<int, int> &direction) {
    if (direction == UP) return RIGHT;
    if (direction == DOWN) return LEFT;
    if (direction == RIGHT) return DOWN;
    if (direction == LEFT) return UP;

    return direction;
}

std::pair<int, int> getNewPosition(const std::pair<int, int> &position, const std::pair<int, int> &direction) {
    return std::make_pair(position.first + direction.first, position.second + direction.second);
}

bool simulateRoute(std::pair<int, int> position, const std::vector<std::vector<char>> &content, std::set<std::pair<int, int>> &seen) {
    std::pair<int, int> direction = UP;
    std::set<std::tuple<std::pair<int, int>, std::pair<int, int>>> pointDirectionMap;

    while (inBounds(position, content)) {
        if (pointDirectionMap.contains(std::make_tuple(position, direction))) return false;

        seen.insert(position);
        pointDirectionMap.emplace(position, direction);

        std::pair<int, int> newPosition = getNewPosition(position, direction);

        if (inBounds(newPosition, content) && isBoundry(newPosition, content)) {
            direction = getNewDirection(direction);
            continue;
        }

        position = newPosition;
    }

    return true;
}

void checkPoint(const std::pair<int, int> *point, const std::pair<int, int> *initialPosition, const std::vector<std::vector<char>> *content, std::mutex *answerMutex, int *answer) {
    println("{}", *point);
}

int main() {
    std::ifstream file = get_input("6.txt");
    std::string line;
    std::vector<std::vector<char> > content;
    std::map<std::pair<int, int>, std::vector<std::pair<int, int> > > graph;
    std::pair<int, int> position;
    std::pair<int, int> initialPosition;
    std::pair<int, int> direction = UP;

    while (getline(file, line)) {
        std::vector<char> characters;

        for (int i = 0; i < line.size(); i++) {
            characters.push_back(line[i]);

            if (line[i] == '^') {
                initialPosition = std::make_pair(content.size(), i);
                position = initialPosition;
            }
        }

        content.push_back(characters);
    }

    std::set<std::pair<int, int>> seen;
    simulateRoute(initialPosition, content, seen);
    std::mutex answerMutex;
    int answer = 0;

    for (auto point : seen) {
        if (point == initialPosition) continue;

        checkPoint(&point, &initialPosition, &content, &answerMutex, &answer);

        std::set<std::pair<int, int>> newSeen;

        if (!simulateRoute(initialPosition, content, newSeen)) {
        }

    }

    std::cout << seen.size() << std::endl;
    std::cout << answer << std::endl;
    return 0;
}