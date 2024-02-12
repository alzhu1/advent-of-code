#include "../file.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

typedef struct Card Card;

struct Card {
    int value;
    Card* below;
};

void endRound(Card* top[2], Card* bottom[2], int winner, int loser) {
    // Winner's card + loser's card goes on bottom of winner
    Card* topLoserCard = top[loser];
    top[loser] = top[loser]->below;

    if (top[loser] == NULL) {
        bottom[loser] = NULL;
    }
    
    Card* topWinnerCard = top[winner];
    top[winner] = top[winner]->below;

    topWinnerCard->below = topLoserCard;
    topLoserCard->below = NULL;
    if (topWinnerCard != bottom[winner]) {
        bottom[winner]->below = topWinnerCard;
    }
    bottom[winner] = topLoserCard;

    if (top[winner] == NULL) {
        top[winner] = topWinnerCard;
    }
}

void cleanupCards(Card* top[2]) {
    Card* cleanup = top[0];
    while (cleanup != NULL) {
        Card* toClean = cleanup;
        cleanup = cleanup->below;
        free(toClean);
    }
    cleanup = top[1];
    while (cleanup != NULL) {
        Card* toClean = cleanup;
        cleanup = cleanup->below;
        free(toClean);
    }
}

int simulateGame(Card* top[2], Card* bottom[2], int cardCount) {
    while (top[0] != NULL && top[1] != NULL) {
        int winner = top[0]->value > top[1]->value ? 0 : 1;
        int loser = (winner + 1) % 2;
        endRound(top, bottom, winner, loser);
    }

    return top[0] != NULL ? 0 : 1;
}

int simulateRecursiveGame(Card* top[2], Card* bottom[2], int cardCount) {
    // Reddit optimization: if p1 has the largest number and it's > cardCount, player 1 wins
    Card* check = top[0];
    int value = check->value;
    while (check != NULL) {
        if (value < check->value) {
            value = check->value;
        }
        check = check->below;
    }
    check = top[1];
    while (check != NULL) {
        if (check->value > value) {
            value = -1;
            break;
        }
        check = check->below;
    }
    if (value > cardCount) {
        return 0;
    }

    // char for each card, plus 2 P's for P1/P2, and null char
    // Do this cause my hash table is bad
    char gameStates[4000][cardCount + 2 + 1];
    int gameStateCount = 0;

    while (top[0] != NULL && top[1] != NULL) {
        // First, check if any game states match the current one
        char gameState[cardCount + 2 + 1];
        int index = 0;
        gameState[index++] = 'P';

        int deckSize1 = 0;
        int deckSize2 = 0;

        Card* c = top[0];
        while (c != NULL) {
            gameState[index++] = (char)c->value;
            c = c->below;
            deckSize1++;
        }
        gameState[index++] = 'Q';
        c = top[1];
        while (c != NULL) {
            gameState[index++] = (char)c->value;
            c = c->below;
            deckSize2++;
        }
        gameState[index] = '\0';

        for (int i = 0; i < gameStateCount; i++) {
            if (strcmp(gameState, gameStates[i]) == 0) {
                // Match means player 1 wins
                return 0;
            }
        }
        memcpy(gameStates[gameStateCount++], gameState, cardCount + 2 + 1);

        // At this point, we can play the game
        Card* card1 = top[0];
        Card* card2 = top[1];

        int value1 = card1->value;
        int value2 = card2->value;

        int winner;
        int loser;
        if (value1 <= deckSize1 - 1 && value2 <= deckSize2 - 1) {
            // This means we have to play a recursive game
            Card* topCopy[2] = {0};
            Card* bottomCopy[2] = {0};

            int newCardCount = 0;

            c = card1->below;
            while (value1 > 0) {
                Card* newCard = calloc(1, sizeof(Card));
                newCard->value = c->value;

                if (topCopy[0] == NULL) {
                    topCopy[0] = newCard;
                }

                if (bottomCopy[0] != NULL) {
                    bottomCopy[0]->below = newCard;
                }
                bottomCopy[0] = newCard;

                c = c->below;
                value1--;
                newCardCount++;
            }

            c = card2->below;
            while (value2 > 0) {
                Card* newCard = calloc(1, sizeof(Card));
                newCard->value = c->value;

                if (topCopy[1] == NULL) {
                    topCopy[1] = newCard;
                }

                if (bottomCopy[1] != NULL) {
                    bottomCopy[1]->below = newCard;
                }
                bottomCopy[1] = newCard;

                c = c->below;
                value2--;
                newCardCount++;
            }

            winner = simulateRecursiveGame(topCopy, bottomCopy, newCardCount);
            loser = (winner + 1) % 2;
            
            cleanupCards(topCopy);
        } else {
            // Play normal game
            winner = value1 > value2 ? 0 : 1;
            loser = (winner + 1) % 2;
        }

        // Winner's card + loser's card goes on bottom of winner
        endRound(top, bottom, winner, loser);
    }

    return top[0] != NULL ? 0 : 1;
}

long getScore(Card* c, int cardCount) {
    long score = 0;

    while (cardCount > 0) {
        score += (c->value * cardCount);

        c = c->below;
        cardCount--;
    }

    return score;
}

void printDecks(Card* top[2]) {
    Card* c = top[0];
    printf("P1 = ");
    while (c != NULL) {
        printf("%d", c->value);

        if (c->below != NULL) {
            printf(" -> ");
        }
        c = c->below;
    }

    printf("\n");

    c = top[1];
    printf("P2 = ");
    while (c != NULL) {
        printf("%d", c->value);

        if (c->below != NULL) {
            printf(" -> ");
        }
        c = c->below;
    }

    printf("\n\n");
}

int main() {
    char* buffer = getFileBuffer("input.txt");
    char* startBuffer = buffer;

    if (buffer) {
        char* sizePtr = buffer;

        // 0 = player 1, 1 = player 2
        Card* top[2] = {0};
        Card* bottom[2] = {0};

        int playerIndex = -1;
        int cardCount = 0;
        while (sizePtr != NULL) {
            char* line = strsep(&sizePtr, "\n");

            if (strlen(line) == 0) {
                continue;
            }

            if (line[0] == 'P') {
                playerIndex++;
                continue;
            }

            Card* card = calloc(1, sizeof(Card));
            card->value = strtol(line, NULL, 10);

            if (top[playerIndex] == NULL) {
                top[playerIndex] = card;
            }

            if (bottom[playerIndex] != NULL) {
                bottom[playerIndex]->below = card;
            }
            bottom[playerIndex] = card;

            cardCount++;
        }

        int winner = simulateRecursiveGame(top, bottom, cardCount);
        printf("Winner: %d\n", winner);
        printDecks(top);
        printf("Score: %ld\n", getScore(top[0] != NULL ? top[0] : top[1], cardCount));

        cleanupCards(top);
    }

    free(startBuffer);
}
