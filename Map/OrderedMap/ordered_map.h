#ifndef _ORDERED_MAP_H_
#define _ORDERED_MAP_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "red_black_tree.h"


typedef struct _KeyValePair {
    void *pKey;
    void *pValue;
} KeyValuePair;


typedef struct _OrderedMap {
    RedBlackTree    *pTree;

    int           (*compare)  (const void*, const void*);
    void          (*destroy)  (void*);
    bool          (*put)      (struct _OrderedMap*, KeyValuePair*);
    KeyValuePair* (*get)      (struct _OrderedMap*, void*);
    bool          (*remove)   (struct _OrderedMap*, void*);
    bool          (*haskey)   (struct _OrderedMap*, void*);
} OrderedMap;


bool OrderedMapPut(OrderedMap *self, KeyValuePair *pPair);


KeyValuePair* OrderedMapGet(OrderedMap *self, void *pKey);


/**
 * The default function for key comparison.
 *
 * @param pSrc          The pointer to the source key.
 * @param pDst          The pointer to the target key.
 *
 * @return               1: The source key goes after the target key with certain ordering criteria.
 *                       0: The source key equals to the target key with certain ordering criteria.
 *                      -1: The source key goes before the target key with certain ordering criteria.
 */
int OrderedMapKeyCompare(const void *pSrc, const void *pTge);


/**
 * The default function for item deallocation.
 * 
 * @param pItem         The pointer to the item which is to be deallocated.
 */
void OrderedMapPairDestroy(void *pPair);


#endif
