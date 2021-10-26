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

//TODO: INTEGRAR ISTO COM O TREE HANDLER DO TREE.C

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

    char currentChar = fgetc(ficheiro);
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
    TipoNode *prev, *next;

    while (1) {
        prev = (*cabeca);
        next = (*cabeca)->next;

        if (next == NULL) {    // se ja so ha 1 elemento esta feito
            return prev;
        }

        //criar o node pai
        TipoNode *parent = (TipoNode *)malloc(sizeof(TipoNode));

        parent->value = 0;
        parent->prev = prev;
        parent->next = next;
        parent->weight = prev->weight + next->weight;

        //refazer a linked list actualizada
        *cabeca = (*cabeca)->next->next;
        (*cabeca)->prev = NULL;
        next->next = NULL;
        
        printLL(*cabeca);

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
        printf("\tn%p [label = \"N/A\"]\n", elemento);
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
    if (elemento->prev != NULL) 
    {
        //imprimir recursivamente para cada filho
        printHuffTree(elemento->prev);    // imprimir primeiro filho

        if(elemento->next != NULL)
            printHuffTree(elemento->next);
    }
    if (elemento->parent == NULL) {
        //fim
        printf("}\n");
    }
}


int main(int argc, char *argv[])
{
    TipoNode *cabeca = NULL;
    createWeightedList(argv[1], &cabeca);
    TipoNode *raiz = huffmanTree(&cabeca);
    printLL(cabeca);
    printf("---------\n");
    //printHuffTree(raiz);

    printf("\nx\n");
    return 0;
}