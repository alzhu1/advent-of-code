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

                if (!info.provideInput) {
                    // Request input, don't advance pc
                    IntcodeInfo inputInfo;
                    inputInfo.program = program;
                    inputInfo.pc = pc;
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
                std::cout << (char)info.input;
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

struct Point {
    Point(): x(0), y(0) {}
    Point(int16_t _x, int16_t _y): x(_x), y(_y) {}
    int16_t x;
    int16_t y;

    // Hash
    size_t operator() (const Point &p) const {
        return ((size_t)p.x << 32) | p.y;
    }

    bool operator == (const Point& p1) const {
        return x == p1.x && y == p1.y;
    }
};

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

    IntcodeInfo info;
    info.program = program;
    info.pc = 0;
    info.relativeBase = 0;
    info.halted = false;

    IntcodeInfo runInfo = info;

    // Part 1
    std::unordered_set<Point, Point> scaffolds;
    Point currPoint;
    Point robotStart;
    while (!info.halted) {
        info = simulateIntcode(info);

        if (!info.halted) {
            char o = info.output;

            if (o == '#') {
                scaffolds.insert(currPoint);
            } else if (o == '^') {
                scaffolds.insert(currPoint);
                robotStart = currPoint;
            }

            if (o == '\n') {
                currPoint.x = 0;
                currPoint.y++;
            } else {
                currPoint.x++;
            }
            std::cout << (char)info.output;
        }
    }

    uint32_t total = 0;
    for (auto scaffold: scaffolds) {
        // Check intersection
        int16_t x = scaffold.x;
        int16_t y = scaffold.y;

        if (scaffolds.count(Point(x, y - 1)) &&
            scaffolds.count(Point(x, y + 1)) &&
            scaffolds.count(Point(x - 1, y)) &&
            scaffolds.count(Point(x + 1, y))) {
            
            total += (x * y);
        }
    }

    std::cout << total << std::endl;

    // Part 2
    runInfo.program[0] = 2;

    std::string allInput = "A,A,B,B,C,B,C,B,C,A\nL,10,L,10,R,6\nR,12,L,12,L,12\nL,6,L,10,R,12,R,12\nn\n";
    int in = 0;
    while (!runInfo.halted) {
        IntcodeInfo nextInfo = simulateIntcode(runInfo);
        // nextInfo.provideInput = false;

        if (nextInfo.requestInput) {
            nextInfo.requestInput = false;
            nextInfo.provideInput = true;
            nextInfo.input = allInput[in++];
        } else if (nextInfo.hasOutput) {
            if (nextInfo.output <= 127) {
                std::cout << (char)nextInfo.output;
            } else {
                std::cout << nextInfo.output << std::endl;
            }
        }

        runInfo = nextInfo;
    }
}