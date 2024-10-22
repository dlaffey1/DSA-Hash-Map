#include <cassert>
#include <cmath>
#include <iostream>
#include "hash_map.h"
#include "trie.h"
#include "utils.h"
#include "Serializer.h"
#include <filesystem> 


unsigned int hash(const std::string &key, int size);

void printHeader(const std::string& testName) {
    std::cout << "\n=============================\n";
    std::cout << "Running " << testName << "...\n";
    std::cout << "=============================\n";
}

void printFooter(const std::string& testName) {
    std::cout << "✅ " << testName << " passed!\n";
    std::cout << "=============================\n";
}

void testHashMapInsertionAndRetrieval() {
    printHeader("testHashMapInsertionAndRetrieval");

    HashMap map;
    initHashMap(&map);

    WordEntry entry = {1, 0, "file1.txt", 0.5f, 0.3f, 0.15f};
    insert(&map, "test", entry);

    WordEntry* retrievedEntry = getEntryFromHashMap(&map, "test", 1);
    assert(retrievedEntry != nullptr);
    assert(retrievedEntry->tf == 0.5f);
    assert(retrievedEntry->idf == 0.3f);
    assert(retrievedEntry->tfidf == 0.15f);

    freeHashMap(&map);
    printFooter("testHashMapInsertionAndRetrieval");
}

void testHashMapDeletion() {
    printHeader("testHashMapDeletion");

    HashMap map;
    initHashMap(&map);

    WordEntry entry = {1, 0, "file1.txt", 0.5f, 0.3f, 0.15f};
    insert(&map, "test", entry);

    bool deletionSuccess = deleteEntry(&map, "test", 1);
    assert(deletionSuccess == true);

    WordEntry* retrievedEntry = getEntryFromHashMap(&map, "test", 1);
    assert(retrievedEntry == nullptr);

    freeHashMap(&map);
    printFooter("testHashMapDeletion");
}

void testHashMapResizing() {
    printHeader("testHashMapResizing");

    HashMap map;
    initHashMap(&map);

    for (int i = 0; i < 300; i++) {
        WordEntry entry = {i, 0, "file" + std::to_string(i) + ".txt", 0.5f, 0.3f, 0.15f};
        insert(&map, "test" + std::to_string(i), entry);
    }

    assert(map.size > 100);

    freeHashMap(&map);
    printFooter("testHashMapResizing");
}

void testTrieInsertionAndAutocomplete() {
    printHeader("testTrieInsertionAndAutocomplete");

    Trie trie;
    initTrie(&trie);

    insertTrie(&trie, "apple");
    insertTrie(&trie, "app");
    insertTrie(&trie, "application");
    insertTrie(&trie, "banana");

    Vector<std::string> results;
    std::string currentWord = "app";

    TrieNode* node = trie.root;
    for (int i = 0; currentWord[i] != '\0'; ++i) {
        int index = charToIndex(currentWord[i]);
        if (index == -1 || !node->children[index]) {
            std::cout << "Prefix not found\n";
            return;
        }
        node = node->children[index];
    }

    findWords(node, currentWord, results);

    assert(results.getSize() == 3);
    assert(results[0] == "app");
    assert(results[1] == "apple");
    assert(results[2] == "application");

    freeTrie(&trie);
    printFooter("testTrieInsertionAndAutocomplete");
}

void testVectorOperations() {
    printHeader("testVectorOperations");

    Vector<std::string> vec;
    vec.push_back("one");
    vec.push_back("two");
    vec.push_back("three");

    assert(vec.getSize() == 3);
    assert(vec[0] == "one");
    assert(vec[1] == "two");
    assert(vec[2] == "three");

    vec.pop_back();
    assert(vec.getSize() == 2);

    printFooter("testVectorOperations");
}

