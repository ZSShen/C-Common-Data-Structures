#include "cds.h"

#define SIZE_BLAH_BUF (128)

int main()
{
    Trie *pTrie;

    /* You should initialize the DS before any operations. */
    int32_t rc = TrieInit(&pTrie);
    if (rc != SUCC)
        return rc;

    char *szAlphaCap = "ABCDEFGHIJKLMNOPQRSTUVWXYZ\0";
    char *szAlpha = "abcdefghijklmnopqrstuvwxyz\0";

    /* Insert a single string into the trie. */
    char szBuf[SIZE_BLAH_BUF];
    int iIdx;
    for (iIdx = 0 ; iIdx < 26 ; ++iIdx) {
        szBuf[iIdx] = szAlphaCap[iIdx];
        szBuf[iIdx + 1] = 0;
        pTrie->insert(pTrie, szBuf);
    }
    for (iIdx = 0 ; iIdx < 26 ; ++iIdx) {
        szBuf[iIdx] = szAlpha[iIdx];
        szBuf[iIdx + 1] = 0;
        pTrie->insert(pTrie, szBuf);
    }

    /* Insert an array of strings into the trie. */
    char *aStrNum[6];
    aStrNum[0] = "202-555-0104\0";
    aStrNum[1] = "202-555-0175\0";
    aStrNum[2] = "202-556-0171\0";
    aStrNum[3] = "202-555-9857\0";
    aStrNum[4] = "202-552-0180\0";
    aStrNum[5] = "202-455-7104\0";
    pTrie->bulk_insert(pTrie, aStrNum, 6);

    /* Check for exact string. */
    assert(pTrie->has_exact(pTrie, "abcdefghijklmn") == SUCC);
    assert(pTrie->has_exact(pTrie, "bcdefghijklmn") == NOKEY);

    /* Check for the strings matching the designated prefix. */
    assert(pTrie->has_prefix_as(pTrie, "A") == SUCC);
    assert(pTrie->has_prefix_as(pTrie, "BCD") == NOKEY);

    /* Delete the string in the trie. */
    pTrie->remove(pTrie, aStrNum[3]);

    /* Get the array of strings matching the designated prefix. */
    char **aStr;
    int32_t iSizeArr;
    pTrie->get_prefix_as(pTrie, "202", &aStr, &iSizeArr);
    assert(iSizeArr == 5);

    /* The returned strings are sorted by lexical order. */
    assert(strcmp(aStr[0], aStrNum[5]) == 0); /* 202-455-7104 */
    assert(strcmp(aStr[1], aStrNum[4]) == 0); /* 202-552-0180 */
    assert(strcmp(aStr[2], aStrNum[0]) == 0); /* 202-555-0104 */
    assert(strcmp(aStr[3], aStrNum[1]) == 0); /* 202-555-0175 */
    assert(strcmp(aStr[4], aStrNum[2]) == 0); /* 202-556-0171 */

    /* Remember to free the returned array of strings. */
    for (iIdx = 0 ; iIdx < iSizeArr ; ++iIdx)
        free(aStr[iIdx]);
    free(aStr);

    /* You should deinitialize the DS after all the relevant tasks. */
    TrieDeinit(&pTrie);

    return SUCC;
}

