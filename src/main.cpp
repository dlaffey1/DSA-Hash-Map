#include <stdio.h>
#include <iostream>
#include <fstream>
#include "hash_map.h"
#include "trie.h"
#include "utils.h"
#include "Serializer.h"
#include <string>
#include "parser.h"
#define MAX_QUERY_LENGTH 100

int main() {
    HashMap index;
    Trie autocompleteTrie;
    Serializer serializer;

    // Initialize the hash map and trie
    initHashMap(&index);
    initTrie(&autocompleteTrie);

    std::string hashFilePath = "index/hashmap.bin";
    std::string trieFilePath = "index/trie.bin";

    std::ifstream hashmapFile(hashFilePath, std::ios::binary);
    std::ifstream trieFile(trieFilePath, std::ios::binary);

    std::cout << "hashmapFile.good() " << hashmapFile.good() << std::endl;
    std::cout << "trieFile.good() " << trieFile.good() << std::endl;

    if (hashmapFile.good() && trieFile.good()) {
        std::cout << "Loading existing hashmap and trie from file..." << std::endl;
        serializer.deserializeHashMap(&index, hashFilePath);
        serializer.deserializeTrie(&autocompleteTrie, trieFilePath);
    } else {
        // Index books from the data directory
        std::cout << "Index file not found. Indexing textbooks..." << std::endl;
        indexBooks(&index, &autocompleteTrie, "data", serializer);
    }

    // Use the parser to handle user commands
    parseCommand(&index, &autocompleteTrie);

    // Free allocated memory
    freeHashMap(&index);
    freeTrie(&autocompleteTrie);
    
    return 0;
}
