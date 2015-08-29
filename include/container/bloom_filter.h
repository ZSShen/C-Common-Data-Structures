/**
 * @file bloom_filter.h The efficient hash set for unique element query.
 */

#ifndef _BLOOM_FILTER_H_
#define _BLOOM_FILTER_H_

#include "../util.h"

/** BloomFilterData is the data type for the container private information. */
typedef struct _BloomFilterData BloomFilterData;

/** The implementation for Bloom filter. */
typedef struct _BloomFilter {
    /** The container private information */
    BloomFilterData *pData;

    /** Insert a key into Bloom filter.
        @see BloomFilterInsert */
    int32_t (*insert) (struct _BloomFilter*, Key, size_t);

    /** Check if Bloom filter contains the designated key.
        @see BloomFilterFind */
    int32_t (*query) (struct _BloomFilter*, Key, size_t);

    /** Return the number of stored unique keys.
        @see BloomFilterSize */
    int32_t (*size) (struct _BloomFilter*);
} BloomFilter;


/*===========================================================================*
 *             Definition for the exported member operations                 *
 *===========================================================================*/
/**
 * @brief The constructor for BloomFilter.
 *
 * @param ppObj         The double pointer to the to be constructed Bloom filter
 * @param size          The
 *
 * @retval SUCC
 * @retval ERR_NOMEM    Insufficient memory for Bloom filter construction
 */
int32_t BloomFilterInit(BloomFilter **ppObj);

/**
 * @brief The destructor for BloomFilter.
 *
 * @param ppObj         The double pointer to the to be destructed Bloom filter
 */
void BloomFilterDeinit(BloomFilter **ppObj);

/**
 * @brief Insert a key into Bloom filter.
 *
 * @param self          The pointer to BloomFilter structure
 * @param key           The designated key
 * @param size          Key size in bytes
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_KEYSIZE  Invalid key size
 *
 * @note The key should be the pointer to the data you plan to hash for.
 */
int32_t BloomFilterInsert(BloomFilter *self, Key key, size_t size);

/**
 * @brief Check if Bloom filter contains the designated key.
 *
 * @param self          The pointer to BloomFilter structure
 * @param key           The designated key
 * @param size          Key size in bytes
 *
 * @retval SUCC         The key can be found
 * @retval NOKEY        The key cannot be found
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_KEYSIZE  Invalid key size
 *
 * @note The key should be the pointer to the data you plan to hash for.
 */
int32_t BloomFilterQuery(BloomFilter *self, Key key, size_t size);

/**
 * @brief Return the number of stored key value pairs.
 *
 * @param self          The pointer to BloomFilter structure
 *
 * @return              The number of stored pairs
 * @retval ERR_NOINIT   Uninitialized container
 */
int32_t BloomFilterSize(BloomFilter *self);

#endif
