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
 * @file vector.h The dynamically growable array.
 */

#ifndef _VECTOR_H_
#define _VECTOR_H_

#include "../util.h"

#ifdef __cplusplus
extern "C" {
#endif

/** VectorData is the data type for the container private information. */
typedef struct _VectorData VectorData;

/** Element comparison function called to sort the vector elements. */
typedef int (*VectorCompare) (const void*, const void*);

/** Element clean function called when an element is removed. */
typedef void (*VectorClean) (void*);


/** The implementation for dynamically growable array. */
typedef struct _Vector {
    /** The container private information */
    VectorData *data;

    /** Push an element to the tail of the vector.
        @see VectorPushBack */
    bool (*push_back) (struct _Vector*, void*);

    /** Insert an element to the specified index of the vector.
        @see VectorInsert */
    bool (*insert) (struct _Vector*, unsigned, void*);

    /** Pop an element from the tail of the vector.
        @see VectorPopBack */
    bool (*pop_back) (struct _Vector*);

    /** Remove an element from the specified index of the vector.
        @see VectorRemove */
    bool (*remove) (struct _Vector*, unsigned);

    /** Set an element at the specified index of the vector.
        @see VectorSet */
    bool (*set) (struct _Vector*, unsigned, void*);

    /** Get an element from the specified index of the vector.
        @see VectorGet */
    bool (*get) (struct _Vector*, unsigned, void**);

    /** Resize the vector so that it contains the specified number of elements.
        @see VectorResize */
    bool (*resize) (struct _Vector*, unsigned);

    /** Return the number of stored elements.
        @see VectorSize */
    unsigned (*size) (struct _Vector*);

    /** Return the vector capacity.
        @see VectorCapacity */
    unsigned (*capacity) (struct _Vector*);

    /** Sort the elements via the specified element comparison function.
        @see VectorSort */
    void (*sort) (struct _Vector*, VectorCompare);

    /** Initialize the vector iterator.
        @see VectorFirst */
    void (*first) (struct _Vector*, bool);

    /** Get the element pointed by the iterator and advance the iterator.
        @see VectorNext */
    bool (*next) (struct _Vector*, void**);

    /** Get the element pointed by the iterator and advance the iterator
        in the reverse order.
        @see VectorNext */
    bool (*reverse_next) (struct _Vector*, void**);

    /** Set the custom element cleanup function.
        @see VectorSetClean */
    void (*set_clean) (struct _Vector*, VectorClean);
} Vector;


/*===========================================================================*
 *             Definition for the exported member operations                 *
 *===========================================================================*/
/**
 * @brief The constructor for Vector.
 *
 * @param cap           The initial capacity
 *
 * @retval obj          The successfully constructed vector
 * @retval NULL         Insufficient memory for vector construction
 */
Vector* VectorInit(unsigned cap);

/**
 * @brief The destructor for Vector.
 *
 * @param obj           The pointer to the to be destructed vector
 */
void VectorDeinit(Vector* obj);

/**
 * @brief Push an element to the tail of the vector.
 *
 * @param self          The pointer to Vector structure
 * @param element       The specified element
 *
 * @retval true         The element is successfully pushed
 * @retval false        The element cannot be pushed due to insufficient memory
 */
bool VectorPushBack(Vector* self, void* element);

/**
 * @brief Insert an element to the specified index of the vector.
 *
 * This function inserts an element to the specified index of the vector and
 * shifts the trailing elements one position to the tail.
 *
 * @param self          The pointer to Vector structure
 * @param idx           The specified index
 * @param element       The specified element
 *
 * @retval true         The element is successfully inserted
 * @retval false        The element cannot be inserted due to invalid index
 *                      (>= vector size) or insufficient memory
 */
bool VectorInsert(Vector* self, unsigned idx, void* element);

/**
 * @brief Pop an element from the tail of the vector.
 *
 * This function removes an element from the tail of the vector. Also, the cleanup
 * function is invoked for the popped element.
 *
 * @param self          The pointer to Vector structure
 *
 * @retval true         The tail element is successfully popped
 * @retval false        The vector is empty
 */
bool VectorPopBack(Vector* self);

/**
 * @brief Remove an element from the specified index of the vector.
 *
 * This function removes an element from the specified index of the vector and
 * shifts the trailing elements one position to the head. Also, the cleanup
 * function is invoked for the removed element.
 *
 * @param self          The pointer to Vector structure
 * @param idx           The specified index
 *
 * @retval true         The specified element is successfully removed.
 * @retval false        Invalid index (>= vector size) or empty vector
 */
bool VectorRemove(Vector* self, unsigned idx);

/**
 * @brief Set an element at the specified index of the vector.
 *
 * This function sets an element at the specified index of the vector. Also, the
 * cleanup function is invoked for the replaced element.
 *
 * @param self          The pointer to Vector structure
 * @param idx           The specified index
 * @param element          The specified element
 *
 * @retval true         The specified element is successfully set.
 * @retval false        Invalid index (>= vector size)
 */
bool VectorSet(Vector* self, unsigned idx, void* element);

/**
 * @brief Get an element from the specified index of the vector.
 *
 * @param self          The pointer to Vector structure
 * @param idx           The specified index
 * @param p_element     The pointer to the returned element
 *
 * @retval true         The specified element is successfully retrieved
 * @retval false        Invalid index (>= vector size)
 */
bool VectorGet(Vector* self, unsigned idx, void** p_element);

/**
 * @brief Resize the vector so that it can contain the specified number of elements.
 *
 * This function resizes the vector so that it can contain the specified number of
 * elements. If the given element count is smaller than the old vector size. The
 * trailing elements are removed. Also, the cleanup function is triggered.
 *
 * @param self          The pointer to Vector structure
 * @param capacity      The specified element count
 *
 * @retval true         The vector is successfully resized
 * @retval false        The vector cannot be resized due to invalid index (>=
 *                      vector size) or insufficient memory
 */
bool VectorResize(Vector* self, unsigned capacity);

/**
 * @brief Return the number of stored elements.
 *
 * @param self          The pointer to Vector structure
 *
 * @retval size         The number of stored elements
 */
unsigned VectorSize(Vector* self);

/**
 * @brief Return the container capacity.
 *
 * @param self          The pointer to Vector structure
 *
 * @retval cap          The vector capacity
 */
unsigned VectorCapacity(Vector* self);

/**
 * @brief Sort the elements via the specified element comparison function.
 *
 * @param self          The pointer to Vector structure
 * @param func          The custom function
 */
void VectorSort(Vector* self, VectorCompare func);

/**
 * @brief Initialize the vector iterator.
 *
 * @param self          The pointer to Vector structure
 * @param is_reverse    Whether to apply the reversed traversal
 */
void VectorFirst(Vector* self, bool is_reverse);

/**
 * @brief Get the element pointed by the iterator and advance the iterator.
 *
 * @param self          The pointer to Vector structure
 * @param p_element     The pointer to the returned element
 *
 * @retval true         The tail end is not reached
 * @retval false        The tail end is reached
 */
bool VectorNext(Vector* self, void** p_element);

/**
 * @brief Get the element pointed by the iterator and advance the iterator
 * in the reverse order.
 *
 * @param self          The pointer to Vector structure
 * @param p_element        The pointer to the returned element
 *
 * @retval true         The head end is not reached
 * @retval false        The head end is reached
 */
bool VectorReverseNext(Vector* self, void** p_element);

/**
 * @brief Set the custom element cleanup function
 *
 * @param self          The pointer to Vector structure
 * @param func          The custom function
 */
void VectorSetClean(Vector* self, VectorClean func);

#ifdef __cplusplus
}
#endif

#endif
