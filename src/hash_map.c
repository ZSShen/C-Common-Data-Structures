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

#include "container/hash_map.h"
#include "math/hash.h"


/*===========================================================================*
 *                        The container private data                         *
 *===========================================================================*/
static const unsigned magic_primes[] = {
    769, 1543, 3079, 6151, 12289, 24593, 49157, 98317, 196613, 393241, 786433,
    1572869, 3145739, 6291469, 12582917, 25165843, 50331653, 100663319,
    201326611, 402653189, 805306457, 1610612741,
};
static const int num_prime = sizeof(magic_primes) / sizeof(unsigned);
static const double load_factor = 0.75;


typedef struct _SlotNode {
    Pair pair_;
    struct _SlotNode* next_;
} SlotNode;

struct _HashMapData {
    int size_;
    int idx_prime_;
    unsigned num_slot_;
    unsigned curr_limit_;
    unsigned iter_slot_;
    SlotNode** arr_slot_;
    SlotNode* iter_node_;
    HashMapHash func_hash_;
    HashMapCompare func_cmp_;
    HashMapCleanKey func_clean_key_;
    HashMapCleanValue func_clean_val_;
};


/*===========================================================================*
 *                  Definition for internal operations                       *
 *===========================================================================*/
#define likely(x)       __builtin_expect(!!(x), 1)
#define unlikely(x)     __builtin_expect(!!(x), 0)

/**
 * @brief The default hash function.
 *
 * @param key           The designated key
 *
 * @retval Hash         The corresponding hash value
 */
unsigned _HashMapHash(void* key);

/**
 * @brief The default hash key comparison function.
 *
 * @param lhs           The source key
 * @param rhs           The target key
 *
 * @retval  1           The source key should go after the target one.
 * @retval  0           The source key is equal to the target one.
 * @retval -1           The source key should go before the target one.
 */
int _HashMapCompare(void* lhs, void* rhs);

/**
 * @brief Extend the slot array and re-distribute the stored pairs.
 *
 * @param data         The pointer to the map private data
 */
void _HashMapReHash(HashMapData* data);


/*===========================================================================*
 *               Implementation for the exported operations                  *
 *===========================================================================*/
HashMap* HashMapInit()
{
    HashMap* obj = (HashMap*)malloc(sizeof(HashMap));
    if (unlikely(!obj))
        return NULL;

    HashMapData* data = (HashMapData*)malloc(sizeof(HashMapData));
    if (unlikely(!data)) {
        free(obj);
        return NULL;
    }

    SlotNode** arr_slot = (SlotNode**)malloc(sizeof(SlotNode*) * magic_primes[0]);
    if (unlikely(!arr_slot)) {
        free(data);
        free(obj);
        return NULL;
    }
    unsigned i;
    for (i = 0 ; i < magic_primes[0] ; ++i)
        arr_slot[i] = NULL;

    data->size_ = 0;
    data->idx_prime_ = 0;
    data->num_slot_ = magic_primes[0];
    data->curr_limit_ = (unsigned)((double)magic_primes[0] * load_factor);
    data->arr_slot_ = arr_slot;
    data->func_hash_ = _HashMapHash;
    data->func_cmp_ = _HashMapCompare;
    data->func_clean_key_ = NULL;
    data->func_clean_val_ = NULL;

    obj->data = data;
    obj->put = HashMapPut;
    obj->get = HashMapGet;
    obj->contain = HashMapContain;
    obj->remove = HashMapRemove;
    obj->size = HashMapSize;
    obj->first = HashMapFirst;
    obj->next = HashMapNext;
    obj->set_hash = HashMapSetHash;
    obj->set_compare = HashMapSetCompare;
    obj->set_clean_key = HashMapSetCleanKey;
    obj->set_clean_value = HashMapSetCleanValue;

    return obj;
}

