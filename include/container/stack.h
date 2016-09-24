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
 * @file stack.h The LIFO stack.
 */

#ifndef _STACK_H_
#define _STACK_H_

#include "../util.h"

#ifdef __cplusplus
extern "C" {
#endif

/** StackData is the data type for the container private information. */
typedef struct _StackData StackData;

/** Element clean function called when an element is removed. */
typedef void (*StackClean) (void*);


/** The implementation for stack. */
typedef struct _Stack {
    /** The container private information */
    StackData *data;

    /** Push an element to the top of the stack.
        @see StackPush */
    bool (*push) (struct _Stack*, void*);

    /** Retrieve an element from the top of the stack.
        @see StackTop */
    bool (*top) (struct _Stack*, void**);

    /** Remove an element from the top of the stack.
        @see StackPop */
    bool (*pop) (struct _Stack*);

    /** Return the number of stored elements.
        @see StackSize */
    unsigned (*size) (struct _Stack*);

    /** Set the custom element cleanup function.
        @see SetClean */
    void (*set_clean) (struct _Stack*, StackClean func);
} Stack;


/*===========================================================================*
 *             Definition for the exported member operations                 *
 *===========================================================================*/
/**
 * @brief The constructor for Stack.
 *
 * @retval obj          The successfully constructed stack
 * @retval NULL         Insufficient memory for stack construction
 */
Stack* StackInit();

/**
 * @brief The destructor for Stack.
 *
 * @param obj           The pointer to the to be destructed stack
 */
void StackDeinit(Stack* obj);

/**
 * @brief Push an element to the top of the stack.
 *
 * @param self          The pointer to Stack structure
 * @param element       The specified element
 *
 * @retval true         The element is successfully pushed
 * @retval false        The element cannot be pushed due to insufficient memory
 */
bool StackPush(Stack* self, void* element);

/**
 * @brief Retrieve an element from the top of the stack.
 *
 * @param self          The pointer to Stack structure
 * @param p_element     The pointer to the returned element
 *
 * @retval true         The element is successfully retrieved
 * @retval false        The stack is empty
 */
bool StackTop(Stack* self, void** p_element);

/**
 * @brief Remove an element from the top of the stack.
 *
 * This function removes an element from the top of the stack. Also, the cleanup
 * function is invoked for the removed element.
 *
 * @param self          The pointer to Stack structure
 *
 * @retval true         The top element is successfully removed
 * @retval false        The stack is empty
 */
bool StackPop(Stack* self);

/**
 * @brief Return the number of stored elements.
 *
 * @param self          The pointer to Stack structure
 *
 * @retval size         The number of stored elements
 */
unsigned StackSize(Stack* self);

/**
 * @brief Set the custom element cleanup function.
 *
 * @param self          The pointer to Stack structure
 * @param func          The custom function
 */
void StackSetClean(Stack* self, StackClean func);

#ifdef __cplusplus
}
#endif

#endif