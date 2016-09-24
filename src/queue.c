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

#include "container/queue.h"


/*===========================================================================*
 *                        The container private data                         *
 *===========================================================================*/
struct _QueueData {
    unsigned front_;
    unsigned back_;
    unsigned size_;
    unsigned capacity_;
    void** elements_;
    QueueClean func_clean_;
};

static const unsigned DEFAULT_CAPACITY = 32;


/*===========================================================================*
 *                  Definition for internal operations                       *
 *===========================================================================*/
#define likely(x)       __builtin_expect(!!(x), 1)
#define unlikely(x)     __builtin_expect(!!(x), 0)


/*===========================================================================*
 *               Implementation for the exported operations                  *
 *===========================================================================*/
Queue* QueueInit()
{
    Queue* obj = (Queue*)malloc(sizeof(Queue));
    if (unlikely(!obj))
        return NULL;

    QueueData* data = (QueueData*)malloc(sizeof(QueueData));
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

    data->front_ = 0;
    data->back_ = 0;
    data->size_ = 0;
    data->capacity_ = DEFAULT_CAPACITY;
    data->elements_ = elements;
    data->func_clean_ = NULL;

    obj->data = data;
    obj->push = QueuePush;
    obj->front = QueueFront;
    obj->back = QueueBack;
    obj->pop = QueuePop;
    obj->size = QueueSize;
    obj->set_clean = QueueSetClean;

    return obj;
}

void QueueDeinit(Queue* obj)
{
    if (unlikely(!obj))
        return;

    QueueData* data = obj->data;
    QueueClean func_clean = data->func_clean_;
    void** elements = data->elements_;
    unsigned front = data->front_;
    unsigned size = data->size_;
    unsigned capacity = data->capacity_;

    while (size > 0) {
        if (unlikely(front == capacity))
            front = 0;
        if (func_clean)
            func_clean(elements[front]);
        ++front;
        --size;
    }

    free(elements);
    free(data);
    free(obj);
    return;
}


bool QueuePush(Queue* self, void* element)
{
    QueueData* data = self->data;
    void** elements = data->elements_;
    unsigned size = data->size_;
    unsigned capacity = data->capacity_;

    /* If the internal array is full, extend it to double capacity. */
    if (unlikely(size == capacity)) {
        unsigned new_capacity = capacity << 1;
        void** new_elements = (void**)realloc(elements, new_capacity * sizeof(void*));
        if (unlikely(!new_elements))
            return false;
        elements = data->elements_ = new_elements;
        capacity = data->capacity_ = new_capacity;

        /* If back index is smaller than front index, we should migrate the
           circularly pushed elements to the newly allocated space. */
        unsigned back = data->back_;
        if (back <= data->front_) {
            memmove(elements + size, elements, sizeof(void*) * back);
            data->back_ += size;
        }
    }

    /* Insert the element to the tail of the array. */
    unsigned back = data->back_;
    elements[back++] = element;
    if (unlikely(back == capacity))
        back = 0;
    data->back_ = back;
    data->size_ = size + 1;

    return true;
}

bool QueueFront(Queue* self, void** p_element)
{
    QueueData* data = self->data;
    if (unlikely(data->size_ == 0))
        return false;

    *p_element = data->elements_[data->front_];
    return true;
}

bool QueueBack(Queue* self, void** p_element)
{
    QueueData* data = self->data;
    if (unlikely(data->size_ == 0))
        return false;

    unsigned back = data->back_;
    if (unlikely(back == 0))
        *p_element = data->elements_[data->capacity_ - 1];
    else
        *p_element = data->elements_[back - 1];
    return true;
}

bool QueuePop(Queue* self)
{
    QueueData* data = self->data;
    void** elements = data->elements_;
    QueueClean func_clean = data->func_clean_;
    unsigned size = data->size_;
    if (unlikely(size == 0))
        return false;

    /* Remove the element from the head of the array. */
    unsigned front = data->front_;
    if (func_clean)
        func_clean(elements[front]);
    ++front;
    if (unlikely(front == data->capacity_))
        front = 0;
    data->front_ = front;
    data->size_ = size - 1;

    return true;
}

unsigned QueueSize(Queue* self)
{
    return self->data->size_;
}

void QueueSetClean(Queue* self, QueueClean func)
{
    self->data->func_clean_ = func;
}
