#include "stdio.h"
#include "stdlib.h"
#include "string.h"

typedef struct Cup Cup;

struct Cup {
    long value;
    Cup* left;
    Cup* right;
};

int main() {
    // char* input = "389125467";
    char* input = "962713854";

    Cup* first = NULL;
    Cup* curr = NULL;
    Cup* one = NULL;

    Cup* allCups[1000001] = {0};
    for (int i = 0; i < strlen(input); i++) {
        long value = input[i] - '0';

        Cup* cup = calloc(1, sizeof(Cup));
        cup->value = value;
        if (first == NULL) {
            first = cup;
            curr = cup;
        } else {
            curr->right = cup;
            cup->left = curr;

            curr = curr->right;
        }

        if (value == 1) {
            one = cup;
        }

        allCups[cup->value] = cup;
    }

    long totalCups = 9;

    // Part 2
    for (long i = 10; i < 1000001; i++) {
        Cup* cup = calloc(1, sizeof(Cup));
        cup->value = i;

        curr->right = cup;
        cup->left = curr;
        curr = curr->right;

        totalCups++;
        allCups[i] = cup;
    }

    curr->right = first;
    first->left = curr;
    curr = first;

    // Play the game
    long moves = 0;
    // int moveCount = 100;
    long moveCount = 10000000;
    while (moves++ < moveCount) {
        Cup* pickup[3] = {0};
        Cup* p = curr->right;
        for (int i = 0; i < 3; i++) {
            pickup[i] = p;
            p = p->right;
        }

        curr->right = p;
        p->left = curr;

        long destination = curr->value - 1;
        int dupeCheck = 0;
        while (dupeCheck < 3) {
            if (pickup[dupeCheck++]->value == destination) {
                dupeCheck = 0;
                destination--;
            }

            if (destination <= 0) {
                dupeCheck = 0;
                destination = totalCups;
            }
        }

        Cup* d = allCups[destination];

        Cup* rightEdge = d->right;
        d->right = pickup[0];
        pickup[0]->left = d;
        pickup[2]->right = rightEdge;
        rightEdge->left = pickup[2];

        curr = curr->right;
    }

    Cup* final = one->right;

    // Part 1
    // while (final != one) {
    //     printf("%ld", final->value);
    //     final = final->right;
    // }
    // printf("\n");

    // Part 2
    long v1 = final->value;
    long v2 = final->right->value;

    printf("%ld * %ld = %ld\n", v1, v2, v1 * v2);

    for (int i = 0; i < 1000001; i++) {
        free(allCups[i]);
    }
}
