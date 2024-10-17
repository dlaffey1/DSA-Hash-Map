#ifndef HASH_MAP_H
#define HASH_MAP_H

#include <string>
#include "Vector.h" 

#define HASH_MAP_SIZE 100


struct WordEntry {
    int docID;
    int position;
    std::string fileName;
    float tf;   // Term Frequency
    float idf;  // Inverse Document Frequency
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
