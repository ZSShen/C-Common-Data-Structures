/**
 * @file hash_map.h The unordered map to store key value pairs.
 */

#ifndef _HASH_MAP_H_
#define _HASH_MAP_H_

#include "../util.h"

#ifdef __cplusplus
extern "C" {
#endif

/** HashMapData is the data type for the container private information. */
typedef struct _HashMapData HashMapData;

/** Calculate the hash of the given key. */
typedef unsigned (*HashMapHash) (void*);

/** Compare the equality of two keys. */
typedef int (*HashMapCompare) (void*, void*);

/** Key cleanup function called whenever a live entry is removed. */
typedef void (*HashMapCleanKey) (void*);

/** Value cleanup function called whenever a live entry is removed. */
typedef void (*HashMapCleanValue) (void*);


/** The implementation for hash map. */
typedef struct _HashMap {
    /** The container private information */
    HashMapData *data;

    /** Insert a key value pair into the map.
        @see HashMapPut */
    bool (*put) (struct _HashMap*, void*, void*);

    /** Retrieve the value corresponding to the designated key.
        @see HashMapGet */
    void* (*get) (struct _HashMap*, void*);

    /** Check if the map contains the designated key.
        @see HashMapFind */
    bool (*find) (struct _HashMap*, void*);

    /** Remove the key value pair corresponding to the designated key.
        @see HashMapRemove */
    bool (*remove) (struct _HashMap*, void*);

    /** Return the number of stored key value pairs.
        @see HashMapSize */
    unsigned (*size) (struct _HashMap*);

    /** Initialize the map iterator.
        @see HashMapFirst */
    void (*first) (struct _HashMap*);

    /** Get the key value pair pointed by the iterator and advance the iterator
        @see HashMapNext */
    Pair* (*next) (struct _HashMap*);

    /** Set the custom hash function.
        @see HashMapSetHash */
    void (*set_hash) (struct _HashMap*, HashMapHash);

    /** Set the custom key comparison function.
        @see HashMapSetCompare */
    void (*set_compare) (struct _HashMap*, HashMapCompare);

    /** Set the custom key cleanup function.
        @see HashMapSetCleanKey */
    void (*set_clean_key) (struct _HashMap*, HashMapCleanKey);

    /** Set the custom value cleanup function.
        @see HashMapSetCleanValue */
    void (*set_clean_value) (struct _HashMap*, HashMapCleanValue);
} HashMap;


/*===========================================================================*
 *             Definition for the exported member operations                 *
 *===========================================================================*/
/**
 * @brief The constructor for HashMap.
 *
 * @retval obj          The successfully constructed map
 * @retval NULL         Insufficient memory for map construction
 */
HashMap* HashMapInit();

/**
 * @brief The destructor for HashMap.
 *
 * @param obj           The pointer to the to be destructed map
 */
void HashMapDeinit(HashMap* obj);

/**
 * @brief Insert a key value pair into the map.
 *
 * This function inserts a key value pair into the map. If the designated key is
 * equal to a certain one stored in the map, the existing pair will be replaced.
 * Also, the cleanup functions are invoked for that replaced pair.
 *
 * @param self          The pointer to HashMap structure
 * @param key           The designated key
 * @param value         The designated value
 *
 * @retval true         The pair is successfully inserted
 * @retval false        The pair cannot be inserted due to insufficient memory
 */
bool HashMapPut(HashMap* self, void* key, void* value);

/**
 * @brief Retrieve the value corresponding to the designated key.
 *
 * @param self          The pointer to HashMap structure
 * @param key           The designated key
 *
 * @retval value        The corresponding value
 * @retval NULL         The key cannot be found
 */
void* HashMapGet(HashMap* self, void* key);

/**
 * @brief Check if the map contains the designated key.
 *
 * @param self          The pointer to HashMap structure
 * @param key           The designated key
 *
 * @retval true         The key can be found
 * @retval false        The key cannot be found
 */
bool HashMapFind(HashMap* self, void* key);

/**
 * @brief Remove the key value pair corresponding to the designated key.
 *
 * This function removes the key value pair corresponding to the designated key.
 * Also, the cleanup functions are invoked for that removed pair.
 *
 * @param self          The pointer to HashMap structure
 * @param key           The designated key
 *
 * @retval true         The pair is successfully removed
 * @retval false        The key cannot be found
 */
bool HashMapRemove(HashMap* self, void* key);

/**
 * @brief Return the number of stored key value pairs.
 *
 * @param self          The pointer to HashMap structure
 *
 * @retval size         The number of stored pairs
 */
unsigned HashMapSize(HashMap* self);

/**
 * @brief Initialize the map iterator.
 *
 * @param self          The pointer to HashMap structure
 */
void HashMapFirst(HashMap* self);

/**
 * @brief Get the key value pair pointed by the iterator and advance the iterator.
 *
 * @param self          The pointer to HashMap structure
 *
 * @retval ptr_pair     The pointer to the current key value pair
 * @retval NULL         The map end is reached
 */
Pair* HashMapNext(HashMap* self);

/**
 * @brief Set the custom hash function.
 *
 * By default, the hash function is HashMurMur32.
 *
 * @param self          The pointer to HashMap structure
 * @param func          The custom function
 */
void HashMapSetHash(HashMap* self, HashMapHash func);

/**
 * @brief Set the custom key comparison function.
 *
 * By default, key is treated as integer.
 *
 * @param self          The pointer to HashMap structure
 * @param func          The custom function
 */
void HashMapSetCompare(HashMap* self, HashMapCompare func);

/**
 * @brief Set the custom key cleanup function.
 *
 * By default, no cleanup operation for key.
 *
 * @param self          The pointer to HashMap structure
 * @param func          The custom function
 */
void HashMapSetCleanKey(HashMap* self, HashMapCleanKey func);

/**
 * @brief Set the custom value cleanup function.
 *
 * By default, no cleanup operation for value.
 *
 * @param self          The pointer to HashMap structure
 * @param func          The custom function
 */
void HashMapSetCleanValue(HashMap* self, HashMapCleanValue func);

#ifdef __cplusplus
}
#endif

#endif
