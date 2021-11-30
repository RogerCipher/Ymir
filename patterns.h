#ifndef __PATTERNS_H__
#define __PATTERNS_H__

#define MAXCHARBUFFER 64001 //the file will be read in these blocks of space (in bytes)

#define UNIQUECHARVALUE 256 //used for an unique value of char

#define DELETEDCHARVALUE 257 //used too replace a char value in the buffer that we deleted

//a node that contains information about ranges and the next trieNode
typedef struct st_RangeTrieNode
{
    //storing the range of this node
    int rangeStart;
    int *rangeEnd;


    //for storing the length of the suffix
    int lenghtOfSuffix;

    //storing how many times this range exists in buffer
    int weight;

    //storing sibling of this range node
    struct st_RangeTrieNode *sibling;

    //storing next internal node if there is one
    struct st_InternalTrieNode *nextInternalNode;

    //storing the previous node se we can determine 
    //the suffix length after
    struct st_InternalTrieNode *prevInternalNode;
}RangeNode;

//a node that contains information about ranges
typedef struct st_InternalTrieNode
{
    //suffix link for Ukkonens algorithm
    struct st_InternalTrieNode *suffixLink;
    //pointer to ranges that emerge from this node
    RangeNode *pathList;

    //storing the previous range so we can calculate 
    //suffix length after
    RangeNode *prevRangeNode;

}InternalNode;


typedef struct st_patternChar
{
    int value;
    struct st_patternChar *next;
}PatternChar;

typedef struct st_patternCharBlock
{
    PatternChar *pattern;
    int weight; //number or repetitions of the pattern in the buffer
}PatternCharBlock;


void determineBestPatterns(int *buffer, int bufferLen);
void printSuffTree(InternalNode *elemento);


#endif