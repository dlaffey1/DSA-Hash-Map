#ifndef HASH_MAP_H
#define HASH_MAP_H

#include <stdbool.h>

#define HASH_MAP_SIZE 1000

typedef struct {
    int docID;
    int position;
} WordEntry;

typedef struct HashMapNode {
    char *key;
    WordEntry *entries; // Array of WordEntry
    int count; // Number of entries
    struct HashMapNode *next;
} HashMapNode;

typedef struct {
    HashMapNode *table[HASH_MAP_SIZE];
} HashMap;

void initHashMap(HashMap *map);
void insert(HashMap *map, const char *word, WordEntry entry);
void searchWord(HashMap *map, const char *word);
void freeHashMap(HashMap *map);

#endif // HASH_MAP_H
