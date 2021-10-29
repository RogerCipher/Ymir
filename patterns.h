#ifndef __PATTERNS_H__
#define __PATTERNS_H__

//struct to save blocks of characters
typedef struct st_charBlock
{
    struct st_charBlock *next;
    int value;

    //used to signal if this is the end of one block and start of other
    int breakHere;
}CharBlock;

//struct for a node in the trie
typedef struct st_node
{
    struct st_node *parent;
    struct st_node *firstChild;

    struct st_node *leftBrother;
    struct st_node *rightBrother;

    //para os dados:
    int weight;
    int depth;
    int value;
} TrieNode;

//struct for a trie
typedef struct st_trieTree
{
    TrieNode *root;
    TrieNode *currentInsertNode;

    struct st_trieTree *next;
} TrieTree;

int freeTrieNode(TrieNode *node);
int freeTries(TrieTree *head);


#endif