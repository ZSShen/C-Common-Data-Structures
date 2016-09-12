/**
 *   The MIT License (MIT)
 *   Copyright (C) 2016 ZongXian Shen <andy.zsshen@gmail.com>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a
 *   copy of this software and associated documentation files (the "Software"),
 *   to deal in the Software without restriction, including without limitation
 *   the rights to use, copy, modify, merge, publish, distribute, sublicense,
 *   and/or sell copies of the Software, and to permit persons to whom the
 *   Software is furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 *   THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 *   IN THE SOFTWARE.
 */

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
    TrieData *data;

    /** Insert a string into the trie.
        @see TrieInsert */
    bool (*insert) (struct _Trie*, const char*);

    /** Insert an array of strings into the trie.
        @see TrieBulkInsert */
    bool (*bulk_insert) (struct _Trie*, const char**, unsigned);

    /** Check if the trie contains the specified string.
        @see TrieHasExact */
    bool (*has_exact) (struct _Trie*, const char*);

    /** Check if the trie contains the strings matching the specified prefix.
        @see TrieHasPrefixAs */
    bool (*has_prefix_as) (struct _Trie*, const char*);

    /** Retrieve the strings from the trie matching the specified prefix.
        @see TrieGetPrefixAs */
    bool (*get_prefix_as) (struct _Trie*, const char*, const char***, unsigned*);

    /** Remove a string from the trie.
        @see TrieRemove */
    bool (*remove) (struct _Trie*, const char*);

    /** Return the number of strings stored in the trie.
        @see TrieSize */
    unsigned (*size) (struct _Trie*);
} Trie;


/*===========================================================================*
 *             Definition for the exported member operations                 *
 *===========================================================================*/
/**
 * @brief The constructor for Trie.
 *
 * @retval obj          The successfully constructed trie
 * @retval NULL         Insufficient memory for trie construction
 */
Trie* TrieInit();

/**
 * @brief The destructor for Trie.
 *
 * @param obj           The pointer to the to be destructed trie
 */
void TrieDeinit(Trie* obj);

/**
 * @brief Insert a string into the trie.
 *
 * @param self          The pointer to Trie structure
 * @param str           The specified string
 *
 * @retval true         The string is successfully inserted
 * @retval false        The string cannot be inserted due to insufficient memory
 */
bool TrieInsert(Trie* self, const char* str);

/**
 * @brief Insert an array of strings into the trie.
 *
 * @param self          The pointer to Trie structure
 * @param strs          Array of to be inserted strings
 * @param size          The array size
 *
 * @retval true         The strings are successfully inserted
 * @retval false        The strings cannot be inserted due to insufficient memory
 */
bool TrieBulkInsert(Trie* self, const char** strs, unsigned size);

/**
 * @brief Check if the trie contains the specified string.
 *
 * @param self          The pointer to Trie structure
 * @param str           The specified string
 *
 * @retval true         The trie contains the given string
 * @retval false        No such string
 */
bool TrieHasExact(Trie* self, const char* str);

/**
 * @brief Check if the trie contains the strings matching the specified prefix.
 *
 * @param self          The pointer to Trie structure
 * @param prefix        The specified prefix
 *
 * @retval true         The trie contains the given prefix
 * @retval false        No such prefix
 */
bool TrieHasPrefixAs(Trie* self, const char* prefix);

/**
 * @brief Retrieve the strings from the trie matching the specified prefix.
 *
 * @param self          The pointer to Trie structure
 * @param prefix        The specified prefix
 * @param p_strs        The pointer to the returned array of strings
 * @param p_size        The pointer to the returned array size
 *
 * @retval true         The strings matching the given prefix are returned
 * @retval false        No string matching the given prefix or insufficient
 *                      memory to store the matched strings
 *
 * @note Please remember to free the returned array of strings.
 */
bool TrieGetPrefixAs(Trie* self, const char* prefix, const char*** p_strs, unsigned* p_size);

/**
 * @brief Remove a string from the trie.
 *
 * @param self          The pointer to Trie structure
 * @param str           The specified string
 *
 * @retval true         The specified string is successfully removed
 * @retval false        No such string
 */
bool TrieRemove(Trie* self, const char* str);

/**
 * @brief Return the number of stored strings.
 *
 * @param self          The pointer to Trie structure
 *
 * @retval size         The number of stored strings
 */
unsigned TrieSize(Trie* self);

#ifdef __cplusplus
}
#endif

#endif
