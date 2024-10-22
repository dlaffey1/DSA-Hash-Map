# DSA-Hash-Map

## Steps to run the project:
1. From the root of project, run:
```bash
make clean
```
2. Then run:
```bash
make
```
3. Finally, execute the program:
```bash
./MiniSearchEngine
```
or, on Windows:
```bash
MiniSearchEngine.exe
```
This will prompt the program.

## File Layout

- **utils.cpp**

Contains the indexing for the project and the TF-IDF for all indexed words. This is used later when a word is searched to quickly find the top 10 files with the most occurences of the searched word. 

- **Vector.cpp and Vector.h**

A custom implementation of the vector class, used throughout most of the project files.

- **Hashmap.cpp and HashMap.h**

This file contains the structure for word entries, which includes:
```cpp
    int docID;
    int position;
    std::string fileName;
    float tf;   // Term Frequency
    float idf;  // Inverse Document Frequency
    float tfidf; // TF-IDF:
```
It also implements a separate chaining hashmap structure.

- **Trie.cpp and Trie.h** 

Contains the logic for initiating, inserting, and facilitating autocomplete using the trie data structure.

- **main.cpp**

The root directory that runs the parser.

- **parser.cpp**

Handles all commands and serves as a CLI for the user to utilize the search and autocomplete features.

- **Vector_test.cpp**

Testing for the Vector class.

