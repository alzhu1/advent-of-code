#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

int main(int argc, char* argv[]) {
    uint32_t count = 0;
    for (uint32_t i = 254032; i < 789860; i++) {
        // Check doubles starting at end
        uint32_t check = i;

        bool possible = true;
        uint32_t multipleToAdd = 1;
        uint32_t finalMultiple = 1;
        while (check >= 10) {
            int currDigit = check % 10;
            int prevDigit = (check / 10) % 10;

            if (prevDigit > currDigit) {
                possible = false;
                finalMultiple = multipleToAdd;
            }

            check /= 10;
            multipleToAdd *= 10;
        }

        if (!possible) {
            i += (finalMultiple - 1);
            continue;
        }

        // Check double (double only)
        bool doubleFound = false;
        uint32_t divisor = 1;
        while (i / divisor >= 10) {
            uint32_t curr = i / divisor;
            int currDigit = curr % 10;
            int prevDigit = (curr / 10) % 10;

            if (currDigit == prevDigit) {
                // Double found, now check if it's part of a group (check prev prev and next)
                // 0 is safe for edges because 0 on either side is invalid based on rules
                int prevPrevDigit = (curr / 100) % 10;
                int nextDigit = divisor == 1 ? 0 : (i / (divisor / 10)) % 10;
                if (prevPrevDigit != currDigit && nextDigit != currDigit) {
                    doubleFound = true;
                }
            }

            divisor *= 10;
        }

        if (doubleFound) {
            count++;
        }
    }

    std::cout << count << std::endl;
}