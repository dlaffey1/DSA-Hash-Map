#include <stdlib.h>
#include <string.h>
#include "trie.h"
#include <stdio.h>
#include "Vector.h"

// Function to convert character to index
int charToIndex(char c) {
    if (c < 'a' || c > 'z') { // Add checks for valid lowercase letters
        return -1; // Return -1 for invalid characters
    }
    return c - 'a';
}

// Initialize the Trie
void initTrie(Trie *trie) {
    trie->root = new TrieNode(); // Ensure memory is allocated for the root node
    memset(trie->root->children, 0, sizeof(trie->root->children)); // Initialize children to nullptr
    trie->root->isEndOfWord = false; // Initialize end of word flag
}

// Insert a word into the trie
void insertTrie(Trie *trie, const char *word) {
    TrieNode *node = trie->root;
    for (int i = 0; word[i] != '\0'; ++i) {
        int index = charToIndex(word[i]); // Using the charToIndex function

        if (index == -1) { // Skip invalid characters
            continue; // Alternatively, handle the error as needed
        }

        if (!node->children[index]) {
            node->children[index] = new TrieNode();
            // Initialize the new node's children to nullptr
            memset(node->children[index]->children, 0, sizeof(node->children[index]->children));
            node->children[index]->isEndOfWord = false; // Ensure the new node is marked correctly
        }

        node = node->children[index];
    }
    node->isEndOfWord = true; // Mark the end of the word
}

// Function to search for suggestions
void searchSuggestions(TrieNode *node, char *prefix, int level) {
    if (node->isEndOfWord) {
        prefix[level] = '\0'; // Null terminate the string
        printf("%s\n", prefix); // Print the valid suggestion
    }

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i]) {
            prefix[level] = i + 'a'; // Add the character to the prefix
            searchSuggestions(node->children[i], prefix, level + 1);
        }
    }
}

// Autocomplete function
void autocomplete(Trie *trie, const char *prefix) {
    TrieNode *node = trie->root;

    // Traverse the trie to the end of the prefix
    for (int i = 0; prefix[i] != '\0'; ++i) {
<<<<<<< HEAD
        int index = charToIndex(prefix[i]); // Using the charToIndex function

        if (index == -1) { // Check for invalid character
            std::cout << "Invalid character in prefix: " << prefix << std::endl;
            return; // Exit if invalid
=======
        int index = charToIndex(prefix[i]); // Use charToIndex function

        if (index == -1) { // Check for invalid character
            std::cout << "Invalid character in prefix: " << prefix << std::endl;
            return; // Exit if invalid character is found
>>>>>>> 0c69badd2aa34f521ff4a8eb08c57394f1cc9030
        }

        if (!node->children[index]) {
            std::cout << "No words found with prefix: " << prefix << std::endl;
<<<<<<< HEAD
            return; // Exit if prefix not found
        }

        node = node->children[index];
    }

    // Find and display results
    Vector<std::string> results;
    std::string currentWord = prefix; // Start with the prefix
    findWords(node, currentWord, results);

    // Display results
    for (const std::string &word : results) {
        std::cout << word << std::endl;
=======
            return; // Exit if the prefix is not found in the trie
        }

        node = node->children[index]; // Move to the next node
    }

    // Collect all words starting with the given prefix
    Vector<std::string> results;
    std::string currentWord = prefix; // Initialize with the prefix itself
    findWords(node, currentWord, results); // Use helper function to find words

    // Display results, limit to 10 if necessary
    if (results.getSize() == 0) {
        std::cout << "No words found with prefix: " << prefix << std::endl;
    } else {
        std::cout << "Words found with prefix '" << prefix << "':" << std::endl;
        for (int i = 0; i < results.getSize() && i < 10; ++i) { // Limit to 10 words
            std::cout << results[i] << std::endl;
        }
>>>>>>> 0c69badd2aa34f521ff4a8eb08c57394f1cc9030
    }
}

// Function to find all words from a node
void findWords(TrieNode *node, std::string &currentWord, Vector<std::string> &results) {
    if (node->isEndOfWord) {
        results.push_back(currentWord); // Add found word to results
    }

    for (int i = 0; i < ALPHABET_SIZE; ++i) {
        if (node->children[i]) {
            currentWord.push_back('a' + i); // Add the character to current word
            findWords(node->children[i], currentWord, results);
            currentWord.pop_back(); // Remove the last character for backtracking
        }
    }
}

// Function to free a TrieNode
void freeTrieNode(TrieNode *node) {
    if (node) {
        for (int i = 0; i < ALPHABET_SIZE; ++i) {
            freeTrieNode(node->children[i]); // Recursively free child nodes
        }
        delete node; // Free the current node
    }
}

// Free the entire trie
void freeTrie(Trie *trie) {
    freeTrieNode(trie->root); // Free the root node
    trie->root = nullptr; // Set root to nullptr after freeing
}
