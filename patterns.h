#ifndef __PATTERNS_H__
#define __PATTERNS_H__

#define MAXCHARBUFFER 64000 //the file will be read in these blocks of space (in bytes)

#define EMPTYCHARVALUE 256 //value for chars that have been replaced in buffer

//for storing each char of a pattern of chars
typedef struct st_SuffixWeight
{
    struct st_patternWeight *next;
    int indexs[2];
}SuffixWeight;

//for storing patterns of chars found
typedef struct st_SuffixTree
{
    SuffixWeight *weight;

    SuffixTree *childList;
    SuffixTree *next;
}SuffixTree;

#endif