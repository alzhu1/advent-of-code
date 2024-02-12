#include "../file.h"
#include "../hashtable.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

void maskP1(long* bitMask0Ptr, long* bitMask1Ptr, char* line) {
    long bitMask0 = 1;
    long bitMask1 = 0;
    for (int i = 0; i < strlen(line); i++) {
        if (line[i] == '0') {
            bitMask0--;
        } else if (line[i] == '1') {
            bitMask1 |= 1;
        }

        bitMask0 = (bitMask0 << 1) | 1;
        bitMask1 <<= 1;
    }

    bitMask0 >>= 1;
    bitMask1 >>= 1;

    *bitMask0Ptr = bitMask0;
    *bitMask1Ptr = bitMask1;
}

long long useMaskP2(char* mask, long long baseAddress, char*** p2AddressesStrPtr) {
    int xTotal = 0;
    long long xPos[strlen(mask)];

    int maskLength = strlen(mask);
    char baseAddressStr[strlen(mask) + 1];

    for (int i = 0; i < maskLength; i++) {
        long long shifter = 1;

        if (mask[i] == 'X') {
            xPos[xTotal++] = (maskLength - 1 - i);
            baseAddressStr[i] = 'X';
        } else if (mask[i] == '1') {
            baseAddressStr[i] = '1';
        } else {
            baseAddressStr[i] = (baseAddress & (shifter << (maskLength - 1 - i))) ? '1' : '0';
        }
    }

    long long addressSize = ((long long)1) << xTotal;
    *p2AddressesStrPtr = calloc(addressSize, sizeof(char*));

    for (long long i = 0; i < addressSize; i++) {
        char* p2AddressStr = (char*)calloc(strlen(baseAddressStr) + 1, sizeof(char));
        int xCount = 0;
        for (int c = 0; c < 36; c++) {
            if (baseAddressStr[c] != 'X') {
                p2AddressStr[c] = baseAddressStr[c];
            } else {
                // If it's x, use i to determine if it should be a 0 or 1
                p2AddressStr[c] = ((i & (((long long)1) << xCount++))) ? '1' : '0';
            }
        }
        (*p2AddressesStrPtr)[i] = p2AddressStr;
    }

    return addressSize;
}

int main() {
    char* buffer = getFileBuffer("input.txt");
    char* startBuffer = buffer;

    if (buffer) {
        char* sizePtr = buffer;

        long long mem[100000];
        memset(mem, 0, sizeof(mem));

        Hashtable* ht = initHashtable();

        long bitMask0; // 0 in bitMask places, 1 everywhere else (use AND)
        long bitMask1; // 1 in bitMask places, 0 everywhere else (use OR)

        char* mask2;
        long long runningSum = 0;

        while (sizePtr != NULL) {
            char* line = strsep(&sizePtr, "\n");

            char* first = strsep(&line, " ");
            if (strcmp(first, "mask") == 0) {
                // Add to line to get past the "= "
                line += 2;
                maskP1(&bitMask0, &bitMask1, line);

                mask2 = line;
            } else {
                strsep(&first, "[");
                long long address = strtoll(strsep(&first, "]"), NULL, 10);

                // Get the number after "= " in line
                long long value = strtoll(line + 2, NULL, 10);

                // Part 1
                mem[address] = (value & bitMask0) | bitMask1;

                // Part 2
                char** p2AddressesStr = NULL;
                long long p2AddressesSize = useMaskP2(mask2, address, &p2AddressesStr);
                for (int i = 0; i < p2AddressesSize; i++) {
                    char* str = p2AddressesStr[i];

                    HashtableEntry* item = getHashtableItem(ht, str);
                    if (item != NULL) {
                        runningSum -= (long long)item->value;
                    }
                    runningSum += value;

                    // Fuck it
                    putHashtableItem(ht, str, (void*)value);

                    // Non-null means we replaced a value, so free the allocated string
                    if (item != NULL) {
                        free(str);
                    }
                }
                free(p2AddressesStr);
            }
        }

        long long sum = 0;
        for (long i; i < 100000; i++) {
            sum += mem[i];
        }
        printf("Sum: %lld\n", sum);
        printf("Sum2: %lld\n", runningSum);

        destroyHashtable(ht, 1, 0);
    }

    free(startBuffer);
}
