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

#include "container/hash_set.h"


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
    void* key_;
    struct _SlotNode* next_;
} SlotNode;

struct _HashSetData {
    int idx_prime_;
    unsigned size_;
    unsigned num_slot_;
    unsigned curr_limit_;
    unsigned iter_slot_;
    SlotNode** arr_slot_;
    SlotNode* iter_node_;
    HashSetHash func_hash_;
    HashSetCompare func_cmp_;
    HashSetCleanKey func_clean_key_;
};


/*===========================================================================*
 *                  Definition for internal operations                       *
 *===========================================================================*/
#define likely(x)       __builtin_expect(!!(x), 1)
#define unlikely(x)     __builtin_expect(!!(x), 0)

/**
 * @brief Initialize the set with the specified slot size.
 *
 * @param idx_prime     The index to the array of magic primes
 *
 * @retval obj          The successfully constructed set
 * @retval NULL         Insufficient memory for set construction
 */
HashSet* _HashSetInit(int idx_prime);

/**
 * @brief The default hash function.
 *
 * @param key           The specified key
 *
 * @retval hash         The corresponding hash value
 */
unsigned _HashSetHash(void* key);

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
int _HashSetCompare(void* lhs, void* rhs);

/**
 * @brief Extend the slot array and re-distribute the stored keys.
 *
 * @param data          The pointer to the map private data
 */
void _HashSetReHash(HashSetData* data);


/*===========================================================================*
 *               Implementation for the exported operations                  *
 *===========================================================================*/
HashSet* HashSetInit()
{
    return _HashSetInit(0);
}

void HashSetDeinit(HashSet* obj)
{
    if (unlikely(!obj))
        return;

    HashSetData* data = obj->data;
    SlotNode** arr_slot = data->arr_slot_;
    HashSetCleanKey func_clean_key = data->func_clean_key_;

    unsigned num_slot = data->num_slot_;
    unsigned i;
    for (i = 0 ; i < num_slot ; ++i) {
        SlotNode* pred;
        SlotNode* curr = arr_slot[i];
        while (curr) {
            pred = curr;
            curr = curr->next_;
            if (func_clean_key)
                func_clean_key(pred->key_);
            free(pred);
        }
    }

    free(arr_slot);
    free(data);
    free(obj);
    return;
}

