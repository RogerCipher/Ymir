#ifndef __PATTERNS_H__
#define __PATTERNS_H__

#define MAXCHARBUFFER 64001 //the file will be read in these blocks of space (in bytes)

#define MAXUNIQUECHARS 256

#define UNIQUECHARVALUE 256 //used for an unique value of char

#define DELETEDCHARVALUE 257 //used too replace a char value in the buffer that we deleted

typedef struct st_trieNode
{
    struct st_trieNode *parent;
    struct st_trieNode *next;
    struct st_trieNode *childList;

    //the char value of the node
    int value;

    //the ammounts of times this char was found
    int weight;

    //the length of the pattern untill this char
    int depth;

}TrieNode;

typedef struct st_trieManager
{
    //information about the 256 (possible) trie trees, 
    //stores the address of the current node we will inser to
    struct st_trieNode *trie[256];


}TrieManager;

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


//void determineBestPatterns(int *buffer, int bufferLen);
//void printSuffTree(InternalNode *elemento);


#endif