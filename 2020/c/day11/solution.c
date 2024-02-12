#include "../file.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

int checkSeatP1(int row, int col, int rows, int cols, char* grid[], char* copy[]) {
    if (grid[row][col] == '.') {
        return 0;
    }

    int neighborOccupied = 0;
    for (int rowDiff = -1; rowDiff <= 1; rowDiff++) {
        for (int colDiff = -1; colDiff <= 1; colDiff++) {
            if (rowDiff == 0 && colDiff == 0) {
                continue;
            }
            
            int checkRow = row + rowDiff;
            int checkCol = col + colDiff;

            if (checkRow < 0 || checkRow >= rows || checkCol < 0 || checkCol >= cols) {
                continue;
            }

            char seat = grid[row + rowDiff][col + colDiff];
            if (seat == '#') {
                neighborOccupied++;
            }
        }
    }

    if (grid[row][col] == 'L' && neighborOccupied == 0) {
        copy[row][col] = '#';
        return 1;
    } else if (grid[row][col] == '#' && neighborOccupied >= 4) {
        copy[row][col] = 'L';
        return 1;
    }
    return 0;
}

int checkSeatP2(int row, int col, int rows, int cols, char* grid[], char* copy[]) {
    if (grid[row][col] == '.') {
        return 0;
    }

    int neighborOccupied = 0;
    for (int rowDiff = -1; rowDiff <= 1; rowDiff++) {
        for (int colDiff = -1; colDiff <= 1; colDiff++) {
            if (rowDiff == 0 && colDiff == 0) {
                continue;
            }
            
            int checkRow = row + rowDiff;
            int checkCol = col + colDiff;

            // TODO:
            while (!(checkRow < 0 || checkRow >= rows || checkCol < 0 || checkCol >= cols)) {
                if (grid[checkRow][checkCol] != '.') {
                    break;
                }
                checkRow += rowDiff;
                checkCol += colDiff;
            }

            if (checkRow < 0 || checkRow >= rows || checkCol < 0 || checkCol >= cols) {
                continue;
            }

            char seat = grid[checkRow][checkCol];
            if (seat == '#') {
                neighborOccupied++;
            }
        }
    }

    if (grid[row][col] == 'L' && neighborOccupied == 0) {
        copy[row][col] = '#';
        return 1;
    } else if (grid[row][col] == '#' && neighborOccupied >= 5) {
        copy[row][col] = 'L';
        return 1;
    }
    return 0;
}

int main() {
    char* buffer = getFileBuffer("input.txt");
    char* startBuffer = buffer;

    if (buffer) {
        char* sizePtr = buffer;

        char* grid[100];
        memset(grid, 0, sizeof(grid));
        int rows = 0;
        while (sizePtr != NULL) {
            grid[rows++] = strsep(&sizePtr, "\n");
        }
        int cols = strlen(grid[0]);

        int iters = 0;
        int changed = 1;
        while (changed) {
            changed = 0;

            char* copy[100];
            memset(copy, 0, sizeof(copy));
            for (int i = 0; i < rows; i++) {
                copy[i] = (char*)malloc(cols * sizeof(char));
                strcpy(copy[i], grid[i]);
            }

            // Set in copy first
            for (int row = 0; row < rows; row++) {
                for (int col = 0; col < cols; col++) {
                    // Part 1 and Part 2, switch functions
                    // changed |= checkSeatP1(row, col, rows, cols, grid, copy);
                    changed |= checkSeatP2(row, col, rows, cols, grid, copy);
                }
            }

            for (int i = 0; i < rows; i++) {
                strcpy(grid[i], copy[i]);
                free(copy[i]);
            }
            iters++;
        }

        int occupied = 0;
        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < cols; col++) {
                if (grid[row][col] == '#') {
                    occupied++;
                }
            }
        }

        printf("Iters: %d, Occupied: %d\n", iters, occupied);
    }

    free(startBuffer);
}
