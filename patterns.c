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


//creation of suffix tree with weights optimized with Ukkonen's algorithm
SuffixTree *createSuffixTree(int *buffer, int bufferLen)
{
    //TODO: THIS IS TOTALY BROKEN, REWRITE EVERYTHING PLS
    //starting values of variables
    int remaining = 0;
    SuffixTree *root = (SuffixTree *)malloc(sizeof(SuffixTree));
    SuffixTree *activeNode = root;
    int activeEdge = 0;
    int activeLength = 0;
    int *end = (int *)malloc(sizeof(int));
    end = -1;


    int rootHasThisPath;
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
                rootHasThisPath = 0;
                SuffixTree *rootChildIter = root->childList;
                while (rootChildIter != NULL)
                {
                    if(buffer[rootChildIter->weight->indexs[0]] == buffer[i])
                    {
                        rootHasThisPath = 1;
                        break;
                    }
                    rootChildIter = rootChildIter->next;
                }
                
                if(!rootHasThisPath)
                {
                    //if root doesnt have a child with this value we create one
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

                    remaining--;
                    break;
                }

                //if it does have that child
                activeEdge = rootChildIter->weight->indexs[0];
                activeLength++;
                break;
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