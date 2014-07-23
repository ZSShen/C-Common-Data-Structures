#ifndef _BINARY_HEAP_H_
#define _BINARY_HEAP_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


/* Wrapper for BinaryHeap initialization. */
#define BinaryHeap_init(p)      p = NULL; \
                                p = (BinaryHeap*)malloc(sizeof(BinaryHeap)); \
                                if (p != NULL) { \
                                    bool check; \
                                    check = BinaryHeapInit(p); \
                                    if (check == false) { \
                                        free(p); \
                                        p = NULL; \
                                    } \
                                }


/* Wrapper for BinaryHeap deinitialization. */
#define BinaryHeap_deinit(p)    if (p != NULL) { \
                                    BinaryHeapDeinit(p); \
                                    free(p); \
                                    p = NULL; \
                                }


typedef struct _BinaryHeap {
    bool          (*insert)   (struct _BinaryHeap*, void*);
    void*         (*delete)   (struct _BinaryHeap*);
    void*         (*top)      (struct _BinaryHeap*);
    bool          (*change)   (struct _BinaryHeap*, int, void*);
    unsigned long (*size)     (struct _BinaryHeap*);

    void          (*set_compare) (struct _BinaryHeap*, int (*) (const void*, const void*));
    void          (*set_update)  (struct _BinaryHeap*, void (*) (const void*, const void*));
    void          (*set_destroy) (struct _BinaryHeap*, void (*) (void*));    
} BinaryHeap;


bool BinaryHeapInit(BinaryHeap *self);


void BinaryHeapDeinit(BinaryHeap *self);


/**
 * This function inserts an item to the appropriate position of the heap. 
 *
 * @param self      The pointer to the BinaryHeap structure.
 * @param pItem     The pointer to the item which is to be inserted.
 * 
 * @return          true : The item is inserted successfully.
 *                  false: The item cannot be inserted due to unenough memory space.
 */
bool BinaryHeapInsert(BinaryHeap *self, void *pItem);


/**
 * This function deletes the root node of the heap and returns the item storing at that node correspondingly.
 *
 * @param self      The pointer to the BinaryHeap structure.
 * 
 * @return          Non-NULL: The pointer to the returned item.
 *                  NULL    : The heap is empty, and thus nothing is returned.
 */
void* BinaryHeapDelete(BinaryHeap *self);


/**
 * This function returns the item stored in the top of the heap.
 *
 * @param self      The pointer to the BinaryHeap structure.
 *
 * @return          Non-NULL: The pointer to the returned item.
 *                  NULL    : The heap is empty, and thus nothing is returned.
 */
void* BinaryHeapTop(BinaryHeap *self);


/**
 * This function updates the content of the designated item.
 * Note that the updating must not violate the heap property. 
 *
 * @param self      The pointer to the BinaryHeap structure.
 * @param idx       The index to the item which is to be updated.
 * @param pTge      The pointer to the item containing new data.
 *
 * @return          true : The data updating is done successfully.
 *                  false: 1. The designated item cannot be found.
 *                         2. The data updating violate the heap property.
 */
bool BinaryHeapChange(BinaryHeap *self, int idx, void *pTge);


/**
 * This function returns the size of the heap.
 *
 * @param sefl      The pointer to the BinaryHeap structure.
 */
unsigned long BinaryHeapSize(BinaryHeap *self);


/**
 * This function sets the user-defined item comparison strategy for the heap.
 *
 * @param self          The pointer to the BinaryHeap structure.
 * @param pFunc         The pointer to the customized function.
 */
void BinaryHeapSetCompare(BinaryHeap *self, int (*pFunc)(const void*, const void*));


/**
 * This function sets the user-defined item deallocation strategy for the heap.
 *
 * @param self          The pointer to the BinaryHeap structure.
 * @param pFunc         The pointer to the customized function.
 */
void BinaryHeapSetDestroy(BinaryHeap *self, void (*pFunc)(void*));


/**
 * This function sets the user-defined item content updating strategy for the heap.
 *
 * @param self          The pointer to the BinaryHeap structure.
 * @param pFunc         The pointer to the customized function.
 */
void BinaryHeapSetUpdate(BinaryHeap *self, void (*pFunc)(const void*, const void*));

#endif
