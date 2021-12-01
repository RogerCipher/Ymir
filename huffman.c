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

#include "huffman.h"
#include "patterns.h"


int printLL(TipoNode *cabeca)
{
    TipoNode *iterador = NULL;
    iterador = cabeca;
    while (iterador != NULL)
    {
        printf("valor: %c, peso: %d\n", iterador->value, iterador->weight);
        iterador = iterador->next;
    }
    return 1;
    
}



//inserir um elemento da linked list no sitio certo dependendo do weight
int insertInWeightedList(TipoNode **cabeca, TipoNode *elemento)
{
    if (*cabeca == NULL) {    // a linked list esta vazia
        *cabeca = elemento;
        return 1;
    }

    TipoNode *iterador = *cabeca;
    TipoNode *cauda = *cabeca;


    while(iterador != NULL) {
        cauda = iterador;
        if (iterador->weight > elemento->weight) { // Insert before
            elemento->next = iterador;
            elemento->prev = iterador->prev;
            iterador->prev = elemento;
            if (elemento->prev == NULL) {
                *cabeca = elemento;
            } else {
                elemento->prev->next = elemento;
            }
            return 1;
        }
        iterador = iterador->next;
    }

    // Add to list
    cauda->next = elemento;
    elemento->prev = cauda;

    return 0;
}

int createWeightedList(char *nomeFicheiro, TipoNode **cabeca)
{
    FILE *ficheiro = fopen(nomeFicheiro, "r");
    if(ficheiro == NULL)
    {
        printf("Couldnt find file\n");
        exit(1);
    }

    int currentChar = fgetc(ficheiro);
    if(currentChar == EOF)
    {
        printf("File was empty\n");
        exit(1);
    }

    //temp array to store weight of char
    int weights[256];

    for(int i = 0; i < 256; i++)
        //inicialize all weights at 0
        weights[i] = 0;


    while (currentChar != EOF)
    {
        //o peso do char sera guardado na posicao (int)char
        weights[(int)currentChar]++;
        currentChar = fgetc(ficheiro);
    }
    
    fclose(ficheiro);

    for(int i = 0; i < 256; i++)
    {
        if(weights[i] != 0)
        {
            TipoNode *elemento = (TipoNode *)malloc(sizeof(TipoNode));
            elemento->value = (char)i;
            elemento->weight = weights[i];
            elemento->prev = NULL;
            elemento->next = NULL;
            elemento->parent = NULL;
            insertInWeightedList(cabeca, elemento);
        }
    }



    printf("EOF\n");

    

    return 1;
}

//recebe uma linked list e retorna uma arvore
TipoNode *huffmanTree(TipoNode **cabeca) {
    TipoNode *left, *right;

    while (1) {
        left = (*cabeca);
        right = (*cabeca)->next;


        if (right == NULL) {    // se ja so ha 1 elemento esta feito
            printf("bateu1\n");
            return left;
        }

        //criar o node pai e preenchelo
        TipoNode *parent = (TipoNode *)malloc(sizeof(TipoNode));
        parent->value = 0;
        parent->prev = NULL;
        parent->next = NULL;
        parent->leftChild = left;
        parent->rightChild = right;
        parent->weight = left->weight + right->weight;

        //ligar os childs ao pai
        parent->leftChild->parent = parent;
        parent->rightChild->parent = parent;

        //refazer a linked list actualizada
        if(right->next == NULL)
        {
            printf("bateu2\n");
            *cabeca = parent;
            return parent;
        }
        *cabeca = right->next;
        (*cabeca)->prev = NULL;
        right->next = NULL;


        /*

        printf("prev: %d : %d\n", prev->value, prev->weight);
        printf("next: %d: %d\n", next->value, next->weight);
        printf("---\n");

        */
        insertInWeightedList(cabeca, parent);
    }
}



void printHuffTree(TipoNode *elemento)
{
    if (elemento->parent == NULL) 
    {
        //estamos na raiz
        printf("Copy the following code to https://dreampuf.github.io/GraphvizOnline\n");
        printf("graph {\n");
    }
    if (elemento->value == 0) 
    {
        //elemento nao tem valor
        printf("\tn%p [label = \"N/A : %d\"]\n", elemento, elemento->weight);
    } 
    else 
    {
        //elemento tem valor
        printf("\tn%p [label = \"%c : %d\"]\n", elemento, elemento->value, elemento->weight);
    }
        
    if (elemento->parent != NULL) 
    {
        //criar ligacao do pai ao filho
        printf("\tn%p -- n%p\n", elemento->parent, elemento);
    }
    if (elemento->leftChild != NULL) 
    {
        //imprimir recursivamente para cada filho
        printHuffTree(elemento->leftChild);    // imprimir primeiro filho

        if(elemento->rightChild != NULL)
            printHuffTree(elemento->rightChild);
    }
    if (elemento->parent == NULL) {
        //fim
        printf("}\n");
    }
}


int loadFileInBlocks(char *fileName)
{

    FILE *file = fopen(fileName, "r");
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

    int len = 0;
    //int allBlocksLoaded = 0;
    int blockBuffer[MAXCHARBUFFER];

    while(currentChar > 0)
    {
        for(len = 0; len < MAXCHARBUFFER; len++)
        {

            blockBuffer[len] = currentChar;

            currentChar = fgetc(file);
            if(currentChar < 0)
                break;
        }
        
        printf("\n\n---------block patterns--------------\n\n");
        bestPatternBlocks(blockBuffer, len+1);
        printf("\n\n-------------------------------------\n\n");
        if(currentChar < 0)
        {
            break;
        }
        currentChar = fgetc(file);
            
    }


    fclose(file);

    return 1;
}




int main(int argc, char *argv[])
{

    loadFileInBlocks(argv[1]);

    /*
    TipoNode *cabeca = NULL;
    createWeightedList(argv[1], &cabeca);
    printLL(cabeca);
    TipoNode *raiz = huffmanTree(&cabeca);

    printf("---------\n");
    printHuffTree(raiz);

    printf("\nx\n");
    */
    return 0;
}