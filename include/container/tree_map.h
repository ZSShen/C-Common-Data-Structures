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

/** The implementation for ordered map. */
typedef struct _TreeMap {
    /** The container private information */
    TreeMapData *pData;

    /** Insert a key value pair into the map.
        @see TreeMapPut */
    int32_t (*put) (struct _TreeMap*, Pair*);

    /** Retrieve the value corresponding to the designated key.
        @see TreeMapGet */
    int32_t (*get) (struct _TreeMap*, Key, Value*);

    /** Check if the map contains the designated key.
        @see TreeMapFind */
    int32_t (*find) (struct _TreeMap*, Key);

    /** Delete the key value pair corresponding to the designated key.
        @see TreeMapRemove */
    int32_t (*remove) (struct _TreeMap*, Key);

    /** Return the number of stored key value pairs.
        @see TreeMapSize */
    int32_t (*size) (struct _TreeMap*);

    /** Retrieve the key value pair with the minimum order from the map.
        @see TreeMapMinimum */
    int32_t (*minimum) (struct _TreeMap*, Pair**);

    /** Retrieve the key value pair with the maximum order from the map.
        @see TreeMapMaximum */
    int32_t (*maximum) (struct _TreeMap*, Pair**);

    /** Retrieve the key value pair which is the predecessor of the given key.
        @see TreeMapPredecessor */
    int32_t (*predecessor) (struct _TreeMap*, Key, Pair**);

    /** Retrieve the key value pair which is the successor of the given key.
        @see TreeMapSuccessor */
    int32_t (*successor) (struct _TreeMap*, Key, Pair**);

    /** Iterate through the map from the minimum order to the maximum order.
        @see TreeMapIterate */
    int32_t (*iterate) (struct _TreeMap*, bool, Pair**);

    /** Iterate through the map from the maximum order to the minimum order.
        @see TreeMapReverseIterate */
    int32_t (*reverse_iterate) (struct _TreeMap*, bool, Pair**);

    /** Set the custom key comparison method.
        @see TreeMapSetCompare */
    int32_t (*set_compare) (struct _TreeMap*, int32_t (*) (Key, Key));

    /** Set the custom key value pair resource clean method.
        @see TreeMapSetDestroy */
    int32_t (*set_destroy) (struct _TreeMap*, void (*) (Pair*));
} TreeMap;


/*===========================================================================*
 *             Definition for the exported member operations                 *
 *===========================================================================*/
/**
 * @brief The constructor for TreeMap.
 *
 * @param ppObj         The double pointer to the to be constructed map
 *
 * @retval SUCC
 * @retval ERR_NOMEM    Insufficient memory for map construction
 */
int32_t TreeMapInit(TreeMap **ppObj);

/**
 * @brief The destructor for TreeMap.
 *
 * If the custom resource clean method is set, it also runs the clean method
 * for each pair.
 *
 * @param ppObj         The double pointer to the to be destructed map
 */
void TreeMapDeinit(TreeMap **ppObj);

/**
 * @brief Insert a key value pair into the map.
 *
 * This function inserts a key value pair into the map. If the order of the
 * designated pair is the same with a certain one stored in the map, that pair
 * will be replaced. Also, if the custom resource clean method is set, it runs
 * the clean method for the replaced pair.
 *
 * @param self          The pointer to TreeMap structure
 * @param pPair         The pointer to the designated pair
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_NOMEM    Insufficient memory for map extension
 */
int32_t TreeMapPut(TreeMap *self, Pair *pPair);

/**
 * @brief Retrieve the value corresponding to the designated key.
 *
 * This function retrieves the value corresponding to the designated key from
 * the map. If the key can be found, the value will be returned by the third
 * parameter. Otherwise, the error code in returned and the third parameter is
 * updated with NULL.
 *
 * @param self          The pointer to TreeMap structure
 * @param key           The designated key
 * @param pValue        The pointer to the returned value
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_NODATA   No map entry can be found
 * @retval ERR_GET      Invalid parameter to store returned value
 */
int32_t TreeMapGet(TreeMap *self, Key key, Value *pValue);

