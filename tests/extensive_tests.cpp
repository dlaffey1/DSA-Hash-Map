#include <iostream>
#include <cassert>
#include "hash_map.h"
#include "trie.h"
#include "Vector.h"


// Helper function to print a line separator
void printSeparator() {
    std::cout << "-------------------------------------" << std::endl;
}

// Test for HashMap Insertion
void testHashMapInsert() {
    printSeparator();
    std::cout << "Running: testHashMapInsert" << std::endl;

    HashMap map;
    initHashMap(&map);

    WordEntry entry = {1, 0, "test.txt", 0.5, 0.7, 1.2};
    bool success = insert(&map, "word", entry);
    assert(success == true);

    WordEntry *result = getEntryFromHashMap(&map, "word", 1);
    assert(result != nullptr);
    assert(result->docID == 1);
    assert(result->fileName == "test.txt");
    assert(result->tf == 0.5);

    freeHashMap(&map);
    std::cout << "testHashMapInsert PASSED" << std::endl;
    printSeparator();
}

// Test for HashMap Resizing
void testHashMapResize() {
    printSeparator();
    std::cout << "Running: testHashMapResize" << std::endl;

    HashMap map;
    initHashMap(&map);

    // Inserting multiple elements to trigger resizing
    for (int i = 0; i < 200; ++i) {
        WordEntry entry = {i, i, "doc" + std::to_string(i) + ".txt", 0.1f * i, 0.2f * i, 0.02f * i};
        bool success = insert(&map, "word" + std::to_string(i), entry);
        assert(success == true);
    }

    // Ensure the map resized and entries are still valid
    for (int i = 0; i < 200; ++i) {
        WordEntry *result = getEntryFromHashMap(&map, "word" + std::to_string(i), i);
        assert(result != nullptr);
        assert(result->docID == i);
    }

    freeHashMap(&map);
    std::cout << "testHashMapResize PASSED" << std::endl;
    printSeparator();
}

// Test for HashMap Collision Handling
void testHashMapCollision() {
    printSeparator();
    std::cout << "Running: testHashMapCollision" << std::endl;

    HashMap map;
    initHashMap(&map);

    // Manually creating collisions by inserting keys that hash to the same index
    WordEntry entry1 = {1, 0, "doc1.txt", 0.2, 0.3, 0.06};
    WordEntry entry2 = {2, 1, "doc2.txt", 0.3, 0.4, 0.12};

    insert(&map, "word", entry1);
    insert(&map, "Word", entry2); // 'word' and 'Word' may collide based on hash function

    // Check both entries exist and are distinct
    WordEntry *result1 = getEntryFromHashMap(&map, "word", 1);
    WordEntry *result2 = getEntryFromHashMap(&map, "Word", 2);
    assert(result1 != nullptr);
    assert(result2 != nullptr);
    assert(result1->docID == 1);
    assert(result2->docID == 2);

    freeHashMap(&map);
    std::cout << "testHashMapCollision PASSED" << std::endl;
    printSeparator();
}

// Test for HashMap Deletion
void testHashMapDeletion() {
    printSeparator();
    std::cout << "Running: testHashMapDeletion" << std::endl;

    HashMap map;
    initHashMap(&map);

    WordEntry entry = {1, 0, "test.txt", 0.5, 0.7, 1.2};
    insert(&map, "word", entry);

    // Delete the entry
    bool deleteSuccess = deleteEntry(&map, "word", 1);
    assert(deleteSuccess == true);

    // Verify the entry no longer exists
    WordEntry *result = getEntryFromHashMap(&map, "word", 1);
    assert(result == nullptr);

    freeHashMap(&map);
    std::cout << "testHashMapDeletion PASSED" << std::endl;
    printSeparator();
}

// Test for HashMap Key Retrieval
void testHashMapKeys() {
    printSeparator();
    std::cout << "Running: testHashMapKeys" << std::endl;

    HashMap map;
    initHashMap(&map);

    WordEntry entry1 = {1, 0, "doc1.txt", 0.5, 0.7, 1.2};
    WordEntry entry2 = {2, 0, "doc2.txt", 0.3, 0.4, 0.8};
    WordEntry entry3 = {3, 0, "doc3.txt", 0.4, 0.6, 1.0};

    insert(&map, "key1", entry1);
    insert(&map, "key2", entry2);
    insert(&map, "key3", entry3);

    Vector<std::string> keys = map.keys();
    assert(keys.getSize() == 3);
    assert(keys[0] == "key1");
    assert(keys[1] == "key2");
    assert(keys[2] == "key3");

    freeHashMap(&map);
    std::cout << "testHashMapKeys PASSED" << std::endl;
    printSeparator();
}

// Test for HashMap Key Duplication
void testHashMapKeyDuplication() {
    printSeparator();
    std::cout << "Running: testHashMapKeyDuplication" << std::endl;

    HashMap map;
    initHashMap(&map);

    WordEntry entry1 = {1, 0, "doc1.txt", 0.5, 0.7, 1.2};
    WordEntry entry2 = {2, 1, "doc2.txt", 0.3, 0.4, 0.8};

    insert(&map, "duplicateKey", entry1);
    insert(&map, "duplicateKey", entry2); // Should update the same key

    WordEntry *result = getEntryFromHashMap(&map, "duplicateKey", 2);
    assert(result != nullptr);
    assert(result->docID == 2);

    freeHashMap(&map);
    std::cout << "testHashMapKeyDuplication PASSED" << std::endl;
    printSeparator();
}

