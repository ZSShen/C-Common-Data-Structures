#include "container/trie.h"
#include "CUnit/Util.h"
#include "CUnit/Basic.h"


#define SIZE_LONG_STR   (1024)


int32_t AddSuite();
void TestInsertDummy();
void TestInsertOnePrefix();
void TestInsertOneSuffix();
void TestBulkInsert();
void TestSearchExact();
void TestSearchPrefix();
void TestDeleteThenVerify();
void TestGetPrefix();


int32_t main()
{
    int32_t rc = SUCC;

    if (CU_initialize_registry() != CUE_SUCCESS) {
        rc = CU_get_error();
        goto EXIT;
    }

    /* Register the test suite for stack structure verification. */
    if (AddSuite() != SUCC) {
        rc = CU_get_error();
        goto CLEAN;
    }

    /* Launch all the tests. */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

CLEAN:
    CU_cleanup_registry();
EXIT:
    return rc;
}

int32_t AddSuite()
{
    int32_t rc = SUCC;

    CU_pSuite pSuite = CU_add_suite("Structure Verification", NULL, NULL);
    if (!pSuite) {
        rc = ERR_REG;
        goto EXIT;
    }

    char *szMsg = "Insert the dummy data and verify the boundary case handling.";
    CU_pTest pTest = CU_add_test(pSuite, szMsg, TestInsertDummy);
    if (!pTest)
        rc = ERR_REG;

    szMsg = "Insert the strings sharing the only one common prefix.";
    pTest = CU_add_test(pSuite, szMsg, TestInsertOnePrefix);
    if (!pTest)
        rc = ERR_REG;

    szMsg = "Insert the strings sharing the only one common suffix.";
    pTest = CU_add_test(pSuite, szMsg, TestInsertOneSuffix);
    if (!pTest)
        rc = ERR_REG;

    szMsg = "Put bulk data with bulk_insert() and insert().";
    pTest = CU_add_test(pSuite, szMsg, TestBulkInsert);
    if (!pTest)
        rc = ERR_REG;

    szMsg = "Search for the extact strings.";
    pTest = CU_add_test(pSuite, szMsg, TestSearchExact);
    if (!pTest)
        rc = ERR_REG;

    szMsg = "Search for the strings matching the designated prefix.";
    pTest = CU_add_test(pSuite, szMsg, TestSearchPrefix);
    if (!pTest)
        rc = ERR_REG;

    szMsg = "Delete strings and then verify the searching operators.";
    pTest = CU_add_test(pSuite, szMsg, TestDeleteThenVerify);
    if (!pTest)
        rc = ERR_REG;

    szMsg = "Get the strings matching the designated prefix.";
    pTest = CU_add_test(pSuite, szMsg, TestGetPrefix);
    if (!pTest)
        rc = ERR_REG;

EXIT:
    return rc;
}

void TestInsertDummy()
{
    Trie *pTrie;
    CU_ASSERT(TrieInit(&pTrie) == SUCC);

    CU_ASSERT(pTrie->insert(pTrie, "ab\0") == SUCC);
    CU_ASSERT(pTrie->insert(pTrie, "aa\0") == SUCC);

    /* Insert NULL into the trie. */
    CU_ASSERT(pTrie->insert(pTrie, NULL) == SUCC);

    /* Insert dummy string into the trie. */
    CU_ASSERT(pTrie->insert(pTrie, "\0") == SUCC);
    CU_ASSERT(pTrie->insert(pTrie, "\0\0") == SUCC);

    CU_ASSERT_EQUAL(pTrie->size(pTrie), 2);

    TrieDeinit(&pTrie);
}

void TestInsertOnePrefix()
{
    Trie *pTrie;
    CU_ASSERT(TrieInit(&pTrie) == SUCC);

    char *szPrefix = "abcdefghijklmnopqrstuvwxyz\0";
    int32_t iLen = strlen(szPrefix);
    char szBuf[iLen + 1];

    /* Insert the prefixes and verify the number of stored strings. */
    int32_t iIdx;
    for (iIdx = 0 ; iIdx < iLen ; ++iIdx) {
        int32_t iOfst = iIdx;
        while (iOfst < iLen) {
            int32_t iCap = iOfst - iIdx + 2;
            memset(szBuf, 0, sizeof(char) * iCap);
            strncpy(szBuf, szPrefix + iIdx, iCap - 1);
            CU_ASSERT(pTrie->insert(pTrie, szBuf) == SUCC);
            ++iOfst;
        }
    }
    CU_ASSERT_EQUAL(pTrie->size(pTrie), ((iLen * (iLen + 1)) >> 1));

    TrieDeinit(&pTrie);
}

