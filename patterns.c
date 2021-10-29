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

//the file will be read in these blocks of space (in bytes):
#define MAXCHARBUFFER 64000

//this function returns the result of the heuristic of a node in a trie
int heuristicResult(TrieNode *node)
{
    if(node->weight == 1)
        return 1;
    return node->weight * node->depth;
}

//we call this when we want to add a child to a trie, it returns the child added
TrieNode *addWeightToChild(int childValue, TrieNode *parent)
{
    //first child
    if(parent->firstChild == NULL)
    {
        TrieNode *child = (TrieNode *)malloc(sizeof(TrieNode));
        if(child == NULL)
        {
            printf("failed to allocate memory\n");
            exit(1);
        } 
        child->parent = parent;
        child->value = childValue;
        child->weight = 1;
        child->depth = parent->depth +1;
        child->leftBrother = NULL;
        child->rightBrother = NULL;
        child->firstChild = NULL;
        parent->firstChild = child;
        return child;
    }

    TrieNode *iter = parent->firstChild;
    while (iter != NULL)
    {
        //theres a child with that char
        if(iter->value == childValue)
        {
            iter->weight++;
            return iter;
        }
        iter = iter->rightBrother;
    }

    //there isnt a child with that value yet
    iter = parent->firstChild;
    while (iter->rightBrother != NULL)
        iter = iter->rightBrother;


    TrieNode *child = (TrieNode *)malloc(sizeof(TrieNode));
    if(child == NULL)
    {
        printf("failed to allocate memory\n");
        exit(1);
    } 
    child->parent = parent;
    child->value = childValue;
    child->weight = 1;
    child->depth = parent->depth +1;
    child->leftBrother = iter;
    child->rightBrother = NULL;
    child->firstChild = NULL;
    iter->rightBrother = child;
    return child;
}


//this is for adding a char to the current state of a trie
int addCharToTries(int c, TrieTree **head)
{
    TrieTree *iter = *head;
    int charHasTrie = 0;
    
    while(iter != NULL)
    {
        if((*head)->root == NULL)
        {
            //first one
            break;
        }
        if(c == iter->root->value)
        {
            //see if we already have a trie starting by that value,
            charHasTrie = 1;
            if(iter->currentInsertNode == iter->root)
            {
                //if we do and we are on the root currently
                iter->root->weight++;
                iter->currentInsertNode = addWeightToChild(c, iter->currentInsertNode);
                iter->currentInsertNode = iter->root;
            }
            else
            {
                //if we do and are not on the root
                iter->currentInsertNode = iter->root;
                iter->root->weight++;
            }
        }
        else
        {
            //just add the new node to wherever we are right now
            iter->currentInsertNode = addWeightToChild(c, iter->currentInsertNode);
        }
        iter = iter->next;
    }

    if(!charHasTrie)
    {
        //we dont have a trie starting with this character:

        if((*head)->root == NULL)
        {
            //we dont even have any tries, create the first one:
            TrieNode *root = (TrieNode *)malloc(sizeof(TrieNode)); 
            if(root == NULL)
            {
                printf("failed to allocate memory\n");
                exit(1);
            } 
            root->firstChild = NULL;
            root->parent = NULL;
            root->leftBrother = NULL;
            root->rightBrother = NULL;
            root->value = c;
            root->weight = 1;
            root->depth = 1;
            (*head)->root = root;
            (*head)->currentInsertNode = root;
            (*head)->next = NULL;
            return 1;
        }


        //create a trie for this char
        TrieTree *newTrie = (TrieTree *)malloc(sizeof(TrieTree));
        newTrie->root = (TrieNode *)malloc(sizeof(TrieNode));
        if((newTrie == NULL) || (newTrie->root == NULL))
        {
            printf("failed to allocate memory\n");
            exit(1);
        } 
        
        newTrie->root->firstChild = NULL;
        newTrie->root->parent = NULL;
        newTrie->root->leftBrother = NULL;
        newTrie->root->rightBrother = NULL;
        newTrie->root->value = c;
        newTrie->root->weight = 1;
        newTrie->root->depth = 1;
 
        newTrie->next = NULL;
        newTrie->currentInsertNode = newTrie->root;



        //put the newly crated trie in the end of the trie list
        iter = *head;
        while (iter->next != NULL)
            iter = iter->next;
        iter->next = newTrie;

    }
    return 1;
}

//building tries from a file
int loadCharacters(char *fileName, TrieTree **head)
{
    /*---error handling---*/
    FILE *file = fopen(fileName, "rb");
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

    /*---char loading---*/
    printf("loading file into tries\n");
    while (currentChar != EOF)
    {
        addCharToTries(currentChar, head);
        currentChar = fgetc(file);
        /*
        if(currentChar < 0)
        {
            printf("something went terrible wrong : %d\n", currentChar);
            exit(1);
        }
        */
    }
    fclose(file);
    return 1;
}

//returns a pointer to the best possible heuristic of a node from a trie
TrieNode *bestHeuristic(TrieNode *node)
{
    //if the weight is 1 we stop looking
    if(node->weight == 1)
        return node;

    //if we lost score from the parent we also stop looking
    if(node->parent != NULL)
    {
        if(heuristicResult(node) < heuristicResult(node->parent))
            return node;
    }

    TrieNode *bestNode = node;
    TrieNode *possibleBetterNode;

    //check if any child has a better heuristic
    TrieNode *iter = node->firstChild;
    while (iter != NULL)
    {
        possibleBetterNode =  bestHeuristic(iter);
        if(heuristicResult(node) < heuristicResult(possibleBetterNode))
            bestNode = possibleBetterNode;
        iter = iter->rightBrother;
    }
    return bestNode;
}


