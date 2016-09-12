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

#include "container/trie.h"


/*===========================================================================*
 *                        The container private data                         *
 *===========================================================================*/
static const char DIRECT_LEFT = 0;
static const char DIRECT_MIDDLE = 1;
static const char DIRECT_RIGHT = 2;

static const char STOP = 0;
static const char DOWN_LEFT = 1;
static const char DOWN_RIGHT = 2;
static const char DOWN_MIDDLE = 3;
static const char UP_LEFT = 4;
static const char UP_RIGHT = 5;
static const char UP_MIDDLE = 6;


typedef struct TrieNode_ {
    bool endstr_;
    char token_;
    struct TrieNode_* left_;
    struct TrieNode_* middle_;
    struct TrieNode_* right_;
    struct TrieNode_* parent_;
} TrieNode;

struct TrieData_ {
    unsigned size_;
    unsigned count_node_;
    unsigned depth_;
    TrieNode* root_;
};


/*===========================================================================*
 *                  Definition for internal operations                       *
 *===========================================================================*/
#define likely(x)       __builtin_expect(!!(x), 1)
#define unlikely(x)     __builtin_expect(!!(x), 0)

/**
 * @brief Traverse all the trie nodes and clean the allocated resource.
 *
 * @param data          The pointer to the trie private data
 */
void _TrieDeinit(TrieData* data);

static inline
char DECIDE_BACKWARD_DIRECTION(TrieNode** p_curr)
{
    TrieNode* curr = *p_curr;
    TrieNode* temp = curr;
    curr = curr->parent_;
    *p_curr = curr;
    if (!curr)
        return STOP;
    return (temp == curr->left_)? UP_LEFT :
           ((temp == curr->middle_)? UP_MIDDLE : UP_RIGHT);
}

static inline
void FREE_LOCAL_RESOURCE(const char** strs, unsigned size, char* record)
{
    unsigned i;
    for (i = 0 ; i < size ; ++i)
        free((char*)strs[i]);
    free(strs);
    free(record);
}


/*===========================================================================*
 *               Implementation for the exported operations                  *
 *===========================================================================*/
Trie* TrieInit()
{
    Trie* obj = (Trie*)malloc(sizeof(Trie));
    if (unlikely(!obj))
        return NULL;

    TrieData* data = (TrieData*)malloc(sizeof(TrieData));
    if (unlikely(!data)) {
        free(obj);
        return NULL;
    }

    data->size_ = 0;
    data->count_node_ = 0;
    data->depth_ = 0;
    data->root_ = NULL;

    obj->data = data;
    obj->insert = TrieInsert;
    obj->bulk_insert = TrieBulkInsert;
    obj->has_exact = TrieHasExact;
    obj->has_prefix_as = TrieHasPrefixAs;
    obj->get_prefix_as = TrieGetPrefixAs;
    obj->remove = TrieRemove;
    obj->size = TrieSize;
    return obj;
}

void TrieDeinit(Trie* obj)
{
    if (unlikely(!obj))
        return;

    TrieData* data = obj->data;
    _TrieDeinit(data);

    free(data);
    free(obj);
    return;
}

bool TrieInsert(Trie* self, const char* str)
{
    if (unlikely(!str))
        return true;
    if (unlikely(*str == 0))
        return true;

    TrieData* data = self->data;
    TrieNode* curr = data->root_;
    TrieNode* pred = NULL;
    unsigned depth = 0;

    /* Longest prefix matching. */
    char ch, direct;
    while (curr && ((ch = *str) != 0)) {
        pred = curr;
        char token = curr->token_;
        if (ch == token) {
            curr = curr->middle_;
            direct = DIRECT_MIDDLE;
            ++str;
            ++depth;
        } else {
            if (ch < token) {
                curr = curr->left_;
                direct = DIRECT_LEFT;
            } else {
                curr = curr->right_;
                direct = DIRECT_RIGHT;
            }
        }
    }

    /* Cascade the trie node for the remaining suffix. */
    while ((ch = *str) != 0) {
        TrieNode* new_node = (TrieNode*)malloc(sizeof(TrieNode));
        if (unlikely(!new_node))
            return false;
        new_node->middle_ = new_node->left_ = new_node->right_ = NULL;
        new_node->parent_ = pred;
        new_node->token_ = ch;
        new_node->endstr_ = false;

        if (unlikely(!pred))
            data->root_ = new_node;
        else {
            if (likely(direct == DIRECT_MIDDLE))
                pred->middle_ = new_node;
            else {
                if (direct == DIRECT_LEFT)
                    pred->left_ = new_node;
                else
                    pred->right_ = new_node;
            }
        }

        pred = new_node;
        direct = DIRECT_MIDDLE;
        data->count_node_++;
        ++str;
        ++depth;
    }

    if (!(pred->endstr_)) {
        pred->endstr_ = true;
        data->size_++;
    }
    if (depth > data->depth_)
        data->depth_ = depth;

    return true;
}

