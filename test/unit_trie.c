#include "container/trie.h"
#include "CUnit/Util.h"
#include "CUnit/Basic.h"


static const int SIZE_TXT_BUFF = 32;
static const int SIZE_TNY_TEST = 128;
static const int SIZE_SML_TEST = 512;


/*-----------------------------------------------------------------------------*
 *            Unit tests relevant to basic structure verification              *
 *-----------------------------------------------------------------------------*/
void TestNewDelete()
{
    /* Generate the random strings. */
    srand(time(NULL));

    char* strs[SIZE_SML_TEST];
    int i, j;
    for (i = 0 ; i < SIZE_SML_TEST ; ++i) {
        strs[i] = (char*)malloc(sizeof(char) * SIZE_TXT_BUFF);
        for (j = 0 ; j < SIZE_TXT_BUFF - 1 ; ++j) {
            char ch = 'a' + (random() % 26);
            strs[i][j] = ch;
        }
        strs[i][SIZE_TXT_BUFF - 1] = 0;
    }

    Trie* trie;
    CU_ASSERT((trie = TrieInit()) != NULL);

    /* Enlarge the trie size to test the destructor. */
    for (i = 0 ; i < SIZE_SML_TEST ; ++i)
        CU_ASSERT(trie->insert(trie, strs[i]) == true);

    TrieDeinit(trie);

    for (i = 0 ; i < SIZE_SML_TEST ; ++i)
        free((char*)strs[i]);
}

void TestInsert()
{
    {
        Trie* trie = TrieInit();

        const char* prefix = "abcdefghijklmnopqrstuvwxyz\0";
        int len = strlen(prefix);
        char buf[len + 1];

        /* Insert the dummy strings. */
        CU_ASSERT(trie->insert(trie, NULL) == true);
        CU_ASSERT(trie->insert(trie, "\0") == true);

        /* Insert the prefixes and verify the number of stored strings. */
        int i, j;
        for (i = 0 ; i < len ; ++i) {
            for (j = i ; j < len ; ++j) {
                int sz = j - i + 2;
                strncpy(buf, prefix + i, sz - 1);
                buf[sz - 1] = 0;
                CU_ASSERT(trie->insert(trie, buf) == true);
            }
        }
        CU_ASSERT_EQUAL(trie->size(trie), ((len * (len + 1)) >> 1));

        TrieDeinit(trie);
    }
    {
        Trie* trie = TrieInit();

        const char* suffix = "abcdefghijklmnopqrstuvwxyz\0";
        int len = strlen(suffix);
        char buf[len + 1];

        /* Insert the suffixes and verify the number of stored strings. */
        int i, j;
        for (i = len - 1 ; i >= 0 ; --i) {
            for (j = i ; j >= 0 ; --j) {
                int sz = i - j + 2;
                strncpy(buf, suffix + j, sz - 1);
                buf[sz - 1] = 0;
                CU_ASSERT(trie->insert(trie, buf) == true);
            }
        }
        CU_ASSERT_EQUAL(trie->size(trie), ((len * (len + 1)) >> 1));

        TrieDeinit(trie);
    }
}

void TestSearchExact()
{
    Trie* trie = TrieInit();

    const char* seq = "nopqrstuvwxyzzyxwvutsrqponmlkjihgfedcba\0";
    int len = strlen(seq);
    char buf[len + 1];

    int i;
    for (i = 0 ; i < len - 2 ; ++i) {
        strncpy(buf, seq + i, 3);
        buf[3] = 0;
        CU_ASSERT(trie->insert(trie, buf) == true);
    }

    /* Search for the existing strings. */
    for (i = 0 ; i < len - 2 ; ++i) {
        strncpy(buf, seq + i, 3);
        buf[3] = 0;
        CU_ASSERT(trie->has_exact(trie, buf) == true);
    }

    /* Search for the non-existing strings */
    for (i = 0 ; i < len - 1 ; ++i) {
        strncpy(buf, seq + i, 2);
        buf[2] = 0;
        CU_ASSERT(trie->has_exact(trie, buf) == false);
    }
    for (i = 0 ; i < len - 3 ; ++i) {
        strncpy(buf, seq + i, 4);
        buf[4] = 0;
        CU_ASSERT(trie->has_exact(trie, buf) == false);
    }

    CU_ASSERT(trie->has_exact(trie, NULL) == false);
    CU_ASSERT(trie->has_exact(trie, "\0") == false);
    CU_ASSERT(trie->has_exact(trie, "123\0") == false);

    TrieDeinit(trie);
}

