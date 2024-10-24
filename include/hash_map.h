#ifndef HASH_MAP_H
#define HASH_MAP_H

#include <string>
#include <iostream>
#include "Vector.h"

#define HASH_MAP_SIZE 100

unsigned int hash(const std::string &key, int size);

struct HashMapNode;
extern HashMapNode* TOMBSTONE;
struct WordEntry {
    int docID;
    int position;
    std::string fileName;
    float tf;    // Term Frequency
    float idf;   // Inverse Document Frequency
    float tfidf; // TF-IDF

    // Define the operator!=
    bool operator!=(const WordEntry &other) const {
        return docID != other.docID ||
               position != other.position ||
               fileName != other.fileName ||
               tf != other.tf ||
               idf != other.idf ||
               tfidf != other.tfidf;
    }
};

// Define operator<< for output
inline std::ostream& operator<<(std::ostream& os, const WordEntry& entry) {
    os << "WordEntry{"
       << " docID: " << entry.docID
       << ", position: " << entry.position
       << ", fileName: \"" << entry.fileName << "\""
       << ", tf: " << entry.tf
       << ", idf: " << entry.idf
       << ", tfidf: " << entry.tfidf
       << " }";
    return os;
}

struct SecondLevelHashMap {
    Vector<WordEntry> entries;   // Secondary table with no collisions
    unsigned int size;           // Size of secondary table
    bool isInitialized;          
};

struct HashMapNode {
    std::string key;
    Vector<WordEntry> entries; // Using Tamara's custom Vector class
    HashMapNode *next;
    SecondLevelHashMap secondLevel;
};

struct HashMap {
    HashMapNode **table;  // Use dynamic allocation for resizing
    size_t size;          
    size_t capacity;      

public:
    Vector<std::string> keys() {  // Change to use custom Vector
        Vector<std::string> keyList;
        for (size_t i = 0; i < capacity; ++i) { 
            if (table[i] != nullptr && table[i] != TOMBSTONE) { // Check for nullptr and tombstone
                keyList.push_back(table[i]->key); // Ensure HashMapNode has a public member `key`
            }
        }
        return keyList;
    }
};

bool compareByTFIDF(const WordEntry &a, const WordEntry &b);
template<typename T>
void sortVector(Vector<T> &vec, bool (*compare)(const T&, const T&));
void initHashMap(HashMap *map);
bool insert(HashMap *map, const std::string &key, const WordEntry &entry);
WordEntry* getEntryFromHashMap(HashMap *map, const std::string &key, int docID);
void searchWord(HashMap *map, const std::string &query);
bool deleteEntry(HashMap *map, const std::string &key, int docID);
void freeHashMap(HashMap *map);
void resizeHashMap(HashMap *map);
void keys(HashMap *map, Vector<std::string> &result);

#endif // HASH_MAP_H
