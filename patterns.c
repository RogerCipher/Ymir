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
                    if(buffer[iterRangeNode->rangeStart] == buffer[i])
                    {
                        //found it, lets increase our active Length
                        activeLength++;
                        activeEdge = iterRangeNode->rangeStart;

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
                RangeNode *newRangeNode =  (RangeNode *)malloc(sizeof(RangeNode));
                newRangeNode->nextInternalNode = NULL;
                newRangeNode->rangeStart = i;
                newRangeNode->rangeEnd = end;
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
                    if(buffer[iterRangeNode->rangeStart] == buffer[activeEdge])
                    {
                        //we have found the range node we want to travel in
                        found = 1;
                        break;
                    }
                    iterRangeNode = iterRangeNode->sibling;
                }
                if(!found)
                {
                    //we havent found the range node we want to travel in, something is very wrong
                    printf("fds\n");
                    exit(1);
                }
                
                //first of all, we check to see if this is going to make us jump to the top of a node:
                if(activeLength == (*iterRangeNode->rangeEnd - iterRangeNode->rangeStart +1))
                {
                    //we do! so all we have to do is change the active node the jumped node
                    //and change the activeLenght to 0 and let the code handle it:
                    if(iterRangeNode->nextInternalNode == NULL)
                    {
                        printSuffTree(root);
                        printf("1\n");
                        exit(1);
                    }
                    activeNode = iterRangeNode->nextInternalNode;
                    activeEdge += activeLength;
                    activeLength = 0;


                }
                //then we check if we are going to jump further than the next node:
                else if(activeLength > (*iterRangeNode->rangeEnd - iterRangeNode->rangeStart +1))
                {
                    //oh no, we have to jump not only the node but we dont know where we are going after, wish us luck
                    if(iterRangeNode->nextInternalNode == NULL)
                    {
                        printf("2\n");
                        exit(1);
                    }
                    activeNode = iterRangeNode->nextInternalNode;
                    activeEdge += activeLength;
                    activeLength = activeLength - (*iterRangeNode->rangeEnd - iterRangeNode->rangeStart +1);


                }
                else
                {
                    //TODO: THIS IS ALMOUST FIXED, FINAL PUSH I SWEAR
                    
                    //yey we dont have to jump anything! so first we see if the next value
                    //is the value that we want:
                    if(buffer[iterRangeNode->rangeStart+activeLength] == buffer[i])
                    {
                        //it is! nice, we just add to active lenght
                        activeLength++;
                        break;
                    }
                    else
                    {

                        //we need to split this here, so:
                        //create an internal node:
                        InternalNode *newInternalNode = (InternalNode *)malloc(sizeof(InternalNode));
                        
                        //handle the suffix links
                        newInternalNode->suffixLink = root;
                        if(prevCreatedNode != root)
                            prevCreatedNode->suffixLink = newInternalNode;
                        prevCreatedNode = newInternalNode;

                        //now we will cut every range in the current range nodes range list
                        //at the same time we create the new range for the new internalNode Ranges:

                        //cut the old range
                        int *thisRangeEnd = (int *)malloc(sizeof(int));
                        *thisRangeEnd = iterRangeNode->rangeStart + activeLength -1;
                        


                        //finally we just need the RangeNode to add here with the new range list:
                        RangeNode *newRangeNode = (RangeNode *)malloc(sizeof(RangeNode));
                        //connect the next internal node like it was before
                        newRangeNode->nextInternalNode = iterRangeNode->nextInternalNode;
                        newRangeNode->rangeStart = iterRangeNode->rangeStart + activeLength;
                        newRangeNode->rangeEnd = iterRangeNode->rangeEnd;

                        //and update it
                        //update old ranges end
                        iterRangeNode->rangeEnd = thisRangeEnd;


                        newRangeNode->repeats = 0;
                        newRangeNode->sibling = NULL;
                        newInternalNode->pathList = newRangeNode;

                        //now we can put this internal node where it should be
                        iterRangeNode->nextInternalNode = newInternalNode;

                        //create the node for the ranges we just split above:
                        RangeNode *newSplitRangeNode = (RangeNode *)malloc(sizeof(RangeNode));
                        newSplitRangeNode->nextInternalNode = NULL;
                        newSplitRangeNode->repeats = 0;
                        newSplitRangeNode->sibling = NULL;
                        newSplitRangeNode->rangeStart = i;
                        newSplitRangeNode->rangeEnd = end;

                        //add it to the ranges of the internal node we created
                        newRangeNode->sibling = newSplitRangeNode;


                        //we are done, now we just continue the ukkonens:
                        if(newInternalNode == NULL)
                        {
                            printf("3\n");
                            exit(1);
                        }
                        remaining --;
                        if(activeNode == root)
                        {
                            activeEdge++;
                            activeLength--;
                        }
                        else
                        {
                            activeNode = activeNode->suffixLink;
                        }
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

        printf("\tn%p [label = \"[%d, %d]\"]\n""", iter, iter->rangeStart, *iter->rangeEnd);
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
    
    int testBuffer[10] = {(int)'x', (int)'y', (int)'z', (int)'x', (int)'y', (int)'a', (int)'x', (int)'y', (int)'z', (int)'$'};
    //int testBuffer[9] = {(int)'m', (int)'i', (int)'s', (int)'s', (int)'i', (int)'s', (int)'s', (int)'i',(int)'$'};
    InternalNode *root = createWeightedSuffixTree(testBuffer, 10);
    printf("Copy the following code to https://dreampuf.github.io/GraphvizOnline\n");
    printf("graph {\n");
    printSuffTree(root);
    printf("}\n");

    return 1;
}