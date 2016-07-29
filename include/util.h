#ifndef _UTIL_H_
#define _UTIL_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

/** The function is finished gracefully. */
static const int32_t SUCC = 0;

/** The data structure is not initialized. */
static const int32_t ERR_NOINIT = -1;

/** Insufficient memory to extend data structure. */
static const int32_t ERR_NOMEM = -2;

/** Invalid argument to index the data structure. */
static const int32_t ERR_IDX = -3;

/** No entry can be found in the associative data structures with the given key. */
static const int32_t ERR_NODATA = -4;

/** Invalid argument to store the returned data from getter operation. */
static const int32_t ERR_GET = -5;

/** Invalid argument to specify the key size for hash data structures. */
static const int32_t ERR_KEYSIZE = -6;

/** The data structure is modified during the traversal. */
static const int32_t ERR_ITER = -7;

/** Fail to register the unit test function. */
static const int32_t ERR_REG = -8;

/** Iteration in progress. */
static const int32_t CONTINUE = 1;

/** Iteration terminated at the end of the data structure */
static const int32_t END = 2;

/** The designated key cannot be found in the map */
static const int32_t NOKEY = 3;


/** Item is the type for the elements of ordinary data structures. */
typedef const void* Item;

/** Key is the type to index associative data structures. */
typedef const void* Key;

/** Value is the type for the data associating with a specific key. */
typedef const void* Value;

/** Pair is the key value pair stored in associative data structures. */
typedef struct _Pair {
    void* key;
    void* value;
} Pair;

#endif
