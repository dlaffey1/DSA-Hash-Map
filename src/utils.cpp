#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem> // For directory handling
#include "hash_map.h"
#include "trie.h"

namespace fs = std::filesystem;

void indexBooks(HashMap *index, Trie *autocompleteTrie, const std::string &directoryPath) {
    int docID = 1;  // Start with document ID 1

    // Iterate through the files in the directory using C++17's filesystem
    for (const auto &entry : fs::directory_iterator(directoryPath)) {
        if (entry.is_regular_file()) {
            std::ifstream file(entry.path());
            if (file.is_open()) {
                std::string word;
                int position = 0;

                std::string fileName = entry.path().filename().string(); // Get the file name for debug
                std::cout << "Indexing file: " << fileName << " (Document ID: " << docID << ")" << std::endl;  // Debug info

                // Read each word from the file
                while (file >> word) {
                    // Create a new WordEntry for the current word, now with the file name
                    WordEntry wordEntry = { docID, position, fileName }; // Directly use std::string
                    insert(index, word.c_str(), wordEntry); // Pass C-string version of word
                    insertTrie(autocompleteTrie, word.c_str()); // Pass C-string version of word
                    position++;
                }
                file.close();
            } else {
                std::cerr << "Error opening file: " << entry.path() << std::endl;
            }
            docID++;  // Increment docID for each file
        }
    }
}
