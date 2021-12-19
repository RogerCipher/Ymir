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


//return a range length
int rangeLen(RangeList *range)
{
    return *range->rangeEnd - range->rangeStart +1;
}

/*
//returns a range lenght from the root
int suffixLength(RangeNode *range)
{

    int myRangeLen = rangeLen(range);

    //check to see if this range is conected to root
    if(range->prevInternalNode->prevRangeNode != NULL)
        myRangeLen += suffixLength(range->prevInternalNode->prevRangeNode);

    return myRangeLen;
}
*/

//this function returns the result of the heuristic of a suffix in our suffix tree
int heuristicResult(RangeNode *range)
{   
    int length = rangeLen(range->ranges);//suffixLength(range); //*range->rangeEnd - range->rangeStart +1;

    return (range->weight -1) * length;
}

RangeList *createRangeList(int rangeStart, int* rangeEnd)
{
    RangeList *newRangeList = (RangeList *)malloc(sizeof(RangeList));

    newRangeList->next = NULL;
    newRangeList->rangeStart = rangeStart;
    newRangeList->rangeEnd = rangeEnd;

    printf("created node with: [%d, %d]\n", rangeStart, *rangeEnd);

    return newRangeList;
}

//calculates and returns the best heuristic result node from a parent internal node
RangeNode *bestRangeNode(InternalNode *elem)
{
    //only range here
    if(elem->pathList->sibling == NULL)
        return elem->pathList;

    //not the only range
    RangeNode *bestNode = elem->pathList;
    RangeNode *iter = elem->pathList;
    RangeNode *possiblyBetterNode = NULL;
    while (iter != NULL)
    {
        // check the current node
        if(heuristicResult(iter) > heuristicResult(bestNode))
            bestNode = iter;

        //check children
        if(iter->nextInternalNode != NULL)
        {
            
            possiblyBetterNode = bestRangeNode(iter->nextInternalNode);
            if(heuristicResult(possiblyBetterNode) > heuristicResult(bestNode))
                bestNode = possiblyBetterNode;
        }
        iter = iter->sibling;
    }

    return bestNode;
}

/*
PatternCharBlock *bestCharBlock(InternalNode *root, int *buffer)
{
    //get the best pattern node:
    RangeNode *bestNode = bestRangeNode(root);

    //buld the char pattern:
    RangeNode *iter = NULL;
    PatternChar *lastPatternChar = NULL;

    //first the best range
    for(int i = 0; i < rangeLen(bestNode); i++)
    {
        PatternChar *nextPatternChar = (PatternChar *)malloc(sizeof(PatternChar));
        nextPatternChar->value = buffer[*bestNode->rangeEnd -i];
        nextPatternChar->next = lastPatternChar;
        lastPatternChar = nextPatternChar;
    }

    //now the rest of the ranges till we reach root
    iter = bestNode->prevInternalNode->prevRangeNode;
    while (iter != NULL)
    {
        for(int i = 0; i < rangeLen(iter); i++)
        {
            PatternChar *nextPatternChar = (PatternChar *)malloc(sizeof(PatternChar));
            nextPatternChar->value = buffer[*iter->rangeEnd -i];
            nextPatternChar->next = lastPatternChar;
            lastPatternChar = nextPatternChar;
        }

        iter = iter->prevInternalNode->prevRangeNode;
    }

    //finally package it in a nice little block
    PatternCharBlock *block = (PatternCharBlock *)malloc(sizeof(PatternCharBlock));
    block->pattern = lastPatternChar;
    block->weight = bestNode->weight;

    return block;
}

//decreases repetitions of everyone above this node
void decreaseRepetitions(RangeNode *range)
{
    range->weight--;
    //decrement the repetitions of every range abouve this one:
    if(range->prevInternalNode->prevRangeNode != NULL)
            decreaseRepetitions(range->prevInternalNode->prevRangeNode);
    
    return;
}

//increases repetitions of everyone above this node
void increaseRepetitions(RangeNode *range)
{
    range->weight++;
    //decrement the repetitions of every range abouve this one:
    if(range->prevInternalNode->prevRangeNode != NULL)
            increaseRepetitions(range->prevInternalNode->prevRangeNode);
    
    return;
}

//removes the uniquechar we used to transform the suffix tree into a true suffix tree
void removeUniqueChar(InternalNode *elem, int positionOfUniqueChar)
{
    RangeNode *iter = elem->pathList;
    RangeNode *next;
    while(iter != NULL)
    {
        next = iter->sibling;

        //do the same for all the below nodes
        if(iter->nextInternalNode != NULL)
            removeUniqueChar(iter->nextInternalNode, positionOfUniqueChar);


        //check to see if this range has the unique char (it will be at the end for sure)
        if(*iter->rangeEnd == positionOfUniqueChar)
        {
            if(iter->rangeStart == *iter->rangeEnd)
            {

                //decreaseRepetitions(iter);

                //remove it from the pathlist of the internal node:
                RangeNode *iterFinder = elem->pathList;
                while (iterFinder->sibling != iter && iterFinder != NULL)
                    iterFinder = iterFinder->sibling;

                free(iter);
                iter = NULL;
                iterFinder->sibling = NULL;
            }
            else
            {
                //its a range with the unique char in the end,
                //so we just remove the unique char:
                int *newEnd = (int *)malloc(sizeof(int));
                *newEnd = positionOfUniqueChar -1;
                iter->rangeEnd = newEnd;
            }

        }
        iter = next;
    }
}
*/


