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

#include "container/priority_queue.h"


/*===========================================================================*
 *                        The container private data                         *
 *===========================================================================*/
struct _PriorityQueueData {
    unsigned size_;
    unsigned capacity_;
    void** elements_;
    PriorityQueueCompare func_cmp_;
    PriorityQueueClean func_clean_;
};

static const unsigned DEFAULT_CAPACITY = 32;


/*===========================================================================*
 *                  Definition for internal operations                       *
 *===========================================================================*/
#define likely(x)       __builtin_expect(!!(x), 1)
#define unlikely(x)     __builtin_expect(!!(x), 0)

static inline unsigned PARENT(unsigned idx)
{
    return idx >> 1;
}

static inline unsigned LEFT(unsigned idx)
{
    return idx << 1;
}

static inline unsigned RIGHT(unsigned idx)
{
    return (idx << 1) + 1;
}

/**
 * @brief The default element comparison function.
 *
 * @param lhs           The source element
 * @param rhs           The target element
 *
 * @retval  1           The source element should go after the target one.
 * @retval  0           The source element is equal to the target one.
 * @retval -1           The source element should go before the target one.
 */
static int _PriorityQueueCompare(const void* lhs, const void* rhs);


/*===========================================================================*
 *               Implementation for the exported operations                  *
 *===========================================================================*/
PriorityQueue* PriorityQueueInit()
{
    PriorityQueue* obj = (PriorityQueue*)malloc(sizeof(PriorityQueue));
    if (unlikely(!obj))
        return NULL;

    PriorityQueueData* data = (PriorityQueueData*)malloc(sizeof(PriorityQueueData));
    if (unlikely(!data)) {
        free(obj);
        return NULL;
    }

    void** elements = (void**)malloc(sizeof(void*) * DEFAULT_CAPACITY);
    if (unlikely(!elements)) {
        free(data);
        free(obj);
        return NULL;
    }

    data->size_ = 0;
    data->capacity_ = DEFAULT_CAPACITY;
    data->elements_ = elements;
    data->func_cmp_ = _PriorityQueueCompare;
    data->func_clean_ = NULL;

    obj->data = data;
    obj->push = PriorityQueuePush;
    obj->top = PriorityQueueTop;
    obj->pop = PriorityQueuePop;
    obj->size = PriorityQueueSize;
    obj->set_compare = PriorityQueueSetCompare;
    obj->set_clean = PriorityQueueSetClean;

    return obj;
}

void PriorityQueueDeinit(PriorityQueue* obj)
{
    if (unlikely(!obj))
        return;

    PriorityQueueData* data = obj->data;
    PriorityQueueClean func_clean = data->func_clean_;
    void** elements = data->elements_;
    unsigned size = data->size_;

    unsigned i;
    for (i = 0 ; i < size ; ++i) {
        if (func_clean)
            func_clean(elements[i]);
    }

    free(elements);
    free(data);
    free(obj);
    return;
}

bool PriorityQueuePush(PriorityQueue* self, void* element)
{
    PriorityQueueData* data = self->data;
    void** elements = data->elements_;
    unsigned size = data->size_;
    unsigned capacity = data->capacity_;

    /* If the heap is full, extend it to double capacity. */
    if (size == capacity) {
        unsigned new_capacity = capacity << 1;
        void** new_elements = (void**)realloc(elements, new_capacity * sizeof(void*));
        if (!new_elements)
            return false;
        elements = data->elements_ = new_elements;
        data->capacity_ = new_capacity;
    }

    /* Push the element to the bottom of the heap. */
    elements[data->size_] = element;
    data->size_ = size + 1;

    /* Adjust the heap structure. */
    PriorityQueueCompare func_cmp = data->func_cmp_;
    unsigned curr = data->size_;
    unsigned parent;
    int order;
    while (curr > 1) {
        parent = PARENT(curr);
        order = func_cmp(elements[curr - 1], elements[parent - 1]);
        if (order > 0)
            break;
        element = elements[curr - 1];
        elements[curr - 1] = elements[parent - 1];
        elements[parent - 1] = element;
        curr = parent;
    }

    return true;
}

bool PriorityQueuePop(PriorityQueue* self)
{
    PriorityQueueData* data = self->data;
    void** elements = data->elements_;
    PriorityQueueCompare func_cmp = data->func_cmp_;
    PriorityQueueClean func_clean = data->func_clean_;
    unsigned size = data->size_;
    if (unlikely(size == 0))
        return false;

    /* Remove the heap top element. */
    if (func_clean)
        func_clean(elements[0]);
    --size;
    elements[0] = elements[size];
    data->size_ = size;

    /* Adjust the heap structure. */
    unsigned curr = 1;
    unsigned child_l, child_r, next;
    int order;
    do {
        child_l = LEFT(curr);
        if (child_l > size)
            break;

        next = curr;
        order = func_cmp(elements[child_l - 1], elements[next - 1]);
        if (order <= 0)
            next = child_l;

        child_r = RIGHT(curr);
        if (child_r <= size) {
            order = func_cmp(elements[child_r - 1], elements[next - 1]);
            if (order <= 0)
                next = child_r;
        }

        if (next == curr)
            break;

        void* element = elements[next - 1];
        elements[next - 1] = elements[curr - 1];
        elements[curr - 1] = element;
        curr = next;
    } while (true);

    return true;
}

bool PriorityQueueTop(PriorityQueue* self, void** p_element)
{
    PriorityQueueData* data = self->data;
    if (unlikely(data->size_ == 0))
        return false;
    *p_element = data->elements_[0];
    return true;
}

unsigned PriorityQueueSize(PriorityQueue* self)
{
    return self->data->size_;
}

void PriorityQueueSetCompare(PriorityQueue* self, PriorityQueueCompare func)
{
    self->data->func_cmp_ = func;
}

void PriorityQueueSetClean(PriorityQueue* self, PriorityQueueClean func)
{
    self->data->func_clean_ = func;
}


/*===========================================================================*
 *               Implementation for internal operations                      *
 *===========================================================================*/
int _PriorityQueueCompare(const void* lhs, const void* rhs)
{
    if ((intptr_t)lhs == (intptr_t)rhs)
        return 0;
    return ((intptr_t)lhs >= (intptr_t)rhs)? 1 : (-1);
}
