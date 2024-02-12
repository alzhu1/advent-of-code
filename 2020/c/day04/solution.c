#include "../file.h"
#include "../hashtable.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

int main() {
    char* buffer = getFileBuffer("input.txt");
    char* startBuffer = buffer;

    if (buffer) {
        Hashtable* eyeHt = initHashtable();
        char* eyes[] = {"amb", "blu", "brn", "gry", "grn", "hzl", "oth"};
        for (int i = 0; i < 7; i++) {
            putHashtableItem(eyeHt, eyes[i], eyes[i]);
        }

        char* sizePtr = buffer;
        Hashtable* ht = initHashtable();

        int validCountP1 = 0;
        int validCountP2 = 0;
        while (strlen(sizePtr) > 0) {
            char* line = strsep(&sizePtr, "\n");

            printf("%s\n", line);

            if (strlen(line) == 0) {
                // Check the table at this point
                if (ht->length == 8 || (ht->length == 7 && getHashtableItem(ht, "cid") == NULL)) {
                    validCountP1++;

                    // Part 2: Do additional validation
                    long byr = strtol((char*)getHashtableItem(ht, "byr")->value, NULL, 10);
                    long iyr = strtol((char*)getHashtableItem(ht, "iyr")->value, NULL, 10);
                    long eyr = strtol((char*)getHashtableItem(ht, "eyr")->value, NULL, 10);

                    char* hgtStr = (char*)getHashtableItem(ht, "hgt")->value;
                    int isCm = hgtStr[strlen(hgtStr) - 1] == 'm';
                    int isIn = hgtStr[strlen(hgtStr) - 1] == 'n';

                    char tempStore = hgtStr[strlen(hgtStr) - 2];
                    hgtStr[strlen(hgtStr) - 2] = '\0';
                    long hgt = strtol(hgtStr, NULL, 10);
                    hgtStr[strlen(hgtStr)] = tempStore;
                    
                    char* hcl = (char*)getHashtableItem(ht, "hcl")->value;
                    char* ecl = (char*)getHashtableItem(ht, "ecl")->value;
                    char* pid = (char*)getHashtableItem(ht, "pid")->value;

                    int hclHex = 1;
                    for (int i = 1; i < 7; i++) {
                        int hexChar = (hcl[i] >= '0' && hcl[i] <= '9') || (hcl[i] >= 'a' && hcl[i] <= 'f');
                        if (!hexChar) {
                            hclHex = 0;
                            break;
                        }
                    }

                    if (
                        (byr >= 1920 && byr <= 2002) &&
                        (iyr >= 2010 && iyr <= 2020) &&
                        (eyr >= 2020 && eyr <= 2030) &&
                        ((isCm && hgt >= 150 && hgt <= 193) || (isIn && hgt >= 59 && hgt <= 76)) &&
                        (strlen(hcl) == 7 && hcl[0] == '#' && hclHex) &&
                        (getHashtableItem(eyeHt, ecl) != NULL) &&
                        (strlen(pid) == 9)
                    ) {
                        validCountP2++;
                    }
                }

                destroyHashtable(ht, 0, 0);
                ht = initHashtable();

                continue;
            }

            while (line != NULL) {
                char* key = strsep(&line, ":");
                char* value = strsep(&line, " ");
                putHashtableItem(ht, key, value);
            }
        }

        printf("Part 1 valid count: %d\n", validCountP1);
        printf("Part 2 valid count: %d\n", validCountP2);

        destroyHashtable(ht, 0, 0);
        destroyHashtable(eyeHt, 0, 0);
    }

    free(startBuffer);
}
