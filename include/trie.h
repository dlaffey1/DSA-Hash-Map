#ifndef TRIE_H
#define TRIE_H
// Function to convert character to index
int charToIndex(char c);

#include <stdbool.h>
#include <string>
#include "Vector.h"
#define ALPHABET_SIZE 26

typedef struct TrieNode {
    struct TrieNode *children[ALPHABET_SIZE];
    bool isEndOfWord;
} TrieNode;

typedef struct {
    TrieNode *root;
} Trie;

void initTrie(Trie *trie);
void insertTrie(Trie *trie, const char *word);
void autocomplete(Trie *trie, const char *prefix);
void freeTrie(Trie *trie);
void findWords(TrieNode* node, std::string &currentWord, Vector<std::string>& results);


#endif // TRIE_H