//returns the best block of repeating chars from all the tries
CharBlock *bestCharBlock(TrieTree *head)
{
    if(head->root == NULL)
    {
        printf("attempt to calculate best char block from a trie without root");
        exit(1);
    }
       

    //first lets determine in what trie is the best match:
    TrieTree *iter = head;

    //initialize as the first one
    TrieNode *bestLastNode = bestHeuristic(head->root);
    TrieNode *possibleBetterNode;
    printf("initialized.. \n");
    iter = iter->next;

    while (iter != NULL)
    {
        if(iter->root == NULL)
        {
            printf("something went terribly terribly wrong\n");
            exit(1);
        }
        //check rest of the tries
        printf("checking: %d\n", (int)iter->root->value);
        possibleBetterNode = bestHeuristic(iter->root);
        
        if(heuristicResult(bestLastNode) < heuristicResult(possibleBetterNode))
        {
            printf("found a best one\n");
            bestLastNode = possibleBetterNode;
        }

        iter = iter->next;
    }

    printf("bestHeur calculated\n");
    
    //now that we have the best node we know that the char block is made
    //by going from its trie root to that node, or in other words:
    //we go from the best node to the parents and add them to the start of the block:

    CharBlock *lastChar = (CharBlock *)malloc(sizeof(CharBlock));
    if(lastChar == NULL)
    {
        printf("failed to allocate memory\n");
        exit(1);
    } 
    lastChar->breakHere = 1;
    lastChar->next = NULL;
    



    CharBlock *prevChar = (CharBlock *)malloc(sizeof(CharBlock));
    if(prevChar == NULL)
    {
        printf("failed to allocate memory\n");
        exit(1);
    } 
    prevChar->breakHere = 0;
    prevChar->next = lastChar;
    prevChar->value = bestLastNode->value;



    TrieNode *iterNode = bestLastNode->parent;
    while (iterNode != NULL)
    {
        CharBlock *currentChar = (CharBlock *)malloc(sizeof(CharBlock));
        if(currentChar == NULL)
        {
            printf("failed to allocate memory\n");
            exit(1);
        }
        currentChar->breakHere = 0;
        currentChar->next = prevChar;
        currentChar->value = iterNode->value;

        iterNode = iterNode->parent;
        prevChar = currentChar;
    }

    return prevChar;
}


/*--------- freeing memory ---------------*/


int freeTrieNode(TrieNode *node)
{
    //free all of its children
    TrieNode *iter = node->firstChild;
    TrieNode *next;
    while (iter != NULL)
    {
        next = iter->rightBrother;
        freeTrieNode(iter);
        iter = next;
    }

    //free the node itself
    free(node);
    node = NULL;
    return 1;
}

int freeTries(TrieTree *head)
{
    TrieTree *iter = head;
    TrieTree *next;
    while (iter!= NULL)
    {
        next = iter->next;
        freeTrieNode(iter->root);
        free(iter);
        iter = NULL;
        iter = next;
    }
    return 1;
}

/*------------ printing data -------*/
void printCharBlock(CharBlock *head)
{
    printf("-------");
    printf("char block: \n\"");
    CharBlock *iter = head;
    while (!iter->breakHere)
    {
        printf("%c", iter->value);
        iter = iter->next;
    }
    printf("\"\n");
    printf("---------\n");
}

void printGraphviz(TrieNode *elem)
{

    if(elem->weight == 1)
        return;

    if((int)elem->value < 0)
    {
        printf("wtf \n");
        exit(1);
    }
    if (((int)elem->value < 32) || ((int)elem->value > 126)) 
    {
        //elemento nao tem valor
        printf("\tn%p [label = \"ascii number = %d : %d\"]\n", elem, (int)elem->value, elem->weight);
    } 
    else 
    {
        //elemento tem valor
        printf("\tn%p [label = \"%c : %d\"]\n", elem, elem->value, elem->weight);
    }
        
    if (elem->parent != NULL) 
    {
        //criar ligacao do pai ao filho
        printf("\tn%p -- n%p\n", elem->parent, elem);
    }
    if (elem->firstChild != NULL) 
    {
        //imprimir recursivamente para cada filho
        TrieNode *iterador = elem->firstChild;
        TrieNode *next;
        while (iterador != NULL) {
            next = iterador->rightBrother;
            
            printGraphviz(iterador);
            iterador = next;
        }
    }
}

void printTriesGrapviz(TrieTree **head)
{
    TrieTree *iter = *head;

    printf("Copy the following code to https://dreampuf.github.io/GraphvizOnline\n");
    printf("graph {\n");

    while (iter != NULL)
    {
        printGraphviz(iter->root);
        iter = iter->next;
    }

    printf("}\n");
}


/*--------------- main -------------*/
int main(int argc, char *argv[])
{
    TrieTree *head = (TrieTree *)malloc(sizeof(TrieTree));
    printf("Loading characters from file..\n");
    loadCharacters(argv[1], &head);
    printf("---------\n");

    printf("Printing Tries..\n");
    printTriesGrapviz(&head);

    printf("---------\n");
    printf("making best block..\n");
    CharBlock *block = bestCharBlock(head);
    printf("lets print it..\n");
    printCharBlock(block);

    freeTries(head);
    return 1;
}