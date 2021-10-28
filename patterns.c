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


//we call this when we want to add a child to a trie, it returns the child added
TrieNode *addWeightToChild(char childValue, TrieNode *parent)
{
    printf("addWeightToChild()\n");
    //first child
    if(parent->firstChild == NULL)
    {
        TrieNode *child = (TrieNode *)malloc(sizeof(TrieNode));
        child->parent = parent;
        child->value = childValue;
        child->weight = 1;
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
    child->parent = parent;
    child->value = childValue;
    child->weight = 1;
    child->leftBrother = iter;
    child->rightBrother = NULL;
    child->firstChild = NULL;
    iter->rightBrother = child;
    return child;
    
    
}


//this is for adding a char to the current state of a trie
int addCharToTries(char c, TrieTree **head)
{
    printf("addCharToTries(%c)\n", c);
    TrieTree *iter = *head;
    int charHasTrie = 0;
    
    while(iter != NULL)
    {
        if((*head)->root == NULL)
        {
            printf("(*head)->root == NULL\n");
            //first one
            break;
        }
        if(c == iter->root->value)
        {
            printf("%c == %c\n", c, iter->root->value);
            //see if we already have a trie starting by that value,
            charHasTrie = 1;
            if(iter->currentInsertNode == iter->root)
            {
                printf("    yes\n");
                //if we do and we are on the root currently
                iter->root->weight++;
                iter->currentInsertNode = addWeightToChild(c, iter->currentInsertNode);
                iter->currentInsertNode = iter->root;
            }
            else
            {
                printf("    no\n");
                //if we do and are not on the root
                iter->currentInsertNode = iter->root;
                iter->root->weight++;
            }
        }
        else
        {
            printf("%c != %c\n", c, iter->root->value);
            //just add the new node to wherever we are right now
            iter->currentInsertNode = addWeightToChild(c, iter->currentInsertNode);
        }
        iter = iter->next;
    }

    if(!charHasTrie)
    {
        printf("!charHasTrie\n");
        if((*head)->root == NULL)
        {
            TrieNode *root = (TrieNode *)malloc(sizeof(TrieNode));   
            root->firstChild = NULL;
            root->parent = NULL;
            root->leftBrother = NULL;
            root->rightBrother = NULL;
            root->value = c;
            root->weight = 1;
            (*head)->root = root;
            (*head)->currentInsertNode = root;
            (*head)->next = NULL;
            (*head)->depth = 1;
            return 1;
        }


        //if we didnt found a trie starting by that char, we create it
        TrieTree *newTrie = (TrieTree *)malloc(sizeof(TrieTree));
        newTrie->root = (TrieNode *)malloc(sizeof(TrieNode));
        
        newTrie->root->firstChild = NULL;
        newTrie->root->parent = NULL;
        newTrie->root->leftBrother = NULL;
        newTrie->root->rightBrother = NULL;
        newTrie->root->value = c;
        newTrie->root->weight = 1;

        newTrie->depth = 0;
        newTrie->next = NULL;
        newTrie->currentInsertNode = newTrie->root;




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
    printf("loadCharacters()\n");
    /*---error handling---*/
    FILE *file = fopen(fileName, "r");
    if(file == NULL)
    {
        printf("Couldnt find file\n");
        exit(1);
    }

    char currentChar = fgetc(file);
    if(currentChar == EOF)
    {
        printf("File was empty\n");
        exit(1);
    }

    /*---char loading---*/

    while (currentChar != EOF)
    {
        addCharToTries(currentChar, head);
        currentChar = fgetc(file);
    }
    fclose(file);
    return 1;
}



int cutDownBranch(TrieNode *branch)
{
    if(branch == NULL)
        return 0;

    //remove all children from this node
    TrieNode *iter = branch->firstChild;
    TrieNode *next;
    while (iter != NULL)
    {
        next = iter->rightBrother;
        cutDownBranch(iter);
        iter = next;
    }
    branch->firstChild = NULL;


    //remove contection from parent to this branch
    if(branch->leftBrother == NULL)
    {
        //this branch was its parents first child
        branch->parent->firstChild = branch->rightBrother;
        if(branch->rightBrother != NULL)
            branch->rightBrother->leftBrother = NULL;
    }
    else
    {
        branch->leftBrother->rightBrother = branch->rightBrother;
        if(branch->rightBrother != NULL)
            branch->rightBrother->leftBrother = branch->leftBrother;
    }

    //finally free this node
    free(branch);
    branch = NULL;
    return 1;
}


int cutDownUnwanted(TrieNode *branch)
{

    if(branch->weight == 1)
    {
        cutDownBranch(branch);
        /*
        //remove contection from parent to this branch
        if(branch->leftBrother == NULL)
        {
            //this branch was its parents first child
            branch->parent->firstChild = branch->rightBrother;
            if(branch->rightBrother != NULL)
                branch->rightBrother->leftBrother = NULL;
        }
        else
        {
            branch->leftBrother->rightBrother = branch->rightBrother;
            branch->rightBrother->leftBrother = branch->leftBrother;
        }
        */
       return 1;
    }

    TrieNode *iter = branch->firstChild;
    TrieNode *next;
    while (iter != NULL)
    {
        next = iter->rightBrother;
        cutDownUnwanted(iter);
        iter = next;
    }
    return 0;
}

//we do this to cut down branches that dont interest us in the current Tries (aka everything with weight of 1)
int cutDownAllUnwanted(TrieTree *head)
{
    TrieTree *iter = head;
    TrieTree *next;
    while (iter!= NULL)
    {
        next = iter->next;
        cutDownUnwanted(iter->root);
        iter = next;
    }
    return 1;
}

void printGraphviz(TrieNode *elem)
{

    if (elem->value == ' ') 
    {
        //elemento nao tem valor
        printf("\tn%p [label = \"N/A\"]\n", elem);
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

int main(int argc, char *argv[])
{
    TrieTree *head = (TrieTree *)malloc(sizeof(TrieTree));
    loadCharacters(argv[1], &head);
    printf("---------\n");
    printTriesGrapviz(&head);
    cutDownAllUnwanted(head);
    printf("---------\n");
    printTriesGrapviz(&head);


    return 1;
}