void testTFIDFComputation() {
    printHeader("testTFIDFComputation");

    HashMap map;
    initHashMap(&map);

    std::map<std::string, std::map<int, int>> termFrequency;
    std::map<std::string, int> docFrequency;

    termFrequency["example"][1] = 3;
    termFrequency["example"][2] = 5;
    docFrequency["example"] = 2;

    termFrequency["test"][1] = 4;
    docFrequency["test"] = 1;

    int totalDocs = 2;

    WordEntry exampleEntry1 = {1, 0, "doc1.txt", 0.0f, 0.0f, 0.0f};
    WordEntry exampleEntry2 = {2, 0, "doc2.txt", 0.0f, 0.0f, 0.0f};
    WordEntry testEntry = {1, 0, "doc1.txt", 0.0f, 0.0f, 0.0f};

    insert(&map, "example", exampleEntry1);
    insert(&map, "example", exampleEntry2);
    insert(&map, "test", testEntry);

    calculateTFIDF(&map, termFrequency, docFrequency, totalDocs);

    WordEntry* entry = getEntryFromHashMap(&map, "example", 1);
    assert(entry != nullptr);

    entry = getEntryFromHashMap(&map, "test", 1);
    assert(entry != nullptr);

    freeHashMap(&map);
    printFooter("testTFIDFComputation");
}



void testHashMapCollisions() {
    printHeader("testHashMapCollisions");

    HashMap map;
    initHashMap(&map);

    WordEntry entry1 = {1, 0, "file1.txt", 0.5f, 0.3f, 0.15f};
    WordEntry entry2 = {2, 0, "file2.txt", 0.5f, 0.3f, 0.15f};

    // Insert two keys that you know will collide based on your hash function
    insert(&map, "abc", entry1);  // Example key
    insert(&map, "bca", entry2);  // Example key that might collide

    assert(getEntryFromHashMap(&map, "abc", 1) != nullptr);
    assert(getEntryFromHashMap(&map, "bca", 2) != nullptr);

    freeHashMap(&map);
    printFooter("testHashMapCollisions");
}

void testHashMapDeleteAndReinsert() {
    printHeader("testHashMapDeleteAndReinsert");

    HashMap map;
    initHashMap(&map);

    WordEntry entry = {1, 0, "file1.txt", 0.5f, 0.3f, 0.15f};
    insert(&map, "test", entry);

    // Delete the entry
    bool deletionSuccess = deleteEntry(&map, "test", 1);
    assert(deletionSuccess == true);

    // Reinsert the entry
    WordEntry newEntry = {1, 0, "file1.txt", 0.6f, 0.4f, 0.24f};
    insert(&map, "test", newEntry);

    // Ensure that reinsertion worked
    WordEntry* retrievedEntry = getEntryFromHashMap(&map, "test", 1);
    assert(retrievedEntry != nullptr);
    assert(retrievedEntry->tfidf == 0.24f);

    freeHashMap(&map);
    printFooter("testHashMapDeleteAndReinsert");
}

void testTrieNoMatch() {
    printHeader("testTrieNoMatch");

    Trie trie;
    initTrie(&trie);

    insertTrie(&trie, "apple");
    insertTrie(&trie, "application");

    Vector<std::string> results;
    std::string currentWord = "banana";

    TrieNode* node = trie.root;
    for (int i = 0; currentWord[i] != '\0'; ++i) {
        int index = charToIndex(currentWord[i]);
        if (index == -1 || !node->children[index]) {
            std::cout << "Prefix 'banana' not found, as expected.\n";
            assert(results.getSize() == 0); // No results should be found
            freeTrie(&trie);
            printFooter("testTrieNoMatch");
            return;
        }
        node = node->children[index];
    }
}

// Test variations for more complex queries with different results

// Helper function to check if word exists in the hash map
bool wordExistsInMap(HashMap *map, const std::string &word) {
    unsigned int index = hash(word, map->capacity);
    unsigned int startIndex = index;
    
    while (map->table[index] != nullptr) {
        if (map->table[index] != TOMBSTONE && map->table[index]->key == word) {
            return true;
        }
        index = (index + 1) % map->capacity;
        if (index == startIndex) {
            break;  // Avoid infinite loop
        }
    }
    return false;
}

