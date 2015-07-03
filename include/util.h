#ifndef _UTIL_H_
#define _UTIL_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

/** The function is finished gracefully. */
#define SUCC            (0)

/** The data structure is not initialized. */
#define ERR_NOINIT      (-1)

/** Insufficient memory for data structure expansion or reallocation. */
#define ERR_NOMEM       (-2)

/** Invalid argument to index the data structure. */
#define ERR_IDX         (-3)

/** Duplicated key to insert associative data structures. */
#define ERR_DUPKEY      (-4)

/** No entry can be found with the given key from the associative data structures. */
#define ERR_NODATA      (-5)

/** Invalid argument to the interface. */
#define ERR_ARG         (-6)

/** Item is the data type for the container element. */
typedef const void* Item;

/** Key is the data type to index map-like containers. */
typedef const void* Key;

/** Value is the data type for the map-like element. */
typedef const void* Value;

/** Entry is the data type which wraps key value pair to manipulate map-like
    containers. */
typedef const void* Entry;

/** Pair is the real key value pair stored in the map-like containers. */
typedef struct _Pair {
    Key key;
    Value value;
} Pair;

#endif
