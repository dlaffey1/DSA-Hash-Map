#ifndef HASH_MAP_H
#define HASH_MAP_H

#include <string>
#include "vector.h" 

#define HASH_MAP_SIZE 100

struct WordEntry {
    int docID;
    int position;
    std::string fileName;
    float tf;   // Term Frequency
    float idf;  // Inverse Document Frequency
    float tfidf; // TF-IDF
};


struct HashMapNode {
    std::string key;
    Vector<WordEntry> entries; // Using Tamara's Vector class
    HashMapNode *next;
};

struct HashMap {
    HashMapNode *table[HASH_MAP_SIZE];
};

void initHashMap(HashMap *map);
bool insert(HashMap *map, const std::string &key, const WordEntry &entry);
void searchWord(HashMap *map, const char *word);
void freeHashMap(HashMap *map);

#endif
