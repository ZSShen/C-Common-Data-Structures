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

/** Insufficient memory for data structure expansion or reallocation. */
#define ERR_NOMEM       (-1)

/** Invalid argument to index the data structure. */
#define ERR_IDX         (-2)

#endif
