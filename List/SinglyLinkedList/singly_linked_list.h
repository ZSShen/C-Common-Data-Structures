#ifndef _SINGLY_LINKED_LIST_H_
#define _SINGLY_LINKED_LIST_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

/* Wrapper for SinglyLinkedList initialization. */
#define SinglyLinkedList_init(p)    p = NULL; \
                                    p = (SinglyLinkedList*)malloc(sizeof(SinglyLinkedList)); \
                                    if (p != NULL) \
                                        SLListInit(p);

/* Wrapper for SinglyLinkedList deinitialization. */
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
    SinglyLinkedNode* (*insert)  (struct _SinglyLinkedList*, unsigned long, void*);
    void*             (*remove)  (struct _SinglyLinkedList*, void*);
    void              (*reverse) (struct _SinglyLinkedList*); 
    void*             (*pop)     (struct _SinglyLinkedList*, unsigned long);
    bool              (*search)  (struct _SinglyLinkedList*, void*);
} SinglyLinkedList;


/* Constructor for SinglyLinkedList structure. */
void SLListInit(SinglyLinkedList*);


/* Destructor for SinglyLinkedList structure. */
void SLListDeinit(SinglyLinkedList*);


/**
 * This function appends a node storing the requested item to the tail of the list.
 *
 * @param self          The pointer to the SinglyLinkedList structure.
 * @param pItem         The pointer to the item which is to be appended to the list.
 *
 * @return              Non-NULL: The pointer to the successfully inserted node.
 *                      NULL    : The node cannot be inserted due to insufficient memory space.
 */
SinglyLinkedNode* SLListAppend(SinglyLinkedList *self, void *pItem);


/**
 * This function inserts a node storing the requested item to the designated index of the list.
 *
 * @param self          The pointer to the SinglyLinkedList structure.
 * @param ulIndex       The designated index.
 * @param pItem         The pointer to the item which is to be inserted to the list.
 *
 * @return              Non-NULL: The pointer to the successfully inserted node.
 *                      NULL    : The node cannot be inserted due to insufficient memory space or
 *                                illegally designated index.
 */
SinglyLinkedNode* SLListInsert(SinglyLinkedList *self, unsigned long ulIndex, void *pItem);


/**
 * This function returns the first item with the logical order equal to the designated item from the list,
 * and removes it.
 *
 * @param self          The pointer to the SinglyLinkedList structure.
 * @param pSrc          The pointer to the designated item.
 *
 * @return              Non-NULL: The pointer to the returned item.
 *                      NULL    : There is no item having the same logical order with the designated one, and thus
 *                                nothing is returned.
 */
void* SLListRemove(SinglyLinkedList *self, void *pItem);


/**
 * This function reverses the list in place.
 *
 * @param self          The pointer to the SinglyLinkedList structure.
 */
void SLListReverse(SinglyLinkedList *self);


/**
 * This function returns the item from the designated index of the list and removes it.
 *
 * @param self          The pointer to the SinglyLinkedList structure.
 * @param ulIndex       The designated index.
 *
 * @return              Non-NULL: The pointer to the returned item.
 *                      NULL    : No item can be returned due to illegally designated index.
 */
void* SLListPop(SinglyLinkedList *self, unsigned long idx);


/**
 * This function checks whethere the list has the designated item.
 *
 * @param self          The pointer to the SinglyLinkedList structure.
 * @param pItem         The pointer to the item which is to be checked.
 *
 * @return              true : The item exists.
 *                      false: The item does not exist.
 */
bool SLListSearch(SinglyLinkedList *self, void *pItem);

#endif
