#ifndef __HUFFMAN_H__
#define __HUFFMAN_H__

#if 0
typedef struct Node
{
    //para a double linked list:
    struct Node *prev;
    struct Node *next;

    //para a arvore binaria:
    struct Node *parent;
    struct Node *leftChild;
    struct Node *rightChild;

    //para os dados:
    int weight;
    char value;
}TipoNode;

typedef struct st_charPattern
{
    char value;
    struct st_charPattern *nextValue;
}CharPattern;

typedef struct st_charPatternList
{
    int len;
    CharPattern *head;
}CharPatternList;
#endif




#endif