/**
 * @file ordered_map.h The ordered map.
 */

#ifndef _ORDERED_MAP_H_
#define _ORDERED_MAP_H_

#include "../util.h"

/** OMapData is the data type for the container private information. */
typedef struct _OMapData OMapData;

/** The implementation for ordered map. */
typedef struct _OrderedMap {
    /** The container private information */
    OMapData *pData;

    /** Insert a key value pair into the map.
        @see OdrMapPut */
    int32_t (*put) (struct _OrderedMap*, Entry, bool);

    /** Retrieve the value corresponding to the designated key.
        @see OdrMapGet */
    int32_t (*get) (struct _OrderedMap*, Key, Value*);

    /** Delete the key value pair corresponding to the designated key.
        @see OdrMapRemove */
    int32_t (*remove) (struct _OrderedMap*, Key, bool);

    /** Return the number of stored key value pairs.
        @see OdrMapSize */
    int32_t (*size) (struct _OrderedMap*);

    /** Set the user defined key value pair comparison method.
        @see OdrMapSetCompare */
    int32_t (*set_compare) (struct _OrderedMap*, int32_t (*) (Entry, Entry));

    /** Set the user defined key value pair clean method.
        @see OdrMapSetDestroy */
    int32_t (*set_destroy) (struct _OrderedMap*, void (*) (Entry));
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
 * If the knob is on, it also runs the resource clean method for all the items.
 *
 * @param ppObj         The double pointer to the to be destructed map
 * @param bClean        The knob to clean item resource
 */
void OdrMapDeinit(OrderedMap **ppObj, bool bClean);

/**
 * @brief Insert a key value pair into the map.
 *
 * This function inserts a key value pair into the map. If the order of the
 * designated pair is the same with a certain one stored in the map, that pair
 * will be replaced. Also, if the knob is on, it runs the resource clean method
 * for the replaced pair.
 *
 * @param self          The pointer to OrderedMap structure
 * @param ent           The pointer to the desiganted key value pair
 * @param bClean        The knob to clean item resource
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_NOMEM    Insufficient memory for pair insertion
 */
int32_t OdrMapPut(OrderedMap *self, Entry ent, bool bClean);

/**
 * @brief Retrieve the value corresponding to the designated key.
 *
 * This function retrieves the value corresponding to the designated key from
 * the map. If the key can be found, the value will be returned by the third
 * parameter. Otherwise, the error code in returned and the third parameter is
 * updated with NULL.
 *
 * @param self          The pointer to OrderedMap structure
 * @param key           The designated key
 * @param pValue        The pointer to the returned value
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_NODATA   No map entry can be found
 */
int32_t OdrMapGet(OrderedMap *self, Key key, Value *pValue);

/**
 * @brief Delete the key value pair corresponding to the designated key.
 *
 * This function deletes the key value pair corresponding to the designated key.
 * If the knob is on, it also runs the resource clean method for the deleted pair.
 *
 * @param self          The pointer to OrderedMap structure
 * @param key           The designated key
 * @param bClean        The knob to clean item resource
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_NODATA   No map entry can be found
 */
int32_t OdrMapRemove(OrderedMap *self, Key key, bool bClean);

/**
 * @brief Return the number of stored key value pairs.
 *
 * @param self          The pointer to OrderedMap structure
 *
 * @return              The number of stored pairs
 * @retval ERR_NOINIT   Uninitialized container
 */
int32_t OdrMapSize(OrderedMap *self);

/**
 * @brief Set the user defined key value pair comparison method.
 *
 * @param self          The pointer to OrderedMap structure
 * @param pFunc         The function pointer to the custom method
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 */
int32_t OdrMapSetCompare(OrderedMap *self, int32_t (*pFunc) (Entry, Entry));

/**
 * @brief Set the user defined key value pair clean method.
 *
 * @param self          The pointer to OrderedMap structure
 * @param pFunc         The function pointer to the custom method
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 */
int32_t OdrMapSetDestroy(OrderedMap *self, void (*pFunc) (Entry));

#endif