#include "../file.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

int main() {
    char* buffer = getFileBuffer("input.txt");
    char* startBuffer = buffer;

    if (buffer) {
        char* sizePtr = buffer;

        __int128_t timestamp = strtoll(strsep(&sizePtr, "\n"), NULL, 10);
        __int128_t buses[1000];
        memset(buses, 0, sizeof(buses));

        __int128_t busCount = 0;
        __int128_t length = 0;
        while (sizePtr != NULL) {
            char* temp = strsep(&sizePtr, ",");
            length+= 1;
            if (temp[0] == 'x') {
                continue;
            }

            buses[length - 1] = strtol(temp, NULL, 10);
        }

        __int128_t busId = buses[0];
        __int128_t bestTimestamp = timestamp + busId; // Should be larger than actual value

        for (__int128_t i = 0; i < length; i+= 1) {
            if (buses[i] == 0) {
                continue;
            }

            __int128_t earliestBusTime = timestamp - (timestamp % buses[i]) + buses[i];
            if (earliestBusTime < bestTimestamp) {
                bestTimestamp = earliestBusTime;
                busId = buses[i];
            }
        }

        __int128_t diff = bestTimestamp - timestamp;

        printf("Bus id (bid) of earliest arrival = %lld", busId);
        printf(" at %lld", bestTimestamp);
        printf(", diff = %lld", diff);
        printf(", bid * diff = %lld\n", busId * diff);

        __int128_t p2Timestamp = 0;
        __int128_t adder = (__int128_t)buses[0];
        for (__int128_t i = 1; i < length; i+= 1) {
            if (buses[i] == 0) {
                continue;
            }

            // Pseudocode from extended Euclidean alg
            __int128_t oldR = adder;
            __int128_t r = buses[i];
            __int128_t oldS = 1;
            __int128_t s = 0;
            __int128_t oldT = 0;
            __int128_t t = 1;

            while (r != 0) {
                __int128_t quotient = oldR / r;
                __int128_t tempR = r;
                r = oldR - (quotient * r);
                oldR = tempR;

                __int128_t tempS = s;
                s = oldS - (quotient * s);
                oldS = tempS;

                __int128_t tempT = t;
                t = oldT - (quotient * t);
                oldT = tempT;
            }

            __int128_t moduloResult = (buses[i] - i);
            while (moduloResult < 0) {
                moduloResult += buses[i];
            }
            moduloResult %= buses[i];

            // oldS is coefficient of adder
            // oldT is coeffecient of buses[i]
            __int128_t nextAdder = adder * buses[i];
            __int128_t res = (oldS * adder * moduloResult) % nextAdder + (buses[i] * (oldT * p2Timestamp) % nextAdder) % nextAdder;

            adder *= buses[i];
            while (res < 0) {
                res += adder;
            }
            res %= adder;

            p2Timestamp = res;
        }

        printf("Timestamp of earliest departure in order: %lld\n", p2Timestamp);
    }

    free(startBuffer);
}