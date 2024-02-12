#include "../file.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

int main() {
    char* buffer = getFileBuffer("input.txt");
    char* startBuffer = buffer;

    if (buffer) {
        char* sizePtr = buffer;

        int preambleSize = 25;
        long long preamble[preambleSize];
        long long list[1000];

        int index = 0;
        long long invalidNum = 0;
        while (sizePtr != NULL) {
            long long num = strtoll(strsep(&sizePtr, "\n"), NULL, 10);
            list[index] = num;

            if (index < preambleSize) {
                preamble[index] = num;
            } else {
                // Check sums of preamble nums to see if num is valid
                int isValid = 0;

                for (int i = 0; i < preambleSize; i++) {
                    for (int j = i + 1; j < preambleSize; j++) {
                        if (preamble[i] + preamble[j] == num) {
                            isValid = 1;
                            goto sumDone;
                        }
                    }
                }
sumDone:
                if (isValid) {
                    preamble[index % preambleSize] = num;
                } else {
                    printf("First num off: %lld\n", num);
                    invalidNum = num;
                    break;
                }
            }

            index++;
        }

        // Need to build a contiguous sum list
        int minIndex = 0;
        int maxIndex = 0;
        long long sum = 0;

        while (sum != invalidNum) {
            if (sum < invalidNum) {
                sum += list[maxIndex++];
            } else {
                sum -= list[minIndex++];
            }
        }

        long long min = list[minIndex];
        long long max = min;
        for (int i = minIndex; i < maxIndex; i++) {
            if (list[i] < min) {
                min = list[i];
            }

            if (list[i] > max) {
                max = list[i];
            }
        }

        printf("Sum: %lld\n", min + max);
    }

    free(startBuffer);
}
