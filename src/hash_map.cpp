#include <iostream>
#include <cstring>
#include "hash_map.h"

// Hash function to map a string to an index
unsigned int hash(const char *key) {
    unsigned int hash = 0;
    while (*key) {
        hash = (hash << 5) + *key++; // hash * 31 + c
    }
    return hash % HASH_MAP_SIZE;
}

void initHashMap(HashMap *map) {
    memset(map->table, 0, sizeof(map->table));
}

void insert(HashMap *map, const char *word, WordEntry entry) {
    unsigned int index = hash(word);
    HashMapNode *node = map->table[index];

    // Debug: Print the index where we're inserting
    std::cout << "Inserting at index: " << index << " for word: " << word << std::endl;

    // Check if the key already exists
    while (node != NULL) {
        if (node->key == word) {
            // Key exists, add the entry
            std::cout << "Key already exists, current count: " << node->entries.size() << std::endl;

            // Debug: Print existing entries before reallocating
            std::cout << "Existing entries before reallocating:" << std::endl;
            for (const auto& existingEntry : node->entries) {
                std::cout << "Entry: Document ID: " << existingEntry.docID 
                          << ", Position: " << existingEntry.position 
                          << ", File Name: " << existingEntry.fileName << std::endl;
            }

            node->entries.push_back(entry); // Use vector's push_back
            std::cout << "Inserted entry for existing key, new count: " << node->entries.size() << std::endl;
            return;
        }
        node = node->next;
    }

    // Key does not exist, create a new node
    node = new HashMapNode; // Use new for dynamic allocation
    node->key = word; // Direct assignment since key is std::string
    node->entries.push_back(entry);  // Assign the first entry
    node->next = map->table[index];
    map->table[index] = node;

    // Debug: Print out the initial state of the new node
    std::cout << "Inserted new node with word: " << node->key << ", count: " << node->entries.size() << std::endl;
}

void searchWord(HashMap *map, const char *word) {
    unsigned int index = hash(word);
    HashMapNode *node = map->table[index];

    while (node != NULL) {
        if (node->key == word) {
            std::cout << "Found in documents:\n";
            for (const auto& entry : node->entries) {
                std::cout << "Document ID: " << entry.docID 
                          << ", Position: " << entry.position 
                          << ", File Name: " << entry.fileName << std::endl;
            }
            return;
        }
        node = node->next;
    }
    std::cout << "Word not found!" << std::endl;
}

void freeHashMap(HashMap *map) {
    for (int i = 0; i < HASH_MAP_SIZE; i++) {
        HashMapNode *node = map->table[i];
        while (node != NULL) {
            HashMapNode *temp = node;
            node = node->next;
            delete temp; // Use delete to free nodes
        }
    }
}
