#include "../file.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

long getTrees(char* buffer, size_t rowLength, int right, int down) {
    int offset = 0;
    long count = 0;
    while (strlen(buffer) > 0) {
        // Buffer should start on beginning of line
        if (buffer[offset] == '#') {
            count++;
        }
        buffer += ((rowLength + 1) * down);
        offset = (offset + right) % rowLength;
    }

    return count;
}

int main() {
    char* buffer = getFileBuffer("input.txt");
    char* startBuffer = buffer;

    if (buffer) {
        // Note: this actually modifies the underlying buffer (replaces \n with NULL char)
        size_t rowLength = strlen(strtok(buffer, "\n"));

        // Part 1
        long total = getTrees(buffer, rowLength, 3, 1);
        printf("Part 1: %ld\n", total);

        // Part 2
        total *= getTrees(buffer, rowLength, 1, 1);
        total *= getTrees(buffer, rowLength, 5, 1);
        total *= getTrees(buffer, rowLength, 7, 1);
        total *= getTrees(buffer, rowLength, 1, 2);

        printf("Part 2: %ld\n", total);
    }

    free(startBuffer);
}
