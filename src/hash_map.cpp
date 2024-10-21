#include <iostream>
#include <cstring>
#include "hash_map.h"
#include "Vector.h"  // Include your custom Vector class
#include <algorithm>
#include <set>
#include <sstream>

// Define a constant for tombstones to handle deletions
HashMapNode* TOMBSTONE = reinterpret_cast<HashMapNode*>(-1);

// Hash function to map a string to an index
unsigned int hash(const std::string &key, int size) {
    unsigned int hash = 0;
    for (char c : key) {
        hash = (hash << 5) - hash + c; // Equivalent to hash * 31 + c
    }
    unsigned int index = hash % size; // Ensure the result is within bounds of the hash map size
    // std::cout << "Hash for key '" << key << "': " << index << std::endl; // Debugging line
    return index;
}

// Function to compare WordEntry based on TF-IDF values
bool compareByTFIDF(const WordEntry &a, const WordEntry &b) {
    return a.tfidf > b.tfidf; // Sort in descending order based on TF-IDF
}

// Function to sort the custom Vector based on a comparison function
template<typename T>
void sortVector(Vector<T> &vec, bool (*compare)(const T&, const T&)) {
    for (size_t i = 0; i < vec.getSize() - 1; ++i) {
        for (size_t j = 0; j < vec.getSize() - 1 - i; ++j) {
            if (compare(vec[j + 1], vec[j])) {
                // Swap elements
                T temp = vec[j];
                vec[j] = vec[j + 1];
                vec[j + 1] = temp;
            }
        }
    }
}

void resizeHashMap(HashMap *map) {
    std::cout << "Resizing HashMap from capacity " << map->capacity << " to " << map->capacity * 2 << std::endl;
    
    size_t oldCapacity = map->capacity; // Store the old capacity
    HashMapNode **oldTable = map->table; // Store the old table
    map->capacity *= 2; // Double the size
    map->table = new HashMapNode*[map->capacity](); // Allocate new table

    // Rehash all existing entries
    for (size_t i = 0; i < oldCapacity; ++i) {
        if (oldTable[i] != nullptr && oldTable[i] != TOMBSTONE) {
            // std::cout << "Rehashing entries from index " << i << " with key: " << oldTable[i]->key << std::endl;
            for (size_t j = 0; j < oldTable[i]->entries.getSize(); ++j) {
                const WordEntry &entry = oldTable[i]->entries[j];
                insert(map, oldTable[i]->key, entry); // Re-insert the entry into the new table
            }
            delete oldTable[i]; // Free the old node
        }
    }
    delete[] oldTable; // Free the old table
}

void initHashMap(HashMap *map) {
    map->capacity = HASH_MAP_SIZE; // Set initial capacity
    map->table = new HashMapNode*[map->capacity](); // Allocate the table
    map->size = 100; // Initialize size to 0
    std::cout << "Initial size: " << map->size << std::endl;
}

bool insert(HashMap *map, const std::string &key, const WordEntry &entry) {
    // Check if the map is effectively full before proceeding
    if (map->size >= map->capacity * 0.7) { 
        std::cerr << "HashMap is effectively full (size: " << map->size <<
         "), resizing..." << std::endl;
        resizeHashMap(map); // Resize the hash map
    }

    unsigned int index = hash(key, map->capacity);
    unsigned int startIndex = index; // Save the starting index for loop detection

    while (map->table[index] != nullptr) {
        if (map->table[index] == TOMBSTONE) {
            break; // Continue searching to insert the new key
        }

        if (map->table[index]->key == key) {
            // Key exists; update the entry for the specific docID
            for (size_t i = 0; i < map->table[index]->entries.getSize(); ++i) {
                if (map->table[index]->entries[i].docID == entry.docID) {
                    // Update the existing entry's position and fileName
                    map->table[index]->entries[i].position = entry.position;
                    map->table[index]->entries[i].fileName = entry.fileName;
                    return true; // Update successful
                }
            }
            // Add a new entry for the docID
            map->table[index]->entries.push_back(entry); // Add the new WordEntry
            return true; // Insertion successful
        }

        // Linear probing: check the next slot
        index = (index + 1) % map->capacity;

        // Check if we have looped back to the start
        if (index == startIndex) {
            std::cerr << "HashMap is full, cannot insert!" << std::endl;
            return false; // The table is full, failed to insert
        }
    }

    // Insert new node if key does not exist
    HashMapNode *newNode = new HashMapNode; // Allocate new node
    if (!newNode) {
        std::cerr << "Failed to allocate memory for HashMapNode." << std::endl;
        return false; // Memory allocation failed
    }

    newNode->key = key; // Set the key
    newNode->entries.push_back(entry); // Add the WordEntry
    map->table[index] = newNode; // Insert new node in the hash map
    map->size++; // Increment the size of the hash map
    //std::cout << "Inserting: " << key << " at index: " << index << std::endl;
        // std::cout << "Inserted new Word: " << key
        //       << ", DocID: " << entry.docID
        //       << " at index: " << index << std::endl;
    return true; // Insertion successful
}

