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
#include <string.h>
#include "patterns.h"

//simple error throwing function
void error(const char *msg) 
{
    perror(msg);
    exit(1);
}

//this function returns the result of the heuristic of a node in the trie
int heuristicResult(TrieNode *node)
{   
    if(node->weight == 1)
        return 0;
    return node->weight * node->depth;
}

TrieNode *bestNodeInTrie(TrieNode *node)
{
    TrieNode *bestNode = node;
    TrieNode *possibleBetterNode = NULL;

    TrieNode *iter = NULL;
    if(node->childList != NULL)
    {
        iter = node->childList;
        while(iter != NULL)
        {
            possibleBetterNode = bestNodeInTrie(iter);
            if(heuristicResult(possibleBetterNode) > heuristicResult(bestNode))
                bestNode = possibleBetterNode;

            iter = iter->next;
        }   
    }

    return bestNode;
}

TrieNode *bestNodeInForest(TrieManager *manager)
{
    TrieNode *iter = NULL;


    //first reset all tries:
    for(int i = 0; i < MAXUNIQUECHARS; i++)
    {
        //nothing there
        if(manager->trie[i] == NULL)
            continue;

        //reset this trie
        iter = manager->trie[i];
        while (iter->parent != NULL)
            iter = iter->parent;

        manager->trie[i] = iter;
    }

    TrieNode *bestNode = NULL;
    //set the best node to the first node we see
    for(int i = 0; i < MAXUNIQUECHARS; i++)
    {
        //nothing there
        if(manager->trie[i] == NULL)
            continue;

        bestNode = bestNodeInTrie(manager->trie[i]);
    }

    TrieNode *possibleBetterNode = NULL;
    for(int i = 0; i < MAXUNIQUECHARS; i++)
    {
        //nothing there
        if(manager->trie[i] == NULL)
            continue;

        possibleBetterNode = bestNodeInTrie(manager->trie[i]);
        if(heuristicResult(bestNode) < heuristicResult(possibleBetterNode))
            bestNode = possibleBetterNode;
    }
    
    return bestNode;
}

TrieNode *makeRoot(int value)
{
    //there's no child with this value, so lets create it
    TrieNode *newRoot = (TrieNode *)malloc(sizeof(TrieNode));
    //error handling
    if(newRoot == NULL)
        error("couldnt alocate memory\n");

    newRoot->parent = NULL;
    newRoot->depth = 0;
    newRoot->childList = NULL;
    newRoot->next = NULL;
    newRoot->value = value;
    newRoot->weight = 1;

    return newRoot;
}

TrieNode *addChild(TrieNode *parent, int value)
{
    //first lets check if there is a child with this value already:
    TrieNode *iter = parent->childList;
    while (iter != NULL)
    {
        if(iter->value == value)
        {
            //there's already a child with this value
            iter->weight++;
            return iter;
        }
        iter = iter->next;
    }
    
    //there's no child with this value, so lets create it
    TrieNode *newChild = (TrieNode *)malloc(sizeof(TrieNode));
    //error handling
    if(newChild == NULL)
        error("couldnt alocate memory\n");

    newChild->parent = parent;
    newChild->depth = parent->depth+1;
    newChild->childList = NULL;
    newChild->next = NULL;
    newChild->value = value;
    newChild->weight = 1;

    //if its the first child we just put it in:
    if(parent->childList == NULL)
        parent->childList = newChild;
    else
    {
        //not the first child,
        //walk to the end of the list:
        TrieNode *iter = parent->childList;
        while (iter->next != NULL)
            iter = iter->next;

        //add it to the end of the list:
        iter->next = newChild;
    }
        return newChild;
}

TrieManager *createPatternTrie(int *buffer, int bufferLen)
{
    TrieManager *manager = (TrieManager *)malloc(sizeof(TrieManager));
    //initialization
    for(int i = 0; i < MAXUNIQUECHARS; i++)
        manager->trie[i] = NULL;

    TrieNode *iter = NULL;


    for(int i = 0; i < bufferLen; i++)
    {
        //if there's no pattern starting with this value we create it:
        if(manager->trie[buffer[i]] == NULL)
            manager->trie[buffer[i]] = makeRoot(buffer[i]);
        else
        {
            //there's a pattern starting with this value, so lets:
            //navigate untill the root:
            iter = manager->trie[buffer[i]];
            while (iter->parent != NULL)
                iter = iter->parent;

            //increment its weight
            iter->weight++;
            
            //finally set it as current node for this trie
            manager->trie[buffer[i]] = iter;
        }

        //now lets check all other tries:
        for(int k = 0; k < MAXUNIQUECHARS; k++)
        {
            //if we are in the trie starting by this value we just skip it
            if(k == buffer[i])
                continue;

            //if there's already a trie there we add this new child:
            if(manager->trie[k] != NULL)
                manager->trie[k] = addChild(manager->trie[k], buffer[i]);
        }

    }

    return manager;
}

