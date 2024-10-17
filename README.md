# DSA-Hash-Map
Steps to run the project:
1.From root of project run "make clean"
2. Then run "make"
3. Then run ./MiniSearchEngine which will prompt the program

Layout for the files:
utils.cpp- Contains the indexing for the project and also contains the TF-IDF for all of the words which are indexed. This is used later once a word is searched to quickly find the files in the top 10 which have the most occurences of the searched word. 

Hashmap.cpp.h: this contains the structure for the word entries which contains :
    int docID;
    int position;
    std::string fileName;
    float tf;   // Term Frequency
    float idf;  // Inverse Document Frequency
    float tfidf; // TF-IDF:

    This file then also contains the Hashmaps structure which I have implemented a Seperate chaining Hashmap.

Trie.cpp.h: This contains all the logic for initiating, inserting and facilitating autocomplete using the trie data structure.

main.cpp- Sequentially runs all of the files according to the spec of the poject



Current Problems on the project:
1.Segmentation error arising once more books are added

2.TF-IDF is correctly calculated but is being incorrectly called so correct sorting but incorrrect final c.out of the tf-idf value

3. Need to add more testing to the project