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
    return node->weight * node->depth*2;
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
    //first reset all tries:
    resetTries(manager, 0);

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
    newRoot->locked = 0;

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
        //we are in a deleted value, we either reset or skip
        if(buffer[i] == DELETEDCHARVALUE)
        {
            resetTries(manager, 1);
            continue;
        }




        //if there's no pattern starting with this value we create it:
        if(manager->trie[buffer[i]] == NULL)
            manager->trie[buffer[i]] = makeRoot(buffer[i]);
        else
        {
            //there's a pattern starting with this value, so lets:
            //navigate untill the root:
            iter = manager->trie[buffer[i]];
            if(iter->parent == NULL)
            {
                //we are already in the root, so:
                if(!iter->locked)
                    manager->trie[buffer[i]] = addChild(iter, buffer[i]);

                iter->weight++;
            }
            else
            {
                while (iter->parent != NULL)
                    iter = iter->parent;

                if(iter->locked == 1)
                    iter->locked = 0;
                
                iter->weight++;
                
                //finally set it as current node for this trie
                manager->trie[buffer[i]] = iter;
            }



        }

        //now lets check all other tries:
        for(int k = 0; k < MAXUNIQUECHARS; k++)
        {
            //if we are in the trie starting by this value we just skip it
            if(k == buffer[i])
                continue;

            //if there's already a trie there we add this new child:
            if(manager->trie[k] != NULL)
            {
                if(!manager->trie[k]->locked)
                    manager->trie[k] = addChild(manager->trie[k], buffer[i]);

            }
        }
    }

    deleteOneWeight(manager);
    return manager;
}

PatternCharBlock *calculateBestPatternChar(TrieManager *manager)
{
    TrieNode *bestNode = bestNodeInForest(manager);
    TrieNode *iter = NULL;
    //build the pattern from the best node to root
    PatternChar *lastChar = (PatternChar *)malloc(sizeof(PatternChar));
    lastChar->next = NULL;
    lastChar->value = bestNode->value;

    iter = bestNode->parent;
    while (iter != NULL)
    {
        PatternChar *prevChar = (PatternChar *)malloc(sizeof(PatternChar));
        prevChar->next = lastChar;
        prevChar->value = iter->value;
        lastChar = prevChar;

        iter = iter->parent;
    }

    //create the block and populate it
    PatternCharBlock *block = (PatternCharBlock *)malloc(sizeof(PatternCharBlock));
    block->weight = bestNode->weight;
    block->next = NULL;
    block->len  = bestNode->depth +1;
    block->pattern = lastChar;

    return block;;
}

void deleteChars(int *buffer, int startPos, int endPos)
{
    for(int i = startPos; i < endPos +1; i++)
    {
        buffer[i] = DELETEDCHARVALUE;
    }
    return;
}

//terribly ineficcient way to remove patterns from a buffer, maybe see KMP algorithm
void removePattern(int *buffer, int bufferLen, PatternCharBlock *patternBlock)
{
    int patternLen = patternBlock->len;
    int remainingPatternsInBuffer = patternBlock->weight;
    PatternChar *pattern = patternBlock->pattern;
    int isPatternHere = 0;
    int startPos = -1;

    while(remainingPatternsInBuffer > 0)
    {
        startPos++;

        for(int i = startPos; i < bufferLen; i++)
        {

            if(buffer[i] != pattern->value)
                break;

            isPatternHere = 1;
            PatternChar *iter = pattern;
            for(int k = 0; k < patternLen; k++)
            {
                isPatternHere = 1;
                if(buffer[i + k] != iter->value)
                {
                    isPatternHere = 0;
                    break;
                }
                iter = iter->next;
            }

            if(isPatternHere == 0)
                break;

            deleteChars(buffer, startPos, startPos + patternLen -1);
            remainingPatternsInBuffer--;
            startPos += patternLen -1;
            break;
        }
    }
}


//delete a child
void deleteNode(TrieNode *parent, TrieNode *child)
{
    //if its a root
    if(parent == NULL)
    {
        freeTrie(child);
        return;
    } 
    //if its not root and its the first child
    if(parent->childList == child)
    {
        //first child:
        parent->childList = child->next;
        freeTrie(child);
        return;
    }

    //its not the first child, find it:
    TrieNode *iter = parent->childList;
    while (iter->next != child)  
        iter = iter->next;

    iter->next = iter->next->next;
    freeTrie(child);
    return;
}

