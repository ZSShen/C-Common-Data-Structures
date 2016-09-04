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

#include "container/list.h"


/*===========================================================================*
 *                        The container private data                         *
 *===========================================================================*/
typedef struct _ListNode {
   void* element_;
   struct _ListNode* pred_;
   struct _ListNode* succ_;
} ListNode;

struct _ListData {
    unsigned size_;
    unsigned iter_round_;
    ListNode* head_;
    ListNode* iter_node_;
    ListClean func_clean_;
};


/*===========================================================================*
 *                  Definition for internal operations                       *
 *===========================================================================*/
#define likely(x)       __builtin_expect(!!(x), 1)
#define unlikely(x)     __builtin_expect(!!(x), 0)


/*===========================================================================*
 *               Implementation for the exported operations                  *
 *===========================================================================*/
List* ListInit()
{
    List* obj = (List*)malloc(sizeof(List));
    if (unlikely(!obj))
        return NULL;

    ListData* data = (ListData*)malloc(sizeof(ListData));
    if (unlikely(!data)) {
        free(obj);
        return NULL;
    }

    data->size_ = 0;
    data->head_ = NULL;
    data->iter_node_ = NULL;
    data->func_clean_ = NULL;

    obj->data = data;
    obj->push_front = ListPushFront;
    obj->push_back = ListPushBack;
    obj->insert = ListInsert;
    obj->pop_front = ListPopFront;
    obj->pop_back = ListPopBack;
    obj->remove = ListRemove;
    obj->set_front = ListSetFront;
    obj->set_back = ListSetBack;
    obj->set_at = ListSetAt;
    obj->get_front = ListGetFront;
    obj->get_back = ListGetBack;
    obj->get_at = ListGetAt;
    obj->size = ListSize;
    obj->reverse = ListReverse;
    obj->first = ListFirst;
    obj->next = ListNext;
    obj->reverse_next = ListReverseNext;
    obj->set_clean = ListSetClean;

    return obj;
}

void ListDeinit(List* obj)
{
    if (unlikely(!obj))
        return;

    ListData* data = obj->data;
    ListNode* curr = data->head_;
    if (curr) {
        ListNode* head = data->head_;
        ListClean func_clean = data->func_clean_;
        do {
            ListNode* pred = curr;
            curr = curr->succ_;
            if (func_clean)
                func_clean(pred->element_);
            free(pred);
        } while (curr != head);
    }

    free(data);
    free(obj);
    return;
}

bool ListPushFront(List* self, void* element)
{
    ListNode* new_node = (ListNode*)malloc(sizeof(ListNode));
    if (unlikely(!new_node))
        return false;
    new_node->element_ = element;

    ListData* data = self->data;
    ListNode* head = data->head_;
    if (unlikely(!head))
        new_node->pred_ = new_node->succ_ = new_node;
    else {
        new_node->succ_ = head;
        new_node->pred_ = head->pred_;
        head->pred_->succ_ = new_node;
        head->pred_ = new_node;
    }

    data->head_ = new_node;
    data->size_++;
    return true;
}

bool ListPushBack(List* self, void* element)
{
    ListNode* new_node = (ListNode*)malloc(sizeof(ListNode));
    if (unlikely(!new_node))
        return false;
    new_node->element_ = element;

    ListData* data = self->data;
    ListNode* head = data->head_;
    if (unlikely(!head)) {
        new_node->pred_ = new_node->succ_ = new_node;
        data->head_ = new_node;
    } else {
        new_node->succ_ = head;
        new_node->pred_ = head->pred_;
        head->pred_->succ_ = new_node;
        head->pred_ = new_node;
    }

    data->size_++;
    return true;
}

bool ListInsert(List* self, unsigned idx, void* element)
{
    ListData* data = self->data;
    unsigned size = data->size_;
    if (unlikely(idx > size))
        return false;

    ListNode* new_node = (ListNode*)malloc(sizeof(ListNode));
    if (unlikely(!new_node))
        return false;
    new_node->element_ = element;

    ListNode* head = data->head_;
    if (unlikely(!head)) {
        new_node->pred_ = new_node->succ_ = new_node;
        data->head_ = new_node;
    } else {
        if (idx == 0)
            data->head_ = new_node;

        ListNode* track = head;
        while (idx > 0) {
            track = track->succ_;
            --idx;
        }
        new_node->succ_ = track;
        new_node->pred_ = track->pred_;
        track->pred_->succ_ = new_node;
        track->pred_ = new_node;
    }

    data->size_++;
    return true;
}

bool ListPopFront(List* self)
{
    ListData* data = self->data;
    ListNode* head = data->head_;
    if (unlikely(!head))
        return false;

    ListClean func_clean = data->func_clean_;
    if (unlikely(head == head->pred_)) {
        if (func_clean)
            func_clean(head->element_);
        free(head);
        data->head_ = NULL;
    } else {
        head->pred_->succ_ = head->succ_;
        head->succ_->pred_ = head->pred_;
        data->head_ = head->succ_;
        if (func_clean)
            func_clean(head->element_);
        free(head);
    }

    data->size_--;
    return true;
}