void HashMapDeinit(HashMap* obj)
{
    if (unlikely(!obj))
        return;

    HashMapData* data = obj->data;
    SlotNode** arr_slot = data->arr_slot_;
    HashMapCleanKey func_clean_key = data->func_clean_key_;
    HashMapCleanValue func_clean_val = data->func_clean_val_;

    unsigned num_slot = data->num_slot_;
    unsigned i;
    for (i = 0 ; i < num_slot ; ++i) {
        SlotNode* pred;
        SlotNode* curr = arr_slot[i];
        while (curr) {
            pred = curr;
            curr = curr->next_;
            if (func_clean_key)
                func_clean_key(pred->pair_.key);
            if (func_clean_val)
                func_clean_val(pred->pair_.value);
            free(pred);
        }
    }

    free(arr_slot);
    free(data);
    free(obj);
    return;
}

bool HashMapPut(HashMap* self, void* key, void* value)
{
    /* Check the loading factor for rehashing. */
    HashMapData* data = self->data;
    if (data->size_ >= data->curr_limit_)
        _HashMapReHash(data);

    /* Calculate the slot index. */
    unsigned hash = data->func_hash_(key);
    hash = hash % data->num_slot_;

    /* Check if the pair conflicts with a certain one stored in the map. If yes,
       replace that one. */
    HashMapCompare func_cmp = data->func_cmp_;
    SlotNode** arr_slot = data->arr_slot_;
    SlotNode* curr = arr_slot[hash];
    while (curr) {
        if (func_cmp(key, curr->pair_.key) == 0) {
            if (data->func_clean_key_)
                data->func_clean_key_(curr->pair_.key);
            if (data->func_clean_val_)
                data->func_clean_val_(curr->pair_.value);
            curr->pair_.key = key;
            curr->pair_.value = value;
            return true;
        }
        curr = curr->next_;
    }

    /* Insert the new pair into the slot list. */
    SlotNode* node = (SlotNode*)malloc(sizeof(SlotNode));
    if (unlikely(!node))
        return false;

    node->pair_.key = key;
    node->pair_.value = value;
    if (!(arr_slot[hash])) {
        node->next_ = NULL;
        arr_slot[hash] = node;
    } else {
        node->next_ = arr_slot[hash];
        arr_slot[hash] = node;
    }
    ++(data->size_);

    return true;
}

void* HashMapGet(HashMap* self, void* key)
{
    HashMapData* data = self->data;

    /* Calculate the slot index. */
    unsigned hash = data->func_hash_(key);
    hash = hash % data->num_slot_;

    /* Search the slot list to check if there is a pair having the same key
       with the designated one. */
    HashMapCompare func_cmp = data->func_cmp_;
    SlotNode* curr = data->arr_slot_[hash];
    while (curr) {
        if (func_cmp(key, curr->pair_.key) == 0)
            return curr->pair_.value;
        curr = curr->next_;
    }

    return NULL;
}

bool HashMapContain(HashMap* self, void* key)
{
    HashMapData* data = self->data;

    /* Calculate the slot index. */
    unsigned hash = data->func_hash_(key);
    hash = hash % data->num_slot_;

    /* Search the slot list to check if there is a pair having the same key
       with the designated one. */
    HashMapCompare func_cmp = data->func_cmp_;
    SlotNode* curr = data->arr_slot_[hash];
    while (curr) {
        if (func_cmp(key, curr->pair_.key) == 0)
            return true;
        curr = curr->next_;
    }

    return false;
}

bool HashMapRemove(HashMap* self, void* key)
{
    HashMapData* data = self->data;

    /* Calculate the slot index. */
    unsigned hash = data->func_hash_(key);
    hash = hash % data->num_slot_;

    /* Search the slot list for the deletion target. */
    HashMapCompare func_cmp = data->func_cmp_;
    SlotNode* pred = NULL;
    SlotNode** arr_slot = data->arr_slot_;
    SlotNode* curr = arr_slot[hash];
    while (curr) {
        if (func_cmp(key, curr->pair_.key) == 0) {
            if (data->func_clean_key_)
                data->func_clean_key_(curr->pair_.key);
            if (data->func_clean_val_)
                data->func_clean_val_(curr->pair_.value);

            if (!pred)
                arr_slot[hash] = curr->next_;
            else
                pred->next_ = curr->next_;

            free(curr);
            --(data->size_);
            return true;
        }
        pred = curr;
        curr = curr->next_;
    }

    return false;
}

