#include <sstream>
#include <iostream>
#include <map>
#include <set>
#include <unordered_set>

#include "helpers.h"

struct Point {
    int x;
    int y;

    Point(int x = 0, int y = 0) : x(x), y(y) {
    };

    auto operator<=>(const Point &other) const {
        if (y != other.y)
            return y <=> other.y;
        return x <=> other.x;
    }

    bool operator==(const Point &point) const { return point.x == x && point.y == y; }
};

namespace std {
    template <>
    struct hash<Point> {
        std::size_t operator()(const Point& point) const noexcept {
            std::size_t hx = std::hash<int>{}(point.x);
            std::size_t hy = std::hash<int>{}(point.y);
            return hx ^ (hy << 1); // A simple combination of the two hashes
        }
    };
}

struct PointPair {
    Point first;
    Point second;

    PointPair() : first(Point()), second(Point()) {}
    PointPair(const Point first, const Point second): first(first), second(second) {
    }

    auto operator==(const PointPair &other) const {
        return (other.first.x == first.x && other.first.y == first.y &&
                other.second.x == second.x && other.second.y == second.y) ||
               (other.first.x == second.x && other.first.y == second.y && other.second.x == first.x && other.second.y ==
                first.y);
    }
};

namespace std {
    template <>
    struct hash<PointPair> {
        std::size_t operator()(const PointPair& pair) const noexcept {
            std::size_t h1 = std::hash<Point>{}(pair.first);
            std::size_t h2 = std::hash<Point>{}(pair.second);
            return h1 ^ h2;
        }
    };
}

std::ostream &operator<<(std::ostream &os, const Point &point) {
    return os << "(" << point.x << "," << point.y << ")";
}

bool inBounds(Point point, std::vector<std::string> &grid) {
    return point.y >= 0 && point.y < grid.size() && point.x >= 0 && point.x < grid[0].size();
}

std::vector<Point> getAntennas(Point first, Point second, std::vector<std::string> &grid, bool explode = false) {
    double slope = (static_cast<double>(first.y - second.y)) / (static_cast<double>(first.x - second.x)) * -1;

    int xDistance = abs(second.x - first.x);
    int yDistance = abs(second.y - first.y);

    int negativeBack[] = {-xDistance, -yDistance};
    int negativeForward[] = {xDistance, yDistance};

    int positiveBack[] = {-xDistance, yDistance};
    int positiveForward[]{xDistance, -yDistance};

    std::vector<Point> points;

    Point one = first < second ? first : second;
    Point two = first < second ? second : first;

    if (explode) {
        points.push_back(one);
        points.push_back(two);
    }

    while (inBounds(one, grid) || inBounds(two, grid)) {
        if (slope < 0) {
            one.x = one.x + negativeBack[0];
            one.y = one.y + negativeBack[1];
            two.x = two.x + negativeForward[0];
            two.y = two.y + negativeForward[1];
        } else {
            one.x = one.x + positiveForward[0];
            one.y = one.y + positiveForward[1];
            two.x = two.x + positiveBack[0];
            two.y = two.y + positiveBack[1];
        }

        if (inBounds(one, grid)) points.push_back(one);
        if (inBounds(two, grid)) points.push_back(two);

        if (!explode) {
            break;
        }
    }

    return points;
}


int main() {
    std::ifstream file = get_input("8.txt");
    std::string line;
    std::vector<std::string> grid;
    std::map<char, std::vector<Point>> antennaMap;

    while (getline(file, line)) {
        for (int i = 0; i < line.size(); i++) {
            if (line[i] != '.') {
                antennaMap[line[i]].emplace_back(i, grid.size());
            }
        }

        grid.push_back(line);
    }

    std::unordered_set<PointPair> seen;
    std::set<Point> answer[2];

    for (const auto &pair: antennaMap) {
        for (auto point: pair.second) {
            for (auto otherPoint: pair.second) {
                if (otherPoint == point) continue;
                if (seen.contains(PointPair(point, otherPoint))) continue;

                auto antennas = getAntennas(point, otherPoint, grid);

                for (auto antenna: antennas) {
                    answer[0].insert(antenna);
                }

                antennas = getAntennas(point, otherPoint, grid, true);

                for (auto antenna: antennas) {
                    answer[1].insert(antenna);
                }

                seen.emplace(point, otherPoint);
            }
        }
    }

    std::cout << answer[0].size() << std::endl;
    std::cout << answer[1].size() << std::endl;

    return 0;
}
