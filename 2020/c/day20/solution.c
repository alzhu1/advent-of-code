#include "../file.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define TILE_SIZE 10

// Neighbors
#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3

typedef struct Tile Tile;

struct Tile {
    int tileNumber;
    char grid[TILE_SIZE][TILE_SIZE];

    Tile* neighbors[4];

    int fixed; // Fixed grid in place
    int searched;
};

// https://www.geeksforgeeks.org/rotate-a-matrix-by-90-degree-in-clockwise-direction-without-using-any-extra-space/
void rotateRight(int length, char grid[length][length]) {
    for (int i = 0; i < length / 2; i++) {
        for (int j = i; j < length - i - 1; j++) {
            // Swap elements of each cycle
            // in clockwise direction
            char temp = grid[i][j];
            grid[i][j] = grid[length - 1 - j][i];
            grid[length - 1 - j][i] = grid[length - 1 - i][length - 1 - j];
            grid[length - 1 - i][length - 1 - j] = grid[j][length - 1 - i];
            grid[j][length - 1 - i] = temp;
        }
    }
}

void flipHorizontal(int length, char grid[length][length]) {
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < length / 2; j++) {
            char temp = grid[i][j];
            grid[i][j] = grid[i][length - 1 - j];
            grid[i][length - 1 - j] = temp;
        }
    }
}

void setBorder(char border[TILE_SIZE], char grid[TILE_SIZE][TILE_SIZE], int baseBorderNumber, int flip) {
    switch (baseBorderNumber) {
        case 0: {
            // Top of grid, left to right
            memcpy(border, grid[0], TILE_SIZE);
            break;
        }

        case 1: {
            // Right column of grid, top to bottom
            for (int row = 0; row < TILE_SIZE; row++) {
                border[row] = grid[row][TILE_SIZE - 1];
            }
            break;
        }

        case 2: {
            // Bottom of grid, right to left
            for (int col = 0; col < TILE_SIZE; col++) {
                border[col] = grid[TILE_SIZE - 1][TILE_SIZE - 1 - col];
            }
            break;
        }

        case 3: {
            // Left column of grid, bottom to top
            for (int row = 0; row < TILE_SIZE; row++) {
                border[row] = grid[TILE_SIZE - 1 - row][0];
            }
            break;
        }
    }

    if (flip) {
        for (int i = 0; i < TILE_SIZE / 2; i++) {
            char temp = border[i];
            border[i] = border[TILE_SIZE - 1 - i];
            border[TILE_SIZE - 1 - i] = temp;
        }
    }
}

int checkBorderMatch(Tile* baseTile, Tile** tiles, int numTiles, int borderNumber) {
    // Base case: if tile hasn't set neighbor yet
    char baseBorder[TILE_SIZE];
    if (baseTile->neighbors[borderNumber] != NULL) {
        return 0;
    }
    setBorder(baseBorder, baseTile->grid, borderNumber, 0);
    int oppositeBorderNumber = (borderNumber + 2) % 4;

    Tile* matchTile = NULL;
    for (int i = 0; i < numTiles; i++) {
        // Skip null or same tile check
        Tile* tileCheck = tiles[i];
        if (tileCheck == NULL || tileCheck == baseTile) {
            continue;
        }

        char borderCheck[TILE_SIZE];

        // Set aside a local grid in case the tileCheck is fixed in place
        // i.e. it's already in the correct position relative to another tile
        char grid[TILE_SIZE][TILE_SIZE];
        if (tileCheck->fixed) {
            memcpy(grid, tileCheck->grid, sizeof(grid));
        }

        for (int r = 0; r < 8; r++) {
            // Check border, the opposite tile checked should be as "flipped" to match
            setBorder(borderCheck, tileCheck->fixed ? grid : tileCheck->grid, oppositeBorderNumber, 1);

            // Assumption: a tile will only have <= 1 border match with another
            if (memcmp(baseBorder, borderCheck, TILE_SIZE) == 0) {
                matchTile = tileCheck;
                goto done;
            }

            // Flip each iteration
            // Every 2 iters (2 flips = normal), rotate the grid
            flipHorizontal(TILE_SIZE, tileCheck->fixed ? grid : tileCheck->grid);
            if (r % 2 == 1) {
                rotateRight(TILE_SIZE, tileCheck->fixed ? grid : tileCheck->grid);
            }
        }
    }

done:
    if (matchTile != NULL) {
        // If matching, then fix both tiles in place (they should no longer edit their grids)
        baseTile->fixed = 1;
        matchTile->fixed = 1;

        baseTile->neighbors[borderNumber] = matchTile;
        matchTile->neighbors[oppositeBorderNumber] = baseTile;
        return 1;
    }

    return 0;
}

void processTiles(Tile* currTile, int numTiles, Tile* tiles[numTiles]) {
    // Skip null or searched tiles
    if (currTile == NULL || currTile->searched) {
        return;
    }

    // If we ever find matches, restart the border check (to revisit past unfound borders)
    for (int border = 0; border < 4; border++) {
        if (checkBorderMatch(currTile, tiles, numTiles, border)) {
            // Set -1 so in next iter, border becomes 0
            border = -1;
        }
    }

    currTile->searched = 1;
    for (int i = 0; i < 4; i++) {
        processTiles(currTile->neighbors[i], numTiles, tiles);
    }
}

