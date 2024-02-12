#include "../file.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

int main() {
    char* buffer = getFileBuffer("input.txt");
    char* startBuffer = buffer;

    if (buffer) {
        char* sizePtr = buffer;

        int questions[26];
        memset(questions, 0, sizeof(questions));

        int sum = 0;
        int allAnswered = 0;
        int groupSize = 0;
        while (sizePtr != NULL) {
            char* line = strsep(&sizePtr, "\n");

            if (sizePtr == NULL || strlen(line) == 0) {
                if (sizePtr == NULL) {
                    for (int i = 0; i < strlen(line); i++) {
                        questions[line[i] - 'a'] += 1;
                    }
                    groupSize++;
                }

                // Check the table at this point
                for (int i = 0; i < 26; i++) {
                    if (questions[i]) {
                        sum++;
                    }

                    if (questions[i] == groupSize) {
                        allAnswered++;
                    }
                }
                memset(questions, 0, sizeof(questions));
                groupSize = 0;
                // printf("%d\n", sum);
                continue;
            }

            for (int i = 0; i < strlen(line); i++) {
                questions[line[i] - 'a'] += 1;
            }
            groupSize++;
        }

        printf("Total unique questions answered: %d\n", sum);
        printf("All answered count: %d\n", allAnswered);
    }

    free(startBuffer);
}
