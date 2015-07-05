/**
 * @file priority_queue.h The priority queue.
 */

#ifndef _PRIORITY_QUEUE_H_
#define _PRIORITY_QUEUE_H_

#include "../util.h"

/** PrioQueueData is the data type for the container private information. */
typedef struct _PrioQueueData PrioQueueData;

/** The implementation for priority queue. */
typedef struct _PrioQueue {
    /** The container private information */
    PrioQueueData *pData;

    /** Push an item onto the queue.
        @see PrioQueuePush */
    int32_t (*push) (struct _PrioQueue*, Item);

    /** Retrieve item from top of the queue.
        @see PrioQueueTop */
    int32_t (*top) (struct _PrioQueue*, Item*);

    /** Delete item from top of the queue.
        @see PrioQueuePop */
    int32_t (*pop) (struct _PrioQueue*);

    /** Return the number of stored items.
        @see PrioQueueSize */
    int32_t (*size) (struct _PrioQueue*);

    /** Set the custom item comparison method.
        @see PrioQueueSetCompare */
    int32_t (*set_compare) (struct _PrioQueue*, int32_t (*) (Item, Item));

    /** Set the custom resource clean method.
        @see PrioQueueSetDestroy */
    int32_t (*set_destroy) (struct _PrioQueue*, void (*) (Item));
} PrioQueue;


/*===========================================================================*
 *             Definition for the exported member operations                 *
 *===========================================================================*/
/**
 * @brief The constructor for PrioQueue.
 *
 * @param ppObj         The double pointer to the to be constructed queue
 *
 * @retval SUCC
 * @retval ERR_NOMEM    Insufficient memory for queue construction
 */
int32_t PrioQueueInit(PrioQueue **ppObj);

/**
 * @brief The destructor for PrioQueue.
 *
 * If the custom resource clean method is set, it also runs the clean method
 * for all the items.
 *
 * @param ppObj         The double pointer to the to be destructed queue
 * @param bClean        The knob to clean item resource
 */
void PrioQueueDeinit(PrioQueue **ppObj);

/**
 * @brief Push an item onto the queue.
 *
 * This function pushes an item onto the queue with the corresponding queue size
 * extension.
 *
 * @param self          The pointer to PrioQueue structure
 * @param item          The designated item
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_NOMEM    Insufficient memory for queue extension
 */
int32_t PrioQueuePush(PrioQueue *self, Item item);

/**
 * @brief Delete item from top of the queue.
 *
 * This function deletes item from top of the queue. If the custom resource clean
 * method is set, it also runs the clean method for the deleted item.
 *
 * @param self          The pointer to PrioQueue structure
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_IDX      Empty queue
 */
int32_t PrioQueuePop(PrioQueue *self);

/**
 * @brief Retrieve item from top of the queue.
 *
 * This function retrieves item from top of the queue. If the queue is not empty,
 * the item is returned by the second parameter. Otherwise, the error code is
 * returned and the second parameter is updated with NULL.
 *
 * @param self          The pointer to PrioQueue structure
 * @param pItem         The pointer to the returned item
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_IDX      Empty queue
 */
int32_t PrioQueueTop(PrioQueue *self, Item *pItem);

/**
 * @brief Return the number of stored items.
 *
 * @param self          The pointer to PrioQueue structure
 *
 * @return              The number of items
 * @retval ERR_NOINIT   Uninitialized container
 */
int32_t PrioQueueSize(PrioQueue *self);

/**
 * @brief Set the custom item comparison method.
 *
 * @param self          The pointer to PrioQueue structure
 * @param pFunc         The function pointer to the custom method
 *
 * @retval ERR_NOINIT   Uninitialized container
 * @retval SUCC
 */
int32_t PrioQueueSetCompare(PrioQueue *self, int32_t (*pFunc) (Item, Item));

/**
 * @brief Set the custom resource clean method.
 *
 * @param self          The pointer to PrioQueue structure
 * @param pFunc         The function pointer to the custom method
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 */
int32_t PrioQueueSetDestroy(PrioQueue *self, void (*pFunc) (Item));

#endif