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

/** Element clean function called when an element is removed. */
typedef void (*QueueClean) (void*);


/** The implementation for queue. */
typedef struct _Queue {
    /** The container private information */
    QueueData *data;

    /** Push an element to the tail of the queue.
        @see QueuePush */
    bool (*push) (struct _Queue*, void*);

    /** Retrieve an element from the head of the queue.
        @see QueueFront */
    bool (*front) (struct _Queue*, void**);

    /** Retrieve an element from the tail of the queue.
        @see QueueBack */
    bool (*back) (struct _Queue*, void**);

    /** Remove an element from the head of the queue.
        @see QueuePop */
    bool (*pop) (struct _Queue*);

    /** Return the number of stored elements.
        @see QueueSize */
    unsigned (*size) (struct _Queue*);

    /** Set the custom element cleanup function.
        @see QueueSetDestroy */
    void (*set_clean) (struct _Queue*, QueueClean func);
} Queue;


/*===========================================================================*
 *             Definition for the exported member operations                 *
 *===========================================================================*/
/**
 * @brief The constructor for Queue.
 *
 * @retval obj          The successfully constructed queue
 * @retval NULL         Insufficient memory for queue construction
 */
Queue* QueueInit();

/**
 * @brief The destructor for Queue.
 *
 * @param obj           The pointer to the to be destructed queue
 */
void QueueDeinit(Queue* obj);

/**
 * @brief Push an element to the tail of the queue.
 *
 * @param self          The pointer to Queue structure
 * @param element       The specified element
 *
 * @retval true         The element is successfully pushed
 * @retval false        The element cannot be pushed due to insufficient memory
 */
bool QueuePush(Queue* self, void* element);

/**
 * @brief Retrieve an element from the head of the queue.
 *
 * @param self          The pointer to PriorityQueue structure
 * @param p_element     The pointer to the returned element
 *
 * @retval true         The head element is successfully retrieved
 * @retval false        The queue is empty
 */
bool QueueFront(Queue* self, void** p_element);

/**
 * @brief Retrieve an element from the tail of the queue.
 *
 * @param self          The pointer to PriorityQueue structure
 * @param p_element     The pointer to the returned element
 *
 * @retval true         The tail element is successfully retrieved
 * @retval false        The queue is empty
 */
bool QueueBack(Queue* self, void** p_element);

/**
 * @brief Remove an element from the head of the queue.
 *
 * This function removes an element from the head of the queue. Also, the cleanup
 * function is invoked for the removed element.
 *
 * @param self          The pointer to Queue structure
 *
 * @retval true         The top element is successfully removed
 * @retval false        The queue is empty
 */
bool QueuePop(Queue* self);

/**
 * @brief Return the number of stored elements.
 *
 * @param self          The pointer to Queue structure
 *
 * @retval size         The number of stored elements
 */
unsigned QueueSize(Queue* self);

/**
 * @brief Set the custom element cleanup function.
 *
 * @param self          The pointer to Queue structure
 * @param func          The custom function
 */
void QueueSetClean(Queue* self, QueueClean func);

#ifdef __cplusplus
}
#endif

#endif