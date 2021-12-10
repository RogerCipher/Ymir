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

#include "huffman.h"
#include "patterns.h"


//insert a new charBlock in the (weighted) charblock list for huffman encoding
int insertInWeightedList(PatternCharBlock **listHead, PatternCharBlock *block)
{

    if (*listHead == NULL) {    //list is empty
        *listHead = block;
        block->next = NULL;
        return 1;
    }

    if((*listHead)->weight > block->weight) //this block is a better head
    {
        block->next = *listHead;
        *listHead = block;
        return 1;
    }
    

    PatternCharBlock *iter = *listHead;
    while (iter->next != NULL)        
    {
        //printf("its this?");
        if(block->weight < iter->next->weight)    //this block fits here
        {
            block->next = iter->next;
            iter->next = block;
            return 1;
        }

        iter = iter->next;
    }

    iter->next = block;          //this block goes in the end
    block->next = NULL;
    return 0;
}

//recieves a linked list of patterns and returns the root to a huffman tree based on those patterns
PatternCharBlock *generateHuffmanTree(PatternCharBlock *patterns)
{
    PatternCharBlock *root = NULL;
    PatternCharBlock *iter = patterns;
    PatternCharBlock *next = NULL;

    //insert everything into a weighted list
    while (iter != NULL)
    {
        next = iter->next;
        insertInWeightedList(&root, iter);
        iter = next;
    }
    
    //start generating actual tree:
    PatternCharBlock *left = NULL;
    PatternCharBlock *right = NULL;
    
    //we will do this untill the list only has one element
    while(root->next != NULL)
    {
        left = root;
        right = root->next;
        
        root = root->next->next;

        //create parent
        PatternCharBlock *parent = (PatternCharBlock *)malloc(sizeof(PatternCharBlock));
        parent->leftChild = left;
        parent->rightChild = right;
        parent->weight = left->weight + right->weight;
        parent->pattern = NULL;
        parent->len = 0;
        parent->next = NULL;
        insertInWeightedList(&root, parent);
    }

    return root;
}



void printHuffTree(PatternCharBlock *node)
{
    if (node->pattern == NULL) 
    {
        //no value, it must have children
        printf("\tn%p [label = \"N/A : %d\"]\n", node, node->weight);
    } 
    else 
    {
        //it has value, cant have children
        printf("\tn%p [label = \"", node);
        printCharPattern(node->pattern);
        printf("\"]\n");
        return;
    }

    //conections with children
    printf("\tn%p -- n%p\n", node, node->leftChild);
    printHuffTree(node->leftChild);
    printf("\tn%p -- n%p\n", node, node->rightChild);
    printHuffTree(node->rightChild);
}


int loadFileInBlocks(char *fileName)
{

    FILE *file = fopen(fileName, "r");
    if(file == NULL)
    {
        printf("Couldnt find file\n");
        exit(1);
    }

    int currentChar = fgetc(file);
    if(currentChar == EOF)
    {
        printf("File was empty\n");
        exit(1);
    }

    int len = 0;
    //int allBlocksLoaded = 0;
    int blockBuffer[MAXCHARBUFFER];

    while(currentChar > 0)
    {
        for(len = 0; len < MAXCHARBUFFER; len++)
        {

            blockBuffer[len] = currentChar;

            currentChar = fgetc(file);
            if(currentChar < 0)
                break;
        }
        
        printf("\n\n---------block patterns--------------\n\n");
        PatternCharBlock *listOfPatterns = bestPatternBlocks(blockBuffer, len+1);
        PatternCharBlock *huffRoot = generateHuffmanTree(listOfPatterns);
        printf("\n\n-------------------------------------\n\n");

        printf("Copy the following code to https://dreampuf.github.io/GraphvizOnline\n");
        printf("graph {\n");
        printHuffTree(huffRoot);
        printf("}\n");


        if(currentChar < 0)
        {
            break;
        }
        currentChar = fgetc(file);
            
    }


    fclose(file);

    return 1;
}




int main(int argc, char *argv[])
{

    loadFileInBlocks(argv[1]);

    /*
    TipoNode *cabeca = NULL;
    createWeightedList(argv[1], &cabeca);
    printLL(cabeca);
    TipoNode *raiz = huffmanTree(&cabeca);

    printf("---------\n");
    printHuffTree(raiz);

    printf("\nx\n");
    */
    return 0;
}