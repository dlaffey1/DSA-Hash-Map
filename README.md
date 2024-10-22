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

- **trie.cpp and trie.h** 

    Contains the logic for initiating, inserting, and facilitating autocomplete using the trie data structure.

- **main.cpp**

    The root directory that runs the parser and looks for the serialized data structures in the "index" folder. If they exist, they're deserialized and used, else the text files in the "data" folder will be indexed.

- **parser.cpp**

    Handles all commands and serves as a CLI for the user to utilize the search and autocomplete features.

- **Vector_test.cpp**

    Testing for the Vector class.

- **extensive_tests.cpp**

    Testing for the whole project.

- **Serializer.cpp and Serializer.h**

    Contains the serialization and deserialization logic for both the HashMap and Trie data structures.


## Folder Structure

```
DSA-Hash-Map/
│
├── data/                    # Storing text files
│   ├── Dracula_BramStoker.txt
│   └── Frankenstein_MaryShelley.txt
|
├── include/                 # Header files for public interfaces
│   ├── dirent.h
│   ├── hash_map.h
│   ├── parser.h
│   ├── Serializer.h
│   ├── trie.h
│   ├── utils.h
│   ├── Vector.h
│   └── Vector.h
│
├── data/                    # For storing serialized hashmap and trie
|   └──
|
├── src/                     # Source files
│   ├── hash_map.cpp
│   ├── main.cpp             # Main entry point of the application
│   ├── parser.cpp
│   ├── Serializer.cpp
│   ├── trie.cpp
│   ├── utils.cpp            # Utility functions
│   └── Vector.cpp
│
├── tests/
│   ├── extensive_tests.cpp
|   └── Vector_test.cpp
│
├── .gitignore
├── Makefile
└── README.md
```