#include <stdlib.h>
#include <string.h>
#include "trie.h"
#include <stdio.h>
int charToIndex(char c) {
    return c - 'a';  // Adjust this logic based on your character set (e.g., lowercase a-z)
}


void initTrie(Trie *trie) {
    trie->root = (TrieNode*) malloc(sizeof(TrieNode)); // Cast to TrieNode*
    memset(trie->root, 0, sizeof(TrieNode));
}

void insertTrie(Trie *trie, const char *word) {
    TrieNode *node = trie->root;
    for (const char *p = word; *p; p++) {
        int index = *p - 'a'; // Assuming lowercase letters
        if (!node->children[index]) {
            node->children[index] = (TrieNode*) malloc(sizeof(TrieNode)); // Cast to TrieNode*
            memset(node->children[index], 0, sizeof(TrieNode));
        }
        node = node->children[index];
    }
    node->isEndOfWord = 1;
}


void searchSuggestions(TrieNode *node, char *prefix, int level) {
    if (node->isEndOfWord) {
        prefix[level] = '\0'; // Null terminate the string
        printf("%s\n", prefix);
    }

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i]) {
            prefix[level] = i + 'a'; // Add the character to the prefix
            searchSuggestions(node->children[i], prefix, level + 1);
        }
    }
}

void autocomplete(Trie *trie, const char *prefix) {
    TrieNode *node = trie->root;
    char prefixBuffer[100];
    int level;

    for (level = 0; prefix[level]; level++) {
        int index = charToIndex(prefix[level]);
        if (!node->children[index]) {
            printf("No suggestions found.\n");
            return;
        }
        node = node->children[index];
    }

    searchSuggestions(node, prefixBuffer, level);
}

void freeTrieNode(TrieNode *node) {
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i]) {
            freeTrieNode(node->children[i]);
        }
    }
    free(node);
}

void freeTrie(Trie *trie) {
    freeTrieNode(trie->root);
}
