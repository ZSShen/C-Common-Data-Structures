/**
 * @file unordered_map.h The unordered map implemented with hash table.
 */

#ifndef _HASH_MAP_H_
#define _HASH_MAP_H_

#include "../util.h"

/** HashMapData is the data type for the container private information. */
typedef struct _HashMapData HashMapData;

/** The implementation for hash map. */
typedef struct _HashMap {
    /** The container private information */
    HashMapData *pData;

    /** Insert a key value pair into the map.
        @see HashMapPut */
    int32_t (*put) (struct _HashMap*, Entry, size_t);

    /** Retrieve the value corresponding to the designated key.
        @see HashMapGet */
    int32_t (*get) (struct _HashMap*, Key, size_t, Value*);

    /** Delete the key value pair corresponding to the designated key.
        @see HashMapRemove */
    int32_t (*remove) (struct _HashMap*, Key, size_t);

    /** Return the number of stored key value pairs.
        @see HashMapSize */
    int32_t (*size) (struct _HashMap*);

    /** Set the custom key value pair resource clean method.
        @see HashMapSetDestroy */
    int32_t (*set_destroy) (struct _HashMap*, void (*) (Entry));

    /** Set the custom hash function.
        @see HashMapSetHash */
    int32_t (*set_hash) (struct _HashMap*, uint32_t (*) (Key, size_t));
} HashMap;


/*===========================================================================*
 *             Definition for the exported member operations                 *
 *===========================================================================*/
/**
 * @brief The constructor for HashMap.
 *
 * @param ppObj         The double pointer to the to be constructed map
 * @param uiCount       The count of buckets
 *
 * @retval SUCC
 * @retval ERR_NOMEM    Insufficient memory for map construction
 *
 * @note Specify uiCount to 0 for default bucket count.
 */
extern int32_t HashMapInit(HashMap **ppObj, uint32_t uiCount);

/**
 * @brief The destructor for HashMap.
 *
 * If the the custom clean method is set, it also runs the resource clean method
 * for all the items.
 *
 * @param ppObj         The double pointer to the to be destructed map
 */
extern void HashMapDeinit(HashMap **ppObj);

/**
 * @brief Insert a key value pair into the map.
 *
 * This function inserts a key value pair into the map. If the hash key of the
 * designated pair is the same with a certain one stored in the map, that pair
 * will be replaced. Also, if the custom clean method is set, it runs the resource
 * clean method for the replaced pair.
 *
 * @param self          The pointer to HashMap structure
 * @param ent           The pointer to the desiganted key value pair
 * @param sizeKey       The key size in bytes
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_NOMEM    Insufficient memory for map extension
 * @retval ERR_KEYSIZE  Invalid key size
 */
extern int32_t HashMapPut(HashMap *self, Entry ent, size_t sizeKey);

/**
 * @brief Retrieve the value corresponding to the designated key.
 *
 * This function retrieves the value corresponding to the designated key from
 * the map. If the key can be found, the value will be returned by the fourth
 * parameter. Otherwise, the error code is returned and the fourth parameter is
 * updated with NULL.
 *
 * @param self          The pointer to Map structure
 * @param key           The designated key
 * @param sizeKey       The key size in bytes
 * @param pValue        The pointer to the returned value
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_NODATA   No map entry can be found
 * @retval ERR_KEYSIZE  Invalid key size
 */
extern int32_t HashMapGet(HashMap *self, Key key, size_t sizeKey, Value *pValue);

/**
 * @brief Delete the key value pair corresponding to the designated key.
 *
 * This function deletes the key value pair corresponding to the designated key.
 * If the custom clean method is set, it also runs the resource clean method for
 * the deleted pair.
 *
 * @param self          The pointer to UnorderedMap structure
 * @param key           The designated key
 * @param sizeKey       The key size in bytes
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_NODATA   No map entry can be found
 * @retval ERR_KEYSIZE  Invalid key size
 */
extern int32_t HashMapRemove(HashMap *self, Key key, size_t sizeKey);

/**
 * @brief Return the number of stored key value pairs.
 *
 * @param self          The pointer to HashMap structure
 *
 * @return              The number of stored pairs
 * @retval ERR_NOINIT   Uninitialized container
 */
extern int32_t HashMapSize(HashMap *self);

/**
 * @brief Set the custom key value pair resource clean method.
 *
 * @param self          The pointer to HashMap structure
 * @param pFunc         The function pointer to the custom method
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 */
extern int32_t HashMapSetDestroy(HashMap *self, void (*pFunc) (Entry));

/**
 * @brief Set the custom hash function.
 *
 * @param self          The pointer to HashMap structure
 * @param pFunc         The function pointer to the custom method
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 */
extern int32_t HashMapSetHash(HashMap *self, uint32_t (*pFunc) (Key, size_t));

#endif