// Function to retrieve a WordEntry from the hash map using linear probing
WordEntry* getEntryFromHashMap(HashMap *map, const std::string &key, int docID) {
    unsigned int index = hash(key, map->capacity); // Use current capacity
    unsigned int startIndex = index;  // Store initial index for loop detection

    while (map->table[index] != nullptr) {
        if (map->table[index] != TOMBSTONE && map->table[index]->key == key) {
            // Search through entries for the matching docID
            for (size_t i = 0; i < map->table[index]->entries.getSize(); ++i) {
                if (map->table[index]->entries[i].docID == docID) {
                    return &map->table[index]->entries[i]; // Return pointer to the entry
                }
            }
        }
        index = (index + 1) % map->capacity; // Linear probing, now based on current capacity
        if (index == startIndex) {
            break;  // We've looped back to the start, no match found
        }
    }

    return nullptr; // No matching entry found
}

void searchWord(HashMap *map, const std::string &query) {
    std::istringstream iss(query);
    std::string token;
    Vector<std::string> includeWords;
    Vector<std::string> excludeWords;

    // Parse the query for operators and words
    std::cout << "[DEBUG] Parsing query: " << query << std::endl;
    while (iss >> token) {
        if (token == "AND" || token == "OR") {
            std::cout << "[DEBUG] Found operator: " << token << std::endl;
            continue;
        } else if (token == "NOT") {
            iss >> token;
            excludeWords.push_back(token);
            std::cout << "[DEBUG] Excluding word: " << token << std::endl;
        } else {
            includeWords.push_back(token);
            std::cout << "[DEBUG] Including word: " << token << std::endl;
        }
    }

    // Vector to store results
    Vector<WordEntry> results;

    // Handle AND logic
    if (!includeWords.empty()) {
        std::set<int> intersectionDocIDs; // Store document IDs found for AND operation
        bool firstWord = true;

        for (const std::string &word : includeWords) {
            std::set<int> currentWordDocIDs; // Store document IDs for the current word
            unsigned int index = hash(word, map->capacity);
            unsigned int startIndex = index;

            // Linear probing to find the word
            while (map->table[index] != nullptr) {
                if (map->table[index] != TOMBSTONE && map->table[index]->key == word) {
                    std::cout << "[DEBUG] Found word: " << word << " in hash map." << std::endl;
                    for (int i = 0; i < map->table[index]->entries.getSize(); ++i) {
                        currentWordDocIDs.insert(map->table[index]->entries[i].docID);
                    }
                    break; // Stop once the word is found
                }
                index = (index + 1) % map->capacity;
                if (index == startIndex) {
                    break; // We've looped back to the start
                }
            }

            if (firstWord) {
                intersectionDocIDs = std::move(currentWordDocIDs);
                firstWord = false; // Now we have the first word's document IDs
            } else {
                // Perform intersection with existing document IDs
                std::set<int> tempSet;
                for (const int docID : currentWordDocIDs) {
                    if (intersectionDocIDs.count(docID) > 0) {
                        tempSet.insert(docID); // Keep only those in both sets
                    }
                }
                intersectionDocIDs = std::move(tempSet);
            }
        }

        std::cout << "[DEBUG] Intersection document IDs for AND: ";
        for (const int docID : intersectionDocIDs) {
            std::cout << docID << " ";
        }
        std::cout << std::endl;

        // Now gather results based on the intersectionDocIDs
        for (const int docID : intersectionDocIDs) {
            // Check if excluded
            bool isExcluded = false;
            for (const std::string &excludeWord : excludeWords) {
                if (getEntryFromHashMap(map, excludeWord, docID)) {
                    isExcluded = true; // Exclude this document
                    std::cout << "[DEBUG] Excluded document ID: " << docID << " for word: " << excludeWord << std::endl;
                    break;
                }
            }
            if (!isExcluded) {
                // If not excluded, gather all entries for this docID
                for (const auto &word : includeWords) {
                    unsigned int index = hash(word, map->capacity);
                    unsigned int startIndex = index;

                    while (map->table[index] != nullptr) {
                        if (map->table[index] != TOMBSTONE && map->table[index]->key == word) {
                            for (int i = 0; i < map->table[index]->entries.getSize(); ++i) {
                                const WordEntry &entry = map->table[index]->entries[i];
                                if (entry.docID == docID) {
                                    results.push_back(entry);
                                }
                            }
                            break;
                        }
                        index = (index + 1) % map->capacity;
                        if (index == startIndex) {
                            break; // We've looped back to the start
                        }
                    }
                }
            }
        }
    }

    // Handle OR logic: if no AND words, use OR logic
    if (includeWords.empty() && !excludeWords.empty()) {
        std::cout << "No included words to search for!" << std::endl;
        return;
    }

    // Handle OR logic for include words (if no AND search was executed)
    if (!results.empty()) {
        std::set<int> docIDs; // To ensure unique document IDs for OR logic
        for (const std::string &word : includeWords) {
            unsigned int index = hash(word, map->capacity);
            unsigned int startIndex = index;

            // Linear probing to find the word
            while (map->table[index] != nullptr) {
                if (map->table[index] != TOMBSTONE && map->table[index]->key == word) {
                    for (int i = 0; i < map->table[index]->entries.getSize(); ++i) {
                        const WordEntry &entry = map->table[index]->entries[i];

                        // Check if the entry is excluded
                        bool exclude = false;
                        for (const std::string &excludeWord : excludeWords) {
                            if (getEntryFromHashMap(map, excludeWord, entry.docID)) {
                                exclude = true; // Exclude this entry
                                break;
                            }
                        }

                        // Add entry only if it's not excluded and not already in results
                        if (!exclude && docIDs.insert(entry.docID).second) {
                            results.push_back(entry);
                            std::cout << "[DEBUG] Adding result for OR: Document ID: " << entry.docID << std::endl;
                        }
                    }
                    break; // Stop once the word is found
                }
                index = (index + 1) % map->capacity;
                if (index == startIndex) {
                    break; // We've looped back to the start
                }
            }
        }
    }

    // Sort results by TF-IDF
    sortVector(results, compareByTFIDF);

    if (results.empty()) {
        std::cout << "No matching results found!" << std::endl;
        return;
    }

    // Display the results in pages of 10
    size_t totalResults = results.getSize();
    size_t count = 0;
    std::cout << "Found in documents (top results based on TF-IDF):\n";

    // Use a set to track already displayed document IDs
    std::set<int> displayedDocIDs;

    while (count < totalResults) {
        // Display the next 10 results
        for (size_t i = count; i < count + 10 && i < totalResults; ++i) {
            const WordEntry &entry = results[i];

            // Check if this document ID has already been displayed
            if (displayedDocIDs.insert(entry.docID).second) { // Insert returns false if the ID was already present
                std::cout << "Document ID: " << entry.docID
                          << ", Position: " << entry.position
                          << ", File Name: " << entry.fileName
                          << ", TF-IDF: " << entry.tfidf
                          << ", TF: " << entry.tf << std::endl;
            }
        }
        count += 10; // Increment the count by 10

        // Check if there are more results to show
        if (count < totalResults) {
            std::string response;
            while (true) {
                std::cout << "Type 'yes' to view the next results or 'no' to exit...\n";
                std::getline(std::cin, response);

                // If the user types 'no', exit the loop
                if (response == "no") {
                    std::cout << "Exiting result display." << std::endl;
                    return; // Exit the searchWord function
                } else if (response == "yes") {
                    break; // User typed 'yes', continue to the next results
                } else {
                    std::cout << "Invalid input. Please type 'yes' or 'no'." << std::endl;
                }
            }
        }
    }
}

