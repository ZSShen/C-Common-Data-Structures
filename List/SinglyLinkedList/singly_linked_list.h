#ifndef _SINGLY_LINKED_LIST_H_
#define _SINGLY_LINKED_LIST_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

/* Wrapper for singly linked list initialization. */
#define SinglyLinkedList_init(p)    p = NULL; \
                                    p = malloc(sizeof(SinglyLinkedList)); \
                                    if (p != NULL) \
                                        SLListInit(p);

/* Wrapper for singly linked list deinitialization. */
#define SinglyLinkedList_deinit(p)  if (p != NULL) { \
                                        SLListDeinit(p); \
                                        free(p); \
                                        p = NULL; \
                                    }

typedef struct _SinglyLinkedNode {
    void    *pItem;
    struct _SinglyLinkedNode *pNext;    
} SinglyLinkedNode;


typedef struct _SinglyLinkedList {
    int              iSize;
    SinglyLinkedNode *pHead, *pTail;
    
    int               (*compare) (const void*, const void*);
    void              (*destroy) (void*);
    SinglyLinkedNode* (*append)  (struct _SinglyLinkedList*, void*);
    SinglyLinkedNode* (*insert)  (struct _SinglyLinkedList*, int, void*);
    void              (*concate) (struct _SinglyLinkedList*, struct _SinglyLinkedList*);
    void*             (*remove)  (struct _SinglyLinkedList*, void*);
    void              (*reverse) (struct _SinglyLinkedList*); 
    void*             (*pop)     (struct _SinglyLinkedList*, int);
} SinglyLinkedList;


/* Constructor for singly linked list. */
void SLListInit(SinglyLinkedList*);


/* Destructor for singly linked list. */
void SLListDeinit(SinglyLinkedList*);


/**
 * This function appends an item to the tail of the list.
 *
 * @param self          The pointer to the SinglyLinkedList structure.
 * @param pItem         The pointer to the item which is to be appended to the list.
 *
 * @return              Non-NULL: The pointer to the successfully inserted node containing the requested item.
 *                      NULL    : Fail to insert the requested item due to unsuccessful memory allocation
 */
SinglyLinkedNode* SLListAppend(SinglyLinkedList *self, void *pItem);


/**
 * This function inserts an item to the designated position of the list.
 *
 * @param self          The pointer to the SinglyLinkedList structure.
 * @param idx           The designated position.
 * @param pItem         The pointer to the item which is to be inserted to the list.
 *
 * @return              Non-NULL: The pointer to the successfully inserted node containing the requested item.
 *                      NULL    : Fail to insert the requested item due to unsuccessful memory allocation or
 *                                the illegally designated position.
 */
SinglyLinkedNode* SLListInsert(SinglyLinkedList *self, int idx, void *pItem);


/**
 * This function concates the designated list to the tail of "this" list.
 *
 * @param self          The pointer to the SinglyLinkedList structure.
 * @param pSrc          The pointer to the designated SinglyLinkedList structure.
 */
void SLListConcate(SinglyLinkedList *self, SinglyLinkedList *pSrc);


/**
 * This function removes the first item with the key equal to the designated item from the list,
 * and returns it.
 *
 * @param self          The pointer to the SinglyLinkedList structure.
 * @param pSrc          The pointer to the designated item.
 *
 * @return              Non-NULL: The pointer to the removed item.
 *                      NULL    : There is no item having the same key with the designated item, and thus the
 *                                remove operation fails.
 */
void* SLListRemove(SinglyLinkedList *self, void *pItem);


/**
 * This function reverses the list in place.
 *
 * @param self          The pointer to the SinglyLinkedList structure.
 */
void SLListReverse(SinglyLinkedList *self);


/**
 * This function removes the item from the designated position of the list and returns it.
 *
 * @param self          The pointer to the SinglyLinkedList structure.
 * @param idx           The designated position.
 *
 * @return              Non-NULL: The pointer to the removed item.
 *                      NULL    : Fail to remove the item due to the illegally designated position.
 */
void* SLListPop(SinglyLinkedList *self, int idx);


/**
 * The default function for node item comparison.
 *
 * @param pSrc          The pointer to the source item.
 * @param pDst          The pointer to the target item.
 *
 * @return               1: The key contained by source item is larger than the target one.
 *                       0: The key contained by source item is equal to the target one.
 *                      -1: The key contained by source item is less than the target one.
 */
int SLListCompare(const void *pSrc, const void *pTge);


/**
 * The default function for item deallocation.
 * 
 * @param pItem         The pointer to the item which is to be deallocated.
 */
void SLListDestroy(void *pItem);

#endif
