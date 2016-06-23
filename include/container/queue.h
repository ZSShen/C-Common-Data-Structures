/**
 * @file queue.h The FIFO queue.
 */

#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "../util.h"

#ifdef __cplusplus
extern "C" {
#endif

/** QueueData is the data type for the container private information. */
typedef struct _QueueData QueueData;

/** The implementation for queue. */
typedef struct _Queue {
    /** The container private information */
    QueueData *pData;

    /** Insert an item to the tail of the queue.
        @see QueuePush */
    int32_t (*push) (struct _Queue*, Item);

    /** Retrieve item from the head of the queue.
        @see QueueFront */
    int32_t (*front) (struct _Queue*, Item*);

    /** Retrieve item from the tail of the queue.
        @see QueueBack */
    int32_t (*back) (struct _Queue*, Item*);

    /** Delete an item from the head of the queue.
        @see QueuePop */
    int32_t (*pop) (struct _Queue*);

    /** Return the number of stored items.
        @see QueueSize */
    int32_t (*size) (struct _Queue*);

    /** Set the custom item resource clean method.
        @see QueueSetDestroy */
    int32_t (*set_destroy) (struct _Queue*, void (*) (Item));
} Queue;


/*===========================================================================*
 *             Definition for the exported member operations                 *
 *===========================================================================*/
/**
 * @brief The constructor for Queue.
 *
 * @param ppObj         The double pointer to the to be constructed queue
 *
 * @retval SUCC
 * @retval ERR_NOMEM    Insufficient memory for queue construction
 */
int32_t QueueInit(Queue **ppObj);

/**
 * @brief The destructor for Queue.
 *
 * If the custom resource clean method is set, it also runs the clean method
 * for all the items.
 *
 * @param ppObj         The double pointer to the to be destructed queue
 */
void QueueDeinit(Queue **ppObj);

/**
 * @brief Insert an item to the tail of the queue.
 *
 * This function inserts an item to the tail of the queue with the corresponding
 * queue size extension.
 *
 * @param self          The pointer to Queue structure
 * @param item          The designated item
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_NOMEM    Insufficient memory for queue extension
 */
int32_t QueuePush(Queue *self, Item item);

/**
 * @brief Delete item from top of the queue.
 *
 * This function deletes item from top of the queue. If the custom resource clean
 * method is set, it also runs the clean method for the deleted item.
 *
 * @param self          The pointer to Queue structure
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_IDX      Empty queue
 */
int32_t QueuePop(Queue *self);

/**
 * @brief Retrieve item from the head of the queue.
 *
 * This function retrieves item from the head of the queue. If the queue is not
 * empty, the item is returned by the second parameter. Otherwise, the error
 * code is returned and the second parameter is updated with NULL.
 *
 * @param self          The pointer to Queue structure
 * @param pItem         The pointer to the returned item
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_IDX      Empty queue
 * @retval ERR_GET      Invalid parameter to store returned item
 */
int32_t QueueFront(Queue *self, Item *pItem);

/**
 * @brief Retrieve item from the tail of the queue.
 *
 * This function retrieves item from the tail of the queue. If the queue is not
 * empty, the item is returned by the second parameter. Otherwise, the error
 * code is returned and the second parameter is updated with NULL.
 *
 * @param self          The pointer to Queue structure
 * @param pItem         The pointer to the returned item
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_IDX      Empty queue
 * @retval ERR_GET      Invalid parameter to store returned item
 */
int32_t QueueBack(Queue *self, Item *pItem);


/**
 * @brief Return the number of stored items.
 *
 * @param self          The pointer to Queue structure
 *
 * @return              The number of items
 * @retval ERR_NOINIT   Uninitialized container
 */
int32_t QueueSize(Queue *self);

/**
 * @brief Set the custom item resource clean method.
 *
 * @param self          The pointer to Queue structure
 * @param pFunc         The function pointer to the custom method
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 */
int32_t QueueSetDestroy(Queue *self, void (*pFunc) (Item));

#ifdef __cplusplus
}
#endif

#endif