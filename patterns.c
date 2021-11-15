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

RangeNode *newRangeNode(int start, int *end)
{
    RangeNode *newRangeNode = (RangeNode *)malloc(sizeof(RangeNode));
    newRangeNode->rangeStart = start;
    newRangeNode->rangeEnd = end;
    newRangeNode->repeats = 0;
    newRangeNode->sibling = NULL;
    return newRangeNode;
}

/*
InternalNode *splitRange(RangeNode *rangeNode, int where, int end)
{
    RangeNode *new = newRangeNode(where, end);
    range->range[1] = where;
}
*/

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
        while(remaining)
        {
            if(!activeLength)
            {
                //it doesnt have any paths yet (only used for root)
                if(root->pathList == NULL)
                {
                    root->pathList = (RangeNode *)malloc(sizeof(RangeNode));
                    root->pathList->rangeStart = 0;
                    root->pathList->nextInternalNode = NULL;
                    root->pathList->rangeEnd = end;
                    root->pathList->repeats = 0;
                    root->pathList->sibling = NULL;
                    remaining--;
                    break;
                }


                //check if active node has a path to current buffer[i] value
                iterRangeNode = root->pathList; //activeNode->pathList;
                found = 0;
                while (iterRangeNode != NULL)
                {
                    if(buffer[iterRangeNode->rangeStart] == buffer[i])
                    {
                        //found it
                        activeEdge = iterRangeNode->rangeStart;
                        iterRangeNode->repeats++;
                        activeLength++;
                        found = 1;
                        break;
                    }
                    iterRangeNode = iterRangeNode->sibling;
                }
                if(found) //trick 2: rule 3 extension -> show stopper
                    break;
                //didnt find it, add this element to the range
                iterRangeNode = activeNode->pathList;
                while (iterRangeNode->sibling != NULL)
                    iterRangeNode = iterRangeNode->sibling;

                iterRangeNode->sibling = (RangeNode *)malloc(sizeof(RangeNode));
                iterRangeNode->sibling->rangeStart = i;
                iterRangeNode->sibling->nextInternalNode = NULL;
                iterRangeNode->sibling->rangeEnd = end;
                iterRangeNode->sibling->repeats = 0;
                iterRangeNode->sibling->sibling = NULL;
                remaining--;
            }
            else
            {
                //lets find our active point! first we go in the way of the right range:
                found = 0;
                iterRangeNode = activeNode->pathList;
                while (iterRangeNode != NULL)
                {
                    if(buffer[iterRangeNode->rangeStart] == buffer[activeEdge])
                    {
                        found = 1;
                        break;
                    }
                    iterRangeNode = iterRangeNode->sibling;
                }
                
                if(!found)
                {
                    //printSuffTree(root);
                    printf("oh no\n");
                    exit(1);
                }


                //now that we are in the right track lets check if we have are going
                //to jump an internal node on this track
                if(activeLength > *iterRangeNode->rangeEnd - iterRangeNode->rangeStart+1) //TODO: IDK WHY THIS IS WORKING
                {

                    /*
                    //NEW */
                    activeLength = activeLength - *iterRangeNode->rangeEnd - iterRangeNode->rangeStart+1;
                    /*
                    */

                    //the range ends here, we need to jump an internal node:
                    activeNode = iterRangeNode->nextInternalNode;


                    //prevNodeLen = *iterRangeNode->rangeEnd - iterRangeNode->rangeStart+1;

                    iterRangeNode = activeNode->pathList;
                    found = 0;


                    //then we need to find the active Edge (if there is one:)
                    while (iterRangeNode != NULL)
                    {
                        if(buffer[iterRangeNode->rangeStart] == buffer[i])
                        {
                            //there is one!
                            activeEdge = iterRangeNode->rangeStart;
                            iterRangeNode->repeats++;
                            //activeLength = 1;
                            found = 1;
                            break;
                        }
                        iterRangeNode = iterRangeNode->sibling;
                    }

                    if(found) //show stoper!
                        break;


                    //there isnt one, what now? -> we change every path in here
                    //one by one we check if the paths already have this char:
                    iterRangeNode = activeNode->pathList;
                    while (iterRangeNode != NULL)
                    {
                        if(activeLength == *iterRangeNode->rangeEnd - iterRangeNode->rangeStart +1)
                        {
                            //we have a node there, lets see if there is a path in that node that we need
                            RangeNode *anotherIterRangeNode;
                            anotherIterRangeNode = iterRangeNode->nextInternalNode->pathList;
                            found = 0;
                            while (anotherIterRangeNode != NULL)
                            {
                                if(buffer[anotherIterRangeNode->rangeStart] == buffer[i])
                                {
                                    //already has it, next
                                    found = 1;
                                    break;
                                }
                            }
                            if(found)
                                continue;

                            //doesnt have it, so we put the activeEdge here
                            activeEdge = iterRangeNode->rangeStart;
                        }
                        else if(buffer[iterRangeNode->rangeStart + activeLength] == buffer[i])
                        {
                            //there is one!
                            activeEdge = iterRangeNode->rangeStart;
                            iterRangeNode->repeats++;
                            activeLength = activeLength - *iterRangeNode->rangeEnd - iterRangeNode->rangeStart +1;
                            found = 1;
                            break;
                        }
                        else
                        {
                            printf("fds n sei mais \n");
                            activeEdge = iterRangeNode->rangeStart;
                            break;
                        }
                        iterRangeNode = iterRangeNode->sibling;
                    }
                    
                    //printf("error on the fucking ukkonens \n");
                    //exit(1);
                    
                }
                else
                {
                    //we dont need to jump a node, lets check if the next thing in this
                    //range is the value we want:
                    if(buffer[iterRangeNode->rangeStart + activeLength] == buffer[i])
                    {
                        //it is! show stopper
                        activeLength++;
                        break;
                    }
                    //the value we want is not in the range, we need to split this range
                    InternalNode *newInternalNode = (InternalNode *)malloc(sizeof(InternalNode));
                    RangeNode *newRangeNode1 = (RangeNode *)malloc(sizeof(RangeNode));
                    RangeNode *newRangeNode2 = (RangeNode *)malloc(sizeof(RangeNode));

                    //new range for the new prefix
                    newRangeNode1->nextInternalNode = NULL;
                    newRangeNode1->sibling = NULL;
                    newRangeNode1->rangeStart = i;
                    newRangeNode1->rangeEnd = end;
                    newRangeNode1->repeats = 0;

                    //new range for the prefix that was there previously
                    newRangeNode2->nextInternalNode = NULL;
                    newRangeNode2->rangeEnd = end;
                    newRangeNode2->rangeStart = iterRangeNode->rangeStart + activeLength;
                    newRangeNode2->repeats = iterRangeNode->repeats -1;
                    newRangeNode2->sibling = newRangeNode1;

                    //split the old range
                    iterRangeNode->rangeEnd = (int *)malloc(sizeof(int));
                    *iterRangeNode->rangeEnd = iterRangeNode->rangeStart + activeLength -1;
                    //create new ranges list
                    newInternalNode->pathList = newRangeNode2;
                    //conect this new internal node to the previous splitted range
                    iterRangeNode->nextInternalNode = newInternalNode;

                    //take care of the suffix link
                    if(prevCreatedNode != root)
                        prevCreatedNode->suffixLink = newInternalNode;

                    newInternalNode->suffixLink = root;
                    prevCreatedNode = newInternalNode;
                    
                    //rest of rules
                    remaining--;

                    if(activeNode == root)
                    {
                        activeLength--;
                        activeEdge++;
                    }
                    else
                    {
                        activeNode = activeNode->suffixLink;
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
        printf("\tn%p [label = \"[%d, %d] (%d)\"]\n", iter, iter->rangeStart, *iter->rangeEnd, iter->repeats);
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