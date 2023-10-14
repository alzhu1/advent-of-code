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

int main(int argc, char* argv[]) {
    std::string line;
    std::ifstream file("input.txt");

    uint32_t value = 0;

    std::vector<char> chars;
    if (file.is_open()) {
        while (getline(file, line)) {
            for (int i = 0; i < 5; i++) {
                chars.push_back(line[i]);
            }
        }
    }

    for (int i = 0; i < chars.size(); i++) {
        if (chars[i] == '#') {
            value |= (1 << i);
        }
    }

    uint32_t startValue = value;
    std::unordered_set<uint32_t> cache;
    cache.insert(value);

    // Part 1
    while (true) {
        uint32_t tempValue = 0;
        for (int i = 0; i < 25; i++) {
            bool currValue = value & (1 << i);
            int neighbors = 0;

            // Right
            if ((i % 5) != 4 && value & (1 << (i + 1))) {
                neighbors++;
            }

            // Left
            if ((i % 5) > 0 && (value & (1 << (i - 1)))) {
                neighbors++;
            }

            // Down
            if (value & (1 << (i + 5))) {
                neighbors++;
            }

            // Up
            if (i >= 5 && (value & (1 << (i - 5)))) {
                neighbors++;
            }

            if (currValue && neighbors != 1) {
                tempValue &= (~(1 << i));
            } else if (!currValue && (neighbors == 1 || neighbors == 2)) {
                tempValue |= (1 << i);
            } else {
                tempValue |= ((currValue ? 1 : 0) << i);
            }
        }

        value = tempValue;
        if (cache.count(value)) {
            std::cout << "Double found: " << value << std::endl;
            break;
        }
        cache.insert(value);
    }

    // Part 2: Need to keep track of bugs on recursive levels
    std::unordered_map<int32_t, uint32_t> levelToGrid;
    int32_t minLevel = -1;
    int32_t maxLevel = 1;

    for (int i = -250; i <= 250; i++) {
        levelToGrid[i] = 0;
    }
    levelToGrid[0] = startValue;

    int minutes = 0;
    while (minutes < 200) {
        std::unordered_map<int32_t, uint32_t> levelToGridCopy(levelToGrid);

        for (int level = minLevel; level <= maxLevel; level++) {
            // Process in order of level
            uint32_t levelValue = levelToGrid[level];
            uint32_t tempValue = levelValue;

            uint32_t outerGrid = levelToGrid[level - 1];
            uint32_t innerGrid = levelToGrid[level + 1];

            for (int i = 0; i < 25; i++) {
                if (i == 12) {
                    // Skip the center cell
                    continue;
                }
                bool currValue = levelValue & (1 << i);
                int neighbors = 0;

                // Right
                if (i % 5 == 4) {
                    // On the right border, check the outer layer (level - 1)
                    if (outerGrid & (1 << 13)) {
                        neighbors++;
                    }
                } else if (i == 11) {
                    // Right is the center cell, need to check all values on the left side
                    for (int j = 0; j < 5; j++) {
                        if (innerGrid & (1 << (j * 5))) {
                            neighbors++;
                        }
                    }
                } else if (levelValue & (1 << (i + 1))) {
                    // For any other cell, check normally
                    neighbors++;
                }

                // Left
                if (i % 5 == 0) {
                    // On the left border, check the outer layer (level - 1)
                    if (outerGrid & (1 << 11)) {
                        neighbors++;
                    }
                } else if (i == 13) {
                    // Left is the center cell, need to check all values on the right side
                    for (int j = 0; j < 5; j++) {
                        if (innerGrid & (1 << ((j * 5) + 4))) {
                            neighbors++;
                        }
                    }
                } else if (levelValue & (levelValue & (1 << (i - 1)))) {
                    // For any other cell, check normally
                    neighbors++;
                }

                // Down
                if (i > 19) {
                    // On the bottom border, check the outer layer (level - 1)
                    if (outerGrid & (1 << 17)) {
                        neighbors++;
                    }
                } else if (i == 7) {
                    // Down is the center cell, need to check all values on the upper side
                    for (int j = 0; j < 5; j++) {
                        if (innerGrid & (1 << j)) {
                            neighbors++;
                        }
                    }
                } else if (levelValue & (1 << (i + 5))) {
                    // For any other cell, check normally
                    neighbors++;
                }

                // Up
                if (i < 5) {
                    // On the top border, check the outer layer (level - 1)
                    if (outerGrid & (1 << 7)) {
                        neighbors++;
                    }
                } else if (i == 17) {
                    // Up is the center cell, need to check all values on the bottom side
                    for (int j = 0; j < 5; j++) {
                        if (innerGrid & (1 << (24 - j))) {
                            neighbors++;
                        }
                    }
                } else if (levelValue & (1 << (i - 5))) {
                    // For any other cell, check normally
                    neighbors++;
                }

                if (currValue && neighbors != 1) {
                    tempValue &= (~(1 << i));
                } else if (!currValue && (neighbors == 1 || neighbors == 2)) {
                    tempValue |= (1 << i);
                } else {
                    tempValue |= ((currValue ? 1 : 0) << i);
                }
            }

            levelToGridCopy[level] = tempValue;
        }

        levelToGrid = levelToGridCopy;
        minutes++;
        minLevel--;
        maxLevel++;
    }

    int bugCount = 0;
    for (int level = minLevel; level <= maxLevel; level++) {
        uint32_t value = levelToGrid[level];
        for (int i = 0; i < 25; i++) {
            if (value & (1 << i)) {
                bugCount++;
            }
        }
    }
    std::cout << bugCount << std::endl;
}