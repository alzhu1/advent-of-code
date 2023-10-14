#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

int simulateIntcode(std::vector<uint32_t> program) {
    // Instruction pointer starts at 1
    int pc = 0;

    while (pc < program.size()) {
        uint32_t opcode = program[pc];
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
                std::string n;
                std::cout << "Add input: ";
                std::cin >> n;

                program[program[pc + 1]] = stoi(n);
                pcMovement = 2;
                break;
            }

            case 4: {
                std::cout << "Output at " << program[pc + 1] << " = " << program[program[pc + 1]] << std::endl;
                pcMovement = 2;
                break;
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

    return program[0];
}

int main(int argc, char* argv[]) {
    std::string line;
    std::ifstream file("input.txt");

    if (file.is_open()) {
        file >> line;
        file.close();
    }

    std::vector<uint32_t> program;

    std::stringstream ss(line);
    std::string word;
    while (!ss.eof()) {
        getline(ss, word, ',');
        program.push_back(stoi(word));
    }

    simulateIntcode(program);
}