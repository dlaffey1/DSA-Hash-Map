#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <iostream>
#include <fstream>
#include <string>
#include "hash_map.h"
#include "trie.h"

class Serializer
{
public:
    static void serializeHashMap(const HashMap *map, const std::string &filename);
    static void deserializeHashMap(HashMap *map, const std::string &filename);

    static void serializeTrie(const Trie *trie, const std::string &filename);
    static void serializeTrieNode(std::ofstream &file, TrieNode *node);
    static void deserializeTrie(Trie *trie, const std::string &filename);
    static TrieNode* deserializeTrieNode(std::ifstream &file);
};

#endif // SERIALIZER_H