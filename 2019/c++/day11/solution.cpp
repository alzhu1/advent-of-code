#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
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
    int64_t output;
    int64_t relativeBase;
    bool halted;
};

IntcodeInfo simulateIntcode(IntcodeInfo info, int64_t currColor) {
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

                uint64_t index = getParameterIndex(program, opcode, 1, pc, relativeBase);
                program[index] = currColor; //stoll(n);
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

    Point getLeft() {
        if (x == 0 && y == 1) {
            return Point(-1, 0);
        }

        if (x == -1 && y == 0) {
            return Point(0, -1);
        }

        if (x == 0 && y == -1) {
            return Point(1, 0);
        }

        if (x == 1 && y == 0) {
            return Point(0, 1);
        }

        return Point();
    }

    Point getRight() {
        if (x == 0 && y == 1) {
            return Point(1, 0);
        }

        if (x == 1 && y == 0) {
            return Point(0, -1);
        }

        if (x == 0 && y == -1) {
            return Point(-1, 0);
        }

        if (x == -1 && y == 0) {
            return Point(0, 1);
        }

        return Point();
    }

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

    std::unordered_map<Point, uint64_t, Point> paintCount;
    std::unordered_set<Point, Point> whitePoints;
    Point currPos;
    Point currDir(0, 1);

    // Part 2
    whitePoints.insert(currPos);
    Point minPoint(0x7fff, 0x7fff);
    Point maxPoint(0x8000, 0x8000);

    while (!info.halted) {
        int64_t currColor = whitePoints.count(currPos);
        IntcodeInfo out1 = simulateIntcode(info, currColor);

        if (out1.halted) {
            break;
        }

        if (out1.output == 0) {
            whitePoints.erase(currPos);
        } else if (out1.output == 1) {
            whitePoints.insert(currPos);
        } else {
            std::cout << "INCORRECT FIRST OUTPUT" << std::endl;
        }

        if (!paintCount.count(currPos)) {
            paintCount[currPos] = 0;
        }
        paintCount[currPos]++;

        if (currPos.x < minPoint.x) { minPoint.x = currPos.x; }
        if (currPos.y < minPoint.y) { minPoint.y = currPos.y; }

        if (currPos.x > maxPoint.x) { maxPoint.x = currPos.x; }
        if (currPos.y > maxPoint.y) { maxPoint.y = currPos.y; }

        IntcodeInfo out2 = simulateIntcode(out1, currColor);
        if (out2.output == 0) {
            currDir = currDir.getLeft();
        } else if (out2.output == 1) {
            currDir = currDir.getRight();
        } else {
            std::cout << "INCORRECT SECOND OUTPUT" << std::endl;
        }

        currPos.x += currDir.x;
        currPos.y += currDir.y;

        info = out2;
    }

    // Part 1
    // std::cout << paintCount.size() << std::endl;

    // Part 2
    std::ofstream outFile;
    outFile.open("output.txt");

    for (int16_t y = maxPoint.y; y >= minPoint.y; y--) {
        for (int16_t x = minPoint.x; x <= maxPoint.x; x++) {
            outFile << (whitePoints.count(Point(x, y)) ? '#' : '.');
        }
        outFile << std::endl;
    }
    outFile.close();
}