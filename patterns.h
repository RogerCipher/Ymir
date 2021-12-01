#ifndef __PATTERNS_H__
#define __PATTERNS_H__

#define MAXCHARBUFFER 64001 //the file will be read in these blocks of space (in bytes)

#define MAXUNIQUECHARS 256 //the number of ascii characters

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

    //to use for locking when we read characters that are deleted
    int locked;

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
    PatternChar *pattern; //actual pattern stored here
    int weight; //number or repetitions of the pattern in the buffer
    int len; //length of the pattern

    struct st_patternCharBlock *next; //next pattern in list


    struct st_patternCharBlock *leftChild;
    struct st_patternCharBlock *rightChild;
}PatternCharBlock;

void printCharPattern(PatternChar *pattern);
void freeAllTries(TrieManager *manager);
void resetTries(TrieManager *manager, int lock);
void freeTrie(TrieNode *node);
void printCharBlock(PatternCharBlock *block);
void printAllTries(TrieManager *manager);
PatternCharBlock *bestPatternBlocks(int *buffer, int bufferLen);
void deleteOneWeight(TrieManager *manager);
//void determineBestPatterns(int *buffer, int bufferLen);
//void printSuffTree(InternalNode *elemento);


#endif