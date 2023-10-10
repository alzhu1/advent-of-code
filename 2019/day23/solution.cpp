#include <deque>
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

struct Computer {
    int networkAddress;
    IntcodeInfo runningProgram;
    std::deque<std::pair<int64_t, int64_t> > messageQueue;
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
    info.input = 0;
    info.output = 0;
    info.relativeBase = 0;
    info.halted = false;
    info.provideInput = false;
    info.requestInput = false;
    info.hasOutput = false;

    // This is starter info
    std::vector<Computer> computers;
    for (int i = 0; i < 50; i++) {
        // std::cout << "comp " << i << std::endl;
        Computer computer;
        computer.networkAddress = i;
        computer.runningProgram = info;
        computer.messageQueue = std::deque<std::pair<int64_t, int64_t> >();

        // Init computer
        computer.runningProgram.provideInput = true;
        computer.runningProgram.input = i;

        computers.push_back(computer);
    }
    std::unordered_set<int64_t> natY;

    bool firstRun = true;
    std::pair<int64_t, int64_t> nat;
    bool foundFirstNatY = false;
    while (true) {
        int idleCount = 0;
        for (int i = 0; i < 50; i++) {
            Computer currComputer = computers[i];
            IntcodeInfo runningInfo = currComputer.runningProgram;

            bool tempStop = false;
            int inputIndex = 0;
            int outputIndex = 0;
            int64_t destination = 0;
            int64_t x = 0;
            int64_t y = 0;

            while (!runningInfo.halted && !tempStop) {
                IntcodeInfo nextInfo = simulateIntcode(runningInfo);

                if (nextInfo.requestInput) {
                    // Poll from currComputer message queue
                    nextInfo.requestInput = false;
                    nextInfo.provideInput = true;
                    if (currComputer.messageQueue.size() == 0) {
                        nextInfo.input = -1;
                        tempStop = true;
                        idleCount++;
                    } else {
                        std::pair<int64_t, int64_t> message = currComputer.messageQueue[0];
                        if (inputIndex == 0) {
                            nextInfo.input = message.first;
                        } else {
                            nextInfo.input = message.second;
                            currComputer.messageQueue.pop_front();
                        }

                        inputIndex = (inputIndex + 1) % 2;
                    }
                } else if (nextInfo.hasOutput) {
                    // Any output means a network in next iter won't be idle
                    idleCount = 0;

                    // Expect 3 outputs
                    if (outputIndex == 0) {
                        destination = nextInfo.output;
                    } else if (outputIndex == 1) {
                        x = nextInfo.output;
                    } else if (outputIndex == 2) {
                        y = nextInfo.output;

                        if (destination == 255) {
                            // std::cout << "OOB network: " << destination << ", x = " << x << ", y = " << y << std::endl;
                            nat = std::make_pair(x, y);

                            if (!foundFirstNatY) {
                                std::cout << "First Y sent to 255: " << y << std::endl;
                                foundFirstNatY = true;
                            }
                        } else {
                            computers[destination].messageQueue.push_back(std::make_pair(x, y));
                        }
                    }

                    outputIndex = (outputIndex + 1) % 3;
                }

                runningInfo = nextInfo;
            }

            // Set back in network address
            currComputer.runningProgram = runningInfo;
            computers[i] = currComputer;
        }

        if (!firstRun && idleCount == 50) {
            // std::cout << "All are idling" << std::endl;
            computers[0].messageQueue.push_back(nat);

            if (natY.count(nat.second)) {
                std::cout << "Duplicate Y is: " << nat.second << std::endl;
                break;
            }
            natY.insert(nat.second);
        }
        firstRun = false;
    }
}