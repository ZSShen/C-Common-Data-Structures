/**
 *   The MIT License (MIT)
 *   Copyright (C) 2016 ZongXian Shen <andy.zsshen@gmail.com>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a
 *   copy of this software and associated documentation files (the "Software"),
 *   to deal in the Software without restriction, including without limitation
 *   the rights to use, copy, modify, merge, publish, distribute, sublicense,
 *   and/or sell copies of the Software, and to permit persons to whom the
 *   Software is furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 *   THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 *   IN THE SOFTWARE.
 */

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

/** Order comparison function for the queue elements. */
typedef int (*PriorityQueueCompare) (const void*, const void*);

/** Element clean function called when an element is removed. */
typedef void (*PriorityQueueClean) (void*);


/** The implementation for priority queue. */
typedef struct _PriorityQueue {
    /** The container private information */
    PriorityQueueData *data;

    /** Push an element to the queue.
        @see PriorityQueuePush */
    bool (*push) (struct _PriorityQueue*, void*);

    /** Get element from top of the queue.
        @see PriorityQueueTop */
    bool (*top) (struct _PriorityQueue*, void**);

    /** Remove element from top of the queue.
        @see PriorityQueuePop */
    bool (*pop) (struct _PriorityQueue*);

    /** Get the number of stored elements.
        @see PriorityQueueSize */
    unsigned (*size) (struct _PriorityQueue*);

    /** Set the custom element comparison function.
        @see PriorityQueueSetCompare */
    void (*set_compare) (struct _PriorityQueue*, PriorityQueueCompare func);

    /** Set the custom element cleanup function.
        @see PriorityQueueSetClean */
    void (*set_clean) (struct _PriorityQueue*, PriorityQueueClean func);
} PriorityQueue;


/*===========================================================================*
 *             Definition for the exported member operations                 *
 *===========================================================================*/
/**
 * @brief The constructor for PriorityQueue.
 *
 * @retval obj          The successfully constructed queue
 * @retval NULL         Insufficient memory for queue construction
 */
PriorityQueue* PriorityQueueInit();

/**
 * @brief The destructor for PriorityQueue.
 *
 * @param obj           The pointer to the to be destructed queue
 */
void PriorityQueueDeinit(PriorityQueue* obj);

/**
 * @brief Push an element to the queue.
 *
 * @param self          The pointer to PriorityQueue structure
 * @param element       The specified element
 *
 * @retval true         The element is successfully pushed
 * @retval false        The element cannot be pushed due to insufficient memory
 */
bool PriorityQueuePush(PriorityQueue* self, void* element);

/**
 * @brief Delete element from top of the queue.
 *
 * This function removes element from top of the queue. Also, the cleanup
 * function is invoked for the popped element.
 *
 * @param self          The pointer to PriorityQueue structure
 *
 * @retval true         The top element is successfully popped
 * @retval false        The queue is empty
 */
bool PriorityQueuePop(PriorityQueue* self);

/**
 * @brief Get element from top of the queue.
 *
 * @param self          The pointer to PriorityQueue structure
 * @param p_element     The pointer to the returned element
 *
 * @retval true         The top element is successfully retrieved
 * @retval false        The queue is empty
 */
bool PriorityQueueTop(PriorityQueue* self, void** p_element);

/**
 * @brief Return the number of stored items.
 *
 * @param self          The pointer to PriorityQueue structure
 *
 * @retval size         The number of stored elements
 */
unsigned PriorityQueueSize(PriorityQueue* self);

/**
 * @brief Set the custom element comparison function.
 *
 * @param self          The pointer to PriorityQueue structure
 * @param func          The custom function
 */
void PriorityQueueSetCompare(PriorityQueue* self, PriorityQueueCompare func);

/**
 * @brief Set the custom element cleanup function.
 *
 * @param self          The pointer to PriorityQueue structure
 * @param func          The custom function
 */
void PriorityQueueSetClean(PriorityQueue* self, PriorityQueueClean func);

#ifdef __cplusplus
}
#endif

#endif