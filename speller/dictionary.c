// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <strings.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// number of buckets in hash table based on word length
const unsigned int N = LENGTH;

// global variable that increments the number of words in dictionary
unsigned int numOfWords;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    node *n = table[hash(word)]; //our starting point for where the word might be
    if (n == NULL)
    {
        return false; //if n is null somehow, return false
    }
    bool returnResult = false; //initialize return as false
    while (n != NULL) //loop that goes forever until n points to null
    {
        if (strcasecmp(n->word, word) == 0) //if the word is found, result becomes true and we break
        {
            returnResult = true;
            break;
        }
        n = n->next; //go to the next node
    }
    return returnResult;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    return (strlen(word) - 1); //this hash function sorts words into buckets based on word length
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary) //if argc(3), take argv[1]; else, do DICTIONARY
{
    numOfWords = 0;
    FILE *infile = fopen(dictionary, "r");
    bool loadResult = true; //assumes true but can be proven false through other events
    if (infile == NULL)
    {
        loadResult = false; //doesn't do anything, so loadResult will be false
    }
    else
    {
        int fscanfReturn = 0;
        char word[LENGTH + 1]; //same as node word
        while (fscanfReturn != EOF)
        {
            fscanfReturn = fscanf(infile, "%s", word);
            node *n = malloc(sizeof(node));
            if (n == NULL)
            {
                loadResult = false;
                break; //break out of the while loop and return false
            }
            else
            {
                strcpy(n->word, word); //copies the word into n's node
                node *head = table[hash(word)]; //the node of our head
                n->next = head;
                table[hash(word)] = n; //makes n the first part of linked list without breaking order
                numOfWords++;
            }

        }
    }
    fclose(infile);

    return loadResult;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return (numOfWords - 1);
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    node *n = 0;
    for (int i = 0; i < N; i++) //iterates through node table
    {
        n = table[i]; //gets our linked list at X place
        node *tmp = 0; //gives us a temporary variable
        while (n !=  NULL)
        {
            tmp = n;
            n = n->next; //go to the next node with n
            free(tmp); //frees our former node;
        }
    }
    return true;
}