bool TrieBulkInsert(Trie* self, const char** strs, unsigned size)
{
    TrieData* data = self->data;

    unsigned i;
    for (i = 0 ; i < size ; ++i) {
        const char* str = strs[i];
        if (!str)
            continue;
        if (*str == 0)
            continue;

        TrieNode* curr = data->root_;
        TrieNode* pred = NULL;
        unsigned depth = 0;

        /* Longest prefix matching. */
        char ch, direct;
        while (curr && ((ch = *str) != 0)) {
            pred = curr;
            char token = curr->token_;
            if (ch == token) {
                curr = curr->middle_;
                direct = DIRECT_MIDDLE;
                ++str;
                ++depth;
            } else {
                if (ch < token) {
                    curr = curr->left_;
                    direct = DIRECT_LEFT;
                } else {
                    curr = curr->right_;
                    direct = DIRECT_RIGHT;
                }
            }
        }

        /* Cascade the trie node for the remaining suffix. */
        while ((ch = *str) != 0) {
            TrieNode* new_node = (TrieNode*)malloc(sizeof(TrieNode));
            if (unlikely(!new_node))
                return false;
            new_node->middle_ = new_node->left_ = new_node->right_ = NULL;
            new_node->parent_ = pred;
            new_node->token_ = ch;
            new_node->endstr_ = false;

            if (unlikely(!pred))
                data->root_ = new_node;
            else {
                if (likely(direct == DIRECT_MIDDLE))
                    pred->middle_ = new_node;
                else {
                    if (direct == DIRECT_LEFT)
                        pred->left_ = new_node;
                    else
                        pred->right_ = new_node;
                }
            }

            pred = new_node;
            direct = DIRECT_MIDDLE;
            data->count_node_++;
            ++str;
            ++depth;
        }

        if (!(pred->endstr_)) {
            pred->endstr_ = true;
            data->size_++;
        }
        if (depth > data->depth_)
            data->depth_ = depth;
    }

    return true;
}

bool TrieHasExact(Trie* self, const char* str)
{
    if (unlikely(!str))
        return false;
    if (unlikely(*str == 0))
        return false;

    TrieData* data = self->data;
    TrieNode* curr = data->root_;
    TrieNode* pred = NULL;

    /* Longest prefix matching. */
    char ch;
    while (curr && ((ch = *str) != 0)) {
        pred = curr;
        char token = curr->token_;
        if (ch == token) {
            curr = curr->middle_;
            ++str;
        } else {
            if (ch < token)
                curr = curr->left_;
            else
                curr = curr->right_;
        }
    }

    /* At the tail of a certain string. */
    return (pred && pred->endstr_ && *str == 0)? true : false;
}