//help for ukkonens
InternalNode *cutThisRangeNode(RangeNode *nodeToSplit, int activeLen, int i, int* end)
{
    //we need to split this here, so:
    //create an internal node:
    InternalNode *newInternalNode = (InternalNode *)malloc(sizeof(InternalNode));

    //handle the suffix links
    //newInternalNode->suffixLink = root;
    //if(prevCreatedNode != root)
    //    prevCreatedNode->suffixLink = newInternalNode;
    //prevCreatedNode = newInternalNode;



    //we will need to split this ranges in to 2 range Nodes, we can calculate the
    //range node formed by the range list previously there at the same time we are cutting the ranges:

    //now we will cut every range in the current range nodes range list
    //at the same time we create the new range for the new internalNode Ranges:

    //cut the old range + create the split range derived from it
    RangeList *derivedRangeListHead = NULL;
    RangeList *derivedRangeListTail = NULL;
    RangeList *iterRangeList = nodeToSplit->ranges;
    while (iterRangeList != NULL)
    {

        

        //calculate the new range end for the node we are splitting
        int *oldRangeEnd = (int *)malloc(sizeof(int));
        *oldRangeEnd = iterRangeList->rangeStart + activeLen /* -1*/;

        //calculate the rangeEnd for the splitted part of it
        RangeList *derivedRangeListNode = createRangeList(iterRangeList->rangeStart + activeLen +1 ,iterRangeList->rangeEnd);

        //add it to the end of the derived range list
        if(derivedRangeListHead == NULL)
        {
            //its the first element
            derivedRangeListHead = derivedRangeListNode;
            derivedRangeListTail = derivedRangeListNode;
        }
        else
        {
            //if its not the head add it in the end of the range list:
            derivedRangeListTail->next = derivedRangeListNode;
            derivedRangeListTail = derivedRangeListNode;
        }
        
        //finish splitting the old range
        iterRangeList->rangeEnd = oldRangeEnd;
        iterRangeList = iterRangeList->next;
    }

    //make the internal node to connect the derived range and the new range to the old range that was there
    newInternalNode->prevRangeNode = nodeToSplit;

    //finally we just need the RangeNode to add here with the new range list:
    RangeNode *derivedRangeNode = (RangeNode *)malloc(sizeof(RangeNode));

    //connect the next internal node like it was before
    derivedRangeNode->nextInternalNode = nodeToSplit->nextInternalNode;

    //store here the derived ranges we calculated previously
    derivedRangeNode->ranges = derivedRangeListHead;

    //connect the dots + fill some more data
    derivedRangeNode->prevInternalNode = newInternalNode;
    derivedRangeNode->weight = nodeToSplit->weight -1;
    derivedRangeNode->sibling = NULL;
    newInternalNode->pathList = derivedRangeNode;

    //now we can put this internal node where it should be
    //iterRangeNode->nextInternalNode = newInternalNode;

    //now, finally, we can create the new range that splitted this node:
    RangeNode *newSplitRangeNode = (RangeNode *)malloc(sizeof(RangeNode));
    newSplitRangeNode->nextInternalNode = NULL;
    newSplitRangeNode->weight = 1;
    newSplitRangeNode->sibling = NULL;
    RangeList *splitRangeList = createRangeList(i, end);
    newSplitRangeNode->ranges = splitRangeList;
    newSplitRangeNode->prevInternalNode = newInternalNode;

    //add it to the ranges of the internal node we created
    derivedRangeNode->sibling = newSplitRangeNode;

    return newInternalNode;
}

