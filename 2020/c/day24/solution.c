#include "../file.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

int getDirectionIndex(char* direction) {
    // e, se, sw, w, nw, and ne
    if (strcmp(direction, "e") == 0) return 0;
    if (strcmp(direction, "se") == 0) return 1;
    if (strcmp(direction, "sw") == 0) return 2;
    if (strcmp(direction, "w") == 0) return 3;
    if (strcmp(direction, "nw") == 0) return 4;
    if (strcmp(direction, "ne") == 0) return 5;

    return -1;
}

void reduceTileMove(int tileMove[3]) {
    // Take the 1st index (se) and turn it into e + sw
    tileMove[0] += tileMove[1];
    tileMove[2] += tileMove[1];
    tileMove[1] = 0;
}

int main() {
    char* buffer = getFileBuffer("input.txt");
    char* startBuffer = buffer;

    if (buffer) {
        char* sizePtr = buffer;

        // e, se, and sw only
        int tileMoves[1000][3] = {0};
        int numTiles = 0;

        while (sizePtr != NULL) {
            char* line = strsep(&sizePtr, "\n");

            int length = strlen(line);
            int index = 0;
            while (index < length) {
                char direction[3] = {0};
                direction[0] = line[index++];
                if (direction[0] == 'n' || direction[0] == 's') {
                    direction[1] = line[index++];
                }

                int directionIndex = getDirectionIndex(direction);
                if (directionIndex < 3) {
                    tileMoves[numTiles][directionIndex]++;
                } else {
                    tileMoves[numTiles][directionIndex - 3]--;
                }
            }

            reduceTileMove(tileMoves[numTiles]);
            numTiles++;
        }

        // Arbitrarily large enough grid to put moves on
        int tileStateLength = 250;
        int halfLength = tileStateLength / 2;
        int tileStates[tileStateLength][tileStateLength];
        memset(tileStates, 0, sizeof(tileStates));

        for (int i = 0; i < numTiles; i++) {
            int* tileMove = tileMoves[i];

            int eDir = tileMove[0] + halfLength;
            int seDir = tileMove[2] + halfLength;
            tileStates[eDir][seDir] = (tileStates[eDir][seDir] + 1) % 2;
        }

        int flipCount = 0;
        for (int i = 0; i < tileStateLength; i++) {
            for (int j = 0; j < tileStateLength; j++) {
                if (tileStates[i][j]) {
                    flipCount++;
                }
            }
        }
        printf("Initial flip count: %d\n", flipCount);

        int neighborDelta[6][2] = {
            { 1, 0 }, // e
            { 1, 1 }, // se
            { 0, 1 }, // sw
            { -1, 0 }, // w
            { -1, -1 }, // nw
            { 0, -1 }, // ne
        };

        int days = 0;
        while (days++ < 100) {
            // Keep a copy, used when checking for flipped tiles in iteration
            int tileStatesCopy[tileStateLength][tileStateLength];
            memcpy(tileStatesCopy, tileStates, sizeof(tileStates));

            // Skip the borders of the tileStates
            for (int eDir = 1; eDir < tileStateLength - 1; eDir++) {
                for (int seDir = 1; seDir < tileStateLength - 1; seDir++) {
                    int neighborCount = 0;
                    for (int n = 0; n < 6; n++) {
                        neighborCount += tileStatesCopy[eDir + neighborDelta[n][0]][seDir + neighborDelta[n][1]];
                    }

                    int currValue = tileStatesCopy[eDir][seDir];
                    if ((currValue && (neighborCount == 0 || neighborCount > 2)) || (!currValue && neighborCount == 2)) {
                        tileStates[eDir][seDir] = (tileStates[eDir][seDir] + 1) % 2;
                    }
                }
            }

            // Search all tiles
            flipCount = 0;
            for (int i = 0; i < tileStateLength; i++) {
                for (int j = 0; j < tileStateLength; j++) {
                    if (tileStates[i][j]) {
                        flipCount++;
                    }
                }
            }
        }

        printf("Day 100: %d\n", flipCount);
    }

    free(startBuffer);
}
