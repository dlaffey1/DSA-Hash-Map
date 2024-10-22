#include <stdio.h>
#include <string.h>
#include "hash_map.h"
#include "trie.h"
#include "utils.h"
#define MAX_QUERY_LENGTH 100

// Parser function
void parseCommand(HashMap* index, Trie* autocompleteTrie) {
    char command[MAX_QUERY_LENGTH];
    char query[MAX_QUERY_LENGTH];

    while (1) {
        printf("\nEnter a command ('search', 'autocomplete', or 'exit'): ");
        fgets(command, sizeof(command), stdin);
        
        // Remove the newline character if present
        size_t commandLen = strlen(command);
        if (commandLen > 0 && command[commandLen - 1] == '\n') {
            command[commandLen - 1] = '\0'; // Replace newline with null terminator
        }

        // If search is pressed
        if (strcmp(command, "search") == 0) {
            printf("Example commands: **** AND ****, **** NOT ***, **** OR ****\nEnter a word to search: ");
            fgets(query, sizeof(query), stdin);
            
            // Remove the newline character from the query if present
            size_t len = strlen(query);
            if (len > 0 && query[len - 1] == '\n') {
                query[len - 1] = '\0'; // Replace newline with null terminator
            }

            // Calls searchWord function with input word
            searchWord(index, query);
        } 
        // If the user enters 'autocomplete', ask for a prefix
        else if (strcmp(command, "autocomplete") == 0) {
            printf("Enter prefix for autocomplete: ");
            fgets(query, sizeof(query), stdin);

            // Remove the newline character from the prefix
            size_t len = strlen(query);
            if (len > 0 && query[len - 1] == '\n') {
                query[len - 1] = '\0'; 
            }
            autocomplete(autocompleteTrie, query);
        } 
        // If 'exit', break loop
        else if (strcmp(command, "exit") == 0) {
            printf("Exiting the program...\n");
            break;
        } 
        else {
            printf("Invalid command. Please enter 'search', 'autocomplete', or 'exit'.\n");
        }
    }
}