bool ListPopBack(List* self)
{
    ListData* data = self->data;
    ListNode* head = data->head_;
    if (unlikely(!head))
        return false;

    ListClean func_clean = data->func_clean_;
    if (unlikely(head == head->succ_)) {
        if (func_clean)
            func_clean(head->element_);
        free(head);
        data->head_ = NULL;
    } else {
        ListNode* tail = head->pred_;
        tail->pred_->succ_ = head;
        head->pred_ = tail->pred_;
        if (func_clean)
            func_clean(tail->element_);
        free(tail);
    }

    data->size_--;
    return true;
}

bool ListRemove(List* self, unsigned idx)
{
    ListData* data = self->data;
    unsigned size = data->size_;
    if (unlikely(idx >= size))
        return false;

    ListNode* head = data->head_;
    ListNode* track = head;
    while (idx > 0) {
        track = track->succ_;
        --idx;
    }

    ListNode* pred = track->pred_;
    ListNode* succ = track->succ_;
    pred->succ_ = succ;
    succ->pred_ = pred;

    if (unlikely(size == 1))
        data->head_ = NULL;
    else {
        if (track == head)
            data->head_ = succ;
    }

    ListClean func_clean = data->func_clean_;
    if (func_clean)
        func_clean(track->element_);
    free(track);

    data->size_ = size - 1;
    return true;
}

bool ListSetFront(List* self, void* element)
{
    ListData* data = self->data;
    ListNode* head = data->head_;
    if (unlikely(!head))
        return false;

    ListClean func_clean = data->func_clean_;
    if (func_clean)
        func_clean(head->element_);
    head->element_ = element;
    return true;
}

bool ListSetBack(List* self, void* element)
{
    ListData* data = self->data;
    ListNode* head = data->head_;
    if (unlikely(!head))
        return false;

    ListNode* tail = head->pred_;
    ListClean func_clean = data->func_clean_;
    if (func_clean)
        func_clean(tail->element_);
    tail->element_ = element;
    return true;
}

bool ListSetAt(List* self, unsigned idx, void* element)
{
    ListData* data = self->data;
    if (unlikely(idx >= data->size_))
        return false;

    ListNode* track = data->head_;
    while (idx > 0) {
        track = track->succ_;
        --idx;
    }

    ListClean func_clean = data->func_clean_;
    if (func_clean)
        func_clean(track->element_);
    track->element_ = element;
    return true;
}

bool ListGetFront(List* self, void** p_element)
{
    ListData* data = self->data;
    ListNode* head = data->head_;
    if (unlikely(!head))
        return false;

    *p_element = head->element_;
    return true;
}

bool ListGetBack(List* self, void** p_element)
{
    ListData* data = self->data;
    ListNode* head = data->head_;
    if (unlikely(!head))
        return false;

    *p_element = head->pred_->element_;
    return true;
}

bool ListGetAt(List* self, unsigned idx, void** p_element)
{
    ListData* data = self->data;
    if (unlikely(idx >= data->size_))
        return false;

    ListNode* track = data->head_;
    while (idx > 0) {
        track = track->succ_;
        --idx;
    }

    *p_element = track->element_;
    return true;
}

unsigned ListSize(List* self)
{
    return self->data->size_;
}

void ListReverse(List* self)
{
    ListData* data = self->data;
    ListNode* head = data->head_;
    ListNode* tail = data->head_->pred_;

    unsigned range = data->size_;
    while (range > 1) {
        void* element_head = head->element_;
        void* element_tail = tail->element_;
        head->element_ = element_tail;
        tail->element_ = element_head;
        head = head->succ_;
        tail = tail->pred_;
        range -= 2;
    }
    return;
}

void ListFirst(List* self, bool is_reverse)
{
    ListData* data = self->data;
    ListNode* head = data->head_;
    if (unlikely(!head))
        return;
    data->iter_round_ = 0;
    data->iter_node_ = (is_reverse == false)? head : head->pred_;
}

bool ListNext(List* self, void** p_element)
{
    ListData* data = self->data;
    unsigned iter_round = data->iter_round_;
    if (unlikely(iter_round == data->size_))
        return false;

    ListNode* iter_node = data->iter_node_;
    *p_element = iter_node->element_;
    data->iter_node_ = iter_node->succ_;
    data->iter_round_ = iter_round + 1;
    return true;
}

bool ListReverseNext(List* self, void** p_element)
{
    ListData* data = self->data;
    unsigned iter_round = data->iter_round_;
    if (unlikely(iter_round == data->size_))
        return false;

    ListNode* iter_node = data->iter_node_;
    *p_element = iter_node->element_;
    data->iter_node_ = iter_node->pred_;
    data->iter_round_ = iter_round + 1;
    return true;
}

void ListSetClean(List* self, ListClean func)
{
    self->data->func_clean_ = func;
}
