#ifndef _UTIL_H_
#define _UTIL_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>
#include <assert.h>


/** The key value pair for associative data structures. */
typedef struct _Pair {
    void* key;
    void* value;
} Pair;

#endif
