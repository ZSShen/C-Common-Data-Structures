/**
 * @file binary_heap.h The binary heap.
 */

#ifndef _BINARY_HEAP_H_
#define _BINARY_HEAP_H_

#include "../util.h"

/** BinHeapData is the data type for the container private information. */
typedef struct _BinHeapData BinHeapData;

/** The implementation for binary heap. */
typedef struct _BinaryHeap {
    /** The container private information */
    BinHeapData *pData;

    /** Push an item onto the heap.
        @see BinHeapPush */
    int32_t (*push) (struct _BinaryHeap*, Item);

    /** Retrieve item from top of the heap.
        @see BinHeapTop */
    int32_t (*top) (struct _BinaryHeap*, Item*);

    /** Delete item from top of the heap.
        @see BinHeapPop */
    int32_t (*pop) (struct _BinaryHeap*);

    /** Return the number of stored items.
        @see BinHeapSize */
    int32_t (*size) (struct _BinaryHeap*);

    /** Set the custom item comparison method.
        @see BinHeapSetCompare */
    int32_t (*set_compare) (struct _BinaryHeap*, int32_t (*) (Item, Item));

    /** Set the custom item resource clean method.
        @see BinHeapSetDestroy */
    int32_t (*set_destroy) (struct _BinaryHeap*, void (*) (Item));
} BinaryHeap;


/*===========================================================================*
 *             Definition for the exported member operations                 *
 *===========================================================================*/
/**
 * @brief The constructor for BinHeap.
 *
 * @param ppObj         The double pointer to the to be constructed heap
 *
 * @retval SUCC
 * @retval ERR_NOMEM    Insufficient memory for heap construction
 */
int32_t BinHeapInit(BinHeap **ppObj);

/**
 * @brief The destructor for BinHeap.
 *
 * If the custom resource clean method is set, it also runs the clean method for
 * all the items.
 *
 * @param ppObj         The double pointer to the to be destructed heap
 */
void BinHeapDeinit(BinHeap **ppObj);

/**
 * @brief Push an item onto the heap.
 *
 * This function pushes an item onto the heap with the corresponding heap size
 * extension.
 *
 * @param self          The pointer to BinHeap structure
 * @param item          The designated item
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_NOMEM    Insufficient memory for heap extension
 */
int32_t BinHeapPush(BinHeap *self, Item item);

/**
 * @brief Delete item from top of the heap.
 *
 * This function deletes item from top of the heap. If the custom resource clean
 * method is set, it also runs the clean method for the deleted item.
 *
 * @param self          The pointer to BinHeap structure
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_IDX      Empty heap
 */
int32_t BinHeapPop(BinHeap *self);

/**
 * @brief Retrieve item from top of the heap.
 *
 * This function retrieves item from top of the heap. If the heap is not empty,
 * the item is returned by the second parameter. Otherwise, the error code is
 * returned and the second parameter is updated with NULL.
 *
 * @param self          The pointer to BinHeap structure
 * @param pItem         The pointer to the returned item
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_IDX      Empty heap
 * @retval ERR_GET      Invalid parameter to store returned item
 */
int32_t BinHeapTop(BinHeap *self, Item *pItem);

/**
 * @brief Return the number of stored items.
 *
 * @param self          The pointer to BinHeap structure
 *
 * @return              The number of items
 * @retval ERR_NOINIT   Uninitialized container
 */
int32_t BinHeapSize(BinHeap *self);

/**
 * @brief Set the custom item comparison method.
 *
 * @param self          The pointer to BinHeap structure
 * @param pFunc         The function pointer to the custom method
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 */
int32_t BinHeapSetCompare(BinHeap *self, int32_t (*pFunc) (Item, Item));

/**
 * @brief Set the custom item resource clean method.
 *
 * @param self          The pointer to BinHeap structure
 * @param pFunc         The function pointer to the custom method
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 */
int32_t BinHeapSetDestroy(BinHeap *self, void (*pFunc) (Item));

#endif
