/**
 * @file tree_map.h The ordered map implemented by red black tree.
 */

#ifndef _TREE_MAP_H_
#define _TREE_MAP_H_

#include "../util.h"

/** TreeMapData is the data type for the container private information. */
typedef struct _TreeMapData TreeMapData;

/** The implementation for ordered map. */
typedef struct _TreeMap {
    /** The container private information */
    TreeMapData *pData;

    /** Insert a key value pair into the map.
        @see TreeMapPut */
    int32_t (*put) (struct _TreeMap*, Entry);

    /** Retrieve the value corresponding to the designated key.
        @see TreeMapGet */
    int32_t (*get) (struct _TreeMap*, Key, Value*);

    /** Delete the key value pair corresponding to the designated key.
        @see TreeMapRemove */
    int32_t (*remove) (struct _TreeMap*, Key);

    /** Return the number of stored key value pairs.
        @see TreeMapSize */
    int32_t (*size) (struct _TreeMap*);

    /** Set the user defined key value pair comparison method.
        @see TreeMapSetCompare */
    int32_t (*set_compare) (struct _TreeMap*, int32_t (*) (Entry, Entry));

    /** Set the user defined key value pair clean method.
        @see TreeMapSetDestroy */
    int32_t (*set_destroy) (struct _TreeMap*, void (*) (Entry));
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
 * If the custom resource clean method is set, it also runs the clean method for
 * all the pairs.
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
 * @param ent           The pointer to the desiganted key value pair
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_NOMEM    Insufficient memory for map extension
 */
int32_t TreeMapPut(TreeMap *self, Entry ent);

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
 * @retval ERR_GET      Invalid parameter to store returned item
 */
int32_t TreeMapGet(TreeMap *self, Key key, Value *pValue);

/**
 * @brief Delete the key value pair corresponding to the designated key.
 *
 * This function deletes the key value pair corresponding to the designated key.
 * If the custom resource clean method is set, it runs the clean method for the
 * removed pair.
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
 * @brief Set the user defined key value pair comparison method.
 *
 * @param self          The pointer to TreeMap structure
 * @param pFunc         The function pointer to the custom method
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 */
int32_t TreeMapSetCompare(TreeMap *self, int32_t (*pFunc) (Entry, Entry));

/**
 * @brief Set the user defined key value pair clean method.
 *
 * @param self          The pointer to TreeMap structure
 * @param pFunc         The function pointer to the custom method
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 */
int32_t TreeMapSetDestroy(TreeMap *self, void (*pFunc) (Entry));

#endif