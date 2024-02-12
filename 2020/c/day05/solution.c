#include "../file.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

int main() {
    char* buffer = getFileBuffer("input.txt");
    char* startBuffer = buffer;

    if (buffer) {
        char* sizePtr = buffer;

        int occupiedSeats[1024];
        memset(occupiedSeats, 0, sizeof(occupiedSeats));
        int max = 0;
        while (sizePtr != NULL) {
            char* line = strsep(&sizePtr, "\n");

            int row = 0;
            int col = 0;
            int index = 0;

            while (index < 7) {
                if (line[index++] == 'B') {
                    row++;
                }
                row <<= 1;
            }
            row >>= 1;

            while (index < 10) {
                if (line[index++] == 'R') {
                    col++;
                }
                col <<= 1;
            }
            col >>= 1;

            int seatId = row * 8 + col;
            occupiedSeats[seatId] = 1;
            if (seatId > max) {
                max = seatId;
            }
        }

        printf("Largest seat id: %u\n", max);
        for (int i = max; i >= 0; i--) {
            if (!occupiedSeats[i]) {
                printf("Missing seat id: %u\n", i);
                break;
            }
        }
    }

    free(startBuffer);
}