//help for ukkonens
InternalNode *myRule1Cut(RangeNode *nodeToSplit, int activeLen, int i, int* end)
{
    //we need to split this here, so:
    //create an internal node:
    InternalNode *newInternalNode = (InternalNode *)malloc(sizeof(InternalNode));

    //handle the suffix links
    //newInternalNode->suffixLink = root;
    //if(prevCreatedNode != root)
    //    prevCreatedNode->suffixLink = newInternalNode;
    //prevCreatedNode = newInternalNode;



    //we will need to split this ranges in to 2 range Nodes, we can calculate the
    //range node formed by the range list previously there at the same time we are cutting the ranges:

    //now we will cut every range in the current range nodes range list
    //at the same time we create the new range for the new internalNode Ranges:

    //cut the old range + create the split range derived from it
    RangeList *derivedRangeListHead = NULL;
    RangeList *derivedRangeListTail = NULL;
    RangeList *iterRangeList = nodeToSplit->ranges;
    while (iterRangeList->next != NULL)
    {
        //calculate the new range end for the node we are splitting
        int *oldRangeEnd = (int *)malloc(sizeof(int));
        *oldRangeEnd = iterRangeList->rangeStart + activeLen /*-1*/;

        //calculate the rangeEnd for the splitted part of it
        RangeList *derivedRangeListNode = createRangeList(iterRangeList->rangeStart + activeLen+1 ,iterRangeList->rangeEnd);

        //add it to the end of the derived range list
        if(derivedRangeListHead == NULL)
        {
            //its the first element
            derivedRangeListHead = derivedRangeListNode;
            derivedRangeListTail = derivedRangeListNode;
        }
        else
        {
            //if its not the head add it in the end of the range list:
            derivedRangeListTail->next = derivedRangeListNode;
            derivedRangeListTail = derivedRangeListNode;
        }
        
        //finish splitting the old range
        iterRangeList->rangeEnd = oldRangeEnd;
        iterRangeList = iterRangeList->next;
    }

    //last one we just cut
    //calculate the new range end for the node we are splitting
    int *oldRangeEnd = (int *)malloc(sizeof(int));
    *oldRangeEnd = iterRangeList->rangeStart + activeLen /*-1*/;
    iterRangeList->rangeEnd = oldRangeEnd;


    //make the internal node to connect the derived range and the new range to the old range that was there
    newInternalNode->prevRangeNode = nodeToSplit;

    //finally we just need the RangeNode to add here with the new range list:
    RangeNode *derivedRangeNode = (RangeNode *)malloc(sizeof(RangeNode));

    //connect the next internal node like it was before
    derivedRangeNode->nextInternalNode = nodeToSplit->nextInternalNode;

    //store here the derived ranges we calculated previously
    derivedRangeNode->ranges = derivedRangeListHead;

    //connect the dots + fill some more data
    derivedRangeNode->prevInternalNode = newInternalNode;
    derivedRangeNode->weight = nodeToSplit->weight -1;
    derivedRangeNode->sibling = NULL;
    newInternalNode->pathList = derivedRangeNode;

    //now we can put this internal node where it should be
    //iterRangeNode->nextInternalNode = newInternalNode;

    //now, finally, we can create the new range that splitted this node:
    RangeNode *newSplitRangeNode = (RangeNode *)malloc(sizeof(RangeNode));
    newSplitRangeNode->nextInternalNode = NULL;
    newSplitRangeNode->weight = 1;
    newSplitRangeNode->sibling = NULL;
    RangeList *splitRangeList = createRangeList(i, end);
    newSplitRangeNode->ranges = splitRangeList;
    newSplitRangeNode->prevInternalNode = newInternalNode;

    //add it to the ranges of the internal node we created
    derivedRangeNode->sibling = newSplitRangeNode;

    return newInternalNode;
}


