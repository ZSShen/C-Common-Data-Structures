/**
 * @file trie.h The string dictionary container
 */
#ifndef _TRIE_H_
#define _TRIE_H_

#include "../util.h"

#ifdef __cplusplus
extern "C" {
#endif

/** TrieData is the data type for the container private information. */
typedef struct TrieData_ TrieData;

/** The implementation for trie. */
typedef struct _Trie {
    /** The container private information. */
    TrieData *pData;

    /** Insert a string into the trie.
        @see TrieInsert */
    int32_t (*insert) (struct _Trie*, char*);

    /** Insert an array of strings into the trie.
        @see TrieBulkInsert */
    int32_t (*bulk_insert) (struct _Trie*, char**, int);

    /** Check if the trie contains the designated string.
        @see TrieHasExact */
    int32_t (*has_exact) (struct _Trie*, char*);

    /** Check if the trie contains the strings matching the designated prefix.
        @see TrieHasPrefixAs */
    int32_t (*has_prefix_as) (struct _Trie*, char*);

    /** Retrieve the strings from the trie matching the designated prefix.
        @see TrieGetPrefixAs */
    int32_t (*get_prefix_as) (struct _Trie*, char*, char***, int*);

    /** Remove a string from the trie.
        @see TrieRemove */
    int32_t (*remove) (struct _Trie*, char*);

    /** Return the number of strings stored in the trie.
        @see TrieSize */
    int32_t (*size) (struct _Trie*);
} Trie;


/*===========================================================================*
 *             Definition for the exported member operations                 *
 *===========================================================================*/
/**
 * @brief The constructor for Trie.
 *
 * @param ppObj         The double pointer to the to be constructed trie
 *
 * @retval SUCC
 * @retval ERR_NOMEM    Insufficient memory for trie construction
 */
int32_t TrieInit(Trie **ppObj);

/**
 * @brief The destructor for Trie.
 *
 * @param ppObj         The double pointer to the to be destructed trie
 */
void TrieDeinit(Trie **ppObj);

/**
 * @brief Insert a string into the trie.
 *
 * @param self          The pointer to Trie structure
 * @param str           The designated string
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_NOMEM    Insufficient memory for trie extension
 */
int32_t TrieInsert(Trie *self, char *str);

/**
 * @brief Insert an array of strings into the trie.
 *
 * @param self          The pointer to Trie structure
 * @param aStr          Array of to be inserted strings
 * @param iNum          The array size
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_NOMEM    Insufficient memory for trie extension
 */
int32_t TrieBulkInsert(Trie *self, char **aStr, int iNum);

/**
 * @brief Check if the trie contains the designated string.
 *
 * @param self          The pointer to Trie structure
 * @param str           The designated string
 *
 * @retval SUCC
 * @retval NOKEY
 * @retval ERR_NOINIT   Uninitialized container
 */
int32_t TrieHasExact(Trie *self, char *str);

/**
 * @brief Check if the trie contains the strings matching the designated prefix.
 *
 * @param self          The pointer to Trie structure
 * @param str           The designated prefix
 *
 * @retval SUCC
 * @retval NOKEY
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_NOMEM    Insufficient memory to prepare trie traversal
 */
int32_t TrieHasPrefixAs(Trie *self, char *str);

/**
 * @brief Retrieve the strings from the trie matching the designated prefix.
 *
 * To retrieve the strings, you need to pass:
 *   - The pointer to the string array to store the returned strings.
 *   - The pointer to the integer for the returned array size.
 *
 * And this function will allocate the memory to store the returned strings.
 * But if no string can be resolved, the string array will be returned as NULL
 * and the array size will be returned as 0.
 *
 * @param self          The pointer to Trie structure
 * @param str           The designated prefix
 * @param paStr         The pointer to the returned array of strings
 * @param piNum         The pointer to the returned array size
 *
 * @retval SUCC
 * @retval NOKEY
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_GET      Invalid parameter to store returned data
 * @retval ERR_NOMEM    Insufficient memory to store the resolved strings
 *
 * @note Please remember to free the following resource:
 *       - Each returned string
 *       - The array to store returned strings
 */
int32_t TrieGetPrefixAs(Trie *self, char* str, char ***paStr, int *piNum);

/**
 * @brief Delete a string from the trie.
 *
 * @param self          The pointer to Trie structure
 * @param str           The designated string
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_NODATA   Non-existent string
 */
int32_t TrieRemove(Trie *self, char *str);

/**
 * @brief Return the number of strings stored in the trie.
 *
 * @param self          The pointer to Trie structure
 *
 * @return              The number of strings
 * @retval ERR_NOINIT   Uninitialized container
 */
int32_t TrieSize(Trie *self);

#ifdef __cplusplus
}
#endif

#endif
