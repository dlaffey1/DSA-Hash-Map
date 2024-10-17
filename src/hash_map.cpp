#include <iostream>
#include <cstring>
#include "hash_map.h"
#include "Vector.h"  // Include your custom Vector class
#include <algorithm>

// Hash function to map a string to an index
unsigned int hash(const std::string &key) {
    unsigned int hash = 0;
    for (char c : key) {
        hash = (hash << 5) + c; // hash * 31 + c
    }
    return hash % HASH_MAP_SIZE;
}

void initHashMap(HashMap *map) {
    memset(map->table, 0, sizeof(map->table));
}

bool insert(HashMap *map, const std::string &key, const WordEntry &entry) {
    unsigned int index = hash(key);
    HashMapNode *node = map->table[index];

    // Traverse the linked list at the index
    while (node != nullptr) {
        if (node->key == key) {
            // Key already exists; update the entry
            node->entries.push_back(entry); // Add the new entry
            return true; // Return true to indicate successful update
        }
        node = node->next;
    }

    // Key does not exist; create a new node
    HashMapNode *newNode = new HashMapNode;
    if (!newNode) {
        std::cerr << "Failed to allocate memory for HashMapNode." << std::endl;
        return false; // Handle memory allocation failure
    }

    newNode->key = key;
    newNode->entries.push_back(entry);
    newNode->next = map->table[index];
    map->table[index] = newNode;

    return true; // Indicate successful insertion
}

// Function to compare WordEntry based on TF-IDF values
bool compareByTFIDF(const WordEntry &a, const WordEntry &b) {
    return a.tfidf > b.tfidf; // Sort in descending order based on TF-IDF
}

// Function to sort the custom Vector based on a comparison function
template<typename T>
void sortVector(Vector<T> &vec, bool (*compare)(const T&, const T&)) {
    for (int i = 0; i < vec.getSize() - 1; ++i) {
        for (int j = 0; j < vec.getSize() - 1 - i; ++j) {
            if (compare(vec[j + 1], vec[j])) {
                // Swap elements
                T temp = vec[j];
                vec[j] = vec[j + 1];
                vec[j + 1] = temp;
            }
        }
    }
}

// Function to search for a word in the hash map and display top 10 documents based on TF-IDF
void searchWord(HashMap *map, const char *word) {
    unsigned int index = hash(word);
    HashMapNode *node = map->table[index];

    Vector<WordEntry> results; // Store results here

    while (node != NULL) {
        if (strcmp(node->key.c_str(), word) == 0) { // Convert std::string to const char*
            for (int i = 0; i < node->entries.getSize(); ++i) {
                results.push_back(node->entries[i]); // Collect entries
            }
            break; // Stop searching once the word is found
        }
        node = node->next;
    }

    if (results.empty()) {
        std::cout << "Word not found!" << std::endl;
        return;
    }

    // Sort results based on TF-IDF values
    sortVector(results, compareByTFIDF); // Custom sorting

    // Display the top 10 results
    std::cout << "Found in documents (top 10 based on TF-IDF):\n";
    for (size_t i = 0; i < std::min(results.getSize(), 10); ++i) {
        const WordEntry &entry = results[i];
        std::cout << "Document ID: " << entry.docID
                  << ", Position: " << entry.position
                  << ", File Name: " << entry.fileName
                  << ", TF-IDF: " << entry.tfidf << std::endl;
    }
}

void freeHashMap(HashMap *map) {
    for (int i = 0; i < HASH_MAP_SIZE; i++) {
        HashMapNode *node = map->table[i];
        while (node != NULL) {
            HashMapNode *temp = node;
            node = node->next;
            delete temp;  // Use delete to free nodes
        }
        map->table[i] = nullptr; // Prevent dangling pointers
    }
}
