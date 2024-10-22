#ifndef PARSER_H
#define PARSER_H

#include "hash_map.h"
#include "trie.h"

// Define the maximum length for the command/query input
#define MAX_QUERY_LENGTH 100

void parseCommand(HashMap* index, Trie* autocompleteTrie);

#endif
