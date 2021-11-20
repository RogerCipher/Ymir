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





//Ukkonens algorithm
InternalNode *createWeightedSuffixTree(int *buffer, int bufferLen)
{
    InternalNode *root = (InternalNode *)malloc(sizeof(InternalNode));
    int remaining = 0;
    InternalNode *activeNode = root;
    int activeEdge = -1;
    int activeLength = 0;
    int *end = (int *)malloc(sizeof(int));
    *end = -1;

    int found = 0;
    InternalNode *prevCreatedNode;
    RangeList *iterRangeList;
    //InternalNode *iterInternalNode;
    RangeNode *iterRangeNode;
    for(int i = 0; i < bufferLen; i++)
    {
        prevCreatedNode = root;
        remaining++;
        (*end)++;
        while(remaining > 0)
        {
            if(activeLength == 0)
            {
                //we are on top of a node, lets see if he has any ranges
                //attached to it that correspond to the current value we want
                iterRangeNode = activeNode->pathList;
                found = 0;
                while(iterRangeNode != NULL)
                {
                    if(buffer[iterRangeNode->ranges->rangeStart] == buffer[i])
                    {
                        //found it, lets increase our active Length
                        activeLength++;
                        activeEdge = iterRangeNode->ranges->rangeStart;

                        //and lets add this range to the range node list
                        RangeList *newRangeList = (RangeList *)malloc(sizeof(RangeList));
                        newRangeList->next = NULL;
                        newRangeList->rangeStart = i;
                        newRangeList->rangeEnd = end;
                        
                        //and finally we add it to the end of the range list:
                        iterRangeList = iterRangeNode->ranges;
                        while(iterRangeList->next != NULL)
                            iterRangeList = iterRangeList->next;

                        iterRangeList->next = newRangeList;

                        //this is a show stopper btw
                        found = 1;
                        break;
                    }

                    iterRangeNode = iterRangeNode->sibling;
                }

                //show stopper
                if(found)
                    break;

                //didnt find it
                //we are going to have to create a node with this value:
                RangeList *newRangeList = (RangeList *)malloc(sizeof(RangeList));
                newRangeList->next = NULL;
                newRangeList->rangeStart = i;
                newRangeList->rangeEnd = end;
                RangeNode *newRangeNode =  (RangeNode *)malloc(sizeof(RangeNode));
                newRangeNode->nextInternalNode = NULL;
                newRangeNode->ranges = newRangeList;
                newRangeNode->repeats = 0;
                newRangeNode->sibling = NULL;

                //and now we add it to the end of the range list of the activeNode:
                if(activeNode->pathList == NULL)
                {
                    //first element:
                    activeNode->pathList = newRangeNode;
                    remaining--;
                }
                else
                {
                    //not the first element, lets put it in the end
                    //of the range Node list
                    iterRangeNode = activeNode->pathList;
                    while (iterRangeNode->sibling != NULL)
                        iterRangeNode = iterRangeNode->sibling;

                    iterRangeNode->sibling = newRangeNode;
                    remaining--;
                }
            }
            else
            {
                //we have to find our active point,
                //we do this by going from activeNode in the direction of activeEdge
                //and we walk the ammount of activeLength
                iterRangeNode = activeNode->pathList;
                found = 0;
                while (iterRangeNode != NULL)
                {
                    if(buffer[iterRangeNode->ranges->rangeStart] == buffer[activeEdge])
                    {
                        //we have found the range node we want
                        found = 1;
                        break;
                    }
                    iterRangeNode = iterRangeNode->sibling;
                }
                if(!found)
                {
                    printf("fds\n");
                    exit(1);
                }




                
                //first of all, we check to see if this is going to make us jump to the top of a node:
                if(activeLength == (*iterRangeNode->ranges->rangeEnd - iterRangeNode->ranges->rangeStart +1))
                {
                    //we do! so all we have to do is change the active node the jumped node
                    //and change the activeLenght to 0 and let the code handle it:
                    activeNode = iterRangeNode->nextInternalNode;
                    activeLength = 0;
                }
                //then we check if we are going to jump further than the next node:
                else if(activeLength > (*iterRangeNode->ranges->rangeEnd - iterRangeNode->ranges->rangeStart +1))
                {
                    //oh no, we have to jump not only the node but we dont know where we are going after, wish us luck
                    activeNode = iterRangeNode->nextInternalNode;
                    activeLength = activeLength - (*iterRangeNode->ranges->rangeEnd - iterRangeNode->ranges->rangeStart +1);
                }
                else
                {
                    //yey we dont have to jump anything! so first we see if the next value
                    //is the value that we want:
                    found = 0;
                    if(buffer[iterRangeNode->ranges->rangeStart+activeLength] == buffer[i])
                    {
                        //it is! nice, we just add to active lenght
                        activeLength++;
                    }
                    else
                    {
                        //we need to create an internal node
                        InternalNode *newInternalNode = (InternalNode *)malloc(sizeof(InternalNode));
                        
                        //handle the suffix links
                        newInternalNode->suffixLink = root;
                        if(prevCreatedNode != root)
                            prevCreatedNode->suffixLink = newInternalNode;
                        prevCreatedNode = newInternalNode;

                        //create the first branch of this node as the one that was already there:
                        RangeList *oldRangeList = (RangeList *)malloc(sizeof(RangeList));
                        RangeList *otherIterRangeList = oldRangeList;
                        iterRangeList = iterRangeNode->ranges;

                        //copy the first range
                        otherIterRangeList->rangeStart = iterRangeList->rangeStart + activeLength;
                        otherIterRangeList->rangeEnd = iterRangeList->rangeEnd;
                        *iterRangeList->rangeEnd = iterRangeList->rangeStart + activeLength -1;
                        iterRangeList = iterRangeList->next;

                        //do the same thing for all other ranges
                        while(iterRangeList != NULL)
                        {
                            RangeList *newRangeList = (RangeList *)malloc(sizeof(RangeList));
                            newRangeList->rangeStart = iterRangeList->rangeStart + activeLength;
                            newRangeList->rangeEnd = (int *)malloc(sizeof(int));
                            *newRangeList->rangeEnd = *iterRangeList->rangeEnd;
                            *iterRangeList->rangeEnd = iterRangeList->rangeStart + activeLength -1;

                            otherIterRangeList->next = newRangeList;
                            otherIterRangeList = newRangeList;

                            iterRangeList = iterRangeList->next;
                        }

                        //prepare the new internal node
                        RangeNode *newRangeNode = (RangeNode *)malloc(sizeof(RangeNode));
                        newRangeNode->nextInternalNode = NULL;
                        newRangeNode->ranges = oldRangeList;
                        newRangeNode->sibling = NULL;
                        newRangeNode->repeats = iterRangeNode->repeats;
                        newInternalNode->pathList = newRangeNode;

                        //now we can put this internal node where it should be
                        iterRangeNode->nextInternalNode = newInternalNode;

                        //and finally jump to it and continue
                        activeNode = newInternalNode;
                        activeLength = 0;
                    }
                }
            }
        }
    }


    return root;
}



