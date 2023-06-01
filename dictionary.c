// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];

// size of dictionnary
int dictionnary_size = 0;

void free_node(node *n);

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // convert word to lowercase
    char *copy_w = malloc(LENGTH + 1);
    for (int c_index = 0; c_index < strlen(word); c_index++)
    {
        if (isalpha(word[c_index]))
        {
            copy_w[c_index] = tolower(word[c_index]);
        }
        else
        {
            copy_w[c_index] = word[c_index];
        }
    }
    copy_w[strlen(word)] = '\0';
    int hash_w = hash(copy_w);

    // check if word is present in bucket
    for (node *n = table[hash_w]; n != NULL; n = n->next)
    {
        if (strcasecmp(n->word, copy_w) == 0)
        {
            free(copy_w);
            return true;
        }
    }
    free(copy_w);
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    // hash based on the first letter
    // si if necessary to use the two first letters
    return toupper(word[0]) - 'A';
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    // 1 - create hash table of size 26 (alphabet size)
    for (int h_index = 0; h_index < LENGTH; h_index++)
    {
        node *n = malloc(sizeof(node));
        n->next = NULL;
        // TODO add word
        table[h_index] = n;
    }

    // open the dictionnary
    FILE *input = fopen(dictionary, "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return false;
    }
    // 2 - read file in loop
    char *entry = malloc(LENGTH + 1);
    while (fscanf(input, "%s", entry) != EOF)
    {
        // 3 - create new node
        node *n = malloc(sizeof(node));

        // return false if out of memory
        if (n == NULL)
        {
            return false;
        }

        // 4 put data in the node
        strcpy(n->word, entry);

        // 5 set next if I don't have a previous element
        n->next = NULL;

        // 6 - find the bucket where to place the new node
        int w_hash = hash(entry);

        // 5 - link the node into the right bucket
        node *head = table[w_hash];
        // if (head->next == NULL)
        // {
        //     // 6 set new node's next the first element of list
        //     n->next = head->next;
        // }
        n->next = head;
        table[w_hash] = n;

        // 7 set head to the new node
        dictionnary_size++;
    }
    free(entry);
    fclose(input);
    // free(input)
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return dictionnary_size;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int bucket_index = 0; bucket_index < N; bucket_index++)
    {
        free_node(table[bucket_index]);
    }

    return true;
}

void free_node(node *n)
{
    if (n->next != NULL)
    {
        free_node(n->next);
    }
    free(n);
}
