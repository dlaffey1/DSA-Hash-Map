#include <iostream>
#include <cstring>
#include "hash_map.h"
#include "Vector.h"  // Include your custom Vector class
#include <algorithm>
#include <set>
#include <map>

// Hash function to map a string to an index
unsigned int hash(const std::string &key) {
    unsigned int hash = 0;
    for (char c : key) {
        hash = (hash << 5) - hash + c; // Equivalent to hash * 31 + c
        // This uses the technique of shifting left by 5 (multiplying by 32), 
        // subtracting the original hash (to make it effectively multiply by 31),
        // and then adding the character's ASCII value.
    }
    return hash % HASH_MAP_SIZE; // Ensure the result is within bounds of the hash map size
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
            // Key exists; update the entry for the specific docID
            for (size_t i = 0; i < node->entries.getSize(); ++i) {
                if (node->entries[i].docID == entry.docID) {
                    // Update the existing entry's position and fileName, but do not modify TF here
                    node->entries[i].position = entry.position; // Update position if needed
                    node->entries[i].fileName = entry.fileName; // Update filename if needed
                    return true; // Successfully updated
                }
            }
            // If no entry for this docID exists, add a new one
            node->entries.push_back(entry); // Add the new WordEntry
            return true;
        }
        node = node->next;
    }

    // Key does not exist; create a new node
    HashMapNode *newNode = new HashMapNode;
    if (!newNode) {
        std::cerr << "Failed to allocate memory for HashMapNode." << std::endl;
        return false;
    }

    newNode->key = key;
    newNode->entries.push_back(entry); // Add the WordEntry
    newNode->next = map->table[index];
    map->table[index] = newNode;

    return true;
}



// Function to retrieve a WordEntry from the hash map based on key and docID
WordEntry* getEntryFromHashMap(HashMap *map, const std::string &key, int docID) {
    size_t hashIndex = hash(key); // Calculate the hash index

    // Ensure the hashIndex is within bounds of the hash map size
    if (hashIndex >= HASH_MAP_SIZE) { // Changed to use HASH_MAP_SIZE
        std::cerr << "Hash index out of bounds: " << hashIndex << std::endl;
        return nullptr;
    }

    // Access the specific HashMapNode at the calculated hash index
    HashMapNode *current = map->table[hashIndex]; // Correct access

    while (current) {
        // Search through the entries for the specific docID
        for (size_t i = 0; i < current->entries.getSize(); ++i) {
            if (current->entries[i].docID == docID) {
                return &current->entries[i]; // Return pointer to the matching WordEntry
            }
        }
        current = current->next; // Move to the next node in the linked list
    }

    return nullptr; // Return nullptr if no matching entry is found
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

void searchWord(HashMap *map, const char *word) {
    unsigned int index = hash(word);
    HashMapNode *node = map->table[index];

    Vector<WordEntry> results; // Store results here

    // Traverse the linked list to find the word
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

    // Display the top 10 results, ensuring no duplicate document IDs are processed
    std::cout << "Found in documents (top 10 based on TF-IDF):\n";
    std::set<int> displayedDocIDs; // To track displayed document IDs
    for (size_t i = 0; i < std::min(results.getSize(), 10); ++i) {
        const WordEntry &entry = results[i];
        if (displayedDocIDs.find(entry.docID) == displayedDocIDs.end()) { // If docID hasn't been displayed yet
            std::cout << "Document ID: " << entry.docID
                      << ", Position: " << entry.position
                      << ", File Name: " << entry.fileName
                      << ", TF-IDF: " << entry.tfidf 
                      << ", TF: " << entry.tf << std::endl;
            displayedDocIDs.insert(entry.docID); // Mark this docID as displayed
        }
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
