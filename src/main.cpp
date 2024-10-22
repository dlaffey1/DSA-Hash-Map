#include <stdio.h>
#include "hash_map.h"
#include "trie.h"
#include "utils.h"
#define MAX_QUERY_LENGTH 100
int main() {
    HashMap index;
    Trie autocompleteTrie;
    initHashMap(&index);
    initTrie(&autocompleteTrie);

    // Index books from the data directory
    indexBooks(&index, &autocompleteTrie, "data");

    // Search for a word
    char query[MAX_QUERY_LENGTH];
        printf("Enter a word to search: ");
        fgets(query, sizeof(query), stdin); // Read the entire line for the query

        // Remove the newline character if present
        size_t len = strlen(query);
        if (len > 0 && query[len - 1] == '\n') {
            query[len - 1] = '\0'; // Replace newline with null terminator
        }

    searchWord(&index, query);

    printf("Enter prefix for autocomplete: ");
    query[0] = '\0'; // Set the first character to null terminator to flush the array

    fgets(query, sizeof(query), stdin);

    len = strlen(query);
    if (len > 0 && query[len - 1] == '\n') {
        query[len - 1] = '\0'; 
    }

    autocomplete(&autocompleteTrie, query); 

    freeHashMap(&index);
    freeTrie(&autocompleteTrie);
    
    return 0;
}
