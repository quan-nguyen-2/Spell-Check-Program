#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dictionary.h"

dictionary_t *create_dictionary() {
    dictionary_t *dict = malloc(sizeof(dictionary_t));
    if (dict == NULL) {
        return NULL;
    }
    dict->root = NULL;
    dict->size = 0;
    return dict;
}

int dict_insert(dictionary_t *dict, const char *word) {
    if (dict == NULL || word == NULL) {
        return -1; // Error: Invalid input
    }

    // Create a new node for the word
    node_t *new_node = malloc(sizeof(node_t));
    if (new_node == NULL) {
        return -1; // Error: Memory allocation failed
    }
    strcpy(new_node->word, word);
    new_node->left = NULL;
    new_node->right = NULL;

    // Handle the case when the tree is empty
    if (dict->root == NULL) {
        dict->root = new_node;
        dict->size = 1;
        return 0; // Word successfully added
    }

    // Insert the word into the binary search tree
    node_t *current = dict->root;
    while (current) {
        int cmp = strcmp(word, current->word);
        if (cmp == 0) {
            free(new_node); // Word already exists, so free the new node
            return -1; // Error: Duplicate word
        } else if (cmp < 0) {
            if (current->left == NULL) {
                current->left = new_node;
                dict->size++;
                return 0; // Word successfully added
            }
            current = current->left;
        } else {
            if (current->right == NULL) {
                current->right = new_node;
                dict->size++;
                return 0; // Word successfully added
            }
            current = current->right;
        }
    }

    return -1; // Error: Shouldn't reach here
}


int dict_find(const dictionary_t *dict, const char *query) {
    if (dict == NULL || query == NULL) {
        return 0; // Error: Invalid input or empty dictionary
    }

    // Search for the word in the binary search tree, using case-insensitive comparison
    node_t *current = dict->root;
    while (current) {
        int cmp = strcasecmp(query, current->word); // Use strcasecmp (case-insensitive comparison)
        if (cmp == 0) {
            return 1; // Word found
        } else if (cmp < 0) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    return 0; // Word not found
}


void print_in_order(node_t *node) {
    if (node == NULL) {
        return;
    }
    print_in_order(node->left);
    printf("%s\n", node->word);
    print_in_order(node->right);
}

void dict_print(const dictionary_t *dict) {
    if (dict != NULL) {
        print_in_order(dict->root);
    }
}


void free_post_order(node_t *node) {
    if (node == NULL) {
        return;
    }
    free_post_order(node->left);
    free_post_order(node->right);
    free(node);
}

void dict_free(dictionary_t *dict) {
    if (dict != NULL) {
        free_post_order(dict->root);
        free(dict);
    }
}

dictionary_t *read_dict_from_text_file(const char *file_name) {
    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        return NULL; // Error: Unable to open the file
    }

    dictionary_t *dict = create_dictionary();
    if (dict == NULL) {
        fclose(file);
        return NULL; // Error: Unable to create the dictionary
    }

    char word[MAX_WORD_LEN];
    while (fgets(word, sizeof(word), file) != NULL) {
        // Remove newline character if present
        size_t len = strlen(word);
        if (len > 0 && word[len - 1] == '\n') {
            word[len - 1] = '\0';
        }

        // Insert the word into the dictionary
        if (dict_insert(dict, word) != 0) {
            // Handle insertion error
            fclose(file);
            dict_free(dict);
            return NULL;
        }
    }

    fclose(file);
    return dict;
}


int write_dict_to_text_file(const dictionary_t *dict, const char *file_name) {
    if (dict == NULL) {
        return -1; // Error: Invalid input
    }

    FILE *file = fopen(file_name, "w");
    if (file == NULL) {
        return -1; // Error: Unable to open the file
    }

    // Helper function for recursive writing
    void write_recursive(node_t *node) {
        if (node == NULL) {
            return;
        }
        write_recursive(node->left);
        fprintf(file, "%s\n", node->word);
        write_recursive(node->right);
    }

    // Write the words to the file using in-order traversal
    write_recursive(dict->root);

    fclose(file);
    return 0; // Success
}
