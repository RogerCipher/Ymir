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
#if 0
int heuristicResult(SuffixTree *node)
{
    int weight = 1;

    //calculate how many times this pattern repeats
    SuffixRange *iter = node->rangeList;
    while (iter->next != NULL)
    {
        weight++;
        iter = iter->next;
    }
    
    //calculate patterns length
    int length = iter->range[1] - iter->range[0];

    return weight * length;
}
#endif
//creates suffix tree and returns its root
InternalNode *createSuffixTree(int *buffer, int bufferLen)
{

    InternalNode *root = (InternalNode *)malloc(sizeof(InternalNode));

    //auxiliar variables to help us build tree
    int remaining = 0;
    InternalNode *activeNode = root;
    int activeEdge = -1;
    int activeLength = 0;
    int *end = (int *)malloc(sizeof(int));
    end = 0;


    end = 1;
    InternalNode *lastCreatedNode;
    InternalNode *iterInternalNode;
    RangeNode *iterRangeNode;

    //Ukkonens algorithm
    for(int i = 0; i < bufferLen; i++)
    {
        lastCreatedNode = root;
        remaining++;
        end++;

        while(remaining > 0)
        {
            if(!activeLength) //activeLength == 0
            {
                //check if root has this char
                iterRangeNode = root->pathList;
                while (iterRangeNode != NULL)
                {
                    if(buffer[iterRangeNode->firstRange->range[0]] == buffer[i])
                    {
                        //root has this path
                        //optional: add here new Range
                        activeEdge = iterRangeNode->firstRange->range[0];
                        activeLength++;
                        break;
                    }
                    
                    iterRangeNode = iterRangeNode->sibling;
                }
                //root doesnt have this char, we need to create it
                RangeLL *newRangeLL = (RangeLL *)malloc(sizeof(RangeLL));
                newRangeLL->next = NULL;
                newRangeLL->range[0] = i;
                newRangeLL->range[1] = end; //TODO: HOW DO I DO THIS???
                RangeNode *newRangeNode = (RangeNode *)malloc(sizeof(RangeNode));
                newRangeNode->firstRange = newRangeLL;
                newRangeNode->sibling = NULL;
                newRangeNode->rangeCount = 1;
                newRangeNode->nextInternalNode = NULL;
                remaining--;
            }
            else
            {
                //check if we jumped an internal node before we do this, if we did:
                if(activeLength = activeNode->pathList->firstRange->range[1] - activeNode->pathList->firstRange->range[1])
                {
                    //we need to go to the next internal node
                    //change the active node to this internalNode we jumped, 
                    activeNode = activeNode->pathList->nextInternalNode;
                    //the active edge becomes the range[0] of this internalNode
                    iterRangeNode = activeNode->pathList->firstRange;
                    while (iterRangeNode != NULL)
                    {
                        if(buffer[iterRangeNode->firstRange->range[0]] == buffer[i])
                        {
                            //root has this path
                            //optional: add here new Range
                            activeEdge = iterRangeNode->firstRange->range[0];
                            activeLength = 1;
                            break;
                        }
                        
                        iterRangeNode = iterRangeNode->sibling;
                    }
                    //if we didnt find it we have to create it

                    
                }
                
                

                //else:
                if(buffer[activeNode->pathList->firstRange->range[0]+activeLength] == buffer[i])
                {
                    activeLength++;
                    break;
                }
                else
                {
                    //split this node on "i"
                    //point this new internalNode suffix link to "lastCreated Node"
                    //this newly created node is now "lastCreatedNode"
                    remaining--;
                    if(activeNode == root)
                    {
                        activeLength--;
                        activeEdge++;
                    }
                    else
                    {

                    }
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