// Function to delete an entry by key and docID
bool deleteEntry(HashMap *map, const std::string &key, int docID) {
    unsigned int index = hash(key, map->capacity);
    unsigned int startIndex = index; // Store the starting index for loop detection

    while (map->table[index] != nullptr) {
        if (map->table[index] != TOMBSTONE && map->table[index]->key == key) {
            // Search for the specific entry to delete
            for (size_t i = 0; i < map->table[index]->entries.getSize(); ++i) {
                if (map->table[index]->entries[i].docID == docID) {
                    // Remove the entry from the Vector
                    map->table[index]->entries.erase(i); // Use the erase method to remove the entry
                    if (map->table[index]->entries.getSize() == 0) {
                        // If no entries left, mark the node as a tombstone
                        delete map->table[index]; // Free the node memory
                        map->table[index] = TOMBSTONE; // Mark as deleted
                    }
                    map->size--; // Decrement size
                    return true; // Deletion successful
                }
            }
            return false; // Entry not found for deletion
        }
        index = (index + 1) % map->capacity; // Linear probing
        if (index == startIndex) {
            break; // Loop back to the start
        }
    }

    return false; // Key not found
}


// Function to retrieve all keys in the hash map
void keys(HashMap *map, Vector<std::string> &keyVector) {
    for (size_t i = 0; i < map->capacity; ++i) {
        if (map->table[i] != nullptr && map->table[i] != TOMBSTONE) {
            keyVector.push_back(map->table[i]->key); // Add the key to the vector
        }
    }
}

// Destructor for HashMap to free allocated memory
void destroyHashMap(HashMap *map) {
    for (size_t i = 0; i < map->capacity; ++i) {
        if (map->table[i] != nullptr && map->table[i] != TOMBSTONE) {
            delete map->table[i]; // Free each node
        }
    }
    delete[] map->table; // Free the table itself
}
// Free the hash map
void freeHashMap(HashMap *map) {
    for (size_t i = 0; i < map->capacity; ++i) {
        if (map->table[i] != nullptr && map->table[i] != TOMBSTONE) {
            delete map->table[i]; // Free nodes
        }
    }
    delete[] map->table; // Free the table
}