void TestSearchPrefix()
{
    Trie* trie = TrieInit();

    char buf[4];
    char ch_i, ch_j, ch_k;
    for (ch_i = 'a' ; ch_i <= 'z' ; ++ch_i) {
        for (ch_j = 'a' ; ch_j <= 'z' ; ++ch_j) {
            for (ch_k = 'a' ; ch_k <= 'z' ; ++ch_k) {
                buf[0] = ch_i;
                buf[1] = ch_j;
                buf[2] = ch_k;
                buf[3] = 0;
                trie->insert(trie, buf);
            }
        }
    }

    /* Search for the dummy strings. */
    CU_ASSERT(trie->has_prefix_as(trie, NULL) == false);
    CU_ASSERT(trie->has_prefix_as(trie, "\0") == false);

    /* Search for the strings matching the specified prefix. */
    for (ch_i = 'a' ; ch_i <= 'z' ; ++ch_i) {
        buf[0] = ch_i;
        buf[1] = 0;
        CU_ASSERT(trie->has_prefix_as(trie, buf) == true);
    }

    for (ch_i = 'a' ; ch_i <= 'z' ; ++ch_i) {
        for (ch_j = 'a' ; ch_j <= 'z' ; ++ch_j) {
            buf[0] = ch_i;
            buf[1] = ch_j;
            buf[2] = 0;
            CU_ASSERT(trie->has_prefix_as(trie, buf) == true);
        }
    }

    for (ch_i = 'a' ; ch_i <= 'z' ; ++ch_i) {
        for (ch_j = 'a' ; ch_j <= 'z' ; ++ch_j) {
            buf[0] = ch_i;
            buf[1] = ch_j;
            buf[2] = 'a';
            buf[3] = 0;
            CU_ASSERT(trie->has_prefix_as(trie, buf) == true);
        }
    }

    for (ch_i = 'a' ; ch_i <= 'z' ; ++ch_i) {
        for (ch_j = 'a' ; ch_j <= 'z' ; ++ch_j) {
            buf[0] = ch_i;
            buf[1] = ch_j;
            buf[2] = '0';
            buf[3] = 0;
            CU_ASSERT(trie->has_prefix_as(trie, buf) == false);
        }
    }

    for (ch_i = 'a' ; ch_i <= 'z' ; ++ch_i) {
        for (ch_j = '0' ; ch_j <= '9' ; ++ch_j) {
            buf[0] = ch_i;
            buf[1] = ch_j;
            buf[2] = 0;
            CU_ASSERT(trie->has_prefix_as(trie, buf) == false);
        }
    }

    for (ch_i = '0' ; ch_i <= '9' ; ++ch_i) {
        buf[0] = ch_i;
        buf[1] = 0;
        CU_ASSERT(trie->has_prefix_as(trie, buf) == false);
    }

    TrieDeinit(trie);
}

