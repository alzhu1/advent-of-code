#include <algorithm>
#include <deque>
#include <iostream>
#include <fstream>
#include <limits>
#include <string>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "stdlib.h"

// https://stackoverflow.com/questions/2590677/how-do-i-combine-hash-values-in-c0x
template <class T>
inline void hash_combine(std::size_t& seed, const T& v) {
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
}

struct Point {
    Point(): x(0), y(0) {}
    Point(int16_t _x, int16_t _y): x(_x), y(_y) {}
    Point(int16_t _x, int16_t _y, uint32_t _keys): x(_x), y(_y) {}
    int16_t x;
    int16_t y;

    // Hash
    size_t operator() (const Point &p) const {
        size_t seed = 0;
        hash_combine<int16_t>(seed, x);
        hash_combine<int16_t>(seed, y);
        return seed;
    }

    bool operator == (const Point& p1) const {
        return x == p1.x && y == p1.y;
    }
};

// Part 1 took 81 minutes with O2 on lol
uint32_t search(Point startingLocation,
                uint32_t currKeys,
                uint32_t startingSteps,
                std::unordered_set<Point, Point>& walls,
                std::unordered_map<Point, char, Point>& keyLocations,
                std::unordered_map<Point, char, Point>& doorLocations,
                std::unordered_map<uint64_t, uint32_t>& cache,
                uint32_t bestSteps) {
    // Calculate hash: point + curr keys
    uint64_t hashable = currKeys;
    hashable |= ((uint64_t)startingLocation.x) << 48;
    hashable |= ((uint64_t)startingLocation.y) << 32;

    if (cache.count(hashable) && cache[hashable] <= startingSteps) {
        // Don't explore this path, return max number to avoid false positives
        return std::numeric_limits<uint32_t>::max();
    }

    // Track our current amount
    cache[hashable] = startingSteps;

    uint32_t keyMax = (1 << keyLocations.size()) - 1;
    if (currKeys == keyMax) {
        std::cout << "Finished a path with steps " << startingSteps << ", cache size " << cache.size() << std::endl;
        return startingSteps;
    }

    // Start at currLocation, do BFS
    std::unordered_set<Point, Point> visited;
    std::deque<std::pair<Point, uint32_t>> queue;
    queue.push_back(std::make_pair(startingLocation, startingSteps));
    bool didFirst = false;
    while (queue.size() > 0) {
        auto item = queue[0];
        queue.pop_front();

        Point currLocation = item.first;
        uint32_t steps = item.second;

        if (steps >= bestSteps) {
            continue;
        }

        hashable = currKeys;
        hashable |= ((uint64_t)currLocation.x) << 48;
        hashable |= ((uint64_t)currLocation.y) << 32;
        if (didFirst && cache.count(hashable) && cache[hashable] <= steps) {
            // Don't explore this path
            continue;
        }
        cache[hashable] = steps;
        didFirst = true;

        // Skip if visited or if this is a wall
        if (visited.count(currLocation) || walls.count(currLocation)) {
            continue;
        }

        // Check if this is a door and you have no key
        if (doorLocations.count(currLocation)) {
            uint32_t shiftAmount = doorLocations[currLocation] - 'a';
            uint32_t doorCheck = 1 << shiftAmount;
            if ((currKeys & doorCheck) == 0) {
                continue;
            }
        }
        
        // If this is a key we haven't picked up, keep track of it
        if (keyLocations.count(currLocation)) {
            uint32_t shiftAmount = keyLocations[currLocation] - 'a';
            uint32_t keyCheck = 1 << shiftAmount;

            // Only do DFS if this is a new key
            if ((currKeys & keyCheck) == 0) {
                uint32_t nextKeys = currKeys | keyCheck;
                uint32_t resultSteps = search(currLocation, nextKeys, steps, walls, keyLocations, doorLocations, cache, bestSteps);
                bestSteps = std::min(bestSteps, resultSteps);
            }
        }

        visited.insert(currLocation);

        // Enqueue neighbors
        queue.push_back(std::make_pair(Point(currLocation.x, currLocation.y + 1), steps + 1));
        queue.push_back(std::make_pair(Point(currLocation.x, currLocation.y - 1), steps + 1));
        queue.push_back(std::make_pair(Point(currLocation.x + 1, currLocation.y), steps + 1));
        queue.push_back(std::make_pair(Point(currLocation.x - 1, currLocation.y), steps + 1));
    }

    return bestSteps;
}

int main(int argc, char* argv[]) {
    std::string line;
    std::ifstream file("input.txt");

    std::unordered_set<Point, Point> walls;
    std::unordered_map<Point, char, Point> keyLocations;
    std::unordered_map<Point, char, Point> doorLocations;
    std::unordered_map<uint64_t, uint32_t> cache;

    Point currPoint;
    Point startPoint;
    if (file.is_open()) {
        while (getline(file, line)) {
            for (int i = 0; i < line.length(); i++) {
                if (line[i] == '#') {
                    walls.insert(currPoint);
                } else if (line[i] == '@') {
                    startPoint = currPoint;
                } else if (line[i] >= 'a' && line[i] <= 'z') {
                    keyLocations[currPoint] = line[i];
                } else if (line[i] >= 'A' && line[i] <= 'Z') {
                    doorLocations[currPoint] = line[i] + 0x20;
                }

                currPoint.x++;
            }
            currPoint.x = 0;
            currPoint.y++;
        }
    }

    std::cout << search(startPoint, 0, 0, walls, keyLocations, doorLocations, cache, std::numeric_limits<uint32_t>::max()) << std::endl;
}