#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

int simulateIntcode(std::vector<int> nums) {
    // Instruction pointer starts at 1
    int pc = 0;

    while (pc < nums.size()) {
        int opcode = nums[pc];
        if (opcode == 99) {
            // Halt instruction
            // std::cout << "Done" << std::endl;
            break;
        }

        // Get params
        int input1 = nums[pc + 1];
        int input2 = nums[pc + 2];
        int output = nums[pc + 3];

        if (opcode == 1) {
            nums[output] = nums[input1] + nums[input2];
        } else if (opcode == 2) {
            nums[output] = nums[input1] * nums[input2];
        } else {
            std::cout << "Invalid instruction" << std::endl;
            break;
        }

        // Increment instruction pointer by num of opcode + params
        pc += 4;
    }

    return nums[0];
}

int main(int argc, char* argv[]) {
    std::string line;
    std::ifstream file("input.txt");

    if (file.is_open()) {
        file >> line;
        file.close();
    }

    std::vector<int> nums;

    std::stringstream ss(line);
    std::string word;
    while (!ss.eof()) {
        getline(ss, word, ',');
        nums.push_back(stoi(word));
    }

    std::cout << nums.size() << std::endl;

    // Part 1
    nums[1] = 12;
    nums[2] = 2;

    std::cout << "At pos 0: " << simulateIntcode(nums) << std::endl;

    for (int noun = 0; noun < 100; noun++) {
        for (int verb = 0; verb < 100; verb++) {
            nums[1] = noun;
            nums[2] = verb;
            int ret0 = simulateIntcode(nums);

            if (ret0 == 19690720) {
                std::cout << "Found it: noun = " << noun << ", verb = " << verb << std::endl;
                std::cout << "100 * noun + verb = " << (100 * noun + verb) << std::endl;
                goto done;
            }
        }
    }

done:
    std::cout << "Done" << std::endl;
}