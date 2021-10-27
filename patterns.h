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


/*
        numa tree(por exemplo a tree value="a")
if (charToInsert == root->value) && (currentInsertNode == root):
    root->weight++;
    currentInsertNode = filho(root, value = charToInsert);
    currentInsertNode->weight++;

    //aka se quisermos adicionar um "a" na arvore com o valor de root "a" e
    //o current insert node for a raiz da arvore vamos incrementar o peso
    //da raiz, criar (ou mover) para o node filho com o mesmo valor ("a")
    //e incrementar (ou igualar a 1) o filho 

else if (charToInsert == root->value):
    currentInsertNode = root;
    root->weight++;


    //aka se quisermos adicionar um "a" na arvore com o valor de root "a"
    //voltamos a raiz da arvore e incrementamos o weight da raiz
else:
    currentInsertNode = filho(root, value = charToInsert);
    currentInsertNode->weight++;

    //para qualquer outro filho vamos simplesmente descer (ou criar)
    //um filho do node que estamos actualmente com o valor "charToInsert"
    //e incrementar o seu weight por 1
*/







#endif