bool HashSetAdd(HashSet* self, void* key)
{
    /* Check the loading factor for rehashing. */
    HashSetData* data = self->data;
    if (data->size_ >= data->curr_limit_)
        _HashSetReHash(data);

    /* Calculate the slot index. */
    unsigned hash = data->func_hash_(key);
    hash = hash % data->num_slot_;

    /* Check if the key conflicts with a certain one stored in the set. If yes,
       replace that one. */
    HashSetCompare func_cmp = data->func_cmp_;
    SlotNode** arr_slot = data->arr_slot_;
    SlotNode* curr = arr_slot[hash];
    while (curr) {
        if (func_cmp(key, curr->key_) == 0) {
            if (data->func_clean_key_)
                data->func_clean_key_(curr->key_);
            curr->key_ = key;
            return true;
        }
        curr = curr->next_;
    }

    /* Insert the new key into the slot list. */
    SlotNode* node = (SlotNode*)malloc(sizeof(SlotNode));
    if (unlikely(!node))
        return false;

    node->key_ = key;
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

bool HashSetFind(HashSet* self, void* key)
{
    HashSetData* data = self->data;

    /* Calculate the slot index. */
    unsigned hash = data->func_hash_(key);
    hash = hash % data->num_slot_;

    /* Search the slot list to check if the specified key exists. */
    HashSetCompare func_cmp = data->func_cmp_;
    SlotNode* curr = data->arr_slot_[hash];
    while (curr) {
        if (func_cmp(key, curr->key_) == 0)
            return true;
        curr = curr->next_;
    }

    return false;
}

bool HashSetRemove(HashSet* self, void* key)
{
    HashSetData* data = self->data;

    /* Calculate the slot index. */
    unsigned hash = data->func_hash_(key);
    hash = hash % data->num_slot_;

    /* Search the slot list for the remove target. */
    HashSetCompare func_cmp = data->func_cmp_;
    SlotNode* pred = NULL;
    SlotNode** arr_slot = data->arr_slot_;
    SlotNode* curr = arr_slot[hash];
    while (curr) {
        if (func_cmp(key, curr->key_) == 0) {
            if (data->func_clean_key_)
                data->func_clean_key_(curr->key_);

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

unsigned HashSetSize(HashSet* self)
{
    return self->data->size_;
}

void HashSetFirst(HashSet* self)
{
    HashSetData* data = self->data;
    data->iter_slot_ = 0;
    data->iter_node_ = data->arr_slot_[0];
    return;
}

void* HashSetNext(HashSet* self)
{
    HashSetData* data = self->data;

    SlotNode** arr_slot = data->arr_slot_;
    while (data->iter_slot_ < data->num_slot_) {
        if (data->iter_node_) {
            void* key = data->iter_node_->key_;
            data->iter_node_ = data->iter_node_->next_;
            return key;
        }
        ++(data->iter_slot_);
        if (data->iter_slot_ == data->num_slot_)
            break;
        data->iter_node_ = arr_slot[data->iter_slot_];
    }
    return NULL;
}

void HashSetSetHash(HashSet* self, HashSetHash func)
{
    self->data->func_hash_ = func;
}

void HashSetSetCompare(HashSet* self, HashSetCompare func)
{
    self->data->func_cmp_ = func;
}

void HashSetSetCleanKey(HashSet* self, HashSetCleanKey func)
{
    self->data->func_clean_key_ = func;
}

HashSet* HashSetUnion(HashSet* lhs, HashSet* rhs)
{
    /* Predict the required slot size for the result set. */
    unsigned size_lhs = lhs->data->size_;
    unsigned size_rhs = rhs->data->size_;
    unsigned size_slot = (unsigned)((double)(size_lhs + size_rhs) / load_factor);
    int idx_prime = 0;
    while (idx_prime < num_prime) {
        if (size_slot < magic_primes[idx_prime])
            break;
        ++idx_prime;
    }
    if (idx_prime == num_prime)
        --idx_prime;

    /* Initialize the result set. */
    HashSet* result = _HashSetInit(idx_prime);
    if (!result)
        return NULL;

    HashSetData* data_lhs = lhs->data;
    HashSetData* data_result = result->data;
    data_result->func_hash_ = data_lhs->func_hash_;
    data_result->func_cmp_ = data_lhs->func_cmp_;

    /* Collect the first source set. */
    SlotNode** arr_slot = data_lhs->arr_slot_;
    unsigned num_slot = data_lhs->num_slot_;
    unsigned i;
    for (i = 0 ; i < num_slot ; ++i) {
        SlotNode* pred;
        SlotNode* curr = arr_slot[i];
        while (curr) {
            pred = curr;
            curr = curr->next_;
            bool status = HashSetAdd(result, pred->key_);
            if (!status) {
                HashSetDeinit(result);
                return NULL;
            }
        }
    }

    /* Merge the second source set. */
    HashSetData* data_rhs = rhs->data;
    arr_slot = data_rhs->arr_slot_;
    num_slot = data_rhs->num_slot_;
    for (i = 0 ; i < num_slot ; ++i) {
        SlotNode* pred;
        SlotNode* curr = arr_slot[i];
        while (curr) {
            pred = curr;
            curr = curr->next_;
            bool status = HashSetAdd(result, pred->key_);
            if (!status) {
                HashSetDeinit(result);
                return NULL;
            }
        }
    }

    return result;
}

HashSet* HashSetIntersect(HashSet* lhs, HashSet* rhs)
{
    /* Predict the required slot size for the result set. */
    unsigned size_lhs = lhs->data->size_;
    unsigned size_rhs = rhs->data->size_;
    unsigned size_elem;
    HashSet *set_src, *set_tge;
    if (size_lhs < size_rhs) {
        size_elem = size_lhs;
        set_src = lhs;
        set_tge = rhs;
    } else {
        size_elem = size_rhs;
        set_src = rhs;
        set_tge = lhs;
    }
    unsigned size_slot = (unsigned)((double)size_elem / load_factor);
    int idx_prime = 0;
    while (idx_prime < num_prime) {
        if (size_slot < magic_primes[idx_prime])
            break;
        ++idx_prime;
    }
    if (idx_prime == num_prime)
        --idx_prime;

    /* Initialize the result set. */
    HashSet* result = _HashSetInit(idx_prime);
    if (!result)
        return NULL;

    HashSetData* data_src = set_src->data;
    HashSetData* data_result = result->data;
    data_result->func_hash_ = data_src->func_hash_;
    data_result->func_cmp_ = data_src->func_cmp_;

    /* Collect the keys belonged to both source sets. */
    SlotNode** arr_slot = data_src->arr_slot_;
    unsigned num_slot = data_src->num_slot_;
    unsigned i;
    for (i = 0 ; i < num_slot ; ++i) {
        SlotNode* pred;
        SlotNode* curr = arr_slot[i];
        while (curr) {
            pred = curr;
            curr = curr->next_;
            void* key = pred->key_;
            bool status = HashSetFind(set_tge, key);
            if (!status)
                continue;
            status = HashSetAdd(result, key);
            if (!status) {
                HashSetDeinit(result);
                return NULL;
            }
        }
    }

    return result;
}

HashSet* HashSetDifference(HashSet* lhs, HashSet* rhs)
{
    /* Predict the required slot size for the result set. */
    unsigned size_lhs = lhs->data->size_;
    unsigned size_rhs = rhs->data->size_;
    unsigned size_elem = (size_lhs > size_rhs)? size_lhs : size_rhs;
    unsigned size_slot = (unsigned)((double)size_elem / load_factor);
    int idx_prime = 0;
    while (idx_prime < num_prime) {
        if (size_slot < magic_primes[idx_prime])
            break;
        ++idx_prime;
    }
    if (idx_prime == num_prime)
        --idx_prime;

    /* Create the result set. */
    HashSet* result = _HashSetInit(idx_prime);
    if (!result)
        return NULL;

    HashSetData* data_lhs = lhs->data;
    HashSetData* data_result = result->data;
    data_result->func_hash_ = data_lhs->func_hash_;
    data_result->func_cmp_ = data_lhs->func_cmp_;

    /* Collect the keys only belonged to the first source set. */
    SlotNode** arr_slot = data_lhs->arr_slot_;
    unsigned num_slot = data_lhs->num_slot_;
    unsigned i;
    for (i = 0 ; i < num_slot ; ++i) {
        SlotNode* pred;
        SlotNode* curr = arr_slot[i];
        while (curr) {
            pred = curr;
            curr = curr->next_;
            void* key = pred->key_;
            bool status = HashSetFind(rhs, key);
            if (status)
                continue;
            status = HashSetAdd(result, key);
            if (!status) {
                HashSetDeinit(result);
                return NULL;
            }
        }
    }

    return result;
}


/*===========================================================================*
 *               Implementation for internal operations                      *
 *===========================================================================*/
HashSet* _HashSetInit(int idx_prime)
{
    HashSet* obj = (HashSet*)malloc(sizeof(HashSet));
    if (unlikely(!obj))
        return NULL;

    HashSetData* data = (HashSetData*)malloc(sizeof(HashSetData));
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
    data->func_hash_ = _HashSetHash;
    data->func_cmp_ = _HashSetCompare;
    data->func_clean_key_ = NULL;

    obj->data = data;
    obj->add = HashSetAdd;
    obj->find = HashSetFind;
    obj->remove = HashSetRemove;
    obj->size = HashSetSize;
    obj->first = HashSetFirst;
    obj->next = HashSetNext;
    obj->set_hash = HashSetSetHash;
    obj->set_compare = HashSetSetCompare;
    obj->set_clean_key = HashSetSetCleanKey;

    return obj;
}

unsigned _HashSetHash(void* key)
{
    return (unsigned)(uintptr_t)key;
}

int _HashSetCompare(void* lhs, void* rhs)
{
    if ((intptr_t)lhs == (intptr_t)rhs)
        return 0;
    return ((intptr_t)lhs > (intptr_t)rhs)? 1 : (-1);
}

void _HashSetReHash(HashSetData* data)
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

    HashSetHash func_hash = data->func_hash_;
    SlotNode** arr_slot = data->arr_slot_;
    unsigned num_slot = data->num_slot_;
    for (i = 0 ; i < num_slot ; ++i) {
        SlotNode* pred;
        SlotNode* curr = arr_slot[i];
        while (curr) {
            pred = curr;
            curr = curr->next_;

            /* Migrate each key value pair to the new slot. */
            unsigned hash = func_hash(pred->key_);
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
