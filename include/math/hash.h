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
 * This is the version 3 to yield 32 bit hash value.
 * https://code.google.com/p/smhasher/wiki/MurmurHash3
 *
 * @param key           The designated key
 * @param sizeKey       The key size in bytes
 */
uint32_t HashMurMur32(Key key, size_t sizeKey);

/**
 * @brief Hash function proposed by Bob Jenkins in 1997.
 *
 * @param key           The designated key
 * @param sizeKey       The key size in bytes
 */
uint32_t HashJenkins(Key key, size_t sizeKey);

#endif