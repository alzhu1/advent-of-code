#include "../file.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "ctype.h"

long calculate(char* problem, int* index, int length) {
    long value = 0;
    char currOp = '+';

    while (*index < length) {
        if (problem[*index] == ' ') {
            (*index)++;
            continue;
        }

        // Check the character
        char next = problem[*index];

        if (next == '(') {
            // Start of paren means we should dive in
            (*index)++;
            long result = calculate(problem, index, length);

            switch (currOp) {
                case '+':
                    value += result;
                    break;
                case '*':
                    value *= result;
                    break;
            }
        }

        if (next == ')') {
            // End of recursion
            return value;
        }

        if (isdigit(next)) {
            long num = next - '0';
            switch (currOp) {
                case '+':
                    value += num;
                    break;
                case '*':
                    value *= num;
                    break;
            }
        } else if (next == '+' || next == '*') {
            currOp = next;
        }

        (*index)++;
    }

    return value;
}

int getPrecedence(char op) {
    switch (op) {
        case '+': return 2;
        case '*': return 1;
        // Unfound operator returns -1, so precedence is ignored
        default: return -1;
    }
}

// Part 2 based on pseudocode from https://en.wikipedia.org/wiki/Operator-precedence_parser#Pseudocode
long calculateWithPrecedence(long lhs, int minPrecedence, char* problem, int* index, int length);

// This function handles incrementing index
long getNextValue(char* problem, int* index, int length) {
    char token = problem[*index];

    // Go to next valid index for value
    while (!isdigit(token) && token != '(') {
        (*index)++;
        token = problem[*index];
    }

    if (isdigit(token)) {
        // Index should always end at the space after the number
        (*index)++;
        return token - '0';
    } else if (token == '(') {
        // Go to space after parentheses, then after calculation finishes move once more to go past right paren
        (*index)++;
        long value = calculateWithPrecedence(getNextValue(problem, index, length), 0, problem, index, length);
        (*index)++;
        return value;
    }

    printf("ERROR: This should never occur\n");
    return 0;
}

char peekNextToken(char* problem, int peekIndex, int length) {
    // Invariant: for peeks, we should NOT increment index (peekIndex is copy)
    char nextToken = problem[peekIndex];
    while (nextToken != '+' && nextToken != '*' && nextToken != ')' && nextToken != '\0') {
        peekIndex++;
        nextToken = problem[peekIndex];
    }
    return nextToken;
}

long calculateWithPrecedence(long lhs, int minPrecedence, char* problem, int* index, int length) {
    char lookahead = peekNextToken(problem, *index, length);

    while (getPrecedence(lookahead) >= minPrecedence) {
        char op = lookahead;
        long rhs = getNextValue(problem, index, length);

        // Check the next lookahead token to see if it's an operator of higher precedence (or ending)
        lookahead = peekNextToken(problem, *index, length);
        while (getPrecedence(lookahead) > getPrecedence(op)) {
            // If higher precedence detected on rhs, we should do that calculation first
            rhs = calculateWithPrecedence(rhs, getPrecedence(op) + 1, problem, index, length);
            lookahead = peekNextToken(problem, *index, length);
        }

        // Apply op
        switch (op) {
            case '+':
                lhs += rhs;
                break;
            case '*':
                lhs *= rhs;
                break;
            default:
                printf("ERROR: This should not be encountered\n");
                break;
        }
    }

    return lhs;
}

int main() {
    char* buffer = getFileBuffer("input.txt");
    char* startBuffer = buffer;

    if (buffer) {
        char* sizePtr = buffer;

        long wholeSum = 0;
        long wholeSum2 = 0;
        while (sizePtr != NULL) {
            char* line = strsep(&sizePtr, "\n");
            int length = strlen(line);

            int index = 0;
            wholeSum += calculate(line, &index, length);
            index = 0;
            wholeSum2 += calculateWithPrecedence(getNextValue(line, &index, length), 0, line, &index, length);
        }

        printf("Part 1 sum = %ld, Part 2 sum = %ld\n", wholeSum, wholeSum2);
    }

    free(startBuffer);
}
