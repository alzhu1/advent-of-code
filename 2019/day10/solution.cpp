#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct Slope {
    Slope(): num(1), denom(1) {}
    Slope(int16_t _num, int16_t _denom): num(_num), denom(_denom) {}
    int16_t num;
    int16_t denom;

    std::vector<Slope> getSlopeVariations() {
        std::vector<Slope> slopeVariations;
        slopeVariations.push_back(Slope(num, denom));
        slopeVariations.push_back(Slope(-num, -denom));
        return slopeVariations;
    }

    bool operator == (const Slope& s) const {
        // Slopes are equal if ratios match
        return (num * s.denom) == (s.num * denom);
    }
};

struct Point {
    Point(): x(0), y(0) {}
    Point(int16_t _x, int16_t _y): x(_x), y(_y) {}
    int16_t x;
    int16_t y;

    // Hash
    size_t operator() (const Point &p) const {
        return ((size_t)p.x << 32) | p.y;
    }

    bool operator == (const Point& p1) const {
        return x == p1.x && y == p1.y;
    }
};

bool compareSlope(const Slope &a, const Slope &b) {
    // a goes first if its denominator is 0 and numerator is neg (that means up)
    if (a.denom == 0) {
        return a.num < 0;
    }

    // Same check for b (b goes first if it's pointing up)
    if (b.denom == 0) {
        return b.num > 0;
    }

    // Otherwise, do a float check
    double aRatio = (double)a.num / (double)a.denom;
    double bRatio = (double)b.num / (double)b.denom;
    return aRatio < bRatio;
}

int main(int argc, char* argv[]) {
    std::string line;
    std::ifstream file("input.txt");

    std::unordered_set<Point, Point> asteroids;
    std::vector<Slope> possibleSlopes;
    int gridWidth = 0;
    int gridHeight = 0;

    if (file.is_open()) {
        while (getline(file, line)) {
            for (int i = 0; i < line.length(); i++) {
                if (line[i] == '#') {
                    // Asteroid found
                    asteroids.insert(Point(i, gridHeight));
                }
            }

            gridWidth = line.length();
            gridHeight++;
        }
    }

    // Calculate all possible slopes
    possibleSlopes.push_back(Slope(0, 1));
    possibleSlopes.push_back(Slope(1, 0));
    for (int dy = 1; dy < gridHeight; dy++) {
        for (int dx = dy; dx < gridWidth; dx++) {
            Slope slope(dy, dx);

            bool shouldAdd = true;
            for (auto s: possibleSlopes) {
                if (s == slope) {
                    shouldAdd = false;
                    break;
                }
            }

            if (shouldAdd) {
                possibleSlopes.push_back(slope);

                // Also add the slope as reflected over x axis (e.g. negative dy)
                possibleSlopes.push_back(Slope(-dy, dx));

                // And add reciprocals (if not same)
                if (dx != dy) {
                    possibleSlopes.push_back(Slope(dx, dy));
                    possibleSlopes.push_back(Slope(-dx, dy));
                }
            }
        }
    }

    // Idea is to look at each asteroid and check possibles with slopes
    // There are 2 variations on slopes: reciprocals + negatives
    std::unordered_map<Point, uint16_t, Point> detectableCount;

    for (auto asteroid: asteroids) {
        detectableCount[asteroid] = 0;

        for (auto slope: possibleSlopes) {
            std::vector<Slope> slopeVariations = slope.getSlopeVariations();

            for (auto slopeVar: slopeVariations) {
                Point currPoint(asteroid.x, asteroid.y);

                while (currPoint.x >= 0 && currPoint.y >= 0 && currPoint.x < gridWidth && currPoint.y < gridHeight) {
                    currPoint.y += slopeVar.num;
                    currPoint.x += slopeVar.denom;

                    if (asteroids.count(currPoint)) {
                        detectableCount[asteroid]++;
                        break;
                    }
                }
            }
        }
    }

    Point point;
    uint16_t bestCount = 0;
    for (auto pair: detectableCount) {
        if (pair.second > bestCount) {
            bestCount = pair.second;
            point = pair.first;
        }
    }

    std::cout << point.x << "," << point.y << ", " << bestCount << std::endl;

    // Add the upward slope, sort slopes based on ratio
    possibleSlopes.push_back(Slope(-1, 0));
    std::sort(possibleSlopes.begin(), possibleSlopes.end(), compareSlope);

    bool goingBackwards = false;
    int slopeIndex = 0;
    int destroyCount = 0;
    while (destroyCount < 200) {
        // Simulate by using slopes in ascending/descending order (emulates a rotation)
        Slope currSlope = possibleSlopes[slopeIndex];
        if (goingBackwards) {
            currSlope.denom *= -1;
        }
        Point currPoint(point.x, point.y);

        while (currPoint.x >= 0 && currPoint.y >= 0 && currPoint.x < gridWidth && currPoint.y < gridHeight) {
            currPoint.y += currSlope.num;
            currPoint.x += currSlope.denom;

            if (asteroids.count(currPoint)) {
                asteroids.erase(currPoint);
                destroyCount++;
                std::cout << "Destroyed asteroid " << destroyCount << " at (" << currPoint.x << "," << currPoint.y << ")" << std::endl;
                break;
            }
        }

        // Check if we need to start/stop going backwards (indicates half-rotation is completed)
        if (!goingBackwards) {
            if (slopeIndex == possibleSlopes.size() - 1) {
                goingBackwards = true;
                slopeIndex--;
            } else {
                slopeIndex++;
            }
        } else {
            if (slopeIndex == 0) {
                goingBackwards = false;
                slopeIndex++;
            } else {
                slopeIndex--;
            }
        }
    }
}