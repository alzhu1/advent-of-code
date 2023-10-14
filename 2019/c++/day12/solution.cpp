#include <algorithm>
#include <iostream>
#include <fstream>
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
    Point(): x(0), y(0), z(0) {}
    Point(int32_t _x, int32_t _y, int32_t _z): x(_x), y(_y), z(_z) {}
    int32_t x;
    int32_t y;
    int32_t z;

    int32_t getEnergy() {
        return abs(x) + abs(y) + abs(z);
    }

    // Hash
    // size_t operator() (const Point &p) const {
    //     size_t seed = 0;
    //     hash_combine(seed, x);
    //     hash_combine(seed, y);
    //     hash_combine(seed, z);
    // }

    bool operator == (const Point& p1) const {
        return x == p1.x && y == p1.y && z == p1.z;
    }

    Point& operator += (const Point& p1) {
        x += p1.x;
        y += p1.y;
        z += p1.z;
        return *this;
    }

    Point& operator -= (const Point& p1) {
        x -= p1.x;
        y -= p1.y;
        z -= p1.z;
        return *this;
    }
};

uint64_t lcm(uint64_t x, uint64_t y) {
    uint64_t num = x * y;
    uint64_t gcd = 1;
    while (x > 0 && y > 0) {
        if (x > y) {
            x %= y;
        } else {
            y %= x;
        }
    }
    gcd = std::max(x, y);
    return num / gcd;
}

uint64_t lcm(uint64_t x, uint64_t y, uint64_t z) {
    return lcm(x, lcm(y, z));
}

int main(int argc, char* argv[]) {
    std::string line;
    std::ifstream file("input.txt");

    std::vector<Point> moons;
    std::vector<Point> velocities;

    // Modify input to just be x,y,z formatted
    if (file.is_open()) {
        std::vector<int64_t> nums;
        while (getline(file, line)) {
            std::stringstream ss(line);
            std::string word;

            while (!ss.eof()) {
                getline(ss, word, ',');
                nums.push_back(stoi(word));
            }

            moons.push_back(Point(nums[0], nums[1], nums[2]));
            velocities.push_back(Point());

            nums.clear();
        }
    }

    // Reddit hint: calculate repetitions of individual dimensions
    // As they are independent from each other
    // Then find the least common multiple
    std::unordered_map<size_t, uint64_t> seenX;
    std::unordered_map<size_t, uint64_t> seenY;
    std::unordered_map<size_t, uint64_t> seenZ;

    size_t seedX = 0;
    size_t seedY = 0;
    size_t seedZ = 0;
    for (int i = 0; i < moons.size(); i++) {
        hash_combine<int32_t>(seedX, moons[i].x);
        hash_combine<int32_t>(seedX, velocities[i].x);

        hash_combine<int32_t>(seedY, moons[i].y);
        hash_combine<int32_t>(seedY, velocities[i].y);

        hash_combine<int32_t>(seedZ, moons[i].z);
        hash_combine<int32_t>(seedZ, velocities[i].z);
    }
    seenX[seedX] = 0;
    seenY[seedY] = 0;
    seenZ[seedZ] = 0;

    int time = 0;

    uint64_t xRepeat = 0;
    uint64_t yRepeat = 0;
    uint64_t zRepeat = 0;
    while (!xRepeat || !yRepeat || !zRepeat) {
        for (int i = 0; i < moons.size(); i++) {
            for (int j = i + 1; j < moons.size(); j++) {
                int xVeloI = moons[i].x < moons[j].x ? 1 : (moons[i].x > moons[j].x ? -1 : 0);
                int yVeloI = moons[i].y < moons[j].y ? 1 : (moons[i].y > moons[j].y ? -1 : 0);
                int zVeloI = moons[i].z < moons[j].z ? 1 : (moons[i].z > moons[j].z ? -1 : 0);

                Point v(xVeloI, yVeloI, zVeloI);
                velocities[i] += v;
                velocities[j] -= v;
            }
        }

        for (int i = 0; i < moons.size(); i++) {
            moons[i] += velocities[i];
        }

        time++;

        seedX = 0;
        seedY = 0;
        seedZ = 0;
        for (int i = 0; i < moons.size(); i++) {
            hash_combine<int32_t>(seedX, moons[i].x);
            hash_combine<int32_t>(seedX, velocities[i].x);

            hash_combine<int32_t>(seedY, moons[i].y);
            hash_combine<int32_t>(seedY, velocities[i].y);

            hash_combine<int32_t>(seedZ, moons[i].z);
            hash_combine<int32_t>(seedZ, velocities[i].z);
        }

        if (!xRepeat && seenX.count(seedX)) {
            std::cout << "X repeats: start t = " << seenX[seedX] << ", end t = " << time << std::endl;
            xRepeat = time - seenX[seedX];
        }
        seenX[seedX] = time;

        if (!yRepeat && seenY.count(seedY)) {
            std::cout << "Y repeats: start t = " << seenY[seedY] << ", end t = " << time << std::endl;
            yRepeat = time - seenY[seedY];
        }
        seenY[seedY] = time;

        if (!zRepeat && seenZ.count(seedZ)) {
            std::cout << "Z repeats: start t = " << seenZ[seedZ] << ", end t = " << time << std::endl;
            zRepeat = time - seenZ[seedZ];
        }
        seenZ[seedZ] = time;
    }

    // Part 1
    // int32_t energy = 0;
    // for (int i = 0; i < moons.size(); i++) {
    //     energy += (moons[i].getEnergy() * velocities[i].getEnergy());
    // }

    // std::cout << energy << std::endl;

    // Part 2
    std::cout << "Repeats at t = " << lcm(xRepeat, yRepeat, zRepeat) << std::endl;
}