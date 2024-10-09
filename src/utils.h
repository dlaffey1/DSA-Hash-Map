#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include "hash_map.h"
#include "trie.h"

// Function to index books and add them to the hash map and trie
void indexBooks(HashMap *index, Trie *autocompleteTrie, const char *dataDir);

#endif // UTILS_H
