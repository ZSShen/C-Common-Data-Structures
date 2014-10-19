#ifndef _SPARSE_TRIE_H_
#define _SPARSE_TRIE_H_


#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>


#define  CHAR_SET 62


typedef struct _TrieNode {
    bool   bLeaf;
    struct _TrieNode *pChildren[CHAR_SET + 1];
} TrieNode;


typedef struct _SparseTrie {
    TrieNode *pRoot;
    bool     bModeAutoInsert;

    bool (*search) (struct _SparseTrie*, char*);
    bool (*insert) (struct _SparseTrie*, char*);
    bool (*delete) (struct _SparseTrie*, char*);
    void (*set_auto_insert) (struct _SparseTrie*, bool);
} SparseTrie;


/* The macros to initialize and deinitialize the SparseTrie.  */
#define init_SparseTrie(p)      bool check; \
                                p = (SparseTrie*)malloc(sizeof(SparseTrie)); \
                                if (p != NULL) { \
                                    check = STrieInit(p); \
                                    if (check == false) { \
                                        free(p); \
                                    } \
                                } \


#define deinit_SparseTrie(p)    if (p != NULL) { \
                                    STrieDeinit(p); \
                                    free(p); \
                                }
                                

bool STrieInit(SparseTrie *pTrie);


bool STrieDeinit(SparseTrie *pTrie);


bool STrieSearch(SparseTrie *pTrie, char *szKey);


bool STrieInsert(SparseTrie *pTrie, char *szKey);


bool STrieDelete(SparseTrie *pTrie, char *szKey);


void STrieSetAutoInsert(SparseTrie *pTrie, bool bMode);


#endif