//recursivelly search for nodes with weight = 1 and delete them
void cutDownTrie(TrieNode *node)
{
    TrieNode *iter = node->childList;
    TrieNode *next = NULL;
    while (iter != NULL)
    {
        next = iter->next;
        if(iter->weight == 1)
            deleteNode(node, iter);
        else
            cutDownTrie(iter);
        iter = next;
    }
    return;
}

void deleteOneWeight(TrieManager *manager)
{
    //first reset all tries:
    resetTries(manager, 0);

    //then cut down everything after
    for(int i = 0; i < MAXUNIQUECHARS; i++)
    {
        //nothing there
        if(manager->trie[i] == NULL)
            continue;

        cutDownTrie(manager->trie[i]);
    }
}


int isBufferEmpty(int *buffer, int bufferLen)
{
    for(int i = 0; i < bufferLen; i++)
    {
        if(buffer[i] != DELETEDCHARVALUE)
            return 0;
    }

    return 1;
}


PatternCharBlock *bestPatternBlocks(int *buffer, int bufferLen) 
{
    //FIXME: THIS IS BUGGED STILL IM PRETTY SURE
    PatternCharBlock *bestPatterns = NULL;
    PatternCharBlock *iter = NULL;

    while (!isBufferEmpty(buffer, bufferLen))
    {
        TrieManager *man = createPatternTrie(buffer, bufferLen);
        /*
        printf("\n---------------------------------------\n");
        printAllTries(man);
        printf("\n---------------------------------------\n");
        */
        PatternCharBlock *currentBestPattern = calculateBestPatternChar(man);
        //freeAllTries(man); //TODO: THIS IS GIVING ME FASTTOP (double free)
        //check if it is the first patter
        if(bestPatterns == NULL)
            bestPatterns = currentBestPattern;
        else
        {
            iter = bestPatterns;
            while (iter->next != NULL)
                iter = iter->next;
            
            iter->next = currentBestPattern;
        }
        removePattern(buffer, bufferLen, currentBestPattern);
    }


    printCharBlock(bestPatterns);
    return bestPatterns;
    
}

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
    printf("\tn%p [label = \"val: %c, weig: %d, dept: %d\"]\n", node, node->value, node->weight, node->depth);
    
    while (iter != NULL)
    {
        printTrie(iter);

        iter = iter->next;
    }
}

void printAllTries(TrieManager *manager)
{
    //first reset all tries:
    resetTries(manager, 0);
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

void printCharPattern(PatternChar *pattern)
{
    PatternChar *iter = pattern;
    while (iter != NULL)
    {
        if(iter->value > 32 && iter->value < 127)
            printf("%c", iter->value);
        else
            printf("%d", iter->value);
        iter = iter->next;
    }
    
}

void printCharBlock(PatternCharBlock *block)
{
    PatternChar *iter = block->pattern;
    printf("\npattern weight: %d", block->weight);
    printf("\n---------------\n");
    while (iter != NULL)
    {
        printf("%c", iter->value);

        iter = iter->next;
    }
    printf("\n---------------");
    if(block->next != NULL)
        printCharBlock(block->next);
}

void resetTries(TrieManager *manager, int lock)
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

        if(lock)
            manager->trie[i]->locked = 1;
    }
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
    node = NULL;
}

void freeAllTries(TrieManager *manager)
{
    //first reset all tries:
    resetTries(manager, 0);

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

#if 0
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
    
    int testBuffer[15] = {(int)'x', (int)'y', (int)'z', (int)'x', (int)'y', (int)'a', (int)'x', (int)'y', (int)'z', (int)'x', (int)'y', (int)'z', (int)'x', (int)'y', (int)'z'};
    //int testBuffer[8] = {(int)'m', (int)'i', (int)'s', (int)'s', (int)'i', (int)'s', (int)'s', (int)'i'};
    
    printCharBlock(bestPatternBlocks(testBuffer, 15));
    
    /*
    TrieManager *man = createPatternTrie(testBuffer, 15);
    deleteOneWeight(man);
    printAllTries(man);

    PatternCharBlock *bestPattern = calculateBestPatternChar(man);
    printCharBlock(bestPattern);

    removePattern(testBuffer, 15, bestPattern);


    printf("\n---------\n");
    //now lets just print the buffer;
    for(int i = 0; i < 15; i++)
    {
        printf("%d ", testBuffer[i]);
    }
    */
    
    /*
    
    InternalNode *root = createWeightedSuffixTree(testBuffer, 10);

    fillRepetitionsInTree(root);


    printSuffTree(root);

    */

    return 1;
}
#endif