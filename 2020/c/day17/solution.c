#include "../file.h"
#include "../hashtable.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

typedef union Point {
    // 4 long longs * 8 bytes each = 32 bytes = 32 chars
    struct {
        long long x;
        long long y;
        long long z;
        long long w;
    };
    char key[32];
} Point;

int handlePointIteration(Point** pointsPtr, int pointsSize, int is4D) {
    Hashtable* seen = initHashtableWithLength(sizeof(Point));
    Point* nextPoints = calloc(3000, sizeof(Point));
    int nextPointsSize = 0;

    Point* points = *pointsPtr;

    for (int i = 0; i < pointsSize; i++) {
        Point point = points[i];

        for (int dx = -1; dx <=1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                for (int dz = -1; dz <= 1; dz++) {
                    for (int dw = -1; dw <= 1; dw++) {
                        if (dx == 0 && dy == 0 && dz == 0 && dw == 0) {
                            continue;
                        }

                        if (!is4D && dw != 0) {
                            continue;
                        }

                        Point neighbor = {
                            .x = point.x + dx,
                            .y = point.y + dy,
                            .z = point.z + dz,
                            .w = point.w + dw
                        };

                        char* key = (char*)malloc(sizeof(Point));
                        memcpy(key, neighbor.key, sizeof(Point));

                        HashtableEntry* entry = getHashtableItem(seen, key);
                        if (entry == NULL) {
                            putHashtableItem(seen, key, 0);
                            entry = getHashtableItem(seen, key);
                        } else {
                            free(key);
                        }

                        entry->value = (void*)(((int)entry->value) + 1);
                    }
                }
            }
        }
    }

    // Start with looping through each item
    for (int i = 0; i < pointsSize; i++) {
        Point point = points[i];

        HashtableEntry* entry = getHashtableItem(seen, point.key);
        int neighbors = entry == NULL ? 0 : (int)entry->value;

        if (neighbors == 2 || neighbors == 3) {
            // This point stays on
            Point nextPoint = {
                .x = point.x,
                .y = point.y, 
                .z = point.z,
                .w = point.w
            };
            nextPoints[nextPointsSize++] = nextPoint;
        }

        removeHashtableItem(seen, point.key, 1, 0);
    }

    // Then loop through remaining
    for (int i = 0; i < seen->capacity; i++) {
        HashtableEntry* hte = seen->table[i];
        while (hte != NULL) {
            HashtableEntry* next = hte->next;

            int value = (int)hte->value;
            if (value == 3) {
                Point nextPoint = {0};
                memcpy(&nextPoint, hte->key, sizeof(Point));

                nextPoints[nextPointsSize++] = nextPoint;
            }

            hte = next;
        }
    }

    destroyHashtable(seen, 1, 0);

    free(points);
    *pointsPtr = nextPoints;

    return nextPointsSize;
}

int main() {
    char* buffer = getFileBuffer("input.txt");
    char* startBuffer = buffer;

    if (buffer) {
        char* sizePtr = buffer;

        long long x = 0;
        long long y = 0;

        Point* points = calloc(10000, sizeof(Point));
        Point* points4D = calloc(10000, sizeof(Point));
        int pointsSize = 0;
        int points4DSize = 0;

        while (sizePtr != NULL) {
            char* line = strsep(&sizePtr, "\n");

            while (x < strlen(line)) {
                if (line[x] == '#') {
                    // Cube found                    
                    Point p = {
                        .x = x,
                        .y = y,
                        .z = 0,
                        .w = 0
                    };

                    memcpy(&points[pointsSize++], &p, sizeof(Point));
                    memcpy(&points4D[points4DSize++], &p, sizeof(Point));
                }

                x++;
            }

            x = 0;
            y--;
        }

        int iters = 0;
        while (iters++ < 6) {
            pointsSize = handlePointIteration(&points, pointsSize, 0);
            points4DSize = handlePointIteration(&points4D, points4DSize, 1);
        }

        printf("Cubes on: part 1 = %d, part 2 = %d\n", pointsSize, points4DSize);

        free(points);
        free(points4D);
    }

    free(startBuffer);
}
