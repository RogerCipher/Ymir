#ifndef __PATTERNS_H__
#define __PATTERNS_H__


typedef struct node
{
    struct node *parent;
    struct node *firstChild;

    struct node *leftBrother;
    struct node *rightBrother;

    //para os dados:
    int weight;
    char value;
} TrieNode;

typedef struct trieTree
{
    TrieNode *root;
    TrieNode *currentInsertNode;

    struct trieTree *next;
    int depth;
} TrieTree;




#endif