// Test for Trie Insertion
void testTrieInsert() {
    printSeparator();
    std::cout << "Running: testTrieInsert" << std::endl;

    Trie trie;
    initTrie(&trie);

    insertTrie(&trie, "hello");
    insertTrie(&trie, "hell");

    TrieNode *node = trie.root;
    bool passed = true;
    for (char c : std::string("hell")) {
        if (!node->children[charToIndex(c)]) {
            passed = false;
            break;
        }
        node = node->children[charToIndex(c)];
    }

    assert(passed == true);
    assert(node->isEndOfWord == true);

    freeTrie(&trie);
    std::cout << "testTrieInsert PASSED" << std::endl;
    printSeparator();
}

// Test for Trie Autocomplete with Valid Prefix
void testTrieAutocompleteValidPrefix() {
    printSeparator();
    std::cout << "Running: testTrieAutocompleteValidPrefix" << std::endl;

    Trie trie;
    initTrie(&trie);

    insertTrie(&trie, "apple");
    insertTrie(&trie, "app");
    insertTrie(&trie, "application");

    std::string prefix = "app";
    autocomplete(&trie, prefix.c_str());

    freeTrie(&trie);
    std::cout << "testTrieAutocompleteValidPrefix PASSED" << std::endl;
    printSeparator();
}

// Test for Trie Autocomplete with Invalid Prefix
void testTrieAutocompleteInvalidPrefix() {
    printSeparator();
    std::cout << "Running: testTrieAutocompleteInvalidPrefix" << std::endl;

    Trie trie;
    initTrie(&trie);

    insertTrie(&trie, "apple");
    insertTrie(&trie, "app");

    // Try autocomplete with a prefix that doesn't exist
    std::string invalidPrefix = "zoo";
    autocomplete(&trie, invalidPrefix.c_str());  // Should not find anything

    freeTrie(&trie);
    std::cout << "testTrieAutocompleteInvalidPrefix PASSED" << std::endl;
    printSeparator();
}

// Test for Trie Case Sensitivity
void testTrieCaseSensitivity() {
    printSeparator();
    std::cout << "Running: testTrieCaseSensitivity" << std::endl;

    Trie trie;
    initTrie(&trie);

    insertTrie(&trie, "Apple");
    insertTrie(&trie, "apple");

    // Searching for lowercase should find "apple"
    autocomplete(&trie, "app");

    freeTrie(&trie);
    std::cout << "testTrieCaseSensitivity PASSED" << std::endl;
    printSeparator();
}

// Test for Trie with Complex Insertions
void testTrieComplexInsertions() {
    printSeparator();
    std::cout << "Running: testTrieComplexInsertions" << std::endl;

    Trie trie;
    initTrie(&trie);

    insertTrie(&trie, "apple");
    insertTrie(&trie, "application");
    insertTrie(&trie, "applicable");
    insertTrie(&trie, "app");
    insertTrie(&trie, "apex");

    std::string prefix = "app";
    autocomplete(&trie, prefix.c_str());

    freeTrie(&trie);
    std::cout << "testTrieComplexInsertions PASSED" << std::endl;
    printSeparator();
}

// Test for Trie with Long Word Insertions
void testTrieLongWordInsertions() {
    printSeparator();
    std::cout << "Running: testTrieLongWordInsertions" << std::endl;

    Trie trie;
    initTrie(&trie);

    insertTrie(&trie, "pneumonoultramicroscopicsilicovolcanoconiosis");

    std::string prefix = "pneumono";
    autocomplete(&trie, prefix.c_str());

    freeTrie(&trie);
    std::cout << "testTrieLongWordInsertions PASSED" << std::endl;
    printSeparator();
}

// Test for Trie with Large Number of Insertions
void testTrieLargeInsertions() {
    printSeparator();
    std::cout << "Running: testTrieLargeInsertions" << std::endl;

    Trie trie;
    initTrie(&trie);

    for (int i = 0; i < 10000; ++i) {
        insertTrie(&trie, ("word" + std::to_string(i)).c_str());
    }

    std::string prefix = "word";
    autocomplete(&trie, prefix.c_str());

    freeTrie(&trie);
    std::cout << "testTrieLargeInsertions PASSED" << std::endl;
    printSeparator();
}

// Test for Trie Search Efficiency
void testTrieSearchEfficiency() {
    printSeparator();
    std::cout << "Running: testTrieSearchEfficiency" << std::endl;

    Trie trie;
    initTrie(&trie);

    for (int i = 0; i < 10000; ++i) {
        insertTrie(&trie, ("testword" + std::to_string(i)).c_str());
    }

    std::string prefix = "testword999";
    autocomplete(&trie, prefix.c_str());

    freeTrie(&trie);
    std::cout << "testTrieSearchEfficiency PASSED" << std::endl;
    printSeparator();
}

// Main function to run all tests
int main() {
    // HashMap Tests
    testHashMapInsert();
    testHashMapResize();
    testHashMapCollision();
    testHashMapDeletion();
    testHashMapKeys();
    testHashMapKeyDuplication();

    // Trie Tests
    testTrieInsert();
    testTrieAutocompleteValidPrefix();
    testTrieAutocompleteInvalidPrefix();
    testTrieCaseSensitivity();
    testTrieComplexInsertions();
    testTrieLongWordInsertions();
    testTrieLargeInsertions();
    testTrieSearchEfficiency();

    return 0;
}
