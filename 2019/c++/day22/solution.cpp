#include <boost/multiprecision/cpp_int.hpp>
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

using namespace boost::multiprecision;

int256_t pow(int256_t base, int256_t exponent, int256_t deckSize) {
    if (exponent == 0) {
        return 1;
    }

   if (exponent % 2 == 0) {
        return pow((base * base) % deckSize, exponent / 2, deckSize);
   } else {
        int256_t recur = pow((base * base) % deckSize, (exponent - 1) / 2, deckSize);
        return (base * recur) % deckSize;
   }
}

int main(int argc, char* argv[]) {
    std::string line;
    std::ifstream file("input.txt");

    int256_t deckSize = 119315717514047;    
    int256_t shuffles = 101741582076661;
    std::vector<std::string> instructions;

    int256_t firstValue = 0;
    int256_t increment = 1;
    if (file.is_open()) {
        while (getline(file, line)) {
            instructions.push_back(line);
        }
    }

    for (int i = 0; i < instructions.size(); i++) {
        std::string line = instructions[i];

        if (line == "deal into new stack") {
            increment = deckSize - increment;

            firstValue = (firstValue + increment) % deckSize;
        } else if (line[0] == 'c') {
            int256_t cutValue = stoi(line.substr(line.find(" ") + 1));

            if (cutValue > 0) {
                firstValue = (firstValue + (increment * cutValue)) % deckSize;
            } else {
                int256_t oppositeIncrement = deckSize - increment;
                firstValue = (firstValue + (oppositeIncrement * -cutValue)) % deckSize;
            }
        } else if (line[0] == 'd') {
            // deal with increment N
            int256_t numIncrement = stoi(line.substr(line.find("increment ") + 10));

            for (int256_t factor = 1; factor < deckSize; factor++) {
                int256_t check = factor * deckSize + 1;
                if (check % numIncrement == 0) {
                    increment = (increment * (check / numIncrement)) % deckSize;
                    break;
                }
            }
        }
    }

    // Part 1
    // for (int256_t i = 0; i < deckSize; i++) {
    //     int256_t currValue = (firstValue + i * increment) % deckSize;

    //     if (currValue == 2019) {
    //         std::cout << i << " " << currValue << std::endl;
    //         break;
    //     }
    // }

    // Part 2
    // Was able to figure out the zeroValueIndex + increment idea myself
    // Doing the N amount of shuffles, looked up Reddit hints
    int256_t finalIncrement = pow(increment, shuffles, deckSize);

    // Also got reddit hint/code example for calculating an inverse with modulo
    int256_t offsetDenom = pow(increment - 1, deckSize - 2, deckSize);
    int256_t finalFirstValue = (firstValue * ((finalIncrement - 1) * offsetDenom) % deckSize) % deckSize;

    std::cout << finalFirstValue << " " << finalIncrement << std::endl;
    std::cout << (finalFirstValue + (2020 * finalIncrement) % deckSize) % deckSize << std::endl;
}