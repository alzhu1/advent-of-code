#include "../file.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

int simulate(char* instructions[], int length, int printRepeat) {
    int callTimes[length];
    memset(callTimes, 0, sizeof(callTimes));

    int accumulator = 0;
    int pc = 0;

    while (pc >= 0 && pc < length) {
        if (callTimes[pc] > 0) {
            if (printRepeat) {
                printf("Accumulator: %d (but we repeated)\n", accumulator);
            }
            return 1;
        }
        char* instruction = instructions[pc];
        int param = strtol(instruction + strlen(instruction) + 1, NULL, 10);
        callTimes[pc]++;

        if (strcmp(instruction, "acc") == 0) {
            accumulator += param;
        } else if (strcmp(instruction, "jmp") == 0) {
            pc += (param - 1);
        }

        pc++;
    }
    printf("Accumulator: %d, normal termination\n", accumulator);
    return 0;
}

int main() {
    char* buffer = getFileBuffer("input.txt");
    char* startBuffer = buffer;

    if (buffer) {
        char* sizePtr = buffer;

        char* instructions[1000];
        memset(instructions, 0, sizeof(instructions));

        int length = 0;
        while (sizePtr != NULL) {
            char* line = strsep(&sizePtr, "\n");
            instructions[length++] = line;

            // Hacky
            line[3] = '\0';
        }

        // Part 1
        simulate(instructions, length, 1);

        int indexFlip = 0;
        while (indexFlip < length) {
            // Find first non-acc index
            while (strcmp(instructions[indexFlip], "acc") == 0) {
                indexFlip++;
            }

            // Find flipped instruction
            int isNop = strcmp(instructions[indexFlip], "nop") == 0;
            char* currInstruction = isNop ? "nop" : "jmp";
            char* flipInstruction = isNop ? "jmp" : "nop";

            // Flip, simulate, and turn back
            memcpy(instructions[indexFlip], flipInstruction, 3);
            if (simulate(instructions, length, 0) == 0) {
                printf("Flipped instruction at index %d\n", indexFlip);
                break;
            }
            memcpy(instructions[indexFlip], currInstruction, 3);
            indexFlip++;
        }
    }

    free(startBuffer);
}
