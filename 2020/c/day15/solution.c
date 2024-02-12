#include "../hashtable.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

int main() {
    char* input = (char*)calloc(100, sizeof(char));
    strcpy(input, "2,20,0,4,1,17");

    char* sizePtr = input;
    long index = 0;

    long prev = -1;
    long curr = -1;

    Hashtable* ht = initHashtable();
    while (sizePtr != NULL) {
        char* num = strsep(&sizePtr, ",");

        prev = curr;
        curr = strtol(num, NULL, 10);

        char* str = (char*)calloc(10, sizeof(char));
        sprintf(str, "%ld", curr);

        putHashtableItem(ht, str, (void*)(-index));
        index++;
    }
    free(input);

    while (1) {
        prev = curr;
        char* str = (char*)calloc(10, sizeof(char));
        sprintf(str, "%ld", prev);

        HashtableEntry* item = getHashtableItem(ht, str);
        long value = item != NULL ? (long)item->value : 0;

        if (value < 0) {
            curr = 0;
            putHashtableItem(ht, str, (void*)(-value));
        } else {
            curr = (index - 1) - value;
            putHashtableItem(ht, str, (void*)(index - 1));
        }

        char* temp = (char*)calloc(10, sizeof(char));
        sprintf(temp, "%ld", curr);

        HashtableEntry* itemTemp = getHashtableItem(ht, temp);
        if (itemTemp == NULL) {
            putHashtableItem(ht, temp, (void*)(-(index)));
        } else {
            long tempValue = (long)itemTemp->value;
            if (tempValue < 0) {
                tempValue = -tempValue;
            }

            putHashtableItem(ht, temp, (void*)(tempValue));
            free(temp);
        }

        if (item != NULL) {
            free(str);
        }

        if (index == (2020 - 1)) {
            printf("At 2020: %ld\n", curr);
        }

        if (index == (30000000 - 1)) {
            printf("At 30000000: %ld\n", curr);
            break;
        }

        index++;
    }

    destroyHashtable(ht, 1, 0);
}