void printSuffTree(InternalNode *elemento)
{
    /*
    if (elemento->suffixLink == NULL) 
    {
        //estamos na raiz
        printf("Copy the following code to https://dreampuf.github.io/GraphvizOnline\n");
        printf("graph {\n");
    }
    */
    if (elemento->suffixLink != NULL)
        printf("\tn%p -- n%p\n", elemento, elemento->suffixLink);
    RangeNode *iter = elemento->pathList;
    printf("\tn%p [label = \"Node\"]\n", elemento);
    
    while (iter != NULL)
    {
        printf("\tn%p [label = \"[%d, %d] (%d)\"]\n", iter, iter->ranges->rangeStart, *iter->ranges->rangeEnd, iter->repeats);
        printf("\tn%p -- n%p\n", elemento, iter);
        if(iter->nextInternalNode != NULL)
        {
            printf("\tn%p -- n%p\n", iter, iter->nextInternalNode);
            printSuffTree(iter->nextInternalNode);
        }
        iter = iter->sibling;
    }

}

int freeSuffTree(InternalNode *elem)
{
    return 1;
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
    
    //int testBuffer[10] = {(int)'x', (int)'y', (int)'z', (int)'x', (int)'y', (int)'a', (int)'x', (int)'y', (int)'z', (int)'$'};
    int testBuffer[9] = {(int)'m', (int)'i', (int)'s', (int)'s', (int)'i', (int)'s', (int)'s', (int)'i',(int)'$'};
    InternalNode *root = createWeightedSuffixTree(testBuffer, 9);
    printf("Copy the following code to https://dreampuf.github.io/GraphvizOnline\n");
    printf("graph {\n");
    printSuffTree(root);
    printf("}\n");

    return 1;
}