#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

uint64_t getParameterIndex(std::unordered_map<uint64_t, int64_t>& program,
                          uint64_t opcode,
                          int paramNum,
                          int64_t pc,
                          int64_t relativeBase) {
    uint64_t startDivisor = 10;
    for (int i = 0; i < paramNum; i++) {
        startDivisor *= 10;
    }

    // Ensure that value at pc + paramNum exists
    if (!program.count(pc + paramNum)) {
        program[pc + paramNum] = 0;
    }
    uint64_t programParam = program[pc + paramNum];

    int mode = (opcode / startDivisor) % 10;
    switch (mode) {
        case 0: {
            // Position mode
            if (!program.count(programParam)) {
                program[programParam] = 0;
            }

            // return program[programParam];
            return programParam;
        }

        case 1: {
            // Immediate mode
            // return programParam;
            return pc + paramNum;
        }

        case 2: {
            // Relative mode
            if (!program.count(programParam + relativeBase)) {
                program[programParam + relativeBase] = 0;
            }
            // return program[programParam + relativeBase];
            return programParam + relativeBase;
        }

        default: {
            return 0;
        }
    }
}

struct IntcodeInfo {
    std::unordered_map<uint64_t, int64_t> program;
    int64_t pc;
    int64_t input;
    int64_t output;
    int64_t relativeBase;
    bool halted;
    bool provideInput;
    bool requestInput;
    bool hasOutput;
};

