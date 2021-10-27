/*
--------------------------------------------------------
by: Rogério Chaves (AKA CandyCrayon), 2021
//    __                       __                       
//   /  )             /       /  )                      
//  /   __.  ____  __/ __  , /   __  __.  __  , __ ____ 
// (__/(_/|_/ / <_(_/_/ (_/_(__// (_(_/|_/ (_/_(_)/ / <_
//                       /                  /           
//                      '                  '            
--------------------------------------------------------
*/
#include <stdio.h>
#include <stdlib.h>
#include "patterns.h"


TrieNode *addWeightToChild(char childValue, TrieNode *parent)
{
    //first child
    if(parent->firstChild == NULL)
    {
        TrieNode *child = (TrieNode *)malloc(sizeof(TrieNode));
        child->parent = parent;
        child->value = childValue;
        child->weight = 1;
        child->leftBrother = NULL;
        child->rightBrother = NULL;
        child->firstChild = NULL;
        parent->firstChild = child;
        return child;
    }

    TrieNode *iter = parent->firstChild;
    while (iter != NULL)
    {
        //theres a child with that char
        if(iter->value == childValue)
        {
            iter->weight++;
            return iter;
        }
        iter->rightBrother;
    }

    //there isnt a child with that value yet
    iter = parent->firstChild;
    while (iter->rightBrother != NULL)
        iter->rightBrother;


    TrieNode *child = (TrieNode *)malloc(sizeof(TrieNode));
    child->parent = parent;
    child->value = childValue;
    child->weight = 1;
    child->leftBrother = iter;
    child->rightBrother = NULL;
    child->firstChild = NULL;
    iter->rightBrother = child;
    return child;
    
    
}



int addCharToTrie(char c, TrieTree **head)
{
    if(*head == NULL)
    {
        TrieTree *head = (TrieTree *)malloc(sizeof(TrieTree));
        head->root = addWeightToChild(c, (TrieNode *)malloc(sizeof(TrieNode)));
        //TODO: YOU ARE HERE 
    }



    TrieTree *iter = *head;
    int charHasTrie = 0;
    //see if we already have that trie
    while(iter != NULL)
    {
        if(c == iter->root->value)
        {
            charHasTrie = 1;
            if(iter->currentInsertNode == iter->root)
            {
                iter->root->weight++;
                iter->currentInsertNode = addWeightToChild(c, iter->currentInsertNode);
            }
            else
            {
                iter->currentInsertNode = iter->root;
                iter->root->weight++;
            }
        }
        else
        {
            iter->currentInsertNode = addWeightToChild(c, iter->currentInsertNode);
        }
        iter = iter->next;
    }

    if(!charHasTrie)
    {
        TrieTree *newTrie = (TrieTree *)malloc(sizeof(TrieTree));
        iter = *head;
        //TODO: FINISH THIS FUNC
        
    }
    return 1;
}


int loadCharacters(char *fileName)
{
    /*---error handling---*/
    FILE *file = fopen(fileName, "r");
    if(file == NULL)
    {
        printf("Couldnt find file\n");
        exit(1);
    }

    char currentChar = fgetc(file);
    if(currentChar == EOF)
    {
        printf("File was empty\n");
        exit(1);
    }

    /*---char loading---*/

    //a character can be any ascii char,
    //aka if we convert it to int it will be from 0 to 255
    //so lets make a buffer for storing the weight of each char in the file:
    int weights[256];

    for(int i = 0; i < 256; i++)
    //inicialize all weights at 0
        weights[i] = 0;

    while (currentChar != EOF)
    {
        weights[(int)currentChar]++;
        currentChar = fgetc(file);
    }
    fclose(file);

}

int main(int argc, char *argv[])
{
    TrieTree *head = (TrieTree *)malloc(sizeof(TrieTree));

}