bool TrieHasPrefixAs(Trie* self, const char* prefix)
{
    if (unlikely(!prefix))
        return false;
    if (unlikely(*prefix == 0))
        return false;

    TrieData* data = self->data;
    TrieNode* curr = data->root_;
    TrieNode* pred = NULL;

    /* Longest prefix matching. */
    char ch;
    while (curr && ((ch = *prefix) != 0)) {
        pred = curr;
        char token = curr->token_;
        if (ch == token) {
            curr = curr->middle_;
            ++prefix;
        } else {
            if (ch < token)
                curr = curr->left_;
            else
                curr = curr->right_;
        }
    }

    /* No such prefix and early return. */
    if (*prefix != 0)
        return false;

    /* Short cut if this prefix is a specific string stored in the trie. */
    if (!curr)
        return (pred && pred->endstr_)? true : false;
    if (curr && pred && pred->endstr_)
        return true;

    /* The slow trie traversal to find any node marked as string end. */
    char direct = DOWN_LEFT;
    while (direct != STOP) {
        if (direct == DOWN_LEFT || direct == DOWN_MIDDLE || direct == DOWN_RIGHT) {
            if (curr->endstr_)
                return true;

            if (curr->left_) {
                curr = curr->left_;
                direct = DOWN_LEFT;
                continue;
            }
            if (curr->middle_) {
                curr = curr->middle_;
                direct = DOWN_MIDDLE;
                continue;
            }
            if (curr->right_) {
                curr = curr->right_;
                direct = DOWN_RIGHT;
                continue;
            }

            direct = DECIDE_BACKWARD_DIRECTION(&curr);
            continue;
        }

        if (direct == UP_LEFT) {
            if (curr->middle_) {
                curr = curr->middle_;
                direct = DOWN_MIDDLE;
                continue;
            }
            if (curr->right_) {
                curr = curr->right_;
                direct = DOWN_RIGHT;
                continue;
            }

            direct = DECIDE_BACKWARD_DIRECTION(&curr);
            continue;
        }

        if (direct == UP_MIDDLE) {
            if (curr->right_) {
                curr = curr->right_;
                direct = DOWN_RIGHT;
                continue;
            }

            direct = DECIDE_BACKWARD_DIRECTION(&curr);
            continue;
        }

        direct = DECIDE_BACKWARD_DIRECTION(&curr);
    }

    return false;
}

bool TrieGetPrefixAs(Trie* self, const char* prefix, const char*** p_strs, unsigned *p_size)
{
    *p_strs = NULL;
    *p_size = 0;

    if (unlikely(!prefix))
        return false;
    if (unlikely(*prefix == 0))
        return false;

    TrieData* data = self->data;
    TrieNode* curr = data->root_;
    TrieNode* pred = NULL;

    /* Longest prefix matching. */
    const char* dup = prefix;
    char ch;
    while (curr && ((ch = *dup) != 0)) {
        pred = curr;
        char token = curr->token_;
        if (ch == token) {
            curr = curr->middle_;
            ++dup;
        } else {
            if (ch < token)
                curr = curr->left_;
            else
                curr = curr->right_;
        }
    }

    /* No such prefix and early return. */
    if (*dup != 0)
        return false;
    if (!curr && !(pred->endstr_))
        return false;

    /* Prepare the prefix record. */
    unsigned depth = data->depth_;
    unsigned length = strlen(prefix);
    unsigned sum = depth + length + 1;
    char* record = (char*)malloc(sizeof(char) * sum);
    if (unlikely(!record))
        return false;
    memset(record, 0, sizeof(char) * sum);
    strncpy(record, prefix, length);

    /* Prepare the to be returned array of strings. */
    unsigned capacity = depth;
    unsigned size = 0;
    const char** strs = (const char**)malloc(sizeof(const char*) * capacity);
    if (unlikely(!strs)) {
        free(record);
        return false;
    }

    /* The given prefix is exactly a stored string. */
    if (pred->endstr_) {
        dup = strdup(record);
        strs[size++] = dup;
    }

    /* The full trie traversal to find any node marked as string end. */
    char direct = DOWN_LEFT;
    while (curr && (curr != pred)) {
        if (direct == DOWN_LEFT || direct == DOWN_MIDDLE || direct == DOWN_RIGHT) {
            if (curr->left_) {
                curr = curr->left_;
                record[length] = curr->token_;
                direct = DOWN_LEFT;
                continue;
            }

            record[length] = curr->token_;
            if (curr->endstr_) {
                if (size == capacity) {
                    capacity <<= 1;
                    const char** new_strs = (const char**)realloc(strs,
                                                sizeof(const char*) * capacity);
                    if (unlikely(!new_strs))
                        FREE_LOCAL_RESOURCE(strs, size, record);
                    strs = new_strs;
                }
                strs[size++] = strdup(record);
            }

            if (curr->middle_) {
                curr = curr->middle_;
                record[++length] = curr->token_;
                direct = DOWN_MIDDLE;
                continue;
            }
            if (curr->right_) {
                curr = curr->right_;
                record[length] = curr->token_;
                direct = DOWN_RIGHT;
                continue;
            }

            direct = DECIDE_BACKWARD_DIRECTION(&curr);
            if (direct == UP_MIDDLE)
                record[length--] = 0;
            continue;
        }

        if (direct == UP_LEFT) {
            record[length] = curr->token_;
            if (curr->endstr_) {
                if (size == capacity) {
                    capacity <<= 1;
                    const char** new_strs = (const char**)realloc(strs,
                                                sizeof(const char*) * capacity);
                    if (unlikely(!new_strs))
                        FREE_LOCAL_RESOURCE(strs, size, record);
                    strs = new_strs;
                }
                strs[size++] = strdup(record);
            }

            if (curr->middle_) {
                curr = curr->middle_;
                record[++length] = curr->token_;
                direct = DOWN_MIDDLE;
                continue;
            }
            if (curr->right_) {
                curr = curr->right_;
                record[length] = curr->token_;
                direct = DOWN_RIGHT;
                continue;
            }

            direct = DECIDE_BACKWARD_DIRECTION(&curr);
            if (direct == UP_MIDDLE)
                record[length--] = 0;
            continue;
        }

        if (direct == UP_MIDDLE) {
            if (curr->right_) {
                curr = curr->right_;
                record[length] = curr->token_;
                direct = DOWN_RIGHT;
                continue;
            }

            direct = DECIDE_BACKWARD_DIRECTION(&curr);
            if (direct == UP_MIDDLE)
                record[length--] = 0;
            continue;
        }

        direct = DECIDE_BACKWARD_DIRECTION(&curr);
        if (direct == UP_MIDDLE)
            record[length--] = 0;
        continue;
    }

    bool ever_found;
    if (size > 0) {
        const char** new_strs = (const char**)realloc(strs, sizeof(const char**) * size);
        if (unlikely(!new_strs)) {
            FREE_LOCAL_RESOURCE(strs, size, record);
            ever_found = false;
        }
        strs = new_strs;
        ever_found = true;
    } else {
        FREE_LOCAL_RESOURCE(strs, size, record);
        ever_found = false;
    }

    if (ever_found) {
        *p_strs = strs;
        *p_size = size;
        free(record);
    }
    return ever_found;
}