void TestBulkInsert()
{
    Trie* trie = TrieInit();

    const char *seq = "abcdefghijklmnopqrstuvwxyzzyxwvutsrqponmlkjihgfedcba"
                      "ABCDEFGHIJKLMNOPQRSTUVWXYZZYXWVUTSRQPONMLKJIHGFEDCBA\0";
    int len = strlen(seq);
    int cnt_str = len * (len + 1);

    char** strs = (char**)malloc(sizeof(char*) * ((cnt_str >> 1) + 2));
    int idx = 0;

    int i, j;
    for (i = 0 ; i < len ; ++i) {
        for (j = i ; j < len ; ++j) {
            int sz = j - i + 2;
            char* str = (char*)malloc(sizeof(char) * sz);
            strncpy(str, seq + i, sz - 1);
            str[sz - 1] = 0;
            strs[idx++] = str;
        }
    }

    /* Apply bulk_insert() to put the first half of the data. */
    CU_ASSERT(trie->bulk_insert(trie, (const char**)strs, idx >> 1) == true);
    CU_ASSERT_EQUAL(trie->size(trie), (cnt_str >> 2) - 5);

    strs[idx] = NULL;
    strs[idx + 1] = "\0";
    CU_ASSERT(trie->bulk_insert(trie, (const char**)strs + idx, 2) == true);
    CU_ASSERT_EQUAL(trie->size(trie), (cnt_str >> 2) - 5);

    /* Apply insert() to put the second half of the data. */
    for (i = idx >> 1 ; i < idx ; ++i)
        CU_ASSERT(trie->insert(trie, strs[i]) == true);
    CU_ASSERT_EQUAL(trie->size(trie), (cnt_str >> 1) - (len >> 1));

    /* Query the string existence. */
    for (i = 0 ; i < idx ; ++i)
        CU_ASSERT(trie->has_exact(trie, strs[i]) == true);

    /* Query the prefix existence. */
    char buf[SIZE_TXT_BUFF];
    char ch_i;
    for (ch_i = 'a' ; ch_i <= 'z' ; ++ch_i) {
        buf[0] = ch_i;
        buf[1] = 0;
        CU_ASSERT(trie->has_prefix_as(trie, buf) == true);
    }
    for (ch_i = 'A' ; ch_i <= 'Z' ; ++ch_i) {
        buf[0] = ch_i;
        buf[1] = 0;
        CU_ASSERT(trie->has_prefix_as(trie, buf) == true);
    }

    TrieDeinit(trie);

    while (idx > 0)
        free((char*)strs[--idx]);
    free((char*)strs);
}

void TestRemoveAndVerify()
{
    Trie* trie = TrieInit();

    char buf[4];
    char ch_i, ch_j, ch_k;
    for (ch_i = 'a' ; ch_i <= 'z' ; ++ch_i) {
        for (ch_j = 'a' ; ch_j <= 'z' ; ++ch_j) {
            for (ch_k = 'a' ; ch_k <= 'z' ; ++ch_k) {
                buf[0] = ch_i;
                buf[1] = ch_j;
                buf[2] = ch_k;
                buf[3] = 0;
                trie->insert(trie, buf);
            }
        }
    }

    /* Remove the dummy strings. */
    CU_ASSERT(trie->remove(trie, NULL) == false);
    CU_ASSERT(trie->remove(trie, "\0") == false);

    /* Remove the unexisting strings. */
    for (ch_i = 'a' ; ch_i <= 'z' ; ++ch_i) {
        buf[0] = ch_i;
        buf[1] = 0;
        CU_ASSERT(trie->remove(trie, buf) == false);
    }

    /* Remove the existing strings. */
    for (ch_i = 'a' ; ch_i <= 'z' ; ++ch_i) {
        for (ch_j = 'a' ; ch_j <= 'y' ; ++ch_j) {
            for (ch_k = 'a' ; ch_k <= 'z' ; ++ch_k) {
                buf[0] = ch_i;
                buf[1] = ch_j;
                buf[2] = ch_k;
                buf[3] = 0;
                CU_ASSERT(trie->remove(trie, buf) == true);
                CU_ASSERT(trie->remove(trie, buf) == false);
            }
        }
    }

    for (ch_i = 'a' ; ch_i <= 'z' ; ++ch_i) {
        buf[0] = ch_i;
        buf[1] = 0;
        CU_ASSERT(trie->has_prefix_as(trie, buf) == true);
    }

    /* Remove all the remaining strings. */
    for (ch_i = 'a' ; ch_i <= 'z' ; ++ch_i) {
        for (ch_j = 'a' ; ch_j <= 'z' ; ++ch_j) {
            buf[0] = ch_i;
            buf[1] = 'z';
            buf[2] = ch_j;
            buf[3] = 0;
            CU_ASSERT(trie->remove(trie, buf) == true);
        }
    }

    for (ch_i = 'a' ; ch_i <= 'z' ; ++ch_i) {
        buf[0] = ch_i;
        buf[1] = 0;
        CU_ASSERT(trie->has_prefix_as(trie, buf) == false);
    }

    TrieDeinit(trie);
}

