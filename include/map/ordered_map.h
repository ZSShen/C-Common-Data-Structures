/**
 * @file ordered_map.h The ordered map.
 */

#ifndef _ORDERED_MAP_H_
#define _ORDERED_MAP_H

#include "../util.h"

/** OMapData is the data type for the container private information. */
typedef struct _OMapData OMapData;

/** The implementation for ordered map. */
typedef struct _OrderedMap {
    /** The container private information */
    OMapData *pData;

    /** Insert a key value pair into the map.
        @see OdrMapPut */
    int32_t (*put) (struct _OrderedMap*, Pair);

    /** Retrieve the value corresponding to the designated key from the map.
        @see OdrMapGet */
    int32_t (*get) (struct _OrderedMap*, Key, Value*)

    /** Delete the map entry corresponding to the designated key.
        @see OdrMapRemove */
    int32_t (*remove) (struct _OrderedMap*, Key);

    /** Return the number of map entries.
        @see OdrMapSize */
    int32_t (*size) (struct _OrderedMap*);

    /** Set the user defined key comparison policy.
        @see OdrMapSetCompare */
    void (*set_compare) (struct _OrderedMap*, int32_t (*) (Key, Key));

    /** Set the user defined value clean policy.
        @see OdrMapSetDestroy */
    void (*set_destroy) (struct _OrderedMap*, void (*) (Value));
} OrderedMap;


/*===========================================================================*
 *             Definition for the exported member operations                 *
 *===========================================================================*/
/**
 * @brief The constructor for OrderedMap.
 *
 * @param ppObj         The double pointer to the to be constructed map
 *
 * @retval SUCC
 * @retval ERR_NOMEM    Insufficient memory for map construction
 */
int32_t OdrMapInit(OrderedMap **ppObj);

/**
 * @brief The destructor for OrderedMap.
 *
 * @param ppObj         The double pointer to the to be destructed map
 */
void OdrMapDeinit(OrderedMap **ppObj);

/**
 * @brief Insert a key value pair into the map.
 *
 * @param self          The pointer to OrderedMap structure
 * @param pair          The key value pair
 *
 * @retval SUCC
 * @retval ERR_NOMEM    Insufficient memory for pair insertion
 *
 * @note If the order of the designated pair is the same with a certain one
 * stored in the map, the old item will be removed and its contained value is
 * cleaned by the configured policy so that the designated pair can take the
 * position.
 */
int32_t OdrMapPut(OrderedMap *self, Pair pair);

/**
 * @brief Retrieve the value corresponding to the designated key from the map.
 *
 * @param self          The pointer to OrderedMap structure
 * @param key           The designated key
 * @param pValue        The pointer to the returned value
 *
 * @retval SUCC
 * @retval ERR_NODATA   No map entry can be found
 */
int32_t OdrMapGet(OrderedMap *self, Key key, Value *pValue);

/**
 * @brief Delete the map entry corresponding to the designated key.
 *
 * This function deletes the map entry corresponding to the designated key and
 * also cleans the resoruce hold by the value of that entry.
 *
 * @param self          The pointer to OrderedMap structure
 * @param key           The designated key
 *
 * @retval SUCC
 * @retval ERR_NODATA   No map entry can be found
 */
int32_t OdrMapRemove(OrderedMap *self, Key key);

/**
 * @brief Return the number of map entries.
 *
 * @param self          The pointer to OrderedMap structure
 *
 * @return              The number of map entries
 */
int32_t OdrMapSize(OrderedMap *self);

/**
 * @brief Set the user defined key comparison policy.
 *
 * @param self          The pointer to OrderedMap structure
 * @param pFunc         The function pointer to the custom policy
 */
void OdrMapSetCompare(OrderedMap *self, int32_t (*pFunc) (Key, Key));

/**
 * @brief Set the user defined value clean policy.
 *
 * @param self          The pointer to OrderedMap structure
 * @param pFunc         The function pointer to the custom policy
 */
void OdrMapSetDestroy(OrderedMap *self, void (*pFunc) (Value));