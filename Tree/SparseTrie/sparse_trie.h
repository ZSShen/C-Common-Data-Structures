#ifndef _SPARSE_TRIE_H_
#define _SPARSE_TRIE_H_


#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>


/* The initial support character sets are: 0-9, a-z, and A-Z. */
#define  CHAR_SET 62


/* Wrapper for SparseTrie initialization. */
#define SparseTrie_init(p)      bool check; \
                                p = (SparseTrie*)malloc(sizeof(SparseTrie)); \
                                if (p != NULL) { \
                                    check = STrieInit(p); \
                                    if (check == false) { \
                                        free(p); \
                                    } \
                                } \


/* Wrapper for SparseTrie deinitialization.  */
#define SparseTrie_deinit(p)    if (p != NULL) { \
                                    STrieDeinit(p); \
                                    free(p); \
                                }


typedef struct _TrieNode {
    int    iCountChildren;
    bool   bKey;
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

                                
/* The constructor for SparseTrie. */
bool STrieInit(SparseTrie *pTrie);


/* The destructor for SparseTrie. */
bool STrieDeinit(SparseTrie *pTrie);


/**
 * This function searches for the given key.
 *
 * @param self      The pointer to SparseTrie.
 * @param szKey     The given key string. 
 *
 * @return          true : The key exists.
 *                  false: The key does not exist.
 */
bool STrieSearch(SparseTrie *self, char *szKey);


/**
 * This function inserts the given key into the trie.
 *
 * @param self      The pointer to SparseTrie.
 * @param szKey     The given key string.
 *
 * @return          true : Insertion is done successfully.
 *                  false: The key cannot be inserted due to insufficient memory.
 */
bool STrieInsert(SparseTrie *self, char *szKey);


/**
 * This function deletes the given key from the trie.
 * 
 * @param self      The pointer to SparseTrie.
 * @param szKey     The given key string.
 *
 * @return          true : Deletion is done successfully.
 *                  false: The key does not exist.
 */
bool STrieDelete(SparseTrie *self, char *szKey);


/**
 * This function sets the flag for auto insertion for search function.
 * (Note: The key will be automatically inserted if the search fails.)
 *
 * @param self      The pointer to SparseTrie.
 * @param bMode     true : Turn on auto insertion.
 *                  false: Turn off auto insertion.
 */
void STrieSetAutoInsert(SparseTrie *pTrie, bool bMode);


#endif