void TestInsertOneSuffix()
{
    Trie *pTrie;
    CU_ASSERT(TrieInit(&pTrie) == SUCC);

    char *szSuffix = "abcdefghijklmnopqrstuvwxyz\0";
    int32_t iLen = strlen(szSuffix);
    char szBuf[iLen + 1];

    /* Insert the suffixes and verify the number of stored strings. */
    int32_t iIdx;
    for (iIdx = iLen - 1 ; iIdx >= 0 ; --iIdx) {
        int32_t iOfst = iIdx;
        while (iOfst >= 0) {
            int32_t iCap = iIdx - iOfst + 2;
            memset(szBuf, 0, sizeof(char) * iCap);
            strncpy(szBuf, szSuffix + iOfst, iCap - 1);
            CU_ASSERT(pTrie->insert(pTrie, szBuf) == SUCC);
            --iOfst;
        }
    }
    CU_ASSERT_EQUAL(pTrie->size(pTrie), ((iLen * (iLen + 1)) >> 1));

    TrieDeinit(&pTrie);
}

void TestBulkInsert()
{
    Trie *pTrie;
    CU_ASSERT(TrieInit(&pTrie) == SUCC);

    char *szPalin = "abcdefghijklmnopqrstuvwxyzzyxwvutsrqponmlkjihgfedcba"
                    "ABCDEFGHIJKLMNOPQRSTUVWXYZZYXWVUTSRQPONMLKJIHGFEDCBA\0";
    int32_t iLen = strlen(szPalin);
    int32_t iShape = iLen * (iLen + 1);

    /* Apply bulk_insert() to put the first half of the data. */
    char **aStr = (char**)malloc(sizeof(char*) * ((iShape >> 1) + 2));
    int32_t iPivot = 0, iIdx;
    for (iIdx = 0 ; iIdx < iLen ; ++iIdx) {
        int32_t iOfst = iIdx;
        while (iOfst < iLen) {
            int32_t iCap = iOfst - iIdx + 2;
            char *str = (char*)malloc(sizeof(char) * iCap);
            memset(str, 0, sizeof(char) * iCap);
            strncpy(str, szPalin + iIdx, iCap - 1);
            aStr[iPivot++] = str;
            ++iOfst;
        }
    }

    CU_ASSERT(pTrie->bulk_insert(pTrie, aStr, iPivot >> 1) == SUCC);
    CU_ASSERT_EQUAL(pTrie->size(pTrie), (iShape >> 2) - 5);

    aStr[iPivot] = NULL;
    aStr[iPivot + 1] = "\0";
    CU_ASSERT(pTrie->bulk_insert(pTrie, aStr + iPivot, 2) == SUCC);
    CU_ASSERT_EQUAL(pTrie->size(pTrie), (iShape >> 2) - 5);

    /* Iteratively apply insert() to put the second half of the data. */
    for (iIdx = iPivot >> 1 ; iIdx < iPivot ; ++iIdx)
        CU_ASSERT(pTrie->insert(pTrie, aStr[iIdx]) == SUCC);
    CU_ASSERT_EQUAL(pTrie->size(pTrie), (iShape >> 1) - (iLen >> 1));

    while (iPivot > 0)
        free(aStr[--iPivot]);
    free(aStr);

    TrieDeinit(&pTrie);
}