/*
void deleteOneWeight(TrieManager *manager)
{

    TrieNode *iter = NULL;
    //first reset all tries:
    for(int i = 0; i < MAXUNIQUECHARS; i++)
    {
        //nothing there
        if(manager->trie[i] == NULL)
            continue;

        //reset this trie
        iter = manager->trie[i];
        while (iter->parent != NULL)
            iter = iter->parent;

        manager->trie[i] = iter;
    }

    //then cut down everything after
    for(int i = 0; i < MAXUNIQUECHARS; i++)
    {
        //nothing there
        if(manager->trie[i] == NULL)
            continue;

        if()
        while (iter->parent != NULL)
            iter = iter->parent;

        manager->trie[i] = iter;
    }
}
*/


//simple print for graphviz
void printTrie(TrieNode *node)
{
    /*
    if (elemento->suffixLink == NULL) 
    {
        //estamos na raiz
        printf("Copy the following code to https://dreampuf.github.io/GraphvizOnline\n");
        printf("graph {\n");
    }
    */
    if (node->parent != NULL)
        printf("\tn%p -- n%p\n", node, node->parent);

    TrieNode *iter = node->childList;
    printf("\tn%p [label = \"val: %c, weig: %d\"]\n", node, node->value, node->weight);
    
    while (iter != NULL)
    {
        printTrie(iter);

        iter = iter->next;
    }
}

void printAllTries(TrieManager *manager)
{
    TrieNode *iter = NULL;


    //first reset all tries:
    for(int i = 0; i < MAXUNIQUECHARS; i++)
    {
        //nothing there
        if(manager->trie[i] == NULL)
            continue;

        //reset this trie
        iter = manager->trie[i];
        while (iter->parent != NULL)
            iter = iter->parent;

        manager->trie[i] = iter;
    }
    printf("Copy the following code to https://dreampuf.github.io/GraphvizOnline\n");
    printf("graph {\n");


    printf("\to726f676572 [label = \"MANAGER\"]\n");
    //print them
    for(int i = 0; i < MAXUNIQUECHARS; i++)
    {
        if(manager->trie[i] == NULL)
            continue;
        
        
        printTrie(manager->trie[i]);
        printf("\tn%p -- o726f676572\n", manager->trie[i]);
    }

    printf("\tbestNode [label = \"BEST NODE I FOUND CHIEF\"]\n");
    printf("\tn%p -- bestNode\n", bestNodeInForest(manager));

    printf("}\n");
}


void freeTrie(TrieNode *node)
{
    if(node->childList != NULL)
    {
        TrieNode *iter = node->childList;
        TrieNode *next = iter->next;
        while (iter != NULL)
        {
            if(iter->childList != NULL)
                freeTrie(iter);
            else
                free(iter);

            iter = NULL;
            iter = next;
        }
    }
    free(node);
}

void freeAllTries(TrieManager *manager)
{
    TrieNode *iter = NULL;

    //first reset all tries:
    for(int i = 0; i < MAXUNIQUECHARS; i++)
    {
        //nothing there
        if(manager->trie[i] == NULL)
            continue;

        //reset this trie
        iter = manager->trie[i];
        while (iter->parent != NULL)
            iter = iter->parent;

        manager->trie[i] = iter;
    }

    //now free
    for(int i = 0; i < MAXUNIQUECHARS; i++)
    {
        //nothing there
        if(manager->trie[i] == NULL)
            continue;

        freeTrie(manager->trie[i]);
        manager->trie[i] = NULL;
    }
    free(manager);
}

/*--------------- main -------------*/
int main(int argc, char *argv[])
{
    #if 0
    printf("Loading characters from file..\n");
    FilePatterns *patterns = loadCharactersInBlocks(argv[1]);
    printf("---------\n");

    //printf("Printing Tries..\n");
    //printTriesGrapviz(&head);

    printf("---------\n");
    /*
    printf("making best block..\n");
    CharBlock *block = bestCharBlock(head);
    */
    printf("lets print it..\n");
    printFilePatterns(patterns);
    #endif
    
    //int testBuffer[15] = {(int)'x', (int)'y', (int)'z', (int)'x', (int)'y', (int)'a', (int)'x', (int)'y', (int)'z', (int)'x', (int)'y', (int)'z', (int)'x', (int)'y', (int)'z'};
    int testBuffer[8] = {(int)'m', (int)'i', (int)'s', (int)'s', (int)'i', (int)'s', (int)'s', (int)'i'};
    
    
    TrieManager *man = createPatternTrie(testBuffer, 8);
    printAllTries(man);
    /*
    
    InternalNode *root = createWeightedSuffixTree(testBuffer, 10);

    fillRepetitionsInTree(root);


    printSuffTree(root);

    */

    return 1;
}