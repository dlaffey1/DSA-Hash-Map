#ifndef UTILS_H
#define UTILS_H

#include <filesystem>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include "hash_map.h"
#include "trie.h"
#include <map>
#include "Serializer.h"

// Function to index books and add them to the hash map and trie
void indexBooks(HashMap* hashMap, Trie* trie, const std::string& directory, Serializer& serializer);
void calculateTFIDF(HashMap *index, 
                     const std::map<std::string, std::map<int, int>> &termFrequency, 
                     const std::map<std::string, int> &docFrequency, 
                     int totalDocs);

#endif // UTILS_H
