#pragma once

#include "stdlib.h"
#include "stdio.h"

char* getFileBuffer(char* filename) {
    char * buffer = 0;
    long length;
    FILE *f = fopen(filename, "r");

    if (f) {
        fseek(f, 0, SEEK_END);
        length = ftell (f);
        fseek(f, 0, SEEK_SET);
        buffer = (char*)malloc(length);
        if (buffer) {
            fread(buffer, 1, length, f);
        }
        fclose(f);
    }

    return buffer;
}