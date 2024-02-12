#include "../file.h"
#include "../hashtable.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

int main() {
    char* buffer = getFileBuffer("input.txt");
    char* startBuffer = buffer;

    if (buffer) {
        char* sizePtr = buffer;

        Hashtable* ht = initHashtable();
        char* types[20];
        memset(types, 0, sizeof(types));
        int typeLength = 0;

        int phase = 0;
        int errorRate = 0;

        char* myTicket = NULL;
        int** nearbyTickets = (int**)calloc(300, sizeof(int*));
        int nearbyTicketCount = 0;

        while (sizePtr != NULL) {
            char* line = strsep(&sizePtr, "\n");

            if (strlen(line) == 0) {
                continue;
            }

            if (strcmp(line, "your ticket:") == 0) {
                phase = 1;
                continue;
            }
            
            if (strcmp(line, "nearby tickets:") == 0) {
                phase = 2;
                continue;
            }

            if (phase == 0) {
                // Reading the ranges
                char* type = strsep(&line, ":");
                line++;
                char* range1 = strsep(&line, " ");
                strsep(&line, " ");
                char* range2 = strsep(&line, " ");

                int startRange1 = strtol(strsep(&range1, "-"), NULL, 10);
                int endRange1 = strtol(range1, NULL, 10);
                int startRange2 = strtol(strsep(&range2, "-"), NULL, 10);
                int endRange2 = strtol(range2, NULL, 10);

                int* validNums = (int*)calloc(1000, sizeof(int));
                for (int i = 0; i < 1000; i++) {
                    if ((i >= startRange1 && i <= endRange1) || (i >= startRange2 && i <= endRange2)) {
                        validNums[i] = 1;
                    }
                }
                
                putHashtableItem(ht, type, validNums);
                types[typeLength++] = type;
            } else if (phase == 1) {
                myTicket = line;
            } else if (phase == 2) {
                int overallValid = 1;

                // Weird bug here? Using typeLength will affect values in myTicket (if array declared earlier)
                int ticketNums[typeLength];
                memset(ticketNums, 0, sizeof(ticketNums));
                int ticketNumsIndex = 0;

                while (line != NULL) {
                    int num = strtol(strsep(&line, ","), NULL, 10);
                    ticketNums[ticketNumsIndex++] = num;

                    int valid = 0;
                    for (int i = 0; i < typeLength; i++) {
                        int* numsForType = getHashtableItem(ht, types[i])->value;
                        if (numsForType[num]) {
                            valid = 1;
                            break;
                        }
                    }

                    if (!valid) {
                        errorRate += num;
                        overallValid = 0;
                        continue;
                    }
                }

                if (overallValid) {
                    // If overall it is valid, then copy it over to another pointer
                    int* ticket = (int*)calloc(typeLength, sizeof(int));
                    memcpy(ticket, ticketNums, sizeof(ticketNums));
                    nearbyTickets[nearbyTicketCount++] = ticket;
                }
            }
        }

        printf("Error rate: %d\n", errorRate);

        // Now create a char* [][] of length typeLength, use this to determine possibles per index
        char* possibleTypes[typeLength][typeLength + 1];
        memset(possibleTypes, 0, sizeof(possibleTypes));

        for (int i = 0; i < typeLength; i++) {
            for (int j = 0; j < typeLength; j++) {
                possibleTypes[i][j] = types[j];
            }
        }

        for (int i = 0; i < nearbyTicketCount; i++) {
            for (int j = 0; j < typeLength; j++) {
                // nearbyTickets[i][j] will be the jth field of the ith ticket
                int* ticket = nearbyTickets[i];
                int field = ticket[j];

                for (int k = 0; k < typeLength; k++) {
                    int* validNumsForType = getHashtableItem(ht, types[k])->value;

                    // For the kth type, check if the field from ticket matches the range it
                    if (!validNumsForType[field]) {
                        // This means this is NOT a possible field, remove it from the list
                        int tempIndex = 0;
                        int exists = 0;
                        while (possibleTypes[j][tempIndex] != NULL) {
                            if (strcmp(possibleTypes[j][tempIndex], types[k]) == 0) {
                                exists = 1;
                            }

                            if (exists) {
                                possibleTypes[j][tempIndex] = possibleTypes[j][tempIndex + 1];
                            }

                            tempIndex++;
                        }
                    }
                }
            }
        }

        int setFields[typeLength];
        memset(setFields, 0, sizeof(setFields));
        while (1) {
            for (int i = 0; i < typeLength; i++) {
                if (setFields[i]) {
                    continue;
                }

                int count = 0;
                while (possibleTypes[i][count] != NULL) {
                    count++;
                }

                if (count == 1) {
                    // This is the only type it can be, it should be set
                    setFields[i] = 1;

                    // loop around and remove the set field from the others
                    for (int j = 0; j < typeLength; j++) {
                        if (j == i) {
                            continue;
                        }

                        int tempIndex = 0;
                        int exists = 0;
                        while (possibleTypes[j][tempIndex] != NULL) {
                            if (strcmp(possibleTypes[j][tempIndex], possibleTypes[i][0]) == 0) {
                                exists = 1;
                            }

                            if (exists) {
                                possibleTypes[j][tempIndex] = possibleTypes[j][tempIndex + 1];
                            }

                            tempIndex++;
                        }
                    }
                }
            }

            int set = 1;
            for (int i = 0; i < typeLength; i++) {
                set &= setFields[i];
            }

            if (set) {
                break;
            }
        }

        int myTicketNums[typeLength];
        int myTicketIndex = 0;
        while (myTicket != NULL) {
            myTicketNums[myTicketIndex++] = strtol(strsep(&myTicket, ","), NULL, 10);
        }

        long product = 1;
        for (int i = 0; i < typeLength; i++) {
            printf("[%d]: ", i);

            if (possibleTypes[i][1] != NULL) {
                printf("PROBLEM\n");
            } else {
                printf("%s\n", possibleTypes[i][0]);

                char copy[100];
                memset(copy, 0, sizeof(copy));
                strcpy(copy, possibleTypes[i][0]);

                if (strcmp(strtok(copy, " "), "departure") == 0) {
                    product *= myTicketNums[i];
                }
            }
        }

        printf("Departure product: %ld\n", product);
        for (int i = 0; i < nearbyTicketCount; i++) {
            free(nearbyTickets[i]);
        }
        free(nearbyTickets);

        destroyHashtable(ht, 0, 1);
    }

    free(startBuffer);
}
