#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <tuple>
#include <vector>

std::tuple<std::vector<uint64_t>, long, int> simulateIntcode(std::vector<uint64_t> program, uint64_t phase, uint64_t input, int pc, bool readInput) {
    // Instruction pointer starts at 1
    // int pc = 0;

    while (pc < program.size()) {
        uint64_t opcode = program[pc];
        if (opcode == 99) {
            // Halt instruction
            // std::cout << "Done" << std::endl;
            break;
        }

        int opType = opcode % 100;
        int pcMovement = 1;
        bool shouldExit = false;

        // std::cout << "Opcode: " << opcode << ", position: " << pc << std::endl;

        switch (opType) {
            case 1: {
                // Add
                int32_t input1 = (opcode / 100) % 10 == 0 ? program[program[pc + 1]] : program[pc + 1];
                int32_t input2 = (opcode / 1000) % 10 == 0 ? program[program[pc + 2]] : program[pc + 2];
                int32_t outputAddress = program[pc + 3];

                // std::cout << input1 << " + " << input2 << " = " << (input1 + input2) << ", store at address " << outputAddress << std::endl;

                program[outputAddress] = input1 + input2;
                pcMovement = 4;
                break;
            }

            case 2: {
                // Add
                int32_t input1 = (opcode / 100) % 10 == 0 ? program[program[pc + 1]] : program[pc + 1];
                int32_t input2 = (opcode / 1000) % 10 == 0 ? program[program[pc + 2]] : program[pc + 2];
                int32_t outputAddress = program[pc + 3];

                // std::cout << input1 << " * " << input2 << " = " << (input1 * input2) << ", store at address " << outputAddress << std::endl;

                program[outputAddress] = input1 * input2;
                pcMovement = 4;
                break;
            }

            case 3: {
                // std::string n;
                // std::cout << "Add input: ";
                // std::cin >> n;

                // program[program[pc + 1]] = stoi(n);
                // For this problem, take input from params
                if (!readInput) {
                    program[program[pc + 1]] = phase;
                    readInput = true;
                } else {
                    program[program[pc + 1]] = input;
                }

                pcMovement = 2;
                break;
            }

            case 4: {
                // std::cout << "Output at " << program[pc + 1] << " = " << program[program[pc + 1]] << std::endl;
                pcMovement = 2;
                // break;
                return std::make_tuple(program, program[program[pc + 1]], pc + pcMovement);
            }

            case 5: {
                int32_t checkParam = (opcode / 100) % 10 == 0 ? program[program[pc + 1]] : program[pc + 1];
                int32_t pcSet = (opcode / 1000) % 10 == 0 ? program[program[pc + 2]] : program[pc + 2];

                if (checkParam != 0) {
                    pc = pcSet;
                    pcMovement = 0;
                } else {
                    pcMovement = 3;
                }
                break;
            }

            case 6: {
                int32_t checkParam = (opcode / 100) % 10 == 0 ? program[program[pc + 1]] : program[pc + 1];
                int32_t pcSet = (opcode / 1000) % 10 == 0 ? program[program[pc + 2]] : program[pc + 2];

                if (checkParam == 0) {
                    pc = pcSet;
                    pcMovement = 0;
                } else {
                    pcMovement = 3;
                }
                break;
            }

            case 7: {
                // Add
                int32_t input1 = (opcode / 100) % 10 == 0 ? program[program[pc + 1]] : program[pc + 1];
                int32_t input2 = (opcode / 1000) % 10 == 0 ? program[program[pc + 2]] : program[pc + 2];
                int32_t outputAddress = program[pc + 3];

                // std::cout << input1 << " + " << input2 << " = " << (input1 + input2) << ", store at address " << outputAddress << std::endl;

                program[outputAddress] = input1 < input2 ? 1 : 0;
                pcMovement = 4;
                break;
            }

            case 8: {
                // Add
                int32_t input1 = (opcode / 100) % 10 == 0 ? program[program[pc + 1]] : program[pc + 1];
                int32_t input2 = (opcode / 1000) % 10 == 0 ? program[program[pc + 2]] : program[pc + 2];
                int32_t outputAddress = program[pc + 3];

                // std::cout << input1 << " + " << input2 << " = " << (input1 + input2) << ", store at address " << outputAddress << std::endl;

                program[outputAddress] = input1 == input2 ? 1 : 0;
                pcMovement = 4;
                break;
            }

            default: {
                std::cout << "[ERROR] Unknown instruction" << std::endl;
                shouldExit = true;
                break;
            }
        }

        if (shouldExit) {
            break;
        }

        // Increment instruction pointer by num of opcode + params
        pc += pcMovement;
    }

    return std::make_tuple(program, -1, pc);
}

int main(int argc, char* argv[]) {
    std::string line;
    std::ifstream file("input.txt");

    if (file.is_open()) {
        file >> line;
        file.close();
    }

    std::vector<uint64_t> program;

    std::stringstream ss(line);
    std::string word;
    while (!ss.eof()) {
        getline(ss, word, ',');
        program.push_back(stoi(word));
    }

    // int phases[] = {0, 1, 2, 3, 4};
    int phases[] = {5, 6, 7, 8, 9};
    int n = sizeof(phases) / sizeof(phases[0]);

    // Truly spaghetti, but it does work
    long bestRet = 0;
    do {
        int index = 0;
        long ret = 0;
        long lastERet = 0;

        // Part 1, just remove the feedback loop for this (and use 01234 for phases)
        // for (char c = 'A'; c <= 'E'; c++) {
        //     ret = std::get<1>(simulateIntcode(program, phases[index++], ret, 0, false));
        // }

        // if (ret > bestRet) {
        //     bestRet = ret;
        //     std::cout << "Better ret of " << ret << " found at ";
        //     for (auto p: phases) {
        //         std::cout << p;
        //     }
        //     std::cout << std::endl;
        // }

        // Part 2, better approach is migrating to struct to manage info
        std::vector<uint64_t> ampA(program);
        std::vector<uint64_t> ampB(program);
        std::vector<uint64_t> ampC(program);
        std::vector<uint64_t> ampD(program);
        std::vector<uint64_t> ampE(program);

        std::vector<uint64_t> amps[] = {
            ampA, ampB, ampC, ampD, ampE
        };
        int pcs[] = {0, 0, 0, 0, 0};
        bool readInputs[] = {false, false, false, false, false};

        while (true) {
            std::tuple<std::vector<uint64_t>, long, int> sim = simulateIntcode(amps[index], phases[index], ret, pcs[index], readInputs[index]);
            amps[index] = std::get<0>(sim);
            ret = std::get<1>(sim);
            pcs[index] = std::get<2>(sim);
            readInputs[index] = true;

            if (amps[index][pcs[index]] == 99 && ret == -1) {
                // The amp is finished, so simulate E to see if it outputs once more
                std::tuple<std::vector<uint64_t>, long, int> simE = simulateIntcode(amps[4], phases[4], ret, pcs[4], readInputs[4]);

                // If simE has a non-negative ret, use that
                int finalRet = std::get<1>(simE);
                if (finalRet > 0) {
                    lastERet = finalRet;
                }
                break;
            }

            if (index == n - 1) {
                lastERet = ret;
            }

            index = (index + 1) % n;
        }

        if (lastERet > bestRet) {
            bestRet = lastERet;
            std::cout << "Better ret of " << lastERet << " found at ";
            for (auto p: phases) {
                std::cout << p;
            }
            std::cout << std::endl;
        }
    } while (std::next_permutation(phases, phases + n));
}