#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_map.h"
#include <iostream>

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
        // Debug: Print the word being inserted
    std::cout << "Inserting word: " << word << ", Document ID: " << entry.docID 
              << ", Position: " << entry.position << ", File Name: " << entry.fileName << std::endl;

    // Check if the key already exists
    while (node != NULL) {
        if (strcmp(node->key, word) == 0) {
            // Key exists, add the entry
            node->entries = (WordEntry*) realloc(node->entries, sizeof(WordEntry) * (node->count + 1));
            node->entries[node->count] = entry;  // No need for conversion here
            node->count++;
            return;
        }
        node = node->next;
    }

    // Key does not exist, create a new node
    node = (HashMapNode*) malloc(sizeof(HashMapNode));
    node->key = strdup(word);  // Duplicate the key string
    node->entries = (WordEntry*) malloc(sizeof(WordEntry));  // Allocate space for entries
    node->entries[0] = entry;  // Assign the first entry
    node->count = 1;
    node->next = map->table[index];
    map->table[index] = node;
}


// hash_map.cpp
void searchWord(HashMap *map, const char *word) {
    unsigned int index = hash(word);
    HashMapNode *node = map->table[index];

    while (node != NULL) {
        if (strcmp(node->key, word) == 0) {
            printf("Found in documents:\n");
            for (int i = 0; i < node->count; i++) {
                printf("Doxcvxcvcument ID: %d, Position: %d, File Name: %s\n",
                       node->entries[i].docID, 
                       node->entries[i].position, 
                       node->entries[i].fileName.c_str()); // Use c_str() here
            }
            return;
        }
        node = node->next;
    }
    printf("Word not found!\n");
}


void freeHashMap(HashMap *map) {
    for (int i = 0; i < HASH_MAP_SIZE; i++) {
        HashMapNode *node = map->table[i];
        while (node != NULL) {
            HashMapNode *temp = node;
            node = node->next;
            free(temp->key);
            free(temp->entries);
            free(temp);
        }
    }
}