bool TrieRemove(Trie* self, const char* str)
{
    if (unlikely(!str))
        return false;
    if (unlikely(*str == 0))
        return false;

    TrieData* data = self->data;
    TrieNode* curr = data->root_;
    TrieNode* pred = NULL;

    /* Longest prefix matching. */
    char ch;
    while (curr && ((ch = *str) != 0)) {
        pred = curr;
        char token = curr->token_;
        if (ch == token) {
            curr = curr->middle_;
            ++str;
        } else {
            if (ch < token)
                curr = curr->left_;
            else
                curr = curr->right_;
        }
    }

    /* At the end of a specific string. */
    if (pred && pred->endstr_ && *str == 0) {
        pred->endstr_ = false;
        data->size_--;
        return true;
    }
    return false;
}

unsigned TrieSize(Trie* self)
{
    return self->data->size_;
}


/*===========================================================================*
 *               Implementation for internal operations                      *
 *===========================================================================*/
void _TrieDeinit(TrieData* data)
{
    TrieNode* curr = data->root_;
    if (!curr)
        return;

    char direct = DOWN_LEFT;
    while (direct != STOP) {
        if (direct == DOWN_LEFT || direct == DOWN_MIDDLE || direct == DOWN_RIGHT) {
            if (curr->left_) {
                curr = curr->left_;
                direct = DOWN_LEFT;
                continue;
            }
            if (curr->middle_) {
                curr = curr->middle_;
                direct = DOWN_MIDDLE;
                continue;
            }
            if (curr->right_) {
                curr = curr->right_;
                direct = DOWN_RIGHT;
                continue;
            }

            TrieNode* temp = curr;
            direct = DECIDE_BACKWARD_DIRECTION(&curr);
            free(temp);
            continue;
        }

        if (direct == UP_LEFT) {
            if (curr->middle_) {
                curr = curr->middle_;
                direct = DOWN_MIDDLE;
                continue;
            }
            if (curr->right_) {
                curr = curr->right_;
                direct = DOWN_RIGHT;
                continue;
            }

            TrieNode* temp = curr;
            direct = DECIDE_BACKWARD_DIRECTION(&curr);
            free(temp);
            continue;
        }

        if (direct == UP_MIDDLE) {
            if (curr->right_) {
                curr = curr->right_;
                direct = DOWN_RIGHT;
                continue;
            }

            TrieNode* temp = curr;
            direct = DECIDE_BACKWARD_DIRECTION(&curr);
            free(temp);
            continue;
        }

        TrieNode* temp = curr;
        direct = DECIDE_BACKWARD_DIRECTION(&curr);
        free(temp);
    }

    return;
}