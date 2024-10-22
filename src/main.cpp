#include <stdio.h>
#include <string.h>
#include "hash_map.h"
#include "trie.h"
#include "utils.h"
#include "parser.h"
#define MAX_QUERY_LENGTH 100

int main() {
    HashMap index;
    Trie autocompleteTrie;

    // Initialize the hash map and trie
    initHashMap(&index);
    initTrie(&autocompleteTrie);

    // Index books from the data directory
    indexBooks(&index, &autocompleteTrie, "data");

    // Use the parser to handle user commands
    parseCommand(&index, &autocompleteTrie);

    // Free allocated memory
    freeHashMap(&index);
    freeTrie(&autocompleteTrie);
    
    return 0;
}
