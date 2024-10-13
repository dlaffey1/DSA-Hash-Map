#include <stdio.h>
#include "hash_map.h"
#include "trie.h"
#include "utils.h"

int main() {
    HashMap index;
    Trie autocompleteTrie;
    initHashMap(&index);
    initTrie(&autocompleteTrie);

    // Index books from the data directory
    indexBooks(&index, &autocompleteTrie, "data");

    // Search for a word
    char query[100];
    printf("Enter a word to search: ");
    scanf("%s", query);
    searchWord(&index, query);

    // Autocomplete example
    printf("Enter prefix for autocomplete: ");
    scanf("%s", query);
    autocomplete(&autocompleteTrie, query);

    // Clean up
    freeHashMap(&index);
    freeTrie(&autocompleteTrie);
    
    return 0;
}
