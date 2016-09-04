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
 * @file list.h The doubly linked list.
 */

#ifndef _LIST_H_
#define _LIST_H_

#include "../util.h"

#ifdef __cplusplus
extern "C" {
#endif

/** ListData is the data type for the container private information. */
typedef struct _ListData ListData;

/** Element clean function called when an element is removed. */
typedef void (*ListClean) (void*);


/** The implementation for doubly linked list. */
typedef struct _List {
    /** The container private information */
    ListData *data;

    /** Push an element to the head of the list.
        @see ListPushFront */
    bool (*push_front) (struct _List*, void*);

    /** Push an element to the tail of the list.
        @see ListPushBack */
    bool (*push_back) (struct _List*, void*);

    /** Insert an element to the specified index of the list.
        @see ListInsert */
    bool (*insert) (struct _List*, unsigned, void*);

    /** Pop an element from the head of the list.
        @see ListPopFront */
    bool (*pop_front) (struct _List*);

    /** Pop an element from the tail of the list.
        @see ListPopBack */
    bool (*pop_back) (struct _List*);

    /** Remove an element from the specified index of the list
        @see ListRemove */
    bool (*remove) (struct _List*, unsigned);

    /** Replace an element at the head of the list.
        @see ListSetFront */
    bool (*set_front) (struct _List*, void*);

    /** Replace an element at the tail of the list.
        @see ListSetBack */
    bool (*set_back) (struct _List*, void*);

    /** Replace an element at the specified index of the list.
        @see ListSetAt */
    bool (*set_at) (struct _List*, unsigned, void*);

    /** Get an element from the head of the list.
        @see ListGetFront */
    bool (*get_front) (struct _List*, void**);

    /** Get an element from the tail of the list.
        @see ListGetBack */
    bool (*get_back) (struct _List*, void**);

    /** Get an element from the specified index of the list.
        @see ListGetAt */
    bool (*get_at) (struct _List*, unsigned, void**);

    /** Return the number of stored elements.
        @see ListSize */
    unsigned (*size) (struct _List*);

    /** Reverse the list.
        @see ListReverse */
    void (*reverse) (struct _List*);

    /** Initialize the list iterator.
        @see ListFirst */
    void (*first) (struct _List*, bool);

    /** Get the element pointed by the iterator and advance the iterator.
        @see ListNext */
    bool (*next) (struct _List*, void**);

    /** Get the element pointed by the iterator and advance the iterator
        in the reverse order.
        @see ListReverseNext */
    bool (*reverse_next) (struct _List*, void**);

    /** Set the custom element cleanup function.
        @see ListSetClean */
    void (*set_clean) (struct _List*, ListClean);
} List;


/*===========================================================================*
 *             Definition for the exported member operations                 *
 *===========================================================================*/
/**
 * @brief The constructor for List.
 *
 * @retval obj          The successfully constructed list
 * @retval NULL         Insufficient memory for list construction
 */
List* ListInit();

/**
 * @brief The destructor for List.
 *
 * @param obj           The pointer to the to be destructed list
 */
void ListDeinit(List* obj);


/**
 * @brief Push an element to the head of the list.
 *
 * @param self          The pointer to the List structure
 * @param element       The specified element
 *
 * @retval true         The element is successfully pushed
 * @retval false        The element cannot be pushed due to insufficient memory
 */
bool ListPushFront(List* self, void* element);

/**
 * @brief Push an element to the tail of the list.
 *
 * @param self          The pointer to the List structure
 * @param element       The specified element
 *
 * @retval true         The element is successfully pushed
 * @retval false        The element cannot be pushed due to insufficient memory
 */
bool ListPushBack(List* self, void* element);

/**
 * @brief Insert an element to the specified index of the list.
 *
 * This function inserts an element to the specified index of the list and
 * shifts the trailing elements one position to the tail.
 *
 * @param self          The pointer to List structure
 * @param idx           The specified index
 * @param element       The specified element
 *
 * @retval true         The element is successfully inserted
 * @retval false        The element cannot be inserted due to invalid index
 *                      (> list size) or insufficient memory
 */
bool ListInsert(List* self, unsigned idx, void* element);

/**
 * @brief Pop an element from the head of the list.
 *
 * This function removes an element from the head of the list. Also, the cleanup
 * function is invoked for the popped element.
 *
 * @param self          The pointer to List structure
 *
 * @retval true         The head element is successfully popped
 * @retval false        The list is empty
 */
bool ListPopFront(List* self);

/**
 * @brief Pop an element from the tail of the list.
 *
 * This function removes an element from the tail of the list. Also, the cleanup
 * function is invoked for the popped element.
 *
 * @param self          The pointer to List structure
 *
 * @retval true         The tail element is successfully popped
 * @retval false        The list is empty
 */
bool ListPopBack(List* self);

/**
 * @brief Remove an element from the specified index of the list.
 *
 * This function removes an element from the specified index of the list and
 * shifts the trailing elements one position to the head. Also, the cleanup
 * function is invoked for the removed element.
 *
 * @param self          The pointer to List structure
 * @param idx           The specified index
 *
 * @retval true         The specified element is successfully removed.
 * @retval false        Invalid index (>= list size) or empty list
 */
bool ListRemove(List* self, unsigned idx);

/**
 * @brief Replace an element at the head of the list.
 *
 * This function replaces the head element of the list. Also, the cleanup
 * function is invoked for the replaced element.
 *
 * @param self          The pointer to the List structure
 * @param element       The specified element
 *
 * @retval true         The specified element is successfully set.
 * @retval false        The list is empty
 */
bool ListSetFront(List* self, void* element);

/**
 * @brief Replace an element at the tail of the list.
 *
 * This function replaces the tail element of the list. Also, the cleanup
 * function is invoked for the replaced element.
 *
 * @param self          The pointer to the List structure
 * @param element       The specified element
 *
 * @retval true         The specified element is successfully set.
 * @retval false        The list is empty
 */
bool ListSetBack(List* self, void* element);

/**
 * @brief Replace an element at the specified index of the list.
 *
 * This function sets an element at the specified index of the list. Also, the
 * cleanup function is invoked for the replaced element.
 *
 * @param self          The pointer to List structure
 * @param idx           The specified index
 * @param element       The specified element
 *
 * @retval true         The specified element is successfully set.
 * @retval false        Invalid index (>= list size)
 */
bool ListSetAt(List* self, unsigned idx, void* element);

/**
 * @brief Get an element from the head of the list.
 *
 * @param self          The pointer to List structure
 * @param p_element     The pointer to the returned element
 *
 * @retval true         The specified element is successfully retrieved
 * @retval false        The list is empty
 */
bool ListGetFront(List* self, void** p_element);

/**
 * @brief Get an element from the tail of the list.
 *
 * @param self          The pointer to List structure
 * @param p_element     The pointer to the returned element
 *
 * @retval true         The specified element is successfully retrieved
 * @retval false        The list is empty
 */
bool ListGetBack(List* self, void** p_element);

/**
 * @brief Get an element from the specified index of the list.
 *
 * @param self          The pointer to List structure
 * @param idx           The specified index
 * @param p_element     The pointer to the returned element
 *
 * @retval true         The specified element is successfully retrieved
 * @retval false        Invalid index (>= list size)
 */
bool ListGetAt(List* self, unsigned idx, void** p_element);

/**
 * @brief Return the number of stored elements.
 *
 * @param self          The pointer to List structure
 *
 * @retval size         The number of stored elements
 */
unsigned ListSize(List* self);

/**
 * @brief Reverse the list.
 *
 * @param self          The pointer to the List structure
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 */
void ListReverse(List* self);

/**
 * @brief Initialize the vector iterator.
 *
 * @param self          The pointer to List structure
 * @param is_reverse    Whether to apply the reversed traversal
 */
void ListFirst(List* self, bool is_reverse);

/**
 * @brief Get the element pointed by the iterator and advance the iterator.
 *
 * @param self          The pointer to List structure
 * @param p_element     The pointer to the returned element
 *
 * @retval true         The tail end is not reached
 * @retval false        The tail end is reached
 */
bool ListNext(List* self, void** p_element);

/**
 * @brief Get the element pointed by the iterator and advance the iterator
 * in the reverse order.
 *
 * @param self          The pointer to List structure
 * @param p_element     The pointer to the returned element
 *
 * @retval true         The head end is not reached
 * @retval false        The head end is reached
 */
bool ListReverseNext(List* self, void** p_element);

/**
 * @brief Set the custom element cleanup function
 *
 * @param self          The pointer to List structure
 * @param func          The custom function
 */
void ListSetClean(List* self, ListClean func);

#ifdef __cplusplus
}
#endif

#endif
