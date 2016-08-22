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
 * @file tree_map.h The ordered map to store key value pairs.
 */

#ifndef _TREE_MAP_H_
#define _TREE_MAP_H_

#include "../util.h"

#ifdef __cplusplus
extern "C" {
#endif

/** TreeMapData is the data type for the container private information. */
typedef struct _TreeMapData TreeMapData;

/** Compare the equality of two keys. */
typedef int (*TreeMapCompare) (void*, void*);

/** Key cleanup function called whenever a live entry is removed. */
typedef void (*TreeMapCleanKey) (void*);

/** Value cleanup function called whenever a live entry is removed. */
typedef void (*TreeMapCleanValue) (void*);


/** The implementation for ordered map. */
typedef struct _TreeMap {
    /** The container private information */
    TreeMapData *data;

    /** Insert a key value pair into the map.
        @see TreeMapPut */
    bool (*put) (struct _TreeMap*, void*, void*);

    /** Retrieve the value corresponding to the designated key.
        @see TreeMapGet */
    void* (*get) (struct _TreeMap*, void*);

    /** Check if the map contains the designated key.
        @see TreeMapFind */
    bool (*find) (struct _TreeMap*, void*);

    /** Delete the key value pair corresponding to the designated key.
        @see TreeMapRemove */
    bool (*remove) (struct _TreeMap*, void*);

    /** Return the number of stored key value pairs.
        @see TreeMapSize */
    unsigned (*size) (struct _TreeMap*);

    /** Retrieve the key value pair with the minimum order from the map.
        @see TreeMapMinimum */
    Pair* (*minimum) (struct _TreeMap*);

    /** Retrieve the key value pair with the maximum order from the map.
        @see TreeMapMaximum */
    Pair* (*maximum) (struct _TreeMap*);

    /** Retrieve the key value pair which is the predecessor of the given key.
        @see TreeMapPredecessor */
    Pair* (*predecessor) (struct _TreeMap*, void*);

    /** Retrieve the key value pair which is the successor of the given key.
        @see TreeMapSuccessor */
    Pair* (*successor) (struct _TreeMap*, void*);

    /** Initialize the map iterator.
        @see TreeMapFirst */
    void (*first) (struct _TreeMap*);

    /** Get the key value pair pointed by the iterator and advance the iterator.
        @see TreeMapNext */
    Pair* (*next) (struct _TreeMap*);

    /** Get the key value pair pointed by the iterator and advance the iterator
        in the reverse order.
        @see TreeMapNext */
    Pair* (*reverse_next) (struct _TreeMap*);

    /** Set the custom key comparison function.
        @see TreeMapSetCompare */
    void (*set_compare) (struct _TreeMap*, TreeMapCompare);

    /** Set the custom key cleanup function.
        @see TreeMapSetCleanKey */
    void (*set_clean_key) (struct _TreeMap*, TreeMapCleanKey);

    /** Set the custom value cleanup function.
        @see TreeMapSetCleanValue */
    void (*set_clean_value) (struct _TreeMap*, TreeMapCleanValue);
} TreeMap;


/*===========================================================================*
 *             Definition for the exported member operations                 *
 *===========================================================================*/
/**
 * @brief The constructor for TreeMap.
 *
 * @retval obj          The successfully constructed map
 * @retval NULL         Insufficient memory for map construction
 */
TreeMap* TreeMapInit();

/**
 * @brief The destructor for TreeMap.
 *
 * @param obj           The pointer to the to be destructed map
 */
void TreeMapDeinit(TreeMap* obj);

/**
 * @brief Insert a key value pair into the map.
 *
 * This function inserts a key value pair into the map. If the designated key is
 * equal to a certain one stored in the map, the existing pair will be replaced.
 * Also, the cleanup functions are invoked for that replaced pair.
 *
 * @param self          The pointer to TreeMap structure
 * @param key           The designated key
 * @param value         The designated value
 *
 * @retval true         The pair is successfully inserted
 * @retval false        The pair cannot be inserted due to insufficient memory
 */
bool TreeMapPut(TreeMap* self, void* key, void* value);

/**
 * @brief Retrieve the value corresponding to the designated key.
 *
 * @param self          The pointer to TreeMap structure
 * @param key           The designated key
 *
 * @retval value        The corresponding value
 * @retval NULL         The key cannot be found
 */
void* TreeMapGet(TreeMap* self, void* key);

/**
 * @brief Check if the map contains the designated key.
 *
 * @param self          The pointer to TreeMap structure
 * @param key           The designated key
 *
 * @retval true         The key can be found
 * @retval false        The key cannot be found
 */
bool TreeMapFind(TreeMap* self, void* key);

/**
 * @brief Remove the key value pair corresponding to the designated key.
 *
 * This function removes the key value pair corresponding to the designated key.
 * Also, the cleanup functions are invoked for that removed pair.
 *
 * @param self          The pointer to TreeMap structure
 * @param key           The designated key
 *
 * @retval true         The pair is successfully removed
 * @retval false        The key cannot be found
 */
bool TreeMapRemove(TreeMap* self, void* key);

/**
 * @brief Return the number of stored key value pairs.
 *
 * @param self          The pointer to TreeMap structure
 *
 * @retval size         The number of stored pairs
 */
unsigned TreeMapSize(TreeMap* self);

/**
 * @brief Retrieve the key value pair with the minimum order from the map.
 *
 * @param self          The pointer to TreeMap structure
 *
 * @retval ptr_pair     The pointer to the target key value pair
 * @retval NULL         The map is empty
 */
Pair* TreeMapMinimum(TreeMap* self);

/**
 * @brief Retrieve the key value pair with the maximum order from the map.
 *
 * @param self          The pointer to TreeMap structure
 *
 * @retval ptr_pair     The pointer to the target key value pair
 * @retval NULL         The map is empty
 */
Pair* TreeMapMaximum(TreeMap* self);

/**
 * @brief Retrieve the key value pair which is the predecessor of the given key.
 *
 * @param self          The pointer to TreeMap structure
 * @param key           The designated key
 *
 * @retval ptr_pair     The pointer to the target key value pair
 * @retval NULL         The map is empty or the key has he minimum order
 */
Pair* TreeMapPredecessor(TreeMap* self, void* key);

/**
 * @brief Retrieve the key value pair which is the successor of the given key.
 *
 * @param self          The pointer to TreeMap structure
 * @param key           The designated key
 *
 * @retval ptr_pair     The pointer to the target key value pair
 * @retval NULL         The map is empty or the key has the maximum order
 */
Pair* TreeMapSuccessor(TreeMap* self, void* key);

/**
 * @brief Initialize the map iterator.
 *
 * @param self          The pointer to TreeMap structure
 */
void TreeMapFirst(TreeMap* self);

/**
 * @brief Get the key value pair pointed by the iterator and advance the iterator.
 *
 * @param self          The pointer to TreeMap structure
 *
 * @retval ptr_pair     The pointer to the current key value pair
 * @retval NULL         The map end is reached
 */
Pair* TreeMapNext(TreeMap* self);

/**
 * @brief Get the key value pair pointed by the iterator and advance the iterator
 * in the reverse order.
 *
 * @param self          The pointer to TreeMap structure
 *
 * @retval ptr_pair     The pointer to the current key value pair
 * @retval NULL         The map end is reached
 */
Pair* TreeMapReverseNext(TreeMap* self);

/**
 * @brief Set the custom key comparison function.
 *
 * By default, key is treated as integer.
 *
 * @param self          The pointer to TreeMap structure
 * @param func          The custom function
 */
void TreeMapSetCompare(TreeMap* self, TreeMapCompare func);

/**
 * @brief Set the custom key cleanup function.
 *
 * By default, no cleanup operation for key.
 *
 * @param self          The pointer to TreeMap structure
 * @param func          The custom function
 */
void TreeMapSetCleanKey(TreeMap* self, TreeMapCleanKey func);

/**
 * @brief Set the custom value cleanup function.
 *
 * By default, no cleanup operation for value.
 *
 * @param self          The pointer to TreeMap structure
 * @param func          The custom function
 */
void TreeMapSetCleanValue(TreeMap* self, TreeMapCleanValue func);

#ifdef __cplusplus
}
#endif

#endif