#ifndef _BINOMIAL_HEAP_H_
#define _BINOMIAL_HEAP_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


/* Wrapper for BinomialHeap initialization. */
#define BinomialHeap_init(p)        p = NULL; \
                                    p = (BinomialHeap*)malloc(sizeof(BinomialHeap)); \
                                    if (p != NULL) { \
                                        bool check; \
                                        check = BinomialHeapInit(p); \
                                        if (check == false) { \
                                            free(p); \
                                            p = NULL; \
                                        } \
                                    }


/* Wrapper for BinomialHeap deinitialization. */
#define BinomialHeap_deinit(p)      if (p != NULL) { \
                                        BinomialHeapDeinit(p); \
                                        free(p); \
                                        p = NULL; \
                                    }

typedef struct _HeapNode {
    void *pItem;
    unsigned long ulDegree;
    struct _HeapNode *pParent;
    struct _HeapNode *pChild;
    struct _HeapNode *pSibling;
} HeapNode;


typedef struct _BinomialHeap {
    bool          (*insert)   (struct _BinomialHeap*, void*);
    void*         (*delete)   (struct _BinomialHeap*);
    void*         (*top)      (struct _BinomialHeap*);
    bool          (*change)   (struct _BinomialHeap*, int, void*);
    unsigned long (*size)     (struct _BinomialHeap*);

    void          (*set_compare) (struct _BinomialHeap*, int (*) (const void*, const void*));
    void          (*set_update)  (struct _BinomialHeap*, void (*) (const void*, const void*));
    void          (*set_destroy) (struct _BinomialHeap*, void (*) (void*));    
} BinomialHeap;


bool BinomialHeapInit(BinomialHeap *self);


void BinomialHeapDeinit(BinomialHeap *self);


/**
 * This function inserts an item to the appropriate position of the heap. 
 *
 * @param self      The pointer to the BinomialHeap structure.
 * @param pItem     The pointer to the item which is to be inserted.
 * 
 * @return          true : The item is inserted successfully.
 *                  false: The item cannot be inserted due to unenough memory space.
 */
bool BinomialHeapInsert(BinomialHeap *self, void *pItem);


/**
 * This function deletes the root node of the heap and returns the item storing at that node correspondingly.
 *
 * @param self      The pointer to the BinomialHeap structure.
 * 
 * @return          Non-NULL: The pointer to the returned item.
 *                  NULL    : The heap is empty, and thus nothing is returned.
 */
void* BinomialHeapDelete(BinomialHeap *self);


/**
 * This function returns the item stored in the top of the heap.
 *
 * @param self      The pointer to the BinomialHeap structure.
 *
 * @return          Non-NULL: The pointer to the returned item.
 *                  NULL    : The heap is empty, and thus nothing is returned.
 */
void* BinomialHeapTop(BinomialHeap *self);


/**
 * This function updates the content of the designated item.
 * Note that the updating must not violate the heap property. 
 *
 * @param self      The pointer to the BinomialHeap structure.
 * @param idx       The index to the item which is to be updated.
 * @param pTge      The pointer to the item containing new data.
 *
 * @return          true : The data updating is done successfully.
 *                  false: 1. The designated item cannot be found.
 *                         2. The data updating violate the heap property.
 */
bool BinomialHeapChange(BinomialHeap *self, int idx, void *pTge);


/**
 * This function returns the size of the heap.
 *
 * @param self      The pointer to the BinomialHeap structure.
 *
 * @return          ulSize: The size of the heap.
 */
unsigned long BinomialHeapSize(BinomialHeap *self);


/**
 * This function sets the user-defined item comparison strategy for the heap.
 *
 * @param self          The pointer to the BinomialHeap structure.
 * @param pFunc         The pointer to the customized function.
 */
void BinomialHeapSetCompare(BinomialHeap *self, int (*pFunc)(const void*, const void*));


/**
 * This function sets the user-defined item deallocation strategy for the heap.
 *
 * @param self          The pointer to the BinomialHeap structure.
 * @param pFunc         The pointer to the customized function.
 */
void BinomialHeapSetDestroy(BinomialHeap *self, void (*pFunc)(void*));


/**
 * This function sets the user-defined item content updating strategy for the heap.
 *
 * @param self          The pointer to the BinomialHeap structure.
 * @param pFunc         The pointer to the customized function.
 */
void BinomialHeapSetUpdate(BinomialHeap *self, void (*pFunc)(const void*, const void*));


/*===========================================================================*
 *                 The interface definition of plugin                        *
 *===========================================================================*/
#ifdef BUILD_PLUGIN
bool init();

void deinit();

bool insert(void*);

void* delete();

void* top();

unsigned long size();

void set_compare(int(*)(const void*, const void*));

void set_destroy(void(*)(void*));
#endif

#endif
