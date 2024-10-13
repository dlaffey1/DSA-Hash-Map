#ifndef HASH_MAP_H
#define HASH_MAP_H

#include <string>

#define HASH_MAP_SIZE 100

typedef struct {
    int docID;
    int position;
    std::string fileName;  // Add this field to store the file name
} WordEntry;

typedef struct HashMapNode {
    char *key;
    WordEntry *entries;
    int count;
    struct HashMapNode *next;
} HashMapNode;

typedef struct {
    HashMapNode *table[HASH_MAP_SIZE];
} HashMap;

void initHashMap(HashMap *map);
void insert(HashMap *map, const char *word, WordEntry entry);
void searchWord(HashMap *map, const char *word);
void freeHashMap(HashMap *map);

#endif
