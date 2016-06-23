/**
 * @file priority_queue.h The priority queue implemented with binary heap.
 */

#ifndef _PRIORITY_QUEUE_H_
#define _PRIORITY_QUEUE_H_

#include "../util.h"

#ifdef __cplusplus
extern "C" {
#endif

/** PriorityQueueData is the data type for the container private information. */
typedef struct _PriorityQueueData PriorityQueueData;

/** The implementation for priority queue. */
typedef struct _PriorityQueue {
    /** The container private information */
    PriorityQueueData *pData;

    /** Push an item onto the queue.
        @see PriorityQueuePush */
    int32_t (*push) (struct _PriorityQueue*, Item);

    /** Retrieve item from top of the queue.
        @see PriorityQueueTop */
    int32_t (*top) (struct _PriorityQueue*, Item*);

    /** Delete item from top of the queue.
        @see PriorityQueuePop */
    int32_t (*pop) (struct _PriorityQueue*);

    /** Return the number of stored items.
        @see PriorityQueueSize */
    int32_t (*size) (struct _PriorityQueue*);

    /** Set the custom item comparison method.
        @see PriorityQueueSetCompare */
    int32_t (*set_compare) (struct _PriorityQueue*, int32_t (*) (Item, Item));

    /** Set the custom item resource clean method.
        @see PriorityQueueSetDestroy */
    int32_t (*set_destroy) (struct _PriorityQueue*, void (*) (Item));
} PriorityQueue;


/*===========================================================================*
 *             Definition for the exported member operations                 *
 *===========================================================================*/
/**
 * @brief The constructor for PriorityQueue.
 *
 * @param ppObj         The double pointer to the to be constructed queue
 *
 * @retval SUCC
 * @retval ERR_NOMEM    Insufficient memory for queue construction
 */
int32_t PriorityQueueInit(PriorityQueue **ppObj);

/**
 * @brief The destructor for PriorityQueue.
 *
 * If the custom resource clean method is set, it also runs the clean method
 * for all the items.
 *
 * @param ppObj         The double pointer to the to be destructed queue
 */
void PriorityQueueDeinit(PriorityQueue **ppObj);

/**
 * @brief Push an item onto the queue.
 *
 * This function pushes an item onto the queue with the corresponding queue size
 * extension.
 *
 * @param self          The pointer to PriorityQueue structure
 * @param item          The designated item
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_NOMEM    Insufficient memory for queue extension
 */
int32_t PriorityQueuePush(PriorityQueue *self, Item item);

/**
 * @brief Delete item from top of the queue.
 *
 * This function deletes item from top of the queue. If the custom resource clean
 * method is set, it also runs the clean method for the deleted item.
 *
 * @param self          The pointer to PriorityQueue structure
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_IDX      Empty queue
 */
int32_t PriorityQueuePop(PriorityQueue *self);

/**
 * @brief Retrieve item from top of the queue.
 *
 * This function retrieves item from top of the queue. If the queue is not empty,
 * the item is returned by the second parameter. Otherwise, the error code is
 * returned and the second parameter is updated with NULL.
 *
 * @param self          The pointer to PriorityQueue structure
 * @param pItem         The pointer to the returned item
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_IDX      Empty queue
 * @retval ERR_GET      Invalid parameter to store returned item
 */
int32_t PriorityQueueTop(PriorityQueue *self, Item *pItem);

/**
 * @brief Return the number of stored items.
 *
 * @param self          The pointer to PriorityQueue structure
 *
 * @return              The number of items
 * @retval ERR_NOINIT   Uninitialized container
 */
int32_t PriorityQueueSize(PriorityQueue *self);

/**
 * @brief Set the custom item comparison method.
 *
 * @param self          The pointer to PriorityQueue structure
 * @param pFunc         The function pointer to the custom method
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 */
int32_t PriorityQueueSetCompare(PriorityQueue *self, int32_t (*pFunc) (Item, Item));

/**
 * @brief Set the custom item resource clean method.
 *
 * @param self          The pointer to PriorityQueue structure
 * @param pFunc         The function pointer to the custom method
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 */
int32_t PriorityQueueSetDestroy(PriorityQueue *self, void (*pFunc) (Item));

#ifdef __cplusplus
}
#endif

#endif