void TestGetPrefix()
{
    Trie *trie = TrieInit();
    const char** strs;
    unsigned size;

    /* Pass dummy prefix. */
    CU_ASSERT(trie->get_prefix_as(trie, NULL, &strs, &size) == false);
    CU_ASSERT(trie->get_prefix_as(trie, "\0", &strs, &size) == false);

    /* Pass non-existing prefix. */
    CU_ASSERT(trie->get_prefix_as(trie, "012\0", &strs, &size) == false);

    char *seq = "nopqrstuvwxyzyxwvutsrqponmlkjihgfedcbabcdefghijklmno\0";
    int len = strlen(seq);
    char buf[len + 1];

    int i;
    for (i = 0 ; i < len - 2 ; ++i) {
        strncpy(buf, seq + i, 3);
        buf[3] = 0;
        CU_ASSERT(trie->insert(trie, buf) == true);
    }
    for (i = 0 ; i < len - 1 ; ++i) {
        strncpy(buf, seq + i, 2);
        buf[2] = 0;
        CU_ASSERT(trie->insert(trie, buf) == true);
    }

    /* Get the strings matching the specified prefixes. */
    for (i = 0 ; i < len - 3 ; ++i) {
        strncpy(buf, seq + i, 3);
        buf[3] = 0;
        CU_ASSERT(trie->get_prefix_as(trie, buf, &strs, &size) == true);

        CU_ASSERT_EQUAL(size, 1);
        CU_ASSERT(strcmp(strs[0], buf) == 0);

        free((char*)strs[0]);
        free((char*)strs);
    }
    for (i = 0 ; i < len - 2 ; ++i) {
        strncpy(buf, seq + i, 2);
        buf[2] = 0;
        CU_ASSERT(trie->get_prefix_as(trie, buf, &strs, &size) == true);

        CU_ASSERT_EQUAL(size, 2);
        CU_ASSERT(strcmp(strs[0], buf) == 0);

        buf[2] = seq[i + 2];
        buf[3] = 0;
        CU_ASSERT(strcmp(strs[1], buf) == 0);

        free((char*)strs[0]);
        free((char*)strs[1]);
        free((char*)strs);
    }

    /* Delete the strings with 2 bytes. */
    for (i = 0 ; i < len - 2 ; ++i) {
        strncpy(buf, seq + i, 2);
        buf[2] = 0;
        CU_ASSERT(trie->remove(trie, buf) == true);
    }

    /* It should be OK to get the strings matching the specified prefixes. */
    for (i = 0 ; i < len ; ++i) {
        buf[0] = seq[i];
        buf[1] = 0;
        CU_ASSERT(trie->get_prefix_as(trie, buf, &strs, &size) == true);

        switch (buf[0]) {
            case 'a':
                CU_ASSERT_EQUAL(size, 1);
                buf[1] = buf[0] + 1;
                buf[2] = buf[0] + 2;
                buf[3] = 0;
                CU_ASSERT(strcmp(strs[0], buf) == 0);
                free((char*)strs[0]);
                break;

            case 'b':
                CU_ASSERT_EQUAL(size, 2);
                buf[1] = buf[0] - 1;
                buf[2] = buf[0];
                buf[3] = 0;
                CU_ASSERT(strcmp(strs[0], buf) == 0);
                free((char*)strs[0]);
                buf[1] = buf[0] + 1;
                buf[2] = buf[0] + 2;
                buf[3] = 0;
                CU_ASSERT(strcmp(strs[1], buf) == 0);
                free((char*)strs[1]);
                break;

            case 'y':
                CU_ASSERT_EQUAL(size, 2);
                buf[1] = buf[0] - 1;
                buf[2] = buf[0] - 2;
                buf[3] = 0;
                CU_ASSERT(strcmp(strs[0], buf) == 0);
                free((char*)strs[0]);
                buf[1] = buf[0] + 1;
                buf[2] = buf[0];
                buf[3] = 0;
                CU_ASSERT(strcmp(strs[1], buf) == 0);
                free((char*)strs[1]);
                break;

            case 'z':
                CU_ASSERT_EQUAL(size, 1);
                buf[1] = buf[0] - 1;
                buf[2] = buf[0] - 2;
                buf[3] = 0;
                CU_ASSERT(strcmp(strs[0], buf) == 0);
                free((char*)strs[0]);
                break;

            default:
                CU_ASSERT_EQUAL(size, 2);
                buf[1] = buf[0] - 1;
                buf[2] = buf[0] - 2;
                buf[3] = 0;
                CU_ASSERT(strcmp(strs[0], buf) == 0);
                free((char*)strs[0]);
                buf[1] = buf[0] + 1;
                buf[2] = buf[0] + 2;
                buf[3] = 0;
                CU_ASSERT(strcmp(strs[1], buf) == 0);
                free((char*)strs[1]);
        }
        free((char*)strs);
    }

    /* Delete the strings with 3 bytes. */
    for (i = 0 ; i < len - 2 ; ++i) {
        strncpy(buf, seq + i, 3);
        buf[3] = 0;
        CU_ASSERT(trie->remove(trie, buf) == true);
    }

    /* Then, all the get_prefix_as() should return false. */
    for (i = 0 ; i < len ; ++i) {
        buf[0] = seq[i];
        buf[1];
        CU_ASSERT(trie->get_prefix_as(trie, buf, &strs, &size) == false);
        CU_ASSERT_EQUAL(strs, NULL);
        CU_ASSERT_EQUAL(size, 0);
    }
    for (i = 0 ; i < len - 1 ; ++i) {
        strncpy(buf, seq + i, 2);
        buf[2] = 0;
        CU_ASSERT(trie->get_prefix_as(trie, buf, &strs, &size) == false);
        CU_ASSERT_EQUAL(strs, NULL);
        CU_ASSERT_EQUAL(size, 0);
    }
    for (i = 0 ; i < len - 2 ; ++i) {
        strncpy(buf, seq + i, 3);
        buf[3] = 0;
        CU_ASSERT(trie->get_prefix_as(trie, buf, &strs, &size) == false);
        CU_ASSERT_EQUAL(strs, NULL);
        CU_ASSERT_EQUAL(size, 0);
    }

    TrieDeinit(trie);
}

