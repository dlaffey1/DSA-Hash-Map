# DSA-Hash-Map
Steps to run the project:
1.From root of project run "make clean"
2. Then run "make"
3. Then run ./MiniSearchEngine or MiniSearchEngine.exe which will prompt the program

Layout for the files:

utils.cpp - Contains the indexing for the project and also contains the TF-IDF for all of the words which are indexed. This is used later once a word is searched to quickly find the files in the top 10 which have the most occurences of the searched word. 

Vector.cpp.h - Custom implementation for the vector class which is used over most of the files in the project

Hashmap.cpp.h - this contains the structure for the word entries which contains :
    int docID;
    int position;
    std::string fileName;
    float tf;   // Term Frequency
    float idf;  // Inverse Document Frequency
    float tfidf; // TF-IDF:

    This file then also contains the Hashmaps structure which I have implemented a Seperate chaining Hashmap.

Trie.cpp.h - This contains all the logic for initiating, inserting and facilitating autocomplete using the trie data structure.

main.cpp - Root directory which runs the parser

parser.cpp - takes all commands and serves as a cli for the user to use the search and autocomplete
e

Vector_test.cpp - Testing for the project