void TestSearchExact()
{
    Trie *pTrie;
    CU_ASSERT(TrieInit(&pTrie) == SUCC);

    char *szSeq = "nopqrstuvwxyzzyxwvutsrqponmlkjihgfedcba\0";
    int32_t iLen = strlen(szSeq);
    char szBuf[iLen + 1];

    int32_t iIdx;
    for (iIdx = 0 ; iIdx < iLen - 2 ; ++iIdx) {
        memset(szBuf, 0, sizeof(char) * 4);
        strncpy(szBuf, szSeq + iIdx, 3);
        CU_ASSERT(pTrie->insert(pTrie, szBuf) == SUCC);
    }

    /* Search for the existing strings. */
    for (iIdx = 0 ; iIdx < iLen - 2 ; ++iIdx) {
        memset(szBuf, 0, sizeof(char) * 4);
        strncpy(szBuf, szSeq + iIdx, 3);
        CU_ASSERT(pTrie->has_exact(pTrie, szBuf) == SUCC);
    }

    /* Search for the non-existing strings */
    for (iIdx = 0 ; iIdx < iLen - 1 ; ++iIdx) {
        memset(szBuf, 0, sizeof(char) * 3);
        strncpy(szBuf, szSeq + iIdx, 2);
        CU_ASSERT(pTrie->has_exact(pTrie, szBuf) == NOKEY);
    }
    for (iIdx = 0 ; iIdx < iLen - 3 ; ++iIdx) {
        memset(szBuf, 0, sizeof(char) * 5);
        strncpy(szBuf, szSeq + iIdx, 4);
        CU_ASSERT(pTrie->has_exact(pTrie, szBuf) == NOKEY);
    }

    CU_ASSERT(pTrie->has_exact(pTrie, NULL) == NOKEY);
    CU_ASSERT(pTrie->has_exact(pTrie, "\0") == NOKEY);
    CU_ASSERT(pTrie->has_exact(pTrie, "123\0") == NOKEY);

    TrieDeinit(&pTrie);
}

void TestSearchPrefix()
{
    Trie *pTrie;
    CU_ASSERT(TrieInit(&pTrie) == SUCC);

    char *szSeq = "nopqrstuvwxyzyxwvutsrqponmlkjihgfedcbabcdefghijklmno\0";
    int32_t iLen = strlen(szSeq);
    char szBuf[iLen + 1];

    int32_t iIdx;
    for (iIdx = 0 ; iIdx < iLen - 2 ; ++iIdx) {
        memset(szBuf, 0, sizeof(char) * 4);
        strncpy(szBuf, szSeq + iIdx, 3);
        CU_ASSERT(pTrie->insert(pTrie, szBuf) == SUCC);
    }

    /* Search for the strings matching the designated prefix. */
    for (iIdx = 0 ; iIdx < iLen - 1 ; ++iIdx) {
        memset(szBuf, 0, sizeof(char) * 3);
        strncpy(szBuf, szSeq + iIdx, 2);
        CU_ASSERT(pTrie->has_prefix_as(pTrie, szBuf) == SUCC);
    }
    for (iIdx = 0 ; iIdx < iLen - 2 ; ++iIdx) {
        memset(szBuf, 0, sizeof(char) * 4);
        strncpy(szBuf, szSeq + iIdx, 3);
        CU_ASSERT(pTrie->has_prefix_as(pTrie, szBuf) == SUCC);
    }
    for (iIdx = 0 ; iIdx < iLen - 3 ; ++iIdx) {
        memset(szBuf, 0, sizeof(char) * 5);
        strncpy(szBuf, szSeq + iIdx, 4);
        CU_ASSERT(pTrie->has_prefix_as(pTrie, szBuf) == NOKEY);
    }

    CU_ASSERT(pTrie->has_prefix_as(pTrie, NULL) == NOKEY);
    CU_ASSERT(pTrie->has_prefix_as(pTrie, "\0") == NOKEY);
    CU_ASSERT(pTrie->has_prefix_as(pTrie, "123\0") == NOKEY);

    TrieDeinit(&pTrie);
}

