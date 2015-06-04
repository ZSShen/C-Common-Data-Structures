/**
 * @file binary_heap.h The binary heap.
 */

#ifndef _BINARY_HEAP_H_
#define _BINARY_HEAP_H_

#include "../util.h"

/** BinHeapData is the data type for the container private information. */
typedef struct _BinHeapData BinHeapData;

/** The implementation for binary heap. */
typedef struct _BinHeap {
    /** The container private information */
    BinHeapData *pData;

    /** Insert an item into the heap.
        @see BinHeapPush */
    int32_t (*push) (struct _BinHeap*, Item);

    /** Retrieve the item from the top of the heap.
        @see BinHeapTop */
    int32_t (*top) (struct _BinHeap*, Item*);

    /** Delete the item from the top of the heap.
        @see BinHeapPop */
    int32_t (*pop) (struct _BinHeap*);

    /** Return the number of items.
        @see BinHeapSize */
    int32_t (*size) (struct _BinHeap*);

    /** Set the user defined item comparison method.
        @see BinHeapSetCompare */
    int32_t (*set_compare) (struct _BinHeap*, int32_t (*) (Item, Item));

    /** Set the user defined item clean method.
        @see BinHeapSetDestroy */
    int32_t (*set_destroy) (struct _BinHeap*, void (*) (Item));
} BinHeap;


/*===========================================================================*
 *             Definition for the exported member operations                 *
 *===========================================================================*/
/**
 * @brief The constructor for BinHeap.
 *
 * @param ppObj         The double pointer to the to be constructure heap
 *
 * @retval SUCC
 * @retval ERR_NOMEM    Insufficient memory for heap construction
 */
int32_t BinHeapInit(BinHeap **ppObj);

/**
 * @brief The destructor for BinHeap.
 *
 * @param ppObj         The double pointer to the to be destructured heap
 */
void BinHeapDeinit(BinHeap **ppObj);

/**
 * @brief Insert an item into the heap.
 *
 * This function inserts an item into the proper position of the heap. If the
 * order of the designated item is the same with a certain one stored in the
 * heap, the old item will be removed and the allocated resource is cleaned by
 * the configured method so that the designated item can take the position.
 *
 * @param self          The pointer to BinHeap structure
 * @param item          The designated item
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_NOMEM    Insufficient memory for item insertion
 */
int32_t BinHeapPush(BinHeap *self, Item item);

/**
 * @brief Delete the item from the top of the heap.
 *
 * This function deletes the item from the top of the heap and maintains the
 * heap property. The resource allocated for the deleted item is cleaned by
 * the configured method.
 *
 * @param self          The pointer to BinHeap structure
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_IDX      Empty heap
 */
int32_t BinHeapPop(BinHeap *self);

/**
 * @brief Retrieve the item from the top of the heap.
 *
 * This function retrieves the item from the top of the heap. If the heap is not
 * empty, the item is returned by the second parameter. Otherwise, the error code
 * is returned and the second parameter is returned with NULL value.
 *
 * @param self          The pointer to BinHeap structure
 * @param pItem         The pointer to the returned item
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_IDX      Empty heap
 */
int32_t BinHeapTop(BinHeap *self, Item *pItem);

/**
 * @brief Return the number of items.
 *
 * @param self          The pointer to BinHeap structure
 *
 * @return              The number of items
 * @retval ERR_NOINIT   Uninitialized container
 */
int32_t BinHeapSize(BinHeap *self);

/**
 * @brief Set the user defined item comparison method.
 *
 * @param self          The pointer to BinHeap structure
 * @param pFunc         The function pointer to the custom method
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 */
int32_t BinHeapSetCompare(BinHeap *self, int32_t (*pFunc) (Item, Item));

/**
 * @brief Set the user defined item clean method.
 *
 * @param self          The pointer to BinHeap structure
 * @param pFunc         The function pointer to the custom method
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 */
int32_t BinHeapSetDestroy(BinHeap *self, void (*pFunc) (Item));

#endif