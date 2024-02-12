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

        while (strlen(sizePtr) > 0) {
            char* token = strtok(sizePtr, "\n");
            sizePtr += (strlen(token) + 1);
            size++;
        }

        printf("Total size: %zu\n", size);

        long* arr = (long*)malloc(size * sizeof(long*));
        int index = 0;
        while (strlen(buffer) > 0) {
            arr[index++] = strtol(buffer, &buffer, 10);
            buffer++;
        }
        printf("Done parsing.\n");

        // Part 1
        for (int i = 0; i < size; i++) {
            for (int j = i + 1; j < size; j++) {
                long a = arr[i];
                long b = arr[j];
                if (a + b == 2020) {
                    printf("Part 1: \n");
                    printf("%ld + %ld = %ld\n", a, b, a + b);
                    printf("%ld * %ld = %ld\n", a, b, a * b);
                    goto doneP1;
                }
            }
        }

doneP1:
        // Part 2
        for (int i = 0; i < size; i++) {
            for (int j = i + 1; j < size; j++) {
                for (int k = j + 1; k < size; k++) {
                    long a = arr[i];
                    long b = arr[j];
                    long c = arr[k];
                    if (a + b + c == 2020) {
                        printf("Part 2:\n");
                        printf("%ld + %ld + %ld = %ld\n", a, b, c, a + b + c);
                        printf("%ld * %ld * %ld = %ld\n", a, b, c, a * b * c);
                        goto doneP2;
                    }
                }
            }
        }

doneP2:
        free(arr);
    }

    free(startBuffer);
}
