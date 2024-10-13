#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem> // For directory handling
#include "hash_map.h"
#include "trie.h"

namespace fs = std::filesystem;
void indexBooks(HashMap *index, Trie *autocompleteTrie, const std::string &directory) {
    int docID = 1;  // Start with document ID 1

    for (const auto &entry : fs::directory_iterator(directory)) {
        if (entry.is_regular_file()) {
            std::ifstream file(entry.path());
            if (file.is_open()) {
                std::string word;
                int position = 0;
                std::string fileName = entry.path().filename().string(); // Get the filename

                std::cout << "Indexing file: " << fileName << " (Document ID: " << docID << ")" << std::endl;  // Debug info

                while (file >> word) {
                    // Create a new WordEntry
                    WordEntry wordEntry = { docID, position, fileName };

                    // Debug: Print the WordEntry details
                    std::cout << "Inserting word: " << word 
                              << ", Document ID: " << wordEntry.docID 
                              << ", Position: " << wordEntry.position 
                              << ", File Name: " << wordEntry.fileName << std::endl;

                    insert(index, word.c_str(), wordEntry);
                    insertTrie(autocompleteTrie, word.c_str());
                    position++;
                }
                file.close();
            } else {
                std::cerr << "Error opening file: " << entry.path() << std::endl;
            }
            docID++;
        }
    }
}

