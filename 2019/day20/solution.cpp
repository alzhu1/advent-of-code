#include <algorithm>
#include <deque>
#include <iostream>
#include <fstream>
#include <limits>
#include <queue>
#include <string>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "stdlib.h"

// This solution assumes a pre-processed input, rules for re-writing maps:
// A and Z is reserved for Start and End respectively
// All other letters and numbers are fair game

// https://stackoverflow.com/questions/2590677/how-do-i-combine-hash-values-in-c0x
template <class T>
inline void hash_combine(std::size_t& seed, const T& v) {
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
}

struct Point {
    Point(): x(0), y(0), level(0) {}
    Point(int16_t _x, int16_t _y, uint16_t _level): x(_x), y(_y), level(_level) {}
    int16_t x;
    int16_t y;
    uint16_t level;

    // Hash
    size_t operator() (const Point &p) const {
        size_t seed = 0;
        hash_combine<int16_t>(seed, x);
        hash_combine<int16_t>(seed, y);
        hash_combine<uint16_t>(seed, level);
        return seed;
    }

    bool operator == (const Point& p1) const {
        return x == p1.x && y == p1.y && level == p1.level;
    }

    bool operator != (const Point& p1) const {
        return x != p1.x || y != p1.y || level != p1.level;
    }
};

int main(int argc, char* argv[]) {
    std::string line;
    std::ifstream file("input.txt");

    std::unordered_set<Point, Point> walls;
    std::unordered_set<Point, Point> emptySpaces;

    std::unordered_map<char, std::vector<Point> > portalToPoints;
    std::unordered_map<Point, Point, Point> portalMap;

    Point currPoint;
    Point startPoint;
    Point endPoint;

    uint32_t maxX = 0;
    uint32_t maxY = 0;
    if (file.is_open()) {
        while (getline(file, line)) {
            maxX = line.length();
            for (int i = 0; i < line.length(); i++) {
                if (line[i] == '.') {
                    currPoint.x++;
                    continue;
                }

                if (line[i] == ' ') {
                    emptySpaces.insert(currPoint);
                } else if (line[i] == '#') {
                    walls.insert(currPoint);
                } else if (line[i] == 'A') {
                    startPoint = currPoint;
                } else if (line[i] == 'Z') {
                    endPoint = currPoint;
                } else {
                    // Portal point found
                    if (!portalToPoints.count(line[i])) {
                        std::vector<Point> points;
                        portalToPoints[line[i]] = points;
                    }
                    portalToPoints[line[i]].push_back(currPoint);
                }

                currPoint.x++;
            }
            currPoint.x = 0;
            currPoint.y++;
        }
    }
    maxY = currPoint.y;

    for (auto portalPair: portalToPoints) {
        std::vector<Point> points = portalPair.second;
        if (points.size() != 2) {
            std::cout << portalPair.first << ": ";
            for (auto p: points) {
                std::cout << "(" << p.x << ", " << p.y << ") ";
            }
            std::cout << ", something wrong with this point pair" << std::endl;
        }
        portalMap[points[0]] = points[1];
        portalMap[points[1]] = points[0];
    }

    std::unordered_map<Point, std::unordered_map<Point, uint32_t, Point>, Point> edgeGraph;
    std::unordered_set<Point, Point> visited;
    Point graphCheckPoint = startPoint;
    auto iter = portalMap.begin();

    uint32_t maxValue = -1;
    do {
        std::deque<std::pair<Point, uint32_t> > queue;
        queue.push_back(std::make_pair(graphCheckPoint, 0));

        // BFS from this point to build weighted edges
        while (queue.size() > 0) {
            auto pair = queue[0];
            queue.pop_front();

            Point checkPoint = pair.first;
            uint32_t steps = pair.second;

            if (checkPoint.x == maxValue || checkPoint.y == maxValue || checkPoint.x >= maxX || checkPoint.y >= maxY) {
                continue;
            }

            if (visited.count(checkPoint) || emptySpaces.count(checkPoint) || walls.count(checkPoint)) {
                continue;
            }
            visited.insert(checkPoint);

            if (checkPoint != graphCheckPoint && (checkPoint == endPoint || portalMap.count(checkPoint))) {
                // Valid destination found, mark down in map
                if (!edgeGraph.count(graphCheckPoint)) {
                    edgeGraph[graphCheckPoint] = std::unordered_map<Point, uint32_t, Point>();
                }

                edgeGraph[graphCheckPoint][checkPoint] = steps;
            }

            queue.push_back(std::make_pair(Point(checkPoint.x, checkPoint.y + 1, checkPoint.level), steps + 1));
            queue.push_back(std::make_pair(Point(checkPoint.x, checkPoint.y - 1, checkPoint.level), steps + 1));
            queue.push_back(std::make_pair(Point(checkPoint.x + 1, checkPoint.y, checkPoint.level), steps + 1));
            queue.push_back(std::make_pair(Point(checkPoint.x - 1, checkPoint.y, checkPoint.level), steps + 1));
        }

        // If graphCheckPoint is a portal point, also add its portal
        if (portalMap.count(graphCheckPoint)) {
            Point portalPoint = portalMap[graphCheckPoint];
            edgeGraph[graphCheckPoint][portalPoint] = 1;
            edgeGraph[portalPoint][graphCheckPoint] = 1;
        }

        // Reset params
        visited.clear();
        if (iter == portalMap.end()) {
            break;
        }
        graphCheckPoint = iter->first;
        iter++;
    } while (true);
    
    auto compare = [] (std::pair<Point, uint32_t> a, std::pair<Point, uint32_t> b) { return a.second > b.second; };
    std::priority_queue<std::pair<Point, uint32_t>, std::vector<std::pair<Point, uint32_t> >, decltype(compare)> pQueue(compare);
    pQueue.push(std::make_pair(startPoint, 0));

    while (pQueue.size() > 0) {
        auto pair = pQueue.top();
        pQueue.pop();

        Point checkPoint = pair.first;
        Point topLevelPoint(checkPoint.x, checkPoint.y, 0);
        uint32_t steps = pair.second;

        if (checkPoint.x == maxValue || checkPoint.y == maxValue || checkPoint.x >= maxX || checkPoint.y >= maxY) {
            continue;
        }

        if (checkPoint == endPoint) {
            std::cout << "Done at " << steps << " steps" << std::endl;
            break;
        }

        if (visited.count(checkPoint) || emptySpaces.count(topLevelPoint) || walls.count(topLevelPoint)) {
            continue;
        }
        visited.insert(checkPoint);

        // Look at edgeGraph to find connections, enqueue them
        std::unordered_map<Point, uint32_t, Point> edges = edgeGraph[topLevelPoint];
        for (auto edge: edges) {
            // First check if it's a portal
            if (portalMap.count(topLevelPoint) && portalMap[topLevelPoint] == edge.first) {
                bool isOuterPortal = (checkPoint.x == 0 || checkPoint.y == 0) || (checkPoint.x == maxX - 1 || checkPoint.y == maxY - 1);
                uint16_t nextLevel = isOuterPortal ? checkPoint.level - 1 : checkPoint.level + 1;

                // Portal access allowed at any level, unless level 0 and is outer
                if (checkPoint.level > 0 || !isOuterPortal) {
                    Point nextPoint(portalMap[topLevelPoint].x, portalMap[topLevelPoint].y, nextLevel);
                    pQueue.push(std::make_pair(nextPoint, steps + 1));
                }
            } else {
                Point nextPoint(edge.first.x, edge.first.y, checkPoint.level);
                pQueue.push(std::make_pair(nextPoint, steps + edge.second));
            }
        }
    }
}