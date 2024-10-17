#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ALPHABET_SIZE 26

// Structure for a Trie node
struct TrieNode {
    char value;  // Character stored at this node
    struct TrieNode *children[ALPHABET_SIZE];  // Array of pointers to child nodes
    int isEndOfWord;  // 1 if the node represents the end of a word
    char *document;  // File path or position of the document
};

// Function to create a new Trie node
struct TrieNode *createNode(char value) {
    struct TrieNode *node = (struct TrieNode *)malloc(sizeof(struct TrieNode));
    if (node) {
        node->value = value;
        node->isEndOfWord = 0;
        node->document = NULL;
        for (int i = 0; i < ALPHABET_SIZE; i++)
            node->children[i] = NULL;
    }
    return node;
}

// Function to convert a character to lowercase and map it to the index
int charToIndex(char c) {
    return tolower(c) - 'a';
}

// Insert a word into the Trie and associate a document (file path or position)
void insert(struct TrieNode *root, const char *word, const char *document) {
    struct TrieNode *pCrawl = root;

    for (int i = 0; word[i] != '\0'; i++) {
        int index = charToIndex(word[i]);

        // If the node does not exist, create it
        if (!pCrawl->children[index])
            pCrawl->children[index] = createNode(word[i]);

        pCrawl = pCrawl->children[index];
    }

    // Mark the end of the word and associate the document with this node
    pCrawl->isEndOfWord = 1;
    pCrawl->document = strdup(document);  // Store the file path or position
}

// Function to tokenize a document (basic word splitting by spaces and punctuation) and insert into the Trie
void tokenizeAndInsert(struct TrieNode *root, const char *document, const char *documentPath) {
    const char *delimiters = " ,.!?;:\n";  // Define delimiters to split words
    char *token = strtok(strdup(document), delimiters);

    while (token != NULL) {
        insert(root, token, documentPath);  // Insert each token (word) with its document
        token = strtok(NULL, delimiters);
    }
}

// Function to read the contents of a file and tokenize them
void readFileAndInsert(struct TrieNode *root, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Could not open file %s\n", filename);
        return;
    }

    char buffer[1024];  // Buffer for reading the file line by line
    while (fgets(buffer, sizeof(buffer), file)) {
        tokenizeAndInsert(root, buffer, filename);  // Insert words from the file into the trie
    }

    fclose(file);
}

int main() {
    // Create the root of the Trie
    struct TrieNode *root = createNode('\0');  // Root node doesn't hold a character
    
    readFileAndInsert(root, "C:/UL/SHARED/Year3-sem1/DSA-Eoin/Project/DSA-Hash-Map/Books/example1.txt");
    readFileAndInsert(root, "C:/UL/SHARED/Year3-sem1/DSA-Eoin/Project/DSA-Hash-Map/Books/example2.txt");

    printf("Words from the files have been inserted into the trie with associated documents.\n");

    return 0;
}