/**
 * @brief Check if the map contains the designated key.
 *
 * @param self          The pointer to TreeMap structure
 * @param key           The designated key
 *
 * @retval SUCC         The key can be found
 * @retval NOKEY        The key cannot be found
 * @retval ERR_NOINIT   Uninitialized container
 */
int32_t TreeMapFind(TreeMap *self, Key key);

/**
 * @brief Delete the key value pair corresponding to the designated key.
 *
 * This function deletes the key value pair corresponding to the designated key.
 * If the custom resource clean method is set, it runs the clean methods for
 * the deleted pair.
 *
 * @param self          The pointer to TreeMap structure
 * @param key           The designated key
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_NODATA   No map entry can be found
 */
int32_t TreeMapRemove(TreeMap *self, Key key);

/**
 * @brief Return the number of stored key value pairs.
 *
 * @param self          The pointer to TreeMap structure
 *
 * @return              The number of stored pairs
 * @retval ERR_NOINIT   Uninitialized container
 */
int32_t TreeMapSize(TreeMap *self);

/**
 * @brief Retrieve the key value pair with the minimum order from the map.
 *
 * @param self          The pointer to TreeMap structure
 * @param ppPair        The double pointer to the returned pair
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_IDX      Empty map
 * @retval ERR_GET      Invalid parameter to store returned pair
 */
int32_t TreeMapMinimum(TreeMap *self, Pair **ppPair);

/**
 * @brief Retrieve the key value pair with the maximum order from the map.
 *
 * @param self          The pointer to TreeMap structure
 * @param ppPair        The double pointer to the returned pair
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_IDX      Empty map
 * @retval ERR_GET      Invalid parameter to store returned pair
 */
int32_t TreeMapMaximum(TreeMap *self, Pair **ppPair);

/**
 * @brief Retrieve the key value pair which is the predecessor of the given key.
 *
 * @param self          The pointer to TreeMap structure
 * @param key           The designated key
 * @param ppPair        The double pointer to the returned pair
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_NODATA   Non-existent immediate predecessor
 * @retval ERR_GET      Invalid parameter to store returned pair
 */
int32_t TreeMapPredecessor(TreeMap *self, Key key, Pair **ppPair);

/**
 * @brief Retrieve the key value pair which is the successor of the given key.
 *
 * @param self          The pointer to TreeMap structure
 * @param key           The designated key
 * @param ppPair        The double pointer to the returned pair
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_NODATA   Non-existent immediate successor
 * @retval ERR_GET      Invalid parameter to store returned pair
 */
int32_t TreeMapSuccessor(TreeMap *self, Key key, Pair **ppPair);

/**
 * @brief Iterate through the map from the minimum order to the maximum order.
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
 * @param self          The pointer to TreeMap structure
 * @param bReset        The knob to restart the iteration
 * @param ppPair        The double pointer to the returned pair
 *
 * @retval SUCC         Iterator initialized successfully
 * @retval CONTINUE     Iteration in progress
 * @retval END          Iteration terminiated
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_GET      Invalid parameter to store returned pair
 */
int32_t TreeMapIterate(TreeMap *self, bool bReset, Pair **ppPair);

/**
 * @brief Reversely iterate through the map from the minimum order to the
 *  maximum order.
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
 * @param self          The pointer to TreeMap structure
 * @param bReset        The knob to restart the iteration
 * @param ppPair        The double pointer to the returned pair
 *
 * @retval SUCC         Iterator initialized successfully
 * @retval CONTINUE     Iteration in progress
 * @retval END          Iteration terminiated
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_GET      Invalid parameter to store returned pair
 */
int32_t TreeMapReverseIterate(TreeMap *self, bool bReset, Pair **ppPair);

/**
 * @brief Set the custom key comparison method.
 *
 * @param self          The pointer to TreeMap structure
 * @param pFunc         The function pointer to the custom method
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 */
int32_t TreeMapSetCompare(TreeMap *self, int32_t (*pFunc) (Key, Key));

/**
 * @brief Set the custom key value pair resource clean method.
 *
 * @param self          The pointer to TreeMap structure
 * @param pFunc         The function pointer to the custom method
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 */
int32_t TreeMapSetDestroy(TreeMap *self, void (*pFunc) (Pair*));

#ifdef __cplusplus
}
#endif

#endif