//ukkonens
InternalNode *createSuffixTree(int *buffer, int bufferLen)
{
    InternalNode *root = (InternalNode *)malloc(sizeof(InternalNode));

    root->prevRangeNode = NULL;

    //Ukkonens algorithm
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
                iterRangeNode = /*root->pathList;*/activeNode->pathList;
                found = 0;
                while(iterRangeNode != NULL)
                {
                    if(buffer[iterRangeNode->ranges->rangeStart] == buffer[i])
                    {
                        //found it, lets increase our active Length and weight
                        activeLength++;
                        activeEdge = iterRangeNode->ranges->rangeStart;

                        //create a new range starting from this point if there's not an equal range there already:
                        if(iterRangeNode->ranges->rangeStart != i)
                        {
                            //this node is repeated
                            iterRangeNode->weight++;

                            RangeList *newRange = createRangeList(i, end);

                            //cut the current last range in the range list
                            int *cuttedEnd = (int *)malloc(sizeof(int));
                            *cuttedEnd = i-1;

                            RangeList *lastRange = iterRangeNode->ranges;
                            while (lastRange->next != NULL)
                                lastRange = lastRange->next;

                            lastRange->rangeEnd = cuttedEnd;

                            //connect the ranges
                            lastRange->next = newRange;
                        }
                        
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
                RangeList *newRangeList = createRangeList(i, end);
                newRangeNode->ranges = newRangeList;
                newRangeNode->weight = 1;
                newRangeNode->sibling = NULL;
                newRangeNode->prevInternalNode = activeNode;
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
                    
                    if(iterRangeNode->ranges == NULL)
                    {
                        iterRangeNode = iterRangeNode->sibling;
                        printf("fdsssssss\n");
                        continue;
                        
                        //printf("deu merda:  \n");
                        //exit(1);
                    }
                    
                    if(buffer[iterRangeNode->ranges->rangeStart] == buffer[activeEdge])
                    {
                        //we have found the range node we want to travel in
                        printf("i: %d\n", i);
                        found = 1;

                        /*------- aaaaaaaahhhhhhhh ---*/

                        //check if we need to split this (according to rule number 1 *see report)
                        if(activeLength ==  rangeLen(iterRangeNode->ranges))
                        {
                            //we do! so all we have to do is change the active node the jumped node
                            //and change the activeLenght to 0 and let the code handle it:
                            if(iterRangeNode->nextInternalNode == NULL)
                            {
                                //ok, we have to split this node, according to my rule number 1 *see report
                                iterRangeNode->nextInternalNode = myRule1Cut(iterRangeNode, activeLength, i, end);

                                //handle the suffix links
                                iterRangeNode->nextInternalNode->suffixLink = root;
                                if(prevCreatedNode != root)
                                    prevCreatedNode->suffixLink = iterRangeNode->nextInternalNode;
                                prevCreatedNode = iterRangeNode->nextInternalNode;

                                //we are done, now we just continue the ukkonens:
                            }

                            activeNode = iterRangeNode->nextInternalNode;
                            activeEdge += activeLength;
                            activeLength = 0;

                        }
                        else if(activeLength > rangeLen(iterRangeNode->ranges))
                        {
                            //oh no, we have to jump not only the node but we dont know where we are going after, wish us luck
                            if(iterRangeNode->nextInternalNode == NULL)
                            {
                                //ok, we have to split this node, according to my rule number 1 *see report
                                iterRangeNode->nextInternalNode = myRule1Cut(iterRangeNode, activeLength, i, end);

                                //handle the suffix links
                                iterRangeNode->nextInternalNode->suffixLink = root;
                                if(prevCreatedNode != root)
                                    prevCreatedNode->suffixLink = iterRangeNode->nextInternalNode;
                                prevCreatedNode = iterRangeNode->nextInternalNode;

                                //we are done, now we just continue the ukkonens:
                            }
                            activeNode = iterRangeNode->nextInternalNode;
                            //activeNode->pathList->weight++;
                            activeEdge += activeLength;
                            activeLength = activeLength - rangeLen(iterRangeNode->ranges);
                        }
                        /*------- aaaaaaaahhhhhhhh ---*/
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
                if(activeLength ==  rangeLen(iterRangeNode->ranges))
                {
                    //we do! so all we have to do is change the active node the jumped node
                    //and change the activeLenght to 0 and let the code handle it:
                    if(iterRangeNode->nextInternalNode == NULL)
                    {
                        //ok, we have to split this node, according to my rule number 1 *see report
                        iterRangeNode->nextInternalNode = myRule1Cut(iterRangeNode, activeLength, i, end);

                        //handle the suffix links
                        iterRangeNode->nextInternalNode->suffixLink = root;
                        if(prevCreatedNode != root)
                            prevCreatedNode->suffixLink = iterRangeNode->nextInternalNode;
                        prevCreatedNode = iterRangeNode->nextInternalNode;

                        //we are done, now we just continue the ukkonens:
                    }

                    activeNode = iterRangeNode->nextInternalNode;
                    activeEdge += activeLength;
                    activeLength = 0;

                }
                //then we check if we are going to jump further than the next node:
                else if(activeLength > rangeLen(iterRangeNode->ranges))
                {
                    //oh no, we have to jump not only the node but we dont know where we are going after, wish us luck
                    if(iterRangeNode->nextInternalNode == NULL)
                    {
                        //ok, we have to split this node, according to my rule number 1 *see report
                        iterRangeNode->nextInternalNode = myRule1Cut(iterRangeNode, activeLength, i, end);

                        //handle the suffix links
                        iterRangeNode->nextInternalNode->suffixLink = root;
                        if(prevCreatedNode != root)
                            prevCreatedNode->suffixLink = iterRangeNode->nextInternalNode;
                        prevCreatedNode = iterRangeNode->nextInternalNode;

                        //we are done, now we just continue the ukkonens:
                    }
                    activeNode = iterRangeNode->nextInternalNode;
                    //activeNode->pathList->weight++;
                    activeEdge += activeLength;
                    activeLength = activeLength - rangeLen(iterRangeNode->ranges);
                }
                else
                {   
                    //yey we dont have to jump anything! so first we see if the next value
                    //is the value that we want:


                    if(buffer[iterRangeNode->ranges->rangeStart+activeLength] == buffer[i])
                    {
                        //it is! nice, we just add to active lenght
                        activeLength++;
                        break;
                    }
                    else
                    {
                        printf("CUTTED\n");
                        printf("[%d, %d]", i, *end);
                        //we need to split this here, so:
                        iterRangeNode->nextInternalNode = myRule1Cut(iterRangeNode, activeLength, i, end); //cutrange?

                        //handle the suffix links
                        iterRangeNode->nextInternalNode->suffixLink = root;
                        if(prevCreatedNode != root)
                            prevCreatedNode->suffixLink = iterRangeNode->nextInternalNode;
                        prevCreatedNode = iterRangeNode->nextInternalNode;

                        //we are done, now we just continue the ukkonens:
                        if(prevCreatedNode == NULL)
                        {
                            printf("error: 3\n");
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

//recursivly count the number of repetitions in a range
int repetitionsOfRange(RangeNode *elem)
{
    //initialization
    elem->weight = 0;

    //if we dont have a nextInternalNode our number of repetitions is 0 (it appears 1 time)
    //if it does we know the ammount of repetitions is the amount of range nodes in the nextInternalNodes of this range
    if(elem->nextInternalNode != NULL)
    {
        
        //count the number of repetitions
        RangeNode *iter = elem->nextInternalNode->pathList;
        while (iter != NULL)
        {
            //count our repetitions
            elem->weight += repetitionsOfRange(iter);

            iter = iter->sibling;
        }
    }
    else
    {
        elem->weight = 1;
    }
    
    return elem->weight;
}

//this removes a certain pattern from a buffer, this could be faster (see KMP substring search : https://www.youtube.com/watch?v=GTJr8OvyEVQ)
int removeCharSetFromBuffer(int *buffer, int bufferLen)
{

    return 1;
}

//count the number of repetitions of each range of the suffix tree
void fillRepetitionsInTree(InternalNode *root)
{
    //we will just fill every range in root (recursively) with
    //the correct repetitions
    RangeNode *iter = root->pathList;
    while (iter != NULL)
    {
        repetitionsOfRange(iter);
        iter = iter->sibling;
    }
    return;
}

//simple print for graphviz
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

    /*
    if(elemento->prevRangeNode != NULL)
        printf("\tn%p -- n%p\n", elemento, elemento->prevRangeNode);
    */

    RangeNode *iter = elemento->pathList;
    printf("\tn%p [label = \"Node\"]\n", elemento);
    
    while (iter != NULL)
    {
        RangeList *iterList = iter->ranges;
        printf("\tn%p [label = \"", iter);
        printf("weight: %d\n", iter->weight);
        while (iterList != NULL)
        {
            printf("[%d, %d]\n", iterList->rangeStart, *iterList->rangeEnd);
            iterList = iterList->next;
        }
        printf("\"]\n");
        printf("\tn%p -- n%p\n", elemento, iter);


        /*
        if(iter->prevInternalNode != NULL)
        {
            printf("\tn%p -- n%p\n", iter, iter->prevInternalNode);
        }
        */


        if(iter->nextInternalNode != NULL)
        {
            printf("\tn%p -- n%p\n", iter, iter->nextInternalNode);
            printSuffTree(iter->nextInternalNode);
        }
        iter = iter->sibling;
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
}

/*
//free the suffix tree (duuh)
int freeSuffTree(InternalNode *elem)
{
    if(elem->pathList != NULL)
    {
        RangeNode *iter = elem->pathList;
        RangeNode *next;
        while (iter != NULL)
        {
            if(iter->nextInternalNode != NULL)
                freeSuffTree(iter->nextInternalNode);

            if(iter->rangeEnd != NULL)
            {
                free(iter->rangeEnd);
                iter->rangeEnd = NULL;
            }
            
            next = iter->sibling;
            free(iter);
            iter = NULL;

            iter = next;
        }
        
    }
    free(elem);
    elem = NULL;
    return 1;
}
*/

void determineBestPatterns(int *buffer, int bufferLen)
{
    if(bufferLen > MAXCHARBUFFER -2)
    {
        printf("I want a blank space in the buffer so I can put in the unique var, ty very much\n");
        exit(1);
    }
    printf("bufferLen: %d", bufferLen);
    buffer[bufferLen] = UNIQUECHARVALUE;
    printf("\n");
    for(int i = 0; i < bufferLen+1; i++)
    {
        printf("%d ", buffer[i]);
    }
    printf("\n ------ %d", bufferLen);

    InternalNode *root = createSuffixTree(buffer, bufferLen+1);

    //we then count the repetitions of each node
    //fillRepetitionsInTree(root);


    //finally we will remove the unique char value that transformed the tree into a 
    //true suffix tree
    //removeUniqueChar(root, bufferLen);





    printf("Copy the following code to https://dreampuf.github.io/GraphvizOnline\n");
    printf("graph {\n");
    printSuffTree(root);
    printf("\tnbestNode [label = \"bestNode\"]\n");
    //printf("\tn%p -- nbestNode\n", bestRangeNode(root));
    printf("}\n");

    //printCharBlock(bestCharBlock(root, buffer));

    
    //freeSuffTree(root);
}

//#if 0
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
    
    
    determineBestPatterns(testBuffer, 15);
    
    /*
    
    InternalNode *root = createWeightedSuffixTree(testBuffer, 10);

    fillRepetitionsInTree(root);

    printf("Copy the following code to https://dreampuf.github.io/GraphvizOnline\n");
    printf("graph {\n");
    printSuffTree(root);
    printf("}\n");
    */

    return 1;
}
//#endif