#include <stdlib.h>
#include <string.h>
#include "trie.h"
#include <stdio.h>
#include "Vector.h"
int charToIndex(char c) {
    return c - 'a';  // Adjust this logic based on your character set (e.g., lowercase a-z)
}


void initTrie(Trie *trie) {
    trie->root = new TrieNode();
    memset(trie->root->children, 0, sizeof(trie->root->children)); // Initialize children to nullptr
    trie->root->isEndOfWord = false;
}

// Insert a word into the trie
void insertTrie(Trie *trie, const char *word) {
    TrieNode *node = trie->root;
    for (int i = 0; word[i] != '\0'; ++i) {
        int index = word[i] - 'a'; // Assuming lowercase English letters

        if (!node->children[index]) {
            node->children[index] = new TrieNode();
            memset(node->children[index]->children, 0, sizeof(node->children[index]->children));
            node->children[index]->isEndOfWord = false;
        }

        node = node->children[index];
    }
    node->isEndOfWord = true; // Mark the end of the word
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

    // Traverse the trie to the end of the prefix
    for (int i = 0; prefix[i] != '\0'; ++i) {
        int index = prefix[i] - 'a';

        if (!node->children[index]) {
            std::cout << "No words found with prefix: " << prefix << std::endl;
            return;
        }

        node = node->children[index];
    }

    // Recursively find all words starting from this node
    Vector<std::string> results;
    std::string currentWord = prefix;
    findWords(node, currentWord, results);

    // Display results
    for (const std::string &word : results) {
        std::cout << word << std::endl;
    }
}
void findWords(TrieNode *node, std::string &currentWord, Vector<std::string> &results) {
    if (node->isEndOfWord) {
        results.push_back(currentWord);
    }

    for (int i = 0; i < ALPHABET_SIZE; ++i) {
        if (node->children[i]) {
            currentWord.push_back('a' + i);
            findWords(node->children[i], currentWord, results);
            currentWord.pop_back();
        }
    }
}

void freeTrieNode(TrieNode *node) {
    if (node) {
        for (int i = 0; i < ALPHABET_SIZE; ++i) {
            freeTrieNode(node->children[i]);
        }
        delete node;
    }
}

void freeTrie(Trie *trie) {
    freeTrieNode(trie->root);
    trie->root = nullptr;
}