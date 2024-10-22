#ifndef UTILS_H
#define UTILS_H
#include <filesystem>
#include <stdio.h>
#ifdef _WIN32
#include "dirent.h"  // Use the custom dirent.h for Windows
#else
#include <dirent.h>  // Use the standard dirent.h for Linux/macOS
#endif
#include <string.h>
#include "hash_map.h"
#include "trie.h"
#include <map>
void indexBooks(HashMap* hashMap, Trie* trie, const std::string& directory);
void calculateTFIDF(HashMap *index, 
                     const std::map<std::string, std::map<int, int>> &termFrequency, 
                     const std::map<std::string, int> &docFrequency, 
                     int totalDocs);

#endif // UTILS_H