unsigned HashMapSize(HashMap* self)
{
    return self->data->size_;
}

void HashMapFirst(HashMap* self)
{
    HashMapData* data = self->data;
    data->iter_slot_ = 0;
    data->iter_node_ = data->arr_slot_[0];
    return;
}

Pair* HashMapNext(HashMap* self)
{
    HashMapData* data = self->data;

    SlotNode** arr_slot = data->arr_slot_;
    while (data->iter_slot_ < data->num_slot_) {
        if (data->iter_node_) {
            Pair* ptr_pair = &(data->iter_node_->pair_);
            data->iter_node_ = data->iter_node_->next_;
            return ptr_pair;
        }
        ++(data->iter_slot_);
        if (data->iter_slot_ == data->num_slot_)
            break;
        data->iter_node_ = arr_slot[data->iter_slot_];
    }
    return NULL;
}

void HashMapSetHash(HashMap* self, HashMapHash func)
{
    self->data->func_hash_ = func;
}

void HashMapSetCompare(HashMap* self, HashMapCompare func)
{
    self->data->func_cmp_ = func;
}

void HashMapSetCleanKey(HashMap* self, HashMapCleanKey func)
{
    self->data->func_clean_key_ = func;
}

void HashMapSetCleanValue(HashMap* self, HashMapCleanValue func)
{
    self->data->func_clean_val_ = func;
}


/*===========================================================================*
 *               Implementation for internal operations                      *
 *===========================================================================*/
unsigned _HashMapHash(void* key)
{
    return (unsigned)(intptr_t)key;
}

int _HashMapCompare(void* lhs, void* rhs)
{
    if ((intptr_t)lhs == (intptr_t)rhs)
        return 0;
    return ((intptr_t)lhs > (intptr_t)rhs)? 1 : (-1);
}

void _HashMapReHash(HashMapData* data)
{
    unsigned num_slot_new;

    /* Consume the next prime for slot array extension. */
    if (likely(data->idx_prime_ < (num_prime - 1))) {
        ++(data->idx_prime_);
        num_slot_new = magic_primes[data->idx_prime_];
    }
    /* If the prime list is completely consumed, we simply extend the slot array
       with treble capacity.*/
    else {
        data->idx_prime_ = num_prime;
        num_slot_new = data->num_slot_ * 3;
    }

    /* Try to allocate the new slot array. The rehashing should be canceled due
       to insufficient memory space.  */
    SlotNode** arr_slot_new = (SlotNode**)malloc(sizeof(SlotNode*) * num_slot_new);
    if (unlikely(!arr_slot_new)) {
        if (data->idx_prime_ < num_prime)
            --(data->idx_prime_);
        return;
    }

    unsigned i;
    for (i = 0 ; i < num_slot_new ; ++i)
        arr_slot_new[i] = NULL;

    HashMapHash func_hash = data->func_hash_;
    SlotNode** arr_slot = data->arr_slot_;
    unsigned num_slot = data->num_slot_;
    for (i = 0 ; i < num_slot ; ++i) {
        SlotNode* pred;
        SlotNode* curr = arr_slot[i];
        while (curr) {
            pred = curr;
            curr = curr->next_;

            /* Migrate each key value pair to the new slot. */
            unsigned hash = func_hash(pred->pair_.key);
            hash = hash % num_slot_new;
            if (!arr_slot_new[hash]) {
                pred->next_ = NULL;
                arr_slot_new[hash] = pred;
            } else {
                pred->next_ = arr_slot_new[hash];
                arr_slot_new[hash] = pred;
            }
        }
    }

    free(arr_slot);
    data->arr_slot_ = arr_slot_new;
    data->num_slot_ = num_slot_new;
    data->curr_limit_ = (unsigned)((double)num_slot_new * load_factor);
    return;
}
