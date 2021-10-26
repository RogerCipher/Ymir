#ifndef __HUFFMAN_H__
#define __HUFFMAN_H__


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
} TipoNode;

#endif