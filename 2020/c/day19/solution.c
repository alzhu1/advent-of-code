#include "../file.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

typedef struct Node Node;

struct Node {
    char* rule1;
    char* rule2;

    long cardinality;
    char** patterns;
};

void buildPatterns(char** patterns, long cardinality, int subRule1, int subRule2, Node** nodes) {
    for (int patternIndex = 0; patternIndex < cardinality; patternIndex++) {
        Node* firstNode = nodes[subRule1];
        char* firstPattern = firstNode->patterns[patternIndex % firstNode->cardinality];

        char* secondPattern = "";
        if (subRule2 >= 0) {
            Node* secondNode = nodes[subRule2];
            secondPattern = secondNode->patterns[patternIndex / firstNode->cardinality];
        }

        int firstLength = strlen(firstPattern);
        int secondLength = strlen(secondPattern);

        // Combine both patterns
        patterns[patternIndex] = calloc(firstLength + secondLength + 1, sizeof(char));
        strncpy(patterns[patternIndex], firstPattern, firstLength);
        strncpy(patterns[patternIndex] + firstLength, secondPattern, secondLength);
    }    
}

// Recursive function assumes rules are at most 2 sub rules per group
void search(int ruleNum, Node** nodes) {
    Node* currNode = nodes[ruleNum];

    if (currNode->cardinality) {
        return;
    }

    if (currNode->rule1[0] == '\"') {
        currNode->cardinality = 1;

        // This is because the rule is "a" or "b", i.e. 2nd char
        currNode->patterns = calloc(currNode->cardinality, sizeof(char*));
        currNode->patterns[0] = calloc(2, sizeof(char));
        currNode->patterns[0][0] = currNode->rule1[1];
        return;
    }

    int tempSubRule[2] = { -1, -1 };
    int index = 0;

    long cardinality1 = 1;
    while (currNode->rule1 != NULL) {
        int subRuleNum = strtol(strsep(&currNode->rule1, " "), NULL, 10);

        search(subRuleNum, nodes);
        cardinality1 *= nodes[subRuleNum]->cardinality;

        tempSubRule[index++] = subRuleNum;
    }

    char* patterns1[cardinality1];
    buildPatterns(patterns1, cardinality1, tempSubRule[0], tempSubRule[1], nodes);

    index = 0;
    long cardinality2 = currNode->rule2 != NULL ? 1 : 0;
    while (currNode->rule2 != NULL) {
        int subRuleNum = strtol(strsep(&currNode->rule2, " "), NULL, 10);

        search(subRuleNum, nodes);
        cardinality2 *= nodes[subRuleNum]->cardinality;

        tempSubRule[index++] = subRuleNum;
    }

    char* patterns2[cardinality2];
    buildPatterns(patterns2, cardinality2, tempSubRule[0], tempSubRule[1], nodes);

    currNode->cardinality = cardinality1 + cardinality2;
    currNode->patterns = calloc(currNode->cardinality, sizeof(char*));

    for (int i = 0; i < currNode->cardinality; i++) {
        char* pattern = i < cardinality1 ? patterns1[i] : patterns2[i - cardinality1];
        currNode->patterns[i] = pattern;
    }
}

int main() {
    char* buffer = getFileBuffer("input.txt");
    char* startBuffer = buffer;

    if (buffer) {
        char* sizePtr = buffer;

        Node** nodes = calloc(200, sizeof(Node*));
        while (sizePtr != NULL) {
            char* line = strsep(&sizePtr, "\n");

            if (strlen(line) == 0) {
                break;
            }

            int ruleNum = strtol(strsep(&line, ":"), NULL, 10);
            line++;

            nodes[ruleNum] = calloc(1, sizeof(Node));

            char* rule1 = strsep(&line, "|");
            nodes[ruleNum]->rule1 = rule1;

            if (line != NULL) {
                rule1[strlen(rule1) - 1] = '\0';
                line++;
                nodes[ruleNum]->rule2 = line;
            }
        }

        // Rule 0 = 8 11
        // Rule 8 = 42
        // Rule 11 = 42 31
        // This means we only need to search nodes 42 and 31
        search(42, nodes);
        search(31, nodes);

        // For my input, verified that nodes 42 and 31 have the same number of items
        if (nodes[42]->cardinality != nodes[31]->cardinality) {
            printf("ERROR: Cardinality does not match\n");
            return -1;
        }

        long cardinality = nodes[42]->cardinality;
        int groupLength = strlen(nodes[42]->patterns[0]);

        // Create array to hold pattern matches, encoded as numbers
        // a = 1, b = 0
        int patternMatches[1 << groupLength];
        memset(patternMatches, 0, sizeof(patternMatches));

        for (int i = 0; i < cardinality; i++) {
            int index42 = 0;
            int index31 = 0;
            for (int j = 0; j < groupLength; j++) {
                if (nodes[42]->patterns[i][j] == 'a') {
                    index42 |= 1;
                }

                if (nodes[31]->patterns[i][j] == 'a') {
                    index31 |= 1;
                }

                index42 <<= 1;
                index31 <<= 1;
            }
            index42 >>= 1;
            index31 >>= 1;

            // 1s place means string matches 42, 2s place means string matches 31
            patternMatches[index42] |= 1;
            patternMatches[index31] |= 2;
        }

        int count = 0;
        int count2 = 0;
        while (sizePtr != NULL) {
            char* line = strsep(&sizePtr, "\n");

            // Split up line into multiple groups
            int groups = strlen(line) / groupLength;
            char group[groupLength + 1];
            group[groupLength] = '\0';

            int rule42Count = 0;
            int rule31Count = 0;
            for (int i = 0; i < groups; i++) {
                strncpy(group, line + (i * groupLength), groupLength);

                int index = 0;
                for (int j = 0; j < groupLength; j++) {
                    if (group[j] == 'a') {
                        index |= 1;
                    }
                    index <<= 1;
                }
                index >>= 1;

                // The group matches 42
                if (patternMatches[index] & 1) {
                    if (rule31Count) {
                        rule42Count = -1;
                        break;
                    }

                    rule42Count++;
                }

                // The group matches 31
                if (patternMatches[index] & 2) {
                    rule31Count++;
                }
            }

            // Part 1: Rule 0 effective matches to (42 42 31)
            if (rule42Count == 2 && rule31Count == 1) {
                count++;
            }

            // Part 2: The recursive rules means 42 count > 31 count, and 42 can't come after
            if (rule42Count > rule31Count && rule31Count > 0) {
                count2++;
            }
        }

        printf("Part 1 count = %d, Part 2 count = %d\n", count, count2);
        for (int i = 0; i < 200; i++) {
            Node* node = nodes[i];

            if (node != NULL) {
                for (int j = 0; j < node->cardinality; j++) {
                    free(node->patterns[j]);
                }
                free(node->patterns);
            }
            free(node);
        }
        free(nodes);
    }

    free(startBuffer);
}
