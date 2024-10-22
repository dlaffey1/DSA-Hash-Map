#include "utils.h"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <map>
#include <cmath>
#include "Vector.h"
#include "hash_map.h"

namespace fs = std::filesystem;

std::string cleanWord(const std::string &word) {
    std::string cleanedWord;

    for (char ch : word) {
        if (std::isalnum(ch)) {  // Keep only alphanumeric characters
            cleanedWord += std::tolower(static_cast<unsigned char>(ch));  // Convert to lowercase
        }
    }

    return cleanedWord;
}

void updateTFIDF(HashMap *index, const std::string &word, int docID, float tf, float idf, float tfidf) {
    // Retrieve the specific WordEntry for the word and document ID
    WordEntry *existingEntry = getEntryFromHashMap(index, word, docID);

    if (existingEntry) {
        // Update the entry with TF, IDF, and TF-IDF values
        existingEntry->tf = tf;
        existingEntry->idf = idf;
        existingEntry->tfidf = tfidf;

        // std::cout << "Updated WordEntry for word: " << word << ", DocID: " << docID
        //           << ", TF: " << tf << ", IDF: " << idf 
        //           << ", Index in HashMap: " << index
        //           << ", TF-IDF: " << tfidf << std::endl;
    } else {
        // std::cerr << "Error: WordEntry for word: \"" << word << "\", DocID: " << docID 
        //           << " not found in HashMap." << std::endl;
    }
}

// Indexing function for books
void indexBooks(HashMap *index, Trie *autocompleteTrie, const std::string &directory) {
    int docID = 1;  // Start with document ID 1
    std::map<std::string, std::map<int, int>> termFrequency;  // word -> (docID -> count)
    std::map<std::string, int> docFrequency;  // word -> count of documents containing it
    int totalDocs = 0;

    for (const auto &entry : fs::directory_iterator(directory)) {
        if (entry.is_regular_file()) {
            // std::cout << "Attempting to open file: " << entry.path() << std::endl;
            std::ifstream file(entry.path());
            if (file.is_open()) {
                std::cout << "Successfully opened file: " << entry.path() << std::endl;
                std::string word;
                int position = 0;
                std::string fileName = entry.path().filename().string(); // Get the filename

                // Reset the word frequency for the new document
                std::map<std::string, int> currentDocTermFrequency;

                while (file >> word) {
   
                    std::string cleanedWord = cleanWord(word);
                    if (cleanedWord.empty()) continue;

                    // Track term frequency for this document
                    currentDocTermFrequency[cleanedWord]++;

                    // Create a new WordEntry
                    WordEntry wordEntry = { docID, position, fileName, 0.0f, 0.0f, 0.0f }; // Initialize TF, IDF, and TF-IDF to 0

                    // Insert into the hash map using the standalone function
                    if (!insert(index, cleanedWord.c_str(), wordEntry)) {  
                        std::cerr << "Error inserting into hash map for word: " << cleanedWord << std::endl;
                    }

                    insertTrie(autocompleteTrie, cleanedWord.c_str());
                    position++;
                }

                // Update term frequency and document frequency after processing the document
                for (const auto &entry : currentDocTermFrequency) {
                    const std::string &word = entry.first;
                    termFrequency[word][docID] = entry.second;
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
    // Iterate through all unique terms in the termFrequency map
    for (const auto &termEntry : termFrequency) {
        const std::string &word = termEntry.first;

        // For each document containing the word, calculate TF-IDF
        for (const auto &docEntry : termEntry.second) {
            int docID = docEntry.first;
            int tf = docEntry.second;

            float tfValue = static_cast<float>(tf);
            int df = docFrequency.at(word);
            float idfValue = log(static_cast<float>(totalDocs) / (df > 0 ? df : 1)); // Prevent division by zero

            float tfidfValue = tfValue * idfValue;

            updateTFIDF(index, word, docID, tfValue, idfValue, tfidfValue);
        }
    }
}

