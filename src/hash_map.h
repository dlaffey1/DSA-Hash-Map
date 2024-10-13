#ifndef HASH_MAP_H
#define HASH_MAP_H

#include <string>
#include <vector>

#define HASH_MAP_SIZE 100

struct WordEntry {
    int docID;
    int position;
    std::string fileName;  // Use std::string for safety
};

struct HashMapNode {
    std::string key;         // Use std::string for keys
    std::vector<WordEntry> entries; // Use vector to manage entries
    HashMapNode *next;
};

struct HashMap {
    HashMapNode *table[HASH_MAP_SIZE];
};

void initHashMap(HashMap *map);
void insert(HashMap *map, const char *word, WordEntry entry);
void searchWord(HashMap *map, const char *word);
void freeHashMap(HashMap *map);

#endif
