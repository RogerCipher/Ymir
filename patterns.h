#ifndef __PATTERNS_H__
#define __PATTERNS_H__

#define MAXCHARBUFFER 64000 //the file will be read in these blocks of space (in bytes)

#define EMPTYCHARVALUE 256 //value for chars that have been replaced in buffer

typedef struct st_rangeList
{
    int rangeStart;
    int *rangeEnd;
    struct st_rangeList *next;
}RangeList;

//a node that contains information about ranges and the next trieNode
typedef struct st_RangeTrieNode
{
    //storing the range of this node
    RangeList *ranges;

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


#endif