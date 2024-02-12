#include "hashtable.h"

// http://www.cse.yorku.ca/~oz/hash.html
size_t hash(char *str, int keyLength) {
    size_t hash = 5381;
    int c;
    int index = 0;

    while ((c = *str++) || keyLength) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
        index++;

        if (keyLength != 0 && index >= keyLength) {
            break;
        }
    }

    return hash;
}

Hashtable* initHashtableWithLength(int keyLength) {
    Hashtable* ht = (Hashtable*)(malloc(sizeof(Hashtable)));
    ht->length = 0;
    ht->capacity = DEFAULT_SIZE;
    ht->keyLength = keyLength;
    ht->table = (HashtableEntry**)(calloc(DEFAULT_SIZE, sizeof(HashtableEntry*)));
    return ht;
}

Hashtable* initHashtable() {
    return initHashtableWithLength(0);
}

HashtableEntry* getHashtableItem(Hashtable* ht, char* key) {
    size_t keyHash = hash(key, ht->keyLength) % ht->capacity;
    size_t index = keyHash;

    HashtableEntry* hte = ht->table[index];
    while (hte != NULL) {
        if (ht->keyLength && memcmp(hte->key, key, ht->keyLength) == 0) {
            break;
        }

        if (ht->keyLength == 0 && strcmp(hte->key, key) == 0) {
            break;
        }
        hte = hte->next;
    }

    if (hte == NULL) {
        return NULL;
    }
    return hte;
}

int putHashtableItem(Hashtable* ht, char* key, void* value) {
    if (ht->length >= ht->capacity) {
        return -1;
    }

    size_t keyHash = hash(key, ht->keyLength) % ht->capacity;
    size_t index = keyHash;

    HashtableEntry* hte = ht->table[index];
    while (hte != NULL) {
        if (ht->keyLength && memcmp(hte->key, key, ht->keyLength) == 0) {
            break;
        }

        if (ht->keyLength == 0 && strcmp(hte->key, key) == 0) {
            break;
        }
        hte = hte->next;
    }

    if (hte != NULL) {
        hte->value = value;
        return 0;
    }

    hte = (HashtableEntry*)malloc(sizeof(HashtableEntry));
    hte->key = key;
    hte->value = value;
    hte->next = ht->table[index];
    ht->table[index] = hte;
    ht->length++;

    float loadFactor = ((float)ht->length) / ht->capacity;
    if (loadFactor >= MAX_LOAD_FACTOR) {
        resize(ht);
    }

    return 0;
}

int removeHashtableItem(Hashtable* ht, char* key, int freeKey, int freeValue) {
    size_t keyHash = hash(key, ht->keyLength) % ht->capacity;
    size_t index = keyHash;

    HashtableEntry* hte = ht->table[index];

    // Not found
    if (hte == NULL) {
        return -1;
    }

    // First thing in list
    if (
        (ht->keyLength == 0 && strcmp(hte->key, key) == 0) ||
        (ht->keyLength && memcmp(hte->key, key, ht->length))
    ) {
        ht->table[index] = hte->next;
        if (freeKey) {
            free(hte->key);
        }

        if (freeValue) {
            free(hte->value);
        }
        free(hte);
        ht->length--;
        return 0;
    }

    HashtableEntry* prev = hte;
    hte = hte->next;

    // Otherwise need to check list (cause it has length > 2)
    while (hte != NULL) {
        if (ht->keyLength && memcmp(hte->key, key, ht->keyLength) == 0) {
            break;
        }

        if (ht->keyLength == 0 && strcmp(hte->key, key) == 0) {
            break;
        }
        prev = hte;
        hte = hte->next;
    }

    // hte should be the one before the key (if found)
    if (hte == NULL) {
        return -1;
    } else {
        prev->next = hte->next;
        if (freeKey) {
            free(hte->key);
        }

        if (freeValue) {
            free(hte->value);
        }
        free(hte);
        ht->length--;
        return 0;
    }
}

void destroyHashtable(Hashtable* ht, int freeKey, int freeValue) {
    // Destroy each item under hashtable
    for (int i = 0; i < ht->capacity; i++) {
        HashtableEntry* hte = ht->table[i];
        while (hte != NULL) {
            HashtableEntry* next = hte->next;
            if (freeKey) {
                free(hte->key);
            }

            if (freeValue) {
                free(hte->value);
            }
            free(hte);
            hte = next;
        }
    }
    free(ht->table);
    free(ht);
}

void resize(Hashtable* ht) {
    // TODO: Seems like resizing has a bug, investigate. Use day15 as an example
    printf("Resizing...\n");

    // Double capacity
    size_t oldCapacity = ht->capacity;
    size_t newCapacity = oldCapacity * 2;
    size_t length = ht->length;

    // Reallocate memory
    ht->capacity = newCapacity;
    ht->length = 0;
    ht->table = (HashtableEntry**)(realloc(ht->table, sizeof(HashtableEntry*) * newCapacity));

    // Temporarily store every item in another array, to be re-inserted
    HashtableEntry* temp[length];
    int index = 0;
    for (int i = 0; i < oldCapacity; i++) {
        HashtableEntry* hte = ht->table[i];
        while (hte != NULL) {
            temp[index++] = hte;
            hte = hte->next;
        }
    }
    memset(ht->table, 0, sizeof(HashtableEntry*) * newCapacity);

    // Re-insert all items
    for (int i = 0; i < length; i++) {
        putHashtableItem(ht, temp[i]->key, temp[i]->value);
        free(temp[i]);
    }
}