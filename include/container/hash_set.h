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

/** The implementation for hash set. */
typedef struct _HashSet {
    /** The container private information */
    HashSetData *pData;

    /** Insert a key into the set.
        @see HashSetAdd */
    int32_t (*add) (struct _HashSet*, Key, size_t);

    /** Check if the set contains the designated key.
        @see HashSetFind */
    int32_t (*find) (struct _HashSet*, Key, size_t);

    /** Delete the designated key from the set.
        @see HashSetRemove */
    int32_t (*remove) (struct _HashSet*, Key, size_t);

    /** Return the number of stored unique keys.
        @see HashSetSize */
    int32_t (*size) (struct _HashSet*);

    /** Iterate through the set to retrieve each key.
        @see HashSetIterate */
    int32_t (*iterate) (struct _HashSet*, bool, Key*);

    /** Set the custom key resource clean method.
        @see HashSetSetDestroy */
    int32_t (*set_destroy) (struct _HashSet*, void (*) (Key));

    /** Set the custom hash function.
        @see HashSetSetHash */
    int32_t (*set_hash) (struct _HashSet*, uint32_t (*) (Key, size_t));
} HashSet;


/*===========================================================================*
 *             Definition for the exported member operations                 *
 *===========================================================================*/
/**
 * @brief The constructor for HashSet.
 *
 * @param ppObj         The double pointer to the to be constructed set
 *
 * @retval SUCC
 * @retval ERR_NOMEM    Insufficient memory for set construction
 */
int32_t HashSetInit(HashSet **ppObj);

/**
 * @brief The destructor for HashSet.
 *
 * If the custom resource clean method is set, it also runs the clean method
 * for each pair.
 *
 * @param ppObj         The double pointer to the to be destructed set
 */
void HashSetDeinit(HashSet **ppObj);

/**
 * @brief Insert a key into the set.
 *
 * This function inserts a key into the set. If the designated key is the same
 * with a certain one stored in the set, that key will be replaced to maintain
 * the element uniqueness. Also, if the custom resource clean method is set, it
 * runs the resource clean method for the replaced key.
 *
 * @param self          The pointer to HashSet structure
 * @param key           The designated key
 * @param size          Key size in bytes
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_NOMEM    Insufficient memory for set extension
 * @retval ERR_KEYSIZE  Invalid key size
 *
 * @note The key should be the pointer to the data you plan to hash for.
 */
int32_t HashSetAdd(HashSet *self, Key key, size_t size);

/**
 * @brief Check if the set contains the designated key.
 *
 * @param self          The pointer to HashSet structure
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
int32_t HashSetFind(HashSet *self, Key key, size_t size);

/**
 * @brief Delete the designated key from the set.
 *
 * This function deletes the designated key from the set. If the custom resource
 * clean method is set, it also runs the clean method for the deleted key.
 *
 * @param self          The pointer to HashSet structure
 * @param key           The designated key
 * @param size          Key size in bytes
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_NODATA   No set entry can be found
 * @retval ERR_KEYSIZE  Invalid key size
 *
 * @note The key should be the pointer to the data you plan to hash for.
 */
int32_t HashSetRemove(HashSet *self, Key key, size_t size);

/**
 * @brief Return the number of stored unique keys.
 *
 * @param self          The pointer to HashSet structure
 *
 * @return              The number of stored pairs
 * @retval ERR_NOINIT   Uninitialized container
 */
int32_t HashSetSize(HashSet *self);

/**
 * @brief Iterate through the set to retrieve each key.
 *
 * Before iterating through the set, it is necessary to pass:
 *  - bReset = true
 *  - pKey = NULL
 * for iterator initialization.
 *
 * After initialization, you can pass:
 *  - bReset = false
 *  - pKey = the pointer to get the returned key at each iteration.
 *
 * @param self          The pointer to HashSet structure
 * @param bReset        The knob to restart the iteration
 * @param pKey          The pointer to the returned key
 *
 * @retval SUCC         Iterator initialized successfully
 * @retval CONTINUE     Iteration in progress
 * @retval END          Iteration terminated
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_GET      Invalid parameter to store returned key
 *
 * @note Please do not insert or delete keys during set traversal
 */
int32_t HashSetIterate(HashSet *self, bool bReset, Key *pKey);

/**
 * @brief Set the custom key resource clean method.
 *
 * @param self          The pointer to HashSet structure
 * @param pFunc         The function pointer to the custom method
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 */
int32_t HashSetSetDestroy(HashSet *self, void (*pFunc) (Key));

/**
 * @brief Set the custom hash function.
 *
 * The default hash function is HashMurMur32.
 *
 * @param self          The pointer to HashSet structure
 * @param pFunc         The function pointer to the custom method
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 */
int32_t HashSetSetHash(HashSet *self, uint32_t (*pFunc) (Key, size_t));

/**
 * @brief Perform union operation for the designated two sets and create the
 *  result set.
 *
 * @param pFst          The pointer to the first source set
 * @param pSnd          The pointer to the second source set
 * @param ppDst         The double pointer to the to be created result set
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized source sets
 * @retval ERR_NOMEM    Insufficient memory for new set creation
 *
 * @note The newly created set will not delegate any key resource clean methods
 *  from two source sets. You can still set the clean method for it. However, to
 *  avoid the "double-free" problem, it is better to let the source sets handle
 *  the resource clean issue and treat the new set as the pure result from the
 *  union operation.
 */
int32_t HashSetUnion(HashSet *pFst, HashSet *pSnd, HashSet **ppDst);

/**
 * @brief Perform intersection operation for the designated two sets and create
 *  the result set.
 *
 * @param pFst          The pointer to the first source set
 * @param pSnd          The pointer to the second source set
 * @param ppDst         The double pointer to the to be created result set
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized source sets
 * @retval ERR_NOMEM    Insufficient memory for new set creation
 *
 * @note The newly created set will not delegate any key resource clean methods
 *  from two source sets. You can still set the clean method for it. However, to
 *  avoid the "double-free" problem, it is better to let the source sets handle
 *  the resource clean issue and treat the new set as the pure result from the
 *  intersection operation.
 */
int32_t HashSetIntersect(HashSet *pFst, HashSet *pSnd, HashSet **ppDst);

/**
 * @brief Perform difference operation for the first source set against the
 *  second source set and create the result set.
 *
 * @param pFst          The pointer to the first source set
 * @param pSnd          The pointer to the second source set
 * @param ppDst         The double pointer to the to be created result set
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized source sets
 * @retval ERR_NOMEM    Insufficient memory for new set creation
 *
 * @note The newly created set will not delegate any key resource clean methods
 *  from two source sets. You can still set the clean method for it. However, to
 *  avoid the "double-free" problem, it is better to let the source sets handle
 *  the resource clean issue and treat the new set as the pure result from the
 *  difference operation.
 */
int32_t HashSetDifference(HashSet *pFst, HashSet *pSnd, HashSet **ppDst);

#ifdef __cplusplus
}
#endif

#endif
