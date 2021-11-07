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

//this function returns the result of the heuristic of a suffix in our suffix tree
int heuristicResult(SuffixTree *node)
{
    int weight = 1;

    //calculate how many times this pattern repeats
    SuffixWeight *iter = node->weight;
    while (iter->next != NULL)
    {
        weight++;
        iter = iter->next;
    }
    
    //calculate patterns length
    int length = iter->indexs[1] - iter->indexs[0];

    return weight * length;
}

//returns 0 if there is a child node with this value, 1 otherwize 
int doesNodeHaveThisChar(SuffixTree *node, int charValue, int *buffer)
{
    SuffixTree *nodeChildIter = node->childList;
    while (nodeChildIter != NULL)
    {
        if(buffer[nodeChildIter->weight->indexs[0]] == charValue)
        {
            return 1;
        }
        nodeChildIter = nodeChildIter->next;
    }
    return 0;
}

//creation of suffix tree with weights optimized with Ukkonen's algorithm
SuffixTree *createSuffixTree(int *buffer, int bufferLen)
{
    //starting values of variables
    int remaining = 0;
    SuffixTree *root = (SuffixTree *)malloc(sizeof(SuffixTree));
    SuffixTree *activeNode = root;
    int activeEdge = 0;
    int activeLength = 0;
    int *end = (int *)malloc(sizeof(int));
    end = -1;


    
    //Ukkonen's algorithm implementation
    for(int i = 0;  i < bufferLen; i++)
    {
        remaining++;
        end++;
        while(remaining > 0)
        {
            if(activeLength == 0)
            {
                //check if root has a path to this character
                if(!doesNodeHaveThisChar(activeNode, buffer[i], buffer))
                {
                    //if root doesnt have a child with this value we create one

                    #if 0
                    SuffixTree *newNode = (SuffixTree *)malloc(sizeof(SuffixTree));
                    newNode->next = NULL;
                    newNode->childList = NULL;
                    newNode->weight->next = NULL;
                    newNode->weight->indexs[0] = i;
                    newNode->weight->indexs[1] = end;

                    if(root->childList == NULL)
                    //first child
                        root->childList = newNode;
                    else
                    {
                        //not the first child

                        //go to the end of root child list
                        SuffixTree *iter = root->childList;
                        while (iter->next != NULL)
                            iter = iter->next;
                        //place new node on the end of the list
                        iter->next = newNode;
                    }
                    #endif

                    remaining--;
                    break;
                }

                //if it does have that child
                activeEdge = rootChildIter->weight->indexs[0];
                activeLength++;
                break;
            }
            else
            {
                if(buffer[rootChildIter->weight->indexs[0] + activeLength]) != buffer[i]
                {
                    splitThisWithInternalNode;
                    pointThisInternalNodeToRoot
                    if(activeNode == root)
                    {
                        activeLength--;
                        remaining--
                    }
                    else
                    {
                        activeLength++ acho eu
                    }
                }
                else
                {
                    activeLength++;
                    break;
                }
            }





            
        }
    }


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
    
    return 1;
}