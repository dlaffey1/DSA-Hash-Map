#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "trie.h"

void initTrie(Trie *trie) {
    trie->root = malloc(sizeof(TrieNode));
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        trie->root->children[i] = NULL;
    }
    trie->root->isEndOfWord = false;
}

int charToIndex(char ch) {
    return ch - 'a'; // Assuming only lowercase letters
}

void insertTrie(Trie *trie, const char *word) {
    TrieNode *node = trie->root;
    for (int level = 0; word[level]; level++) {
        int index = charToIndex(word[level]);
        if (!node->children[index]) {
            node->children[index] = malloc(sizeof(TrieNode));
            for (int i = 0; i < ALPHABET_SIZE; i++) {
                node->children[index]->children[i] = NULL;
            }
            node->children[index]->isEndOfWord = false;
        }
        node = node->children[index];
    }
    node->isEndOfWord = true;
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
