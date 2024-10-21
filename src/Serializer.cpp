#include "Serializer.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include "hash_map.h"

void Serializer::serializeHashMap(const HashMap *map, const std::string &filename) {
    std::string directory = "index";
    if (!std::filesystem::exists(directory)) {
        std::filesystem::create_directory(directory);
    }
    
    std::ofstream file(filename, std::ios::out | std::ios::binary);
    if (!file) {
        std::cerr << "Error: Unable to open file for serialization: " << filename << std::endl;
        return;
    }

    file.write(reinterpret_cast<const char*>(&map->capacity), sizeof(map->capacity));

    for (size_t i = 0; i < map->capacity; ++i) {
        if (map->table[i] != nullptr && map->table[i] != TOMBSTONE) {
            // Serialize the key
            size_t keySize = map->table[i]->key.size();
            file.write(reinterpret_cast<const char*>(&keySize), sizeof(keySize));
            file.write(map->table[i]->key.data(), keySize);

            // Serialize the number of WordEntries
            size_t entriesSize = map->table[i]->entries.getSize();
            file.write(reinterpret_cast<const char*>(&entriesSize), sizeof(entriesSize));

            // Serialize the WordEntries
            for (size_t j = 0; j < entriesSize; ++j) {
                WordEntry entry = map->table[i]->entries[j];
                file.write(reinterpret_cast<const char*>(&entry.docID), sizeof(entry.docID));
                file.write(reinterpret_cast<const char*>(&entry.position), sizeof(entry.position));

                // Serialize fileName (as string)
                size_t fileNameSize = entry.fileName.size();
                file.write(reinterpret_cast<const char*>(&fileNameSize), sizeof(fileNameSize));
                file.write(entry.fileName.data(), fileNameSize);

                file.write(reinterpret_cast<const char*>(&entry.tf), sizeof(entry.tf));
                file.write(reinterpret_cast<const char*>(&entry.idf), sizeof(entry.idf));
                file.write(reinterpret_cast<const char*>(&entry.tfidf), sizeof(entry.tfidf));
            }
        }
    }

    file.close();
    std::cout << "HashMap successfully serialized to file: " << filename << std::endl;
}

void Serializer::deserializeHashMap(HashMap *map, const std::string &filename) {
    std::ifstream file(filename, std::ios::in | std::ios::binary);
    if (!file) {
        std::cerr << "Error: Unable to open file for deserialization: " << filename << std::endl;
        return;
    }

    file.read(reinterpret_cast<char*>(&map->capacity), sizeof(map->capacity));

    map->table = new HashMapNode*[map->capacity]();
    map->size = 0;

    while (file.peek() != EOF) {
        size_t keySize;
        file.read(reinterpret_cast<char*>(&keySize), sizeof(keySize));
        std::string key(keySize, ' ');
        file.read(&key[0], keySize);

        // Deserialize the number of WordEntries
        size_t entriesSize;
        file.read(reinterpret_cast<char*>(&entriesSize), sizeof(entriesSize));

        // Create a new HashMapNode for this key
        HashMapNode *node = new HashMapNode;
        node->key = key;

        // Deserialize the WordEntries
        for (size_t i = 0; i < entriesSize; ++i) {
            WordEntry entry;
            file.read(reinterpret_cast<char*>(&entry.docID), sizeof(entry.docID));
            file.read(reinterpret_cast<char*>(&entry.position), sizeof(entry.position));

            // Deserialize fileName (as string)
            size_t fileNameSize;
            file.read(reinterpret_cast<char*>(&fileNameSize), sizeof(fileNameSize));
            entry.fileName.resize(fileNameSize);
            file.read(&entry.fileName[0], fileNameSize);

            file.read(reinterpret_cast<char*>(&entry.tf), sizeof(entry.tf));
            file.read(reinterpret_cast<char*>(&entry.idf), sizeof(entry.idf));
            file.read(reinterpret_cast<char*>(&entry.tfidf), sizeof(entry.tfidf));

            // Add the WordEntry to the node
            node->entries.push_back(entry);
        }

        // Insert the node into the HashMap
        unsigned int index = hash(key, map->capacity);
        while (map->table[index] != nullptr && map->table[index] != TOMBSTONE) {
            index = (index + 1) % map->capacity;
        }
        map->table[index] = node;
        map->size++;
    }
    file.close();
    std::cout << "HashMap successfully deserialized from file: " << filename << std::endl;
}
