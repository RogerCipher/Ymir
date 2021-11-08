#ifndef __PATTERNS_H__
#define __PATTERNS_H__

#define MAXCHARBUFFER 64000 //the file will be read in these blocks of space (in bytes)

#define EMPTYCHARVALUE 256 //value for chars that have been replaced in buffer

//struct for linked list containing ranges
typedef struct st_rangeLinkedList
{
    //next element in the list
    struct st_rangeLinkedList *next;
    //value of the range
    int range[2];
}RangeLL;

//a node that contains information about ranges and the next trieNode
typedef struct st_RangeTrieNode
{
    //storing the ranges of this node
    RangeLL *firstRange;
    //storing how many ranges there are
    int rangeCount;

    //storing sibling of thins range node
    struct st_RangeTrieNode *sibling;

    //storing next internal node if there is one
    struct st_InternalTrieNode *nextInternalNode;
}RangeNode;


typedef struct st_InternalTrieNode
{
    //suffix link for Ukkonens algorithm
    struct st_InternalTrieNode *suffixLink;
    //pointer to ranges that emerge from this node
    RangeNode *pathList;

}InternalNode;


#endif