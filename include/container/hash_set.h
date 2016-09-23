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
 * @file hash_set.h The unordered set to store unique keys.
 */

#ifndef _HASH_SET_H_
#define _HASH_SET_H_

#include "../util.h"

#ifdef __cplusplus
extern "C" {
#endif

/** HashSetData is the data type for the container private information. */
typedef struct _HashSetData HashSetData;

/** Calculate the hash of the given key. */
typedef unsigned (*HashSetHash) (void*);

/** Compare the equality of two keys. */
typedef int (*HashSetCompare) (void*, void*);

/** void* cleanup function called whenever a live entry is removed. */
typedef void (*HashSetCleanKey) (void*);


/** The implementation for hash set. */
typedef struct _HashSet {
    /** The container private information */
    HashSetData* data;

    /** Insert a key into the set.
        @see HashSetAdd */
    bool (*add) (struct _HashSet*, void*);

    /** Check if the set contains the specified key.
        @see HashSetFind */
    bool (*find) (struct _HashSet*, void*);

    /** Remove the specified key from the set.
        @see HashSetRemove */
    bool (*remove) (struct _HashSet*, void*);

    /** Return the number of stored unique keys.
        @see HashSetSize */
    unsigned (*size) (struct _HashSet*);

    /** Initialize the set iterator.
        @see HashSetFirst */
    void (*first) (struct _HashSet*);

    /** Get the key pointed by the iterator and advance the iterator.
        @see HashSetNext */
    void* (*next) (struct _HashSet*);

    /** Set the custom hash function.
        @see HashSetSetHash */
    void (*set_hash) (struct _HashSet*, HashSetHash);

    /** Set the custom key comparison function.
        @see HashSetSetCompare */
    void (*set_compare) (struct _HashSet*, HashSetCompare);

    /** Set the custom key cleanup function.
        @see HashSetSetCleanKey */
    void (*set_clean_key) (struct _HashSet*, HashSetCleanKey);
} HashSet;


/*===========================================================================*
 *             Definition for the exported member operations                 *
 *===========================================================================*/
/**
 * @brief The constructor for HashSet.
 *
 * @retval obj          The successfully constructed set
 * @retval NULL         Insufficient memory for set construction
 */
HashSet* HashSetInit();

/**
 * @brief The destructor for HashSet.
 *
 * @param obj           The pointer to the to be destructed set
 */
void HashSetDeinit(HashSet* obj);

/**
 * @brief Insert a key into the set.
 *
 * @param self          The pointer to HashSet structure
 * @param key           The specified key
 *
 * @retval true         The key is successfully inserted
 * @retval false        The key cannot be inserted due to insufficient memory
 */
bool HashSetAdd(HashSet* self, void* key);

/**
 * @brief Check if the set contains the specified key.
 *
 * @param self          The pointer to HashSet structure
 * @param key           The specified key
 *
 * @retval true         The key can be found
 * @retval false        The key cannot be found
 */
bool HashSetFind(HashSet* self, void* key);

/**
 * @brief Remove the specified key from the set.
 *
 * @param self          The pointer to HashSet structure
 * @param key           The specified key
 *
 * @retval true         The key is successfully removed
 * @retval false        The key cannot be found
 */
bool HashSetRemove(HashSet* self, void* key);

/**
 * @brief Return the number of stored keys.
 *
 * @param self          The pointer to HashSet structure
 *
 * @retval size         The number of stored keys
 */
unsigned HashSetSize(HashSet* self);

/**
 * @brief Initialize the set iterator.
 *
 * @param self          The pointer to HashSet structure
 */
void HashSetFirst(HashSet* self);

/**
 * @brief Get the current key pointed by the iterator and advance the iterator.
 *
 * @param self          The pointer to HashSet structure
 *
 * @retval key          The current key
 * @retval NULL         The set end is reached
 */
void* HashSetNext(HashSet* self);

/**
 * @brief Set the custom hash function.
 *
 * @param self          The pointer to HashSet structure
 * @param func          The custom function
 */
void HashSetSetHash(HashSet* self, HashSetHash func);

/**
 * @brief Set the custom key comparison function.
 *
 * By default, key is treated as signed integer.
 *
 * @param self          The pointer to HashSet structure
 * @param func          The custom function
 */
void HashSetSetCompare(HashSet* self, HashSetCompare func);

/**
 * @brief Set the custom key cleanup function.
 *
 * By default, no cleanup operation for key.
 *
 * @param self          The pointer to HashSet structure
 * @param func          The custom function
 */
void HashSetSetCleanKey(HashSet* self, HashSetCleanKey func);

/**
 * @brief Perform union operation for the specified two sets.
 *
 * @param lhs           The first source set
 * @param rhs           The second source set
 *
 * @retval result       The result set of union operation
 * @retval NULL         Insufficient memory for result set
 *
 * @note The result set will not delegate any key clean functions from two source
 *  sets. You can still put the clean function for it. However, to avoid the
 *  "double-free" problem, it is better to let the source sets handle the key
 *  clean issue and treat the result set as the pure key collection.
 */
HashSet* HashSetUnion(HashSet* lhs, HashSet* rhs);

/**
 * @brief Perform intersection operation for the specified two sets.
 *
 * @param lhs           The first source set
 * @param rhs           The second source set
 *
 * @retval result       The result set of intersection operation
 * @retval NULL         Insufficient memory for result set
 *
 * @note The result set will not delegate any key clean functions from two source
 *  sets. You can still put the clean function for it. However, to avoid the
 *  "double-free" problem, it is better to let the source sets handle the key
 *  clean issue and treat the result set as the pure key collection.
 */
HashSet* HashSetIntersect(HashSet* lhs, HashSet* rhs);

/**
 * @brief Perform difference operation for the specified two sets.
 *
 * @param lhs           The first source set
 * @param rhs           The second source set
 *
 * @retval result       The result set of difference operation
 * @retval NULL         Insufficient memory for result set
 *
 * @note The result set will not delegate any key clean functions from two source
 *  sets. You can still put the clean function for it. However, to avoid the
 *  "double-free" problem, it is better to let the source sets handle the key
 *  clean issue and treat the result set as the pure key collection.
 */
HashSet* HashSetDifference(HashSet* lhs, HashSet* rhs);

#ifdef __cplusplus
}
#endif

#endif
