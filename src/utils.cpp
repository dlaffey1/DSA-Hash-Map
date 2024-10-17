#include "utils.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <map>
#include "Vector.h"
#include <string>
#include "hash_map.h"
#include "trie.h"
#include <cmath>

namespace fs = std::filesystem;

void indexBooks(HashMap *index, Trie *autocompleteTrie, const std::string &directory) {
    int docID = 1;  // Start with document ID 1
    std::map<std::string, std::map<int, int>> termFrequency;  // word -> (docID -> count)
    std::map<std::string, int> docFrequency;  // word -> count of documents containing it
    int totalDocs = 0;

    for (const auto &entry : fs::directory_iterator(directory)) {
        if (entry.is_regular_file()) {
            std::cout << "Attempting to open file: " << entry.path() << std::endl;  // Debug: File opening attempt
            std::ifstream file(entry.path());
            if (file.is_open()) {
                std::cout << "Successfully opened file: " << entry.path() << std::endl;  // Debug: File opened successfully
                std::string word;
                int position = 0;
                std::string fileName = entry.path().filename().string(); // Get the filename

                std::cout << "Indexing file: " << fileName << " (Document ID: " << docID << ")" << std::endl;

                // Reset the word frequency for the new document
                std::map<std::string, int> currentDocTermFrequency;

                while (file >> word) {
                    // Track term frequency for this document
                    currentDocTermFrequency[word]++;
                    
                    // Create a new WordEntry
                    WordEntry wordEntry = { docID, position, fileName };
                    std::cout << "Creating WordEntry for word: " << word 
                              << " with Document ID: " << docID 
                              << ", Position: " << position 
                              << ", File Name: " << fileName << std::endl;  // Debug: WordEntry creation

                    // Insert into the hash map
                    if (!insert(index, word.c_str(), wordEntry)) {
                        std::cerr << "Error inserting into hash map for word: " << word << std::endl;
                    } else {
                        std::cout << "Inserted into hash map: " << word << std::endl;  // Debug: Insertion success
                    }
                    insertTrie(autocompleteTrie, word.c_str());
                    std::cout << "Inserted into Trie: " << word << std::endl;  // Debug: Trie insertion

                    position++;
                }

                // Update term frequency and document frequency
                for (const auto &entry : currentDocTermFrequency) {
                    termFrequency[entry.first][docID] = entry.second;  // Update TF
                    docFrequency[entry.first]++;  // Increment document frequency for this word
                }

                file.close();
                std::cout << "Completed indexing file: " << fileName << std::endl;  // Debug: File indexing complete
                totalDocs++;
            } else {
                std::cerr << "Error opening file: " << entry.path() << std::endl;
            }
            docID++;
        }
    }

    // Now you can compute TF-IDF and store or use it as needed.
    // Example: Calculate TF-IDF and print it
    for (const auto &entry : termFrequency) {
        const std::string &word = entry.first;
        for (const auto &docEntry : entry.second) {
            int docID = docEntry.first;
            int tf = docEntry.second;
            int df = docFrequency[word];

            // Calculate TF-IDF: TF * log(totalDocs / df)
            double tfidf = tf * log((double)totalDocs / (df > 0 ? df : 1));  // Prevent division by zero

            // Store or print the TF-IDF value as needed
            std::cout << "TF-IDF for word: " << word << " in Document ID: " << docID << " is: " << tfidf << std::endl;
        }
    }
}
