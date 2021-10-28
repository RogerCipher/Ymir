#ifndef __PATTERNS_H__
#define __PATTERNS_H__

//struct to save blocks of characters
typedef struct charBlock
{
    struct charBlock *next;
    char *value;
    int len;
}CharBlock;

//struct for a node in the trie
typedef struct node
{
    struct node *parent;
    struct node *firstChild;

    struct node *leftBrother;
    struct node *rightBrother;

    //para os dados:
    int weight;
    int depth;
    char value;
} TrieNode;

//struct for a trie
typedef struct trieTree
{
    TrieNode *root;
    TrieNode *currentInsertNode;

    struct trieTree *next;
} TrieTree;




#endif