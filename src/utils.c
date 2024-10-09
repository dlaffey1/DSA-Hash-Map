#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include "hash_map.h"
#include "trie.h"

// Function to index books and add them to the hash map and trie
void indexBooks(HashMap *index, Trie *autocompleteTrie, const char *dataDir) {
    DIR *dir;
    struct dirent *entry;

    // Open the directory containing the book files
    if ((dir = opendir(dataDir)) == NULL) {
        perror("opendir");
        return;
    }

    // Iterate through the files in the directory
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) { // Only process regular files
            char filePath[512]; // Increase buffer size
            snprintf(filePath, sizeof(filePath), "%s/%s", dataDir, entry->d_name);
            FILE *file = fopen(filePath, "r");
            if (file) {
                char word[100];
                int position = 0;

                // Read each word from the file
                while (fscanf(file, "%s", word) == 1) {
                    // Create a new WordEntry for the current word
                    WordEntry entry = { .docID = 1, .position = position }; // You can update docID based on the file number
                    insert(index, word, entry);
                    insertTrie(autocompleteTrie, word);
                    position++;
                }
                fclose(file);
            } else {
                perror("fopen");
            }
        }
    }
    closedir(dir);
}