void TestDeleteThenVerify()
{
    Trie *pTrie;
    CU_ASSERT(TrieInit(&pTrie) == SUCC);

    char *szSeq = "nopqrstuvwxyzyxwvutsrqponmlkjihgfedcbabcdefghijklmn\0";
    int32_t iLen = strlen(szSeq);
    char szBuf[iLen + 1];

    int32_t iIdx;
    for (iIdx = 0 ; iIdx < iLen - 2 ; ++iIdx) {
        memset(szBuf, 0, sizeof(char) * 4);
        strncpy(szBuf, szSeq + iIdx, 3);
        CU_ASSERT(pTrie->insert(pTrie, szBuf) == SUCC);
    }

    for (iIdx = 0 ; iIdx < iLen - 1 ; ++iIdx) {
        memset(szBuf, 0, sizeof(char) * 3);
        strncpy(szBuf, szSeq + iIdx, 2);
        CU_ASSERT(pTrie->insert(pTrie, szBuf) == SUCC);
    }

    /* Delete the strings with 3 bytes. */
    for (iIdx = 0 ; iIdx < iLen - 2 ; ++iIdx) {
        memset(szBuf, 0, sizeof(char) * 4);
        strncpy(szBuf, szSeq + iIdx, 3);
        CU_ASSERT(pTrie->delete(pTrie, szBuf) == SUCC);
        CU_ASSERT(pTrie->delete(pTrie, szBuf) == NOKEY);
    }

    CU_ASSERT(pTrie->delete(pTrie, NULL) == NOKEY);
    CU_ASSERT(pTrie->delete(pTrie, "\0") == NOKEY);
    CU_ASSERT_EQUAL(pTrie->size(pTrie), iLen - 1);

    /* Search for the existing strings. */
    for (iIdx = 0 ; iIdx < iLen - 1 ; ++iIdx) {
        memset(szBuf, 0, sizeof(char) * 3);
        strncpy(szBuf, szSeq + iIdx, 2);
        CU_ASSERT(pTrie->has_exact(pTrie, szBuf) == SUCC);
    }
    for (iIdx = 0 ; iIdx < iLen - 2 ; ++iIdx) {
        memset(szBuf, 0, sizeof(char) * 4);
        strncpy(szBuf, szSeq + iIdx, 3);
        CU_ASSERT(pTrie->has_exact(pTrie, szBuf) == NOKEY);
    }

    /* Search for the strings matching the designated prefix. */
    for (iIdx = 0 ; iIdx < iLen - 1 ; ++iIdx) {
        memset(szBuf, 0, sizeof(char) * 3);
        strncpy(szBuf, szSeq + iIdx, 2);
        CU_ASSERT(pTrie->has_prefix_as(pTrie, szBuf) == SUCC);
    }
    for (iIdx = 0 ; iIdx < iLen - 2 ; ++iIdx) {
        memset(szBuf, 0, sizeof(char) * 4);
        strncpy(szBuf, szSeq + iIdx, 3);
        CU_ASSERT(pTrie->has_prefix_as(pTrie, szBuf) == NOKEY);
    }

    /* Delete the strings with 2 bytes. */
    for (iIdx = 0 ; iIdx < iLen - 1 ; ++iIdx) {
        memset(szBuf, 0, sizeof(char) * 3);
        strncpy(szBuf, szSeq + iIdx, 2);
        CU_ASSERT(pTrie->delete(pTrie, szBuf) == SUCC);
    }
    CU_ASSERT_EQUAL(pTrie->size(pTrie), 0);

    for (iIdx = 0 ; iIdx < iLen ; ++iIdx) {
        szBuf[0] = szSeq[iIdx]; szBuf[1] = 0;
        CU_ASSERT(pTrie->has_prefix_as(pTrie, szBuf) == NOKEY);
    }

    TrieDeinit(&pTrie);
}

void TestGetPrefix()
{
    Trie *pTrie;
    CU_ASSERT(TrieInit(&pTrie) == SUCC);

    /* Pass invalid parameters for the returned data. */
    char **aStr;
    int32_t iSizeArr;
    CU_ASSERT(pTrie->get_prefix_as(pTrie, NULL, NULL, &iSizeArr) == ERR_GET);
    CU_ASSERT(pTrie->get_prefix_as(pTrie, NULL, &aStr, NULL) == ERR_GET);

    /* Pass dummy prefix. */
    CU_ASSERT(pTrie->get_prefix_as(pTrie, NULL, &aStr, &iSizeArr) == NOKEY);
    CU_ASSERT(pTrie->get_prefix_as(pTrie, "\0", &aStr, &iSizeArr) == NOKEY);


    TrieDeinit(&pTrie);
}
