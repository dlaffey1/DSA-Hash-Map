#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

    // Check if the key already exists
    while (node != NULL) {
        if (strcmp(node->key, word) == 0) {
            // Key exists, add the entry
            node->entries = realloc(node->entries, sizeof(WordEntry) * (node->count + 1));
            node->entries[node->count] = entry;
            node->count++;
            return;
        }
        node = node->next;
    }

    // Key does not exist, create a new node
    node = malloc(sizeof(HashMapNode));
    node->key = strdup(word);
    node->entries = malloc(sizeof(WordEntry));
    node->entries[0] = entry;
    node->count = 1;
    node->next = map->table[index];
    map->table[index] = node;
}

void searchWord(HashMap *map, const char *word) {
    unsigned int index = hash(word);
    HashMapNode *node = map->table[index];

    while (node != NULL) {
        if (strcmp(node->key, word) == 0) {
            printf("Found in documents:\n");
            for (int i = 0; i < node->count; i++) {
                printf("Document ID: %d, Position: %d\n", node->entries[i].docID, node->entries[i].position);
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
