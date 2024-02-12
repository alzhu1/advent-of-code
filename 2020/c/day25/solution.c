#include "stdio.h"
#include "stdlib.h"
#include "string.h"

// If loopSize == 0, returns loop size. Otherwise returns value
long transform(long subjectNumber, int loopSize, long matcher) {
    long value = 1;
    int loopCount = 0;

    while (loopSize == 0 || loopCount < loopSize) {
        loopCount++;
        value *= subjectNumber;
        value %= 20201227;

        if (loopSize == 0 && value == matcher) {
            return loopCount;
        }
    }

    return value;
}

int main() {
    long cardPublicKey = 17115212;
    long doorPublicKey = 3667832;

    long cardLoopSize = transform(7, 0, cardPublicKey);
    long doorLoopSize = transform(7, 0, doorPublicKey);

    printf("Card loop size: %ld\n", cardLoopSize);
    printf("Door loop size: %ld\n", doorLoopSize);

    long value1 = transform(doorPublicKey, cardLoopSize, 0);
    long value2 = transform(cardPublicKey, doorLoopSize, 0);

    if (value1 == value2) {
        printf("Value: %ld\n", value1);
    } else {
        printf("ERROR: no match\n");
    }
}