IntcodeInfo simulateIntcode(IntcodeInfo info) {
    std::unordered_map<uint64_t, int64_t> program = info.program;
    int64_t pc = info.pc;
    int64_t relativeBase = info.relativeBase;

    while (pc < program.size()) {
        int64_t opcode = program[pc];
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
                uint64_t inputIndex1 = getParameterIndex(program, opcode, 1, pc, relativeBase);
                uint64_t inputIndex2 = getParameterIndex(program, opcode, 2, pc, relativeBase);
                uint64_t outputIndex = getParameterIndex(program, opcode, 3, pc, relativeBase);

                // std::cout << input1 << " + " << input2 << " = " << (input1 + input2) << ", store at address " << outputAddress << std::endl;

                program[outputIndex] = program[inputIndex1] + program[inputIndex2];
                pcMovement = 4;
                break;
            }

            case 2: {
                // Multiply
                uint64_t inputIndex1 = getParameterIndex(program, opcode, 1, pc, relativeBase);
                uint64_t inputIndex2 = getParameterIndex(program, opcode, 2, pc, relativeBase);
                uint64_t outputIndex = getParameterIndex(program, opcode, 3, pc, relativeBase);

                // std::cout << input1 << " * " << input2 << " = " << (input1 * input2) << ", store at address " << outputAddress << std::endl;

                program[outputIndex] = program[inputIndex1] * program[inputIndex2];
                pcMovement = 4;
                break;
            }

            case 3: {
                // std::string n;
                // std::cout << "Add input: ";
                // std::cin >> n;
                // info.input = stoi(n);

                if (!info.provideInput) {
                    // Request input, don't advance pc
                    IntcodeInfo inputInfo;
                    inputInfo.program = program;
                    inputInfo.pc = pc;
                    inputInfo.input = 0;
                    inputInfo.output = 0;
                    inputInfo.relativeBase = relativeBase;
                    inputInfo.halted = false;
                    inputInfo.provideInput = false;
                    inputInfo.requestInput = true;
                    inputInfo.hasOutput = false;
                    return inputInfo;
                }

                // Otherwise, use input from the info

                uint64_t index = getParameterIndex(program, opcode, 1, pc, relativeBase);
                program[index] = info.input;
                info.provideInput = false;
                pcMovement = 2;
                break;
            }

            case 4: {
                uint64_t index = getParameterIndex(program, opcode, 1, pc, relativeBase);
                // std::cout << "Output = " << program[index] << std::endl;

                IntcodeInfo outputInfo;
                outputInfo.program = program;
                outputInfo.pc = pc + 2;
                outputInfo.input = 0;
                outputInfo.output = program[index];
                outputInfo.relativeBase = relativeBase;
                outputInfo.halted = false;
                outputInfo.provideInput = false;
                outputInfo.requestInput = false;
                outputInfo.hasOutput = true;
                return outputInfo;

                // pcMovement = 2;
                // break;
            }

            case 5: {
                uint64_t checkParamIndex = getParameterIndex(program, opcode, 1, pc, relativeBase);
                uint64_t pcSetIndex = getParameterIndex(program, opcode, 2, pc, relativeBase);

                if (program[checkParamIndex] != 0) {
                    pc = program[pcSetIndex];
                    pcMovement = 0;
                } else {
                    pcMovement = 3;
                }
                break;
            }

            case 6: {
                uint64_t checkParamIndex = getParameterIndex(program, opcode, 1, pc, relativeBase);
                uint64_t pcSetIndex = getParameterIndex(program, opcode, 2, pc, relativeBase);

                if (program[checkParamIndex] == 0) {
                    pc = program[pcSetIndex];
                    pcMovement = 0;
                } else {
                    pcMovement = 3;
                }
                break;
            }

            case 7: {
                // Less than
                uint64_t inputIndex1 = getParameterIndex(program, opcode, 1, pc, relativeBase);
                uint64_t inputIndex2 = getParameterIndex(program, opcode, 2, pc, relativeBase);
                uint64_t outputIndex = getParameterIndex(program, opcode, 3, pc, relativeBase);

                // std::cout << input1 << " + " << input2 << " = " << (input1 + input2) << ", store at address " << outputAddress << std::endl;

                program[outputIndex] = program[inputIndex1] < program[inputIndex2] ? 1 : 0;
                pcMovement = 4;
                break;
            }

            case 8: {
                // Equals
                uint64_t inputIndex1 = getParameterIndex(program, opcode, 1, pc, relativeBase);
                uint64_t inputIndex2 = getParameterIndex(program, opcode, 2, pc, relativeBase);
                uint64_t outputIndex = getParameterIndex(program, opcode, 3, pc, relativeBase);

                // std::cout << input1 << " + " << input2 << " = " << (input1 + input2) << ", store at address " << outputAddress << std::endl;

                program[outputIndex] = program[inputIndex1] == program[inputIndex2] ? 1 : 0;
                pcMovement = 4;
                break;
            }

            case 9: {
                uint64_t index = getParameterIndex(program, opcode, 1, pc, relativeBase);
                relativeBase += program[index];
                pcMovement = 2;
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

    IntcodeInfo haltedInfo;
    haltedInfo.program = program;
    haltedInfo.pc = pc;
    haltedInfo.output = 0;
    haltedInfo.relativeBase = relativeBase;
    haltedInfo.halted = true;
    return haltedInfo;
}

bool isValidSpace(uint64_t x, uint64_t y, std::unordered_map<uint64_t, int64_t>& program) {
    std::unordered_map<uint64_t, int64_t> copy(program);

    IntcodeInfo info;
    info.program = copy;
    info.pc = 0;
    info.input = 0;
    info.output = 0;
    info.relativeBase = 0;
    info.halted = false;
    info.provideInput = false;
    info.requestInput = false;
    info.hasOutput = false;

    bool provideX = true;
    bool found = false;
    while (!info.halted) {
        IntcodeInfo nextInfo = simulateIntcode(info);

        if (nextInfo.requestInput) {
            nextInfo.requestInput = false;
            nextInfo.provideInput = true;
            nextInfo.input = provideX ? x : y;
            provideX = false;
        } else if (nextInfo.hasOutput) {
            if (nextInfo.output != 0) {
                found = true;
            } else {
                found = false;
            }
        }

        info = nextInfo;
    }

    return found;
}

int main(int argc, char* argv[]) {
    std::string line;
    std::ifstream file("input.txt");

    if (file.is_open()) {
        file >> line;
        file.close();
    }

    std::unordered_map<uint64_t, int64_t> program;
    std::stringstream ss(line);
    std::string word;
    int i = 0;
    while (!ss.eof()) {
        getline(ss, word, ',');
        // program.push_back(stoi(word));
        program[i++] = stoll(word);
    }

    // Part 1
    // std::ofstream outFile;
    // outFile.open("output.txt");
    // int count = 0;
    // for (int y = 0; y < 100; y++) {
    //     for (int x = 0; x < 100; x++) {
    //         IntcodeInfo info;
    //         info.program = program;
    //         info.pc = 0;
    //         info.input = 0;
    //         info.output = 0;
    //         info.relativeBase = 0;
    //         info.halted = false;
    //         info.provideInput = false;
    //         info.requestInput = false;
    //         info.hasOutput = false;

    //         bool provideX = true;
    //         while (!info.halted) {
    //             IntcodeInfo nextInfo = simulateIntcode(info);

    //             if (nextInfo.requestInput) {
    //                 nextInfo.requestInput = false;
    //                 nextInfo.provideInput = true;
    //                 nextInfo.input = provideX ? x : y;
    //                 provideX = false;
    //             } else if (nextInfo.hasOutput) {
    //                 if (nextInfo.output != 0) {
    //                     // outFile << "(" << x << ", " << y << ")" << std::endl;
    //                     outFile << "#";
    //                     count++;
    //                 } else {
    //                     outFile << ".";
    //                 }
    //             }

    //             info = nextInfo;
    //         }
    //     }
    //     outFile << std::endl;
    // }

    // std::cout << count << std::endl;

    // Part 2
    // Start search past 100
    uint64_t lowestRow = 200;
    uint64_t incrementer = 100;

    std::cout << "Start finding first row with 100 valid col" << std::endl;
    while (incrementer > 0) {
        uint64_t checkRow = lowestRow + incrementer;
        // Look for first occurrence in the row
        uint64_t x = 0;
        while (!isValidSpace(x++, checkRow, program)) {}

        // Then go up to reach 100
        int count = 0;
        while (isValidSpace(x, checkRow - count, program)) {
            count++;
        }

        if (count < 100) {
            // Not big enough, go down more
            lowestRow += incrementer;
        } else {
            // Could be right, or too big, reduce inc
            std::cout << "reducing incrementer to " << incrementer / 10 << std::endl;
            incrementer /= 10;
        }
    }
    lowestRow++;
    uint64_t bestRow = lowestRow - 100;
    uint64_t bestCol = 0;

    // With the lowest row, now check if 100 above it and to the right works
    incrementer = 100;

    std::cout << "Start finding the bottom row of a 100x100 square" << std::endl;
    while (incrementer > 0) {
        uint64_t checkRow = bestRow + incrementer;

        // Look for first occurrence in the row
        bestCol = 0;
        while (!isValidSpace(bestCol, checkRow, program)) {
            bestCol++;
        }

        // Then go up to reach 100 (subtract 99)
        uint64_t x = bestCol;
        uint64_t y = checkRow - 99;

        // Now check if x to the right is 100
        int count = 0;
        while (isValidSpace(x++, y, program)) {
            count++;
        }

        if (count < 100) {
            // Not big enough, go down more
            bestRow += incrementer;
        } else {
            // Could be right, or too big, reduce inc
            std::cout << "reducing incrementer to " << incrementer / 10 << std::endl;
            incrementer /= 10;
        }
    }
    bestRow++;
    bestRow -= 99;

    std::cout << "Best row (y): " << bestRow << ", best col (x): " << bestCol << std::endl;
    std::cout << "Ans: " << (bestCol * 10000) + bestRow << std::endl;

    // Just verification below

    // uint64_t x = bestCol;
    // uint64_t y = bestRow;
    // uint64_t xMax = x + 100;
    // uint64_t yMax = y + 100;

    // int iters = 0;
    // int count = 0;

    // std::ofstream outFile;
    // outFile.open("output.txt");
    // while (y < yMax) {
    //     while (x < xMax) {
    //         if (isValidSpace(x++, y, program)) {
    //             count++;
    //             outFile << "#";
    //         } else {
    //             outFile << ".";
    //         }
    //         iters++;
    //     }
    //     x = bestCol;
    //     y++;
    //     outFile << std::endl;
    // }

    // std::cout << count << std::endl;
    // std::cout << iters << std::endl;
}