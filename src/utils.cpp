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
            std::cout << "Attempting to open file: " << entry.path() << std::endl;  // Debug
            std::ifstream file(entry.path());
            if (file.is_open()) {
                std::cout << "Successfully opened file: " << entry.path() << std::endl;  // Debug
                std::string word;
                int position = 0;
                std::string fileName = entry.path().filename().string(); // Get the filename

                // Reset the word frequency for the new document
                std::map<std::string, int> currentDocTermFrequency;

                while (file >> word) {
                    // Track term frequency for this document
                    currentDocTermFrequency[word]++;

                    // Create a new WordEntry
                    WordEntry wordEntry = { docID, position, fileName, 0.0f, 0.0f, 0.0f }; // Initialize TF, IDF, and TF-IDF to 0

                    // Insert into the hash map
                    if (!insert(index, word.c_str(), wordEntry)) {
                        std::cerr << "Error inserting into hash map for word: " << word << std::endl;
                    }
                    insertTrie(autocompleteTrie, word.c_str());
                    position++;
                }

                // Update term frequency and document frequency after processing the document
                for (const auto &entry : currentDocTermFrequency) {
                    const std::string &word = entry.first;
                    termFrequency[word][docID] = entry.second;  // Update TF
                    docFrequency[word]++;  // Increment document frequency for this word
                }

                file.close();
                std::cout << "Completed indexing file: " << fileName << std::endl;
                totalDocs++;
            } else {
                std::cerr << "Error opening file: " << entry.path() << std::endl;
            }
            docID++;
        }
    }

    // Calculate TF-IDF and update the entries in the hash map
    calculateTFIDF(index, termFrequency, docFrequency, totalDocs);
}

void calculateTFIDF(HashMap *index, 
                     const std::map<std::string, std::map<int, int>> &termFrequency, 
                     const std::map<std::string, int> &docFrequency, 
                     int totalDocs) {
    for (const auto &termEntry : termFrequency) {
        const std::string &word = termEntry.first;

        // For each document containing the word, calculate TF-IDF
        for (const auto &docEntry : termEntry.second) {
            int docID = docEntry.first;
            int tf = docEntry.second;

            // Calculate TF and IDF
            float tfValue = static_cast<float>(tf);
            int df = docFrequency.at(word); // Document frequency for this word
            float idfValue = log(static_cast<float>(totalDocs) / (df > 0 ? df : 1)); // Prevent division by zero

            // Calculate TF-IDF
            float tfidfValue = tfValue * idfValue;
            // Update the TF, IDF, and TF-IDF in the corresponding WordEntry in the hash map
            WordEntry *existingEntry = getEntryFromHashMap(index, word.c_str(), docID); // Your method to get the entry
            if (existingEntry) {
                existingEntry->tf = tfValue;    // Update TF
                existingEntry->idf = idfValue;  // Update IDF
                existingEntry->tfidf = tfidfValue; // Update TF-IDF
                        // Print for debugging
        std::cout << "Word: " << word << ", DocID: " << docID << ", TF: " << tf 
                  << ", DF: " << df << ", IDF: " << idfValue << ", TF-IDF: " << tfidfValue << std::endl;
            }
        }
    }
}

