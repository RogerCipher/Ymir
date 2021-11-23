#ifndef __PATTERNS_H__
#define __PATTERNS_H__

#define MAXCHARBUFFER 64001 //the file will be read in these blocks of space (in bytes)

#define UNIQUECHARVALUE 256 //used for an unique value of char

//a node that contains information about ranges and the next trieNode
typedef struct st_RangeTrieNode
{
    //storing the range of this node
    int rangeStart;
    int *rangeEnd;

    //storing how many times this range repeats
    int repeats;

    //storing sibling of this range node
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

void printSuffTree(InternalNode *elemento);


#endif