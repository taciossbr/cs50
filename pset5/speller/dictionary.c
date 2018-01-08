/**
 * Implements a dictionary's functionality.
 */
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dictionary.h"

node root = {false,{NULL}};
int count = 0;

/**
 * Returns true if word is in dictionary else false.
 */
void insert(char *, node*);
unsigned int sizeRecur(node *);
void unloadRecur(node *no);
void strlwr(char *);

int charNumber(char c)
{
    int num;
    if (c == '\'')
        return 26;
    else if(c >= 'A' && c <= 'Z')
        c += 32;
    num = c - 'a';
    return num;
}
bool check(const char *word)
{
    node *no = &root;
    char word1[70];
    strcpy(word1, word);
    strlwr(word1);
    for (int i = 0, n = strlen(word); i < n; i++)
    {
        int k = 0;

        if (word1[i] == '\'')
        {
            k = 26;
        }
        else
        {
            k = word1[i] - 'a';
        }

        if (no->children[k] == NULL)
        {
            return false;
        }
        no = no->children[k];
    }
    if (!no->is_word)
    {
        return false;
    }
    return true;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        return false;
    }


    while (!feof(file))
    {
        count++;
        node *ptr = &root;
        char word[LENGTH + 1] = {};
        fscanf(file, "%s\n", word);
        for (int i=0;i<strlen(word);i++)
        {
            if (ptr->children[charNumber(word[i])] == NULL)  // if it's NULL, malloc a new node,point to it.
            {
                node *new = malloc(sizeof(node));
                *new = (node) {false,{NULL}};       // initiallization
                ptr->children[charNumber(word[i])] = new;
                ptr = new;
            }
            else
            {
                ptr = ptr->children[charNumber(word[i])];  // if it's not, just point to next node.
            }
        }
        ptr->is_word = true;
    }
    fclose(file);
    return true;

}


unsigned int size(void)
{
    return count;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
 void freeNode(node *currentNode)
{
    for (int i=0;i<27;i++)
    {
        if (currentNode->children[i] != NULL)   // free node recursively, the method is elegant!
            freeNode(currentNode->children[i]); // first free all the children,then free itself.
    }
    free(currentNode);
}
bool unload(void)
{
    for (int i = 0; i < 27; i++)
    {
        if (root.children[i] != NULL)
        {
            freeNode(root.children[i]);
        }
    }
    return true;
}

void unloadRecur(node *no)
{
    for(int i = 0; i < 27; i++)
    {
        if (no->children[i] != NULL)
        {
            unloadRecur(no->children[i]);
            //free(no->children[i]);
        }
    }
    free(no);
}

void strlwr(char *word)
{
    for (int i = 0, n = strlen(word); i < n; i++)
    {
        word[i] = tolower(word[i]);
    }
}