// Returns corner product, cornerPtr should end up in top left
long getCornerProduct(Tile** cornerPtr) {
    Tile* corner = *cornerPtr;
    long value = 1;

    // Start collecting values at top right corner
    while (corner->neighbors[UP] != NULL) { corner = corner->neighbors[UP]; }
    while (corner->neighbors[RIGHT] != NULL) { corner = corner->neighbors[RIGHT]; }
    value *= corner->tileNumber;

    // Bottom right corner
    while (corner->neighbors[DOWN] != NULL) { corner = corner->neighbors[DOWN]; }
    value *= corner->tileNumber;

    // Bottom left corner
    while (corner->neighbors[LEFT] != NULL) { corner = corner->neighbors[LEFT]; }
    value *= corner->tileNumber;

    // Top left corner (and set the corner pointer)
    while (corner->neighbors[UP] != NULL) { corner = corner->neighbors[UP]; }
    value *= corner->tileNumber;
    *cornerPtr = corner;

    return value;
}

int monsterSearch(int length, char picture[length][length], int baseRow, int baseCol) {
    // 15 in total
    int rowOffsets[] = {
        0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, -1, 0
    };
    int colOffsets[] = {
        0, 1, 4, 5, 6, 7, 10, 11, 12, 13, 16, 17, 18, 18, 19
    };

    int isMonster = 1;
    for (int i = 0; i < 15; i++) {
        if (picture[baseRow + rowOffsets[i]][baseCol + colOffsets[i]] == '.') {
            isMonster = 0;
            break;
        }
    }

    if (isMonster) {
        for (int i = 0; i < 15; i++) {
            picture[baseRow + rowOffsets[i]][baseCol + colOffsets[i]] = 'O';
        }
    }

    return isMonster;
}

int main() {
    char* buffer = getFileBuffer("input.txt");
    char* startBuffer = buffer;

    if (buffer) {
        char* sizePtr = buffer;

        Tile* tiles[150] = {0};

        int numTiles = 0;
        int charIndex = 0;
        while (sizePtr != NULL) {
            char* line = strsep(&sizePtr, "\n");

            if (line[0] == 'T') {
                strsep(&line, " ");
                int tileNumber = strtol(strsep(&line, ":"), NULL, 10);

                Tile* tile = calloc(1, sizeof(Tile));
                tile->tileNumber = tileNumber;
                tiles[numTiles] = tile;
                continue;
            }

            if (strlen(line) == 0) {
                numTiles++;
                charIndex = 0;
                continue;
            }

            memcpy(tiles[numTiles]->grid[charIndex++], line, strlen(line));
        }

        int sideLength = 1;
        while (numTiles / sideLength != sideLength) {
            sideLength++;
        }
        printf("Side length: %d\n", sideLength);

        Tile* corner = tiles[0];
        processTiles(corner, numTiles, tiles);

        long value = getCornerProduct(&corner);
        printf("Part 1 value: %ld\n", value);

        // Part 2
        int innerTileSize = TILE_SIZE - 2;
        int pictureLength = sideLength * innerTileSize;
        char picture[pictureLength][pictureLength];
        memset(picture, 0, sizeof(picture));

        int row = 0;
        while (row < sideLength) {
            Tile* leftBorderTile = corner;

            int colOffset = 0;
            while (corner != NULL) {
                // Copy each inner row of the tile to the overall picture
                for (int rowOffset = 1; rowOffset < TILE_SIZE - 1; rowOffset++) {
                    memcpy(&picture[row * innerTileSize + (rowOffset - 1)][colOffset], &corner->grid[rowOffset][1], innerTileSize);
                }

                // Move over to the next x offset along with right tile
                colOffset += innerTileSize;
                corner = corner->neighbors[RIGHT];
            }

            // Move down a set of tiles (restart on left side)
            corner = leftBorderTile;
            corner = corner->neighbors[DOWN];
            row++;
        }

        // Search for monsters
        int monsterCount = 0;
        int iters = 0;
        while (!monsterCount && iters < 8) {
            // Start at the 1st row, go to 2nd last row
            for (int row = 1; row < pictureLength - 1; row++) {
                // Monsters are 20 in width
                for (int col = 0; col < pictureLength - 20; col++) {
                    monsterCount += monsterSearch(pictureLength, picture, row, col);
                }
            }

            flipHorizontal(pictureLength, picture);
            if (iters % 2 == 1) {
                rotateRight(pictureLength, picture);
            }
            iters++;
        }

        int hashCount = 0;
        for (int i = 0; i < pictureLength; i++) {
            for (int j = 0; j < pictureLength; j++) {
                if (picture[i][j] == '#') {
                    hashCount++;
                }
            }
        }

        printf("Part 2 hash count: %d\n", hashCount);
        for (int i = 0; i < numTiles; i++) {
            free(tiles[i]);
        }
    }

    free(startBuffer);
}
