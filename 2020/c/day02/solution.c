#include "../file.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

int main() {
    char* buffer = getFileBuffer("input.txt");
    char* startBuffer = buffer;

    if (buffer) {
        size_t size = 0;
        char* sizePtr = buffer;

        int totalP1 = 0;
        int totalP2 = 0;
        while (strlen(sizePtr) > 0) {
            char* token = strtok(sizePtr, "\n");
            sizePtr += (strlen(token) + 1);

            // Now parse token
            long minNum = strtol(token, &token, 10);
            token++;
            long maxNum = strtol(token, &token, 10);
            token++;

            char* letter = strtok(token, ": ");
            token += (strlen(letter) + strlen(": "));

            // Part 1
            int count = 0;
            for (int i = 0; i < strlen(token); i++) {
                if (token[i] == *letter) {
                    count++;
                }
            }
            if (count >= minNum && count <= maxNum) {
                totalP1++;
            }

            // Part 2
            int minMatch = token[minNum - 1] == *letter;
            int maxMatch = token[maxNum - 1] == *letter;
            if (minMatch ^ maxMatch) {
                totalP2++;
            }

            size++;
        }

        printf("Total size: %zu\n", size);
        printf("Total (part 1): %d\n", totalP1);
        printf("Total (part 2): %d\n", totalP2);
    }

    free(startBuffer);
}
