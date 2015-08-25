/**
 * @file hash_map.h The unordered map to store key value pairs.
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
    int32_t (*put) (struct _HashMap*, Pair*, size_t);

    /** Retrieve the value corresponding to the designated key.
        @see HashMapGet */
    int32_t (*get) (struct _HashMap*, Key, size_t, Value*);

    /** Check if the map contains the designated key.
        @see TreeMapFind */
    int32_t (*find) (struct _HashMap*, Key, size_t);

    /** Delete the key value pair corresponding to the designated key.
        @see HashMapRemove */
    int32_t (*remove) (struct _HashMap*, Key, size_t);

    /** Return the number of stored key value pairs.
        @see HashMapSize */
    int32_t (*size) (struct _HashMap*);

    /** Iterate through the map to retrieve each key value pair.
        @see HashMapIterate */
    int32_t (*iterate) (struct _HashMap*, bool, Pair**);

    /** Set the custom key value pair resource clean method.
        @see HashMapSetDestroy */
    int32_t (*set_destroy) (struct _HashMap*, void (*) (Pair*));

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
 *
 * @retval SUCC
 * @retval ERR_NOMEM    Insufficient memory for map construction
 */
int32_t HashMapInit(HashMap **ppObj);

/**
 * @brief The destructor for HashMap.
 *
 * If the custom resource clean method is set, it also runs the clean method
 * for each pair.
 *
 * @param ppObj         The double pointer to the to be destructed map
 */
void HashMapDeinit(HashMap **ppObj);

/**
 * @brief Insert a key value pair into the map.
 *
 * This function inserts a key value pair into the map. If the hash key of the
 * designated pair is the same with a certain one stored in the map, that pair
 * will be replaced. Also, if the custom resource clean method is set, it runs
 * the resource clean method for the replaced pair.
 *
 * @param self          The pointer to HashMap structure
 * @param pPair         The pointer to the designated pair
 * @param size          Key size in bytes
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_NOMEM    Insufficient memory for map extension
 * @retval ERR_KEYSIZE  Invalid key size
 */
int32_t HashMapPut(HashMap *self, Pair *pPair, size_t size);

/**
 * @brief Retrieve the value corresponding to the designated key.
 *
 * This function retrieves the value corresponding to the designated key from
 * the map. If the key can be found, the value will be returned by the fourth
 * parameter. Otherwise, the error code is returned and the fourth parameter is
 * updated with NULL.
 *
 * @param self          The pointer to HashMap structure
 * @param key           The designated key
 * @param size          Key size in bytes
 * @param pValue        The pointer to the returned value
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_NODATA   No map entry can be found
 * @retval ERR_GET      Invalid parameter to store returned value
 * @retval ERR_KEYSIZE  Invalid key size
 *
 * @note The key should be the pointer to the data you plan to hash for.
 */
int32_t HashMapGet(HashMap *self, Key key, size_t size, Value *pValue);

/**
 * @brief Check if the map contains the designated key.
 *
 * @param self          The pointer to HashMap structure
 * @param key           The designated key
 * @param size          Key size in bytes
 *
 * @retval SUCC         The key can be found
 * @retval NOKEY        The key cannot be found
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_KEYSIZE  Invalid key size
 *
 * @note The key should be the pointer to the data you plan to hash for.
 */
int32_t HashMapFind(HashMap *self, Key key, size_t size);

/**
 * @brief Delete the key value pair corresponding to the designated key.
 *
 * This function deletes the key value pair corresponding to the designated key.
 * If the custom resource clean method is set, it also runs the clean method for
 * the deleted pair.
 *
 * @param self          The pointer to HashMap structure
 * @param key           The designated key
 * @param size          Key size in bytes
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_NODATA   No map entry can be found
 * @retval ERR_KEYSIZE  Invalid key size
 *
 * @note The key should be the pointer to the data you plan to hash for.
 */
int32_t HashMapRemove(HashMap *self, Key key, size_t size);

/**
 * @brief Return the number of stored key value pairs.
 *
 * @param self          The pointer to HashMap structure
 *
 * @return              The number of stored pairs
 * @retval ERR_NOINIT   Uninitialized container
 */
int32_t HashMapSize(HashMap *self);

/**
 * @brief Iterate through the map to retrieve each key value pair.
 *
 * Before iterating through the map, it is necessary to pass:
 *  - bReset = true
 *  - pPair = NULL
 * for iterator initialization.
 *
 * After initialization, you can pass:
 *  - bReset = false
 *  - pPair = the pointer to get the returned pair at each iteration.
 *
 * @param self          The pointer to HashMap structure
 * @param bReset        The knob to restart the iteration
 * @param ppPair        The double pointer to the returned pair
 *
 * @retval SUCC         Iterator initialized successfully
 * @retval CONTINUE     Iteration in progress
 * @retval END          Iteration terminiated
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_GET      Invalid parameter to store returned pair
 *
 * @note Please do not insert or delete pairs during map traversal
 */
int32_t HashMapIterate(HashMap *self, bool bReset, Pair **ppPair);

/**
 * @brief Set the custom key value pair resource clean method.
 *
 * @param self          The pointer to HashMap structure
 * @param pFunc         The function pointer to the custom method
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 */
int32_t HashMapSetDestroy(HashMap *self, void (*pFunc) (Pair*));

/**
 * @brief Set the custom hash function.
 *
 * The default hash function is HashMurMur32.
 *
 * @param self          The pointer to HashMap structure
 * @param pFunc         The function pointer to the custom method
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 */
int32_t HashMapSetHash(HashMap *self, uint32_t (*pFunc) (Key, size_t));

#endif
