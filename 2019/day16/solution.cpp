#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "stdlib.h"

int main(int argc, char* argv[]) {
    std::string line;
    std::ifstream file("input.txt");

    std::vector<int32_t> nums;
    if (file.is_open()) {
        file >> line;
        for (int i = 0; i < line.length(); i++) {
            nums.push_back(line[i] - '0');
        }
    }

    std::vector<int32_t> p2Nums(nums);

    int iters = 0;
    while (iters++ < 0) {
        std::vector<int32_t> nextNums;
        for (int i = 0; i < nums.size(); i++) {
            int32_t currValue = 0;
            int modValue = i + 1;

            int modIndex = 0;
            int index = i;
            bool positive = true;
            while (index < nums.size()) {
                // Starting at index, multiplier is 1
                currValue += positive ? nums[index] : -nums[index];

                modIndex++;
                if (modIndex % modValue == 0) {
                    modIndex = 0;
                    index += (modValue + 1);
                    positive = !positive;
                } else {
                    index++;
                }
            }

            if (currValue < 0) {
                currValue = -currValue;
            }
            nextNums.push_back(currValue % 10);
        }
        nums = nextNums;
    }

    // Reddit hint: since the 7 digit number (5970417 for me) is
    //   greater than half of the end size of 650 * 10000,
    //   and the second half of the FFT is always done with a consecutive sum,
    //   can just calculate the end (650 * 10000 - 5970417) digits of a FFT, and repeat

    // Part 2
    uint32_t digitTarget = 0;
    for (int i = 0; i < 7; i++) {
        digitTarget += p2Nums[i];
        if (i < 6) {
            digitTarget *= 10;
        }
    }
    uint32_t totalBackwards = (p2Nums.size() * 10000) - digitTarget + 1;
    int lastIndex = p2Nums.size() - 1;
    while (p2Nums.size() < totalBackwards) {
        p2Nums.insert(p2Nums.begin(), p2Nums[lastIndex]);
    }

    iters = 0;
    while (iters++ < 100) {
        uint32_t endIndex = p2Nums.size() - 1;
        uint32_t sum = p2Nums[endIndex--];

        while (endIndex > 0) {
            sum += p2Nums[endIndex];
            p2Nums[endIndex--] = sum % 10;
        }
    }

    int i = 1;
    while (i < 9) {
        std::cout << p2Nums[i];
        i++;
    }
    std::cout << std::endl;
}