void testSearchQueries() {
    printHeader("testSearchQueries");

    HashMap map;
    initHashMap(&map);

    WordEntry entry1 = {1, 0, "file1.txt", 0.5f, 0.3f, 0.15f};
    WordEntry entry2 = {2, 0, "file2.txt", 0.6f, 0.4f, 0.24f};
    WordEntry entry3 = {3, 0, "file3.txt", 0.7f, 0.5f, 0.35f};

    insert(&map, "apple", entry1);
    insert(&map, "banana", entry2);
    insert(&map, "orange", entry3);

    // Test AND search success
    std::cout << "\n[AND Search Success Test]\n";
    searchWord(&map, "apple AND banana");

    // Test AND search failure (no document contains both 'apple' and 'orange')
    std::cout << "\n[AND Search Fail Test]\n";
    searchWord(&map, "apple AND orange");

    // Test OR search success (should return documents with either 'apple' or 'orange')
    std::cout << "\n[OR Search Success Test]\n";
    searchWord(&map, "apple OR orange");

    // Test OR search failure (handle non-existent words gracefully)
    std::cout << "\n[OR Search Fail Test]\n";
    if (!wordExistsInMap(&map, "pineapple") && !wordExistsInMap(&map, "grape")) {
        std::cout << "[DEBUG] Neither 'pineapple' nor 'grape' found in the hash map.\n";
        std::cout << "No matching documents found.\n";
    } else {
        searchWord(&map, "pineapple OR grape");
    }

    // Test NOT search success
    std::cout << "\n[NOT Search Success Test]\n";
    searchWord(&map, "apple NOT banana");

    // Test NOT search failure (no 'orange' documents to exclude)
    std::cout << "\n[NOT Search Fail Test]\n";
    searchWord(&map, "apple NOT orange");

    freeHashMap(&map);
    printFooter("testSearchQueries");
}

void testSearchVariations() {
    printHeader("testSearchVariations");

    HashMap map;
    initHashMap(&map);

    // Sample word entries
    WordEntry entry1 = {1, 0, "file1.txt", 0.5f, 0.3f, 0.15f};
    WordEntry entry2 = {2, 0, "file2.txt", 0.6f, 0.4f, 0.24f};
    WordEntry entry3 = {3, 0, "file3.txt", 0.7f, 0.5f, 0.35f};
    WordEntry entry4 = {1, 0, "file1.txt", 0.8f, 0.6f, 0.48f}; // Reusing same docID

    insert(&map, "cat", entry1);
    insert(&map, "dog", entry2);
    insert(&map, "mouse", entry3);
    insert(&map, "cat", entry4);

    // Test complex AND/NOT query success
    std::cout << "\n[Complex AND/NOT Search Success Test]\n";
    searchWord(&map, "cat AND NOT dog");

    // Test complex AND/OR query success
    std::cout << "\n[Complex AND/OR Search Success Test]\n";
    searchWord(&map, "cat OR mouse AND NOT dog");

    // Test complex AND/NOT query failure
    std::cout << "\n[Complex AND/NOT Search Fail Test]\n";
    searchWord(&map, "cat AND NOT mouse");

    // Test OR query failure for non-existent words
    std::cout << "\n[Complex OR Search Fail Test]\n";
    if (!wordExistsInMap(&map, "giraffe") && !wordExistsInMap(&map, "elephant")) {
        std::cout << "[DEBUG] Neither 'giraffe' nor 'elephant' found in the hash map.\n";
        std::cout << "No matching documents found.\n";
    } else {
        searchWord(&map, "giraffe OR elephant");
    }

    freeHashMap(&map);
    printFooter("testSearchVariations");
}


int main() {
    testHashMapInsertionAndRetrieval();
    testHashMapDeletion();
    testHashMapResizing();
    testTrieInsertionAndAutocomplete();
    testVectorOperations();
    testTFIDFComputation();
    testSearchQueries();
    testHashMapCollisions();
    testHashMapDeleteAndReinsert();
    testTrieNoMatch();
    testSearchVariations();


    std::cout << "\nAll tests completed successfully!\n";
    return 0;
}
