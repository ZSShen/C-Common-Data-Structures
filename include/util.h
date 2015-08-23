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

/** Insufficient memory to extend data structure. */
#define ERR_NOMEM       (-2)

/** Invalid argument to index the data structure. */
#define ERR_IDX         (-3)

/** No entry can be found in the associative data structures with the given key. */
#define ERR_NODATA      (-4)

/** Invalid argument to store the returned data from getter operation. */
#define ERR_GET         (-5)

/** Invalid argument to specify the key size for hash data structures. */
#define ERR_KEYSIZE     (-6)

/** The data structure is modified during the traversal. */
#define ERR_ITER        (-7)

/** Fail to register the unit test function. */
#define ERR_REG         (-8)

/** Iteration in progress. */
#define CONTINUE		(1)

/** Iteration terminated at the end of the data structure */
#define END             (2)

/** The designated key cannot be found in the map */
#define NOKEY           (3)


/** Item is the type for the elements of ordinary data structures. */
typedef const void* Item;

/** Key is the type to index associative data structures. */
typedef const void* Key;

/** Value is the type for the data associating with a specific key. */
typedef const void* Value;

/** Pair is the key value pair stored in associative data structures. */
typedef struct _Pair {
    Key key;
    Value value;
} Pair;

#endif
