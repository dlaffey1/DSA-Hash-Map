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

// Function to clean the word by removing leading and trailing punctuation
std::string cleanWord(const std::string &word) {
    std::string cleanedWord = word;

    // Remove leading punctuation
    cleanedWord.erase(cleanedWord.begin(), std::find_if(cleanedWord.begin(), cleanedWord.end(),
        [](unsigned char ch) {
            return !std::ispunct(ch);
        }
    ));

    // Remove trailing punctuation
    cleanedWord.erase(std::find_if(cleanedWord.rbegin(), cleanedWord.rend(),
        [](unsigned char ch) {
            return !std::ispunct(ch);
        }
    ).base(), cleanedWord.end());

    return cleanedWord;
}

// Indexing function for books
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
                    // Clean the word to remove punctuation
                    std::string cleanedWord = cleanWord(word);
                    if (cleanedWord.empty()) continue; // Skip if the cleaned word is empty

                    // Track term frequency for this document
                    currentDocTermFrequency[cleanedWord]++;

                    // Create a new WordEntry
                    WordEntry wordEntry = { docID, position, fileName, 0.0f, 0.0f, 0.0f }; // Initialize TF, IDF, and TF-IDF to 0

                    // Insert into the hash map using the standalone function
                    if (!insert(index, cleanedWord.c_str(), wordEntry)) {  // Changed to standalone function
                        std::cerr << "Error inserting into hash map for word: " << cleanedWord << std::endl;
                    }

                    // Insert into the trie
                    insertTrie(autocompleteTrie, cleanedWord.c_str());
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

// Calculate TF-IDF function
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

            // Calculate TF and IDF
            float tfValue = static_cast<float>(tf);
            int df = docFrequency.at(word); // Document frequency for this word
            float idfValue = log(static_cast<float>(totalDocs) / (df > 0 ? df : 1)); // Prevent division by zero

            // Calculate TF-IDF
            float tfidfValue = tfValue * idfValue;

            // Retrieve the specific WordEntry for the word and document ID
            WordEntry *existingEntry = getEntryFromHashMap(index, word, docID);
            if (existingEntry) {
                // Update only the specific entry for this document
                existingEntry->tf = tfValue;    // Update TF
                existingEntry->idf = idfValue;  // Update IDF
                existingEntry->tfidf = tfidfValue; // Update TF-IDF

                // Debugging information
                std::cout << "Updated WordEntry for word: " << word << ", DocID: " << docID
                          << ", TF: " << tfValue << ", IDF: " << idfValue 
                          << ", TF-IDF: " << tfidfValue << std::endl;
            } else {
                // Handle the case where the entry does not exist
                // std::cerr << "Error: WordEntry for word: " << word << ", DocID: " << docID 
                //           << " not found in HashMap." << std::endl;
            }
        }
    }

    // Validate all keys exist
    for (const auto& key : index->keys()) {
        // Here, we do not need docID since we want to ensure all keys are present
        WordEntry* entry = getEntryFromHashMap(index, key, 1); // Use a valid docID or handle appropriately
        if (entry) {
            // std::cout << "Confirmed WordEntry for word: " << key << " exists." << std::endl;
        } else {
            // std::cerr << "Warning: WordEntry for key: " << key << " is missing in HashMap." << std::endl;
        }
    }
}
