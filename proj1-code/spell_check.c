#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h> // for exit()
#include "dictionary.h"

#define MAX_CMD_LEN 128
// A helper function to spell check a specific file
// 'file_name': Name of the file to spell check
// 'dict': A dictionary containing correct words
int spell_check_file(const char *file_name, const dictionary_t *dict) {
    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        printf("Spell check failed\n");
        return -1; // Error: Unable to open the file
    }

    char word[MAX_WORD_LEN];
    int misspelled = 0;

    while (fscanf(file, "%s", word) != EOF) {
        // Remove non-alphabetic characters from the word
        int i = 0, j = 0;
        while (word[i]) {
            if (isalpha(word[i])) {
                word[j++] = tolower(word[i]);
            }
            i++;
        }
        word[j] = '\0';

        // Check if the word is in the dictionary
        if (!dict_find(dict, word)) {
            if (misspelled) {
                printf(" "); // Add a space before misspelled words (except the first)
            }
            printf("%s[X]", word);
            misspelled = 1;
        } else {
            if (misspelled) {
                printf(" "); // Add a space before words (except the first misspelled)
            }
            printf("%s", word);
        }

        if (fgetc(file) == '\n') {
            if (misspelled) {
                printf("\n"); // Add a newline if there are misspelled words in the line
                misspelled = 0; // Reset the misspelled counter
            } else {
                printf("\n"); // Add a newline between lines if no misspelled words
            }
        } else {
            printf(" "); // Add a space between words
        }
    }

    fclose(file);

    return 0;
}

/*
 * This is in general *very* similar to the list_main file seen in lab
 */
int main(int argc, char **argv) {
    dictionary_t *dict = create_dictionary();
    char cmd[MAX_CMD_LEN];
     // Handle command-line arguments
       if (argc >= 2) {
        dictionary_t *new_dict = read_dict_from_text_file(argv[1]);
        
        if (new_dict != NULL) {
            dict_free(dict);
            dict = new_dict;
            printf("Dictionary successfully read from text file\n");
        } else {
            printf("Failed to read dictionary from text file\n");
            dict_free(dict);
            exit(EXIT_FAILURE);
        }
    }

    if (argc == 3) {
        spell_check_file(argv[2], dict);
        dict_free(dict);
        exit(EXIT_SUCCESS);
    }
    
    printf("CSCI 2021 Spell Check System\n");
    printf("Commands:\n");
    printf("  add <word>:              adds a new word to dictionary\n");
    printf("  lookup <word>:           searches for a word\n");
    printf("  print:                   shows all words currently in the dictionary\n");
    printf("  load <file_name>:        reads in dictionary from a file\n");
    printf("  save <file_name>:        writes dictionary to a file\n");
    printf("  check <file_name>: spell checks the specified file\n");
    printf("  exit:                    exits the program\n");



    while (1) {
        printf("spell_check> ");
        if (scanf("%s", cmd) == EOF) {
            printf("\n");
            break;
        }

        if (strcmp("exit", cmd) == 0) {
            break;
        }

        // Handle other commands
        if (strcmp("add", cmd) == 0) {
            char word[MAX_WORD_LEN];
            if (scanf("%s", word) != EOF) {
                if (dict_insert(dict, word) == 0) {
                    //printf("'%s' added to dictionary\n", word);
                } else {
                    //printf("Failed to add '%s' to dictionary\n", word);
                }
            }
        } else if (strcmp("lookup", cmd) == 0) {
            char word[MAX_WORD_LEN];
            if (scanf("%s", word) != EOF) {
                if (dict_find(dict, word)) {
                    printf("'%s' present in dictionary\n", word);
                } else {
                    printf("'%s' not found\n", word);
                }
            }
        } else if (strcmp("print", cmd) == 0) {
            dict_print(dict);
        } else if (strcmp("load", cmd) == 0) {
            char file_name[MAX_CMD_LEN];
            if (scanf("%s", file_name) != EOF) {
                dictionary_t *new_dict = read_dict_from_text_file(file_name);
                if (new_dict != NULL) {
                    dict_free(dict);
                    dict = new_dict;
                    printf("Dictionary successfully read from text file\n");
                } else {
                    printf("Failed to read dictionary from text file\n");
                }
            }
        } else if (strcmp("save", cmd) == 0) {
            char file_name[MAX_CMD_LEN];
            if (scanf("%s", file_name) != EOF) {
                if (write_dict_to_text_file(dict, file_name) == 0) {
                    printf("Dictionary successfully written to text file\n");
                } else {
                    printf("Failed to write dictionary to text file\n");
                }
            }
        } else if (strcmp("check", cmd) == 0) {
            char file_name[MAX_CMD_LEN];
            if (scanf("%s", file_name) != EOF) {
                spell_check_file(file_name, dict);
                printf("\n");
            }
        } else {
            printf("Unknown command %s\n", cmd);
        }
    }

    dict_free(dict);
    return 0;
}
