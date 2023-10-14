#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct Point {
    Point(): x(0), y(0) {}
    Point(double _x, double _y): x(_x), y(_y) {}
    uint16_t x;
    uint16_t y;

    uint32_t manhattanDistance() const {
        return (uint32_t)x + (uint32_t)y;
    }

    // Hash
    size_t operator() (const Point &p) const {
        return ((size_t)p.x << 32) | p.y;
    }

    bool operator == (const Point& p1) const {
        return x == p1.x && y == p1.y;
    }
};

// Output stream
std::ostream& operator << (std::ostream &os, const Point &p) {
    os << "(" << p.x << "," << p.y << ")";
    return os;
}

void processPipes(std::string line,
                  std::unordered_map<Point, uint32_t, Point>& firstPipeSteps,
                  std::unordered_map<Point, uint32_t, Point>& intersections,
                  bool checkIntersections) {
    Point currPoint;
    std::stringstream ss(line);
    std::string word;

    uint32_t steps = 0;
    while (!ss.eof()) {
        getline(ss, word, ',');
        int amount = stoi(word.substr(1));
        while (amount > 0) {
            switch (word[0]) {
                case 'U': {
                    currPoint.y++;
                    break;
                }

                case 'D': {
                    currPoint.y--;
                    break;
                }

                case 'R': {
                    currPoint.x ++;
                    break;
                }

                case 'L': {
                    currPoint.x--;
                    break;
                }

                default: {
                    break;
                }
            }
            steps++;

            if (!checkIntersections) {
                // Only update firstPipeSteps if not checking intersections
                if (!firstPipeSteps.count(currPoint)) {
                    firstPipeSteps[currPoint] = steps;
                }
            } else {
                // If checking for intersections, update intersections with collision
                if (firstPipeSteps.count(currPoint) && !intersections.count(currPoint)) {
                    intersections[currPoint] = firstPipeSteps[currPoint] + steps;
                }
            }
            // if (checkIntersections && pipes.count(currPoint)) {
            //     // Add sum of steps

            // } else {
            //     pipes.insert(currPoint);
            // }
            amount--;
        }
    }
}

int main(int argc, char* argv[]) {
    std::string line;
    std::string line2;
    std::ifstream file("input.txt");

    std::unordered_map<Point, uint32_t, Point> firstPipeSteps;
    std::unordered_map<Point, uint32_t, Point> intersections;
    Point currPoint;
    firstPipeSteps[currPoint] = 0;

    if (file.is_open()) {
        file >> line;
        file >> line2;
        std::cout << line2 << std::endl;
        file.close();
    }

    processPipes(line, firstPipeSteps, intersections, false);
    processPipes(line2, firstPipeSteps, intersections, true);

    std::cout << firstPipeSteps.size() << " " << intersections.size() << std::endl;

    Point bestManhattanPoint(0xffff, 0xffff);
    Point bestPoint(0xffff, 0xffff);
    uint32_t bestSteps = 0xffffffff;
    for (auto pair: intersections) {
        if (pair.first.manhattanDistance() < bestManhattanPoint.manhattanDistance()) {
            bestManhattanPoint = pair.first;
        }

        if (pair.second < bestSteps) {
            bestPoint = pair.first;
            bestSteps = pair.second;
        }
    }
    std::cout << bestManhattanPoint << ", Manhattan = " << bestManhattanPoint.manhattanDistance() << std::endl;
    std::cout << bestPoint << ", steps = " << bestSteps << std::endl;
}