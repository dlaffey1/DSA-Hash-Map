#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <iostream>
#include <fstream>
#include <string>
#include "hash_map.h"

class Serializer
{
public:
    static void serializeHashMap(const HashMap *map, const std::string &filename);
    static void deserializeHashMap(HashMap *map, const std::string &filename);
};

#endif // SERIALIZER_H