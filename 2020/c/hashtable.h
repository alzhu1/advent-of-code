#pragma once

#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#define DEFAULT_SIZE 70000000
#define MAX_LOAD_FACTOR 0.5

typedef struct Hashtable Hashtable;
typedef struct HashtableEntry HashtableEntry;

struct HashtableEntry {
    char* key;
    void* value;
    HashtableEntry* next;
};

struct Hashtable {
    size_t length;
    size_t capacity;
    int keyLength;
    HashtableEntry** table; // basically a array of pointers
};

Hashtable* initHashtableWithLength(int keyLength);
Hashtable* initHashtable();
HashtableEntry* getHashtableItem(Hashtable* ht, char* key);
int putHashtableItem(Hashtable* ht, char* key, void* value);
int removeHashtableItem(Hashtable* ht, char* key, int freeKey, int freeValue);
void destroyHashtable(Hashtable* ht, int freeKey, int freeValue);
void resize(Hashtable* ht);
