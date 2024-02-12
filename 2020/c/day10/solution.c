#include "../file.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

// https://www.tutorialspoint.com/c_standard_library/c_function_qsort.htm
int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

long long search(int curr, int target, int joltageMap[], long long memo[]) {
    if (curr > target || (curr != 0 && joltageMap[curr] == 0)) {
        return 0;
    }

    if (curr == target) {
        return 1;
    }

    if (memo[curr]) {
        return memo[curr];
    }

    // Take the current, and add search for any joltages that exist for it
    long long result = 0;
    for (int i = 1; i <= 3; i++) {
        result += search(curr + i, target, joltageMap, memo);
    }
    memo[curr] = result;

    return result;
}

int main() {
    char* buffer = getFileBuffer("input.txt");
    char* startBuffer = buffer;

    if (buffer) {
        char* sizePtr = buffer;

        int joltages[100];
        joltages[0] = 0;

        int length = 1;
        int max = 0;
        while (sizePtr != NULL) {
            joltages[length] = strtol(strsep(&sizePtr, "\n"), NULL, 10);
            if (joltages[length] > max) {
                max = joltages[length];
            }

            length++;
        }
        joltages[length++] = max + 3;

        qsort(joltages, length, sizeof(int), cmpfunc);

        int diffs[3];
        memset(diffs, 0, sizeof(diffs));

        for (int i = 0; i < length - 1; i++) {
            diffs[joltages[i + 1] - joltages[i] - 1]++;
        }

        printf("(1-joltage x 3-joltage): %d x %d = %d\n", diffs[0], diffs[2], diffs[0] * diffs[2]);

        int joltageMap[max + 3];
        memset(joltageMap, 0, sizeof(joltageMap));
        for (int i = 0; i < length; i++) {
            joltageMap[joltages[i]] = joltages[i];
        }

        long long memo[max + 3];
        memset(memo, 0, sizeof(memo));
        long long result = search(0, max + 3, joltageMap, memo);

        printf("%lld\n", result);
    }

    free(startBuffer);
}
