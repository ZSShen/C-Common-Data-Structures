/**
 * @file hash.h The collection of well-known hash functions.
 */

#ifndef _HASH_H_
#define _HASH_H_

#include "../util.h"


/*-------------------------------------------------------*
 *            Non-cryptographic hash function            *
 *-------------------------------------------------------*/
/**
 * @brief Google MurMur hash proposed by Austin Appleby in 2008.
 *
 * This is the version 3 MurMur implementation to yield 32 bit hash value.
 * https://code.google.com/p/smhasher/wiki/MurmurHash3
 *
 * @param key           The designated key
 * @param size          Size of the data pointed by the key in bytes
 *
 * @note The key should be the pointer to the data you plan to hash for.
 */
uint32_t HashMurMur32(Key key, size_t size);

/**
 * @brief Hash function proposed by Bob Jenkins in 1997.
 *
 * @param key           The designated key
 * @param size          Size of the data pointed by the key in bytes
 *
 * @note The key should be the pointer to the data you plan to hash for.
 */
uint32_t HashJenkins(Key key, size_t size);

#endif
