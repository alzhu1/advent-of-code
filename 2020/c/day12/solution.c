#include "../file.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

int main() {
    char* buffer = getFileBuffer("input.txt");
    char* startBuffer = buffer;

    if (buffer) {
        char* sizePtr = buffer;

        int x1 = 0;
        int y1 = 0;
        int x2 = 0;
        int y2 = 0;

        // N = 0, E = 1, S = 2, W = 3
        int direction = 1;
        int wx = 10;
        int wy = 1;
        while (sizePtr != NULL) {
            char* line = strsep(&sizePtr, "\n");

            char action = line[0];
            int value = strtol(line + 1, NULL, 10);

            // printf("[%s] and [%s]\n", line, line + 1);
            // printf("Action: %c, value: %d, direction: %d\n", action, value, direction);
            // break;

            // Handle L/R
            if (action == 'R' || action == 'L') {
                int turns = (value % 360) / 90;
                if (action == 'L') {
                    turns = 4 - turns;
                }

                direction = (direction + turns) % 4;

                // Clockwise turns
                while (turns-- > 0) {
                    int tempX = wx;
                    wx = wy;
                    wy = -tempX;
                }
                
                continue;
            }

            // Handle F (just sets action)
            if (action == 'F') {
                // Part 1
                switch (direction) {
                    case 0: y1 += value; break;
                    case 1: x1 += value; break;
                    case 2: y1 -= value; break;
                    case 3: x1 -= value; break;
                    default: break;
                }

                // Part 2 moves x2 and y2
                x2 += (wx * value);
                y2 += (wy * value);
                continue;
            }

            switch (action) {
                case 'N': y1 += value; wy += value; break;
                case 'E': x1 += value; wx += value; break;
                case 'S': y1 -= value; wy -= value; break;
                case 'W': x1 -= value; wx -= value; break;
                default: break;
            }
        }

        printf("(x1, y1) = (%d, %d), manhattan = %d\n", x1, y1, abs(x1) + abs(y1));
        printf("(x2, y2) = (%d, %d), manhattan = %d\n", x2, y2, abs(x2) + abs(y2));
    }

    free(startBuffer);
}