/*-----------------------------------------------------------------------------*
 *                       The driver for Trie unit test                         *
 *-----------------------------------------------------------------------------*/
bool AddSuite()
{
    CU_pSuite suite = CU_add_suite("Structure Verification", NULL, NULL);
    if (!suite)
        return false;

    CU_pTest unit = CU_add_test(suite, "Trie New and Delete", TestNewDelete);
    if (!unit)
        return false;

    unit = CU_add_test(suite, "String Insert", TestInsert);
    if (!unit)
        return false;

    unit = CU_add_test(suite, "String Search Exact Match", TestSearchExact);
    if (!unit)
        return false;

    unit = CU_add_test(suite, "String Search Prefix Match", TestSearchPrefix);
    if (!unit)
        return false;

    unit = CU_add_test(suite, "String Bulk Insert", TestBulkInsert);
    if (!unit)
        return false;

    unit = CU_add_test(suite, "String Remove and Search Verification", TestRemoveAndVerify);
    if (!unit)
        return false;

    unit = CU_add_test(suite, "String Retrieve Prefix As", TestGetPrefix);
    if (!unit)
        return false;

    return true;
}

int main()
{
    int rc = 0;

    if (CU_initialize_registry() != CUE_SUCCESS) {
        rc = CU_get_error();
        goto EXIT;
    }

    /* Register the test suites to verify Trie functionalities. */
    if (AddSuite() == false) {
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
