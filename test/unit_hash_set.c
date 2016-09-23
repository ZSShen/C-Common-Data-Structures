#include "container/hash_set.h"
#include "CUnit/Util.h"
#include "CUnit/Basic.h"


static const int SIZE_TNY_TEST = 128;
static const int SIZE_SML_TEST = 512;
static const int SIZE_MID_TEST = 1024;


/*-----------------------------------------------------------------------------*
 * The utilities for hash value generation, key comparison, and resource clean *
 *-----------------------------------------------------------------------------*/
/**
 * The famous djb2 string hash function directly pulled from:
 * http://www.cse.yorku.ca/~oz/hash.html
 */
unsigned HashKey(void* key)
{
    char* str = (char*)key;
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

int CompareKey(void* lhs, void* rhs)
{
    return strcmp((char*)lhs, (char*)rhs);
}

void CleanKey(void* key)
{
    free(key);
}


/*-----------------------------------------------------------------------------*
 *            Unit tests relevant to basic structure verification              *
 *-----------------------------------------------------------------------------*/
void TestNewDelete()
{
    HashSet* set;
    CU_ASSERT((set = HashSetInit()) != NULL);

    /* Enlarge the set size to test the destructor. */
    int i;
    for (i = 0 ; i < SIZE_SML_TEST ; ++i)
        CU_ASSERT(set->add(set, (void*)(intptr_t)i) == true);

    HashSetDeinit(set);
}

void TestPutGetNum()
{
    HashSet* set = HashSetInit();

    int i;
    for (i = 0 ; i < SIZE_TNY_TEST ; ++i)
        CU_ASSERT(set->add(set, (void*)(intptr_t)i) == true);
    for (i = 0 ; i < SIZE_TNY_TEST ; ++i)
        CU_ASSERT(set->find(set, (void*)(intptr_t)i) == true);

    HashSetDeinit(set);
}

void TestRemoveNum()
{
    HashSet* set = HashSetInit();

    int i;
    for (i = 0 ; i < SIZE_TNY_TEST ; ++i)
        set->add(set, (void*)(intptr_t)i);

    /* Remove the first half of the keys. */
    for (i = 0 ; i < SIZE_TNY_TEST >> 1 ; ++i)
        CU_ASSERT(set->remove(set, (void*)(intptr_t)i) == true);

    /* Querying for the keys that are already removed should fail. */
    for (i = 0 ; i < SIZE_TNY_TEST >> 1 ; ++i) {
        CU_ASSERT(set->remove(set, (void*)(intptr_t)i) == false);
        CU_ASSERT(set->find(set, (void*)(intptr_t)i) == false);
    }

    /* Querying for the keys that still exist should success. */
    for (i = SIZE_TNY_TEST >> 1 ; i < SIZE_TNY_TEST ; ++i)
        CU_ASSERT(set->find(set, (void*)(intptr_t)i) == true);

    CU_ASSERT_EQUAL(set->size(set), SIZE_TNY_TEST >> 1);

    HashSetDeinit(set);
}

void TestIterateNum()
{
    HashSet* set = HashSetInit();

    int i;
    for (i = 1 ; i < SIZE_TNY_TEST ; ++i)
        set->add(set, (void*)(intptr_t)i);

    /* Since the data size won't trigger re-hashing, we can predict the
       data distribution. */
    i = 1;
    void* key;
    set->first(set);
    while ((key = set->next(set)) != NULL) {
        CU_ASSERT_EQUAL(i, (int)(intptr_t)key);
        ++i;
    }
    CU_ASSERT(set->next(set) == NULL);

    /* The previous iteration should not change the structure layout. */
    i = 1;
    set->first(set);
    while ((key = set->next(set)) != NULL) {
        CU_ASSERT_EQUAL(i, (int)(intptr_t)key);
        ++i;
    }

    HashSetDeinit(set);
}


/*-----------------------------------------------------------------------------*
 *              Unit tests relevant to complex data maintenance                *
 *-----------------------------------------------------------------------------*/
void TestPutGetTxt()
{
    char buf[SIZE_TNY_TEST];
    char* keys[SIZE_TNY_TEST];
    HashSet* set = HashSetInit();
    set->set_hash(set, HashKey);
    set->set_compare(set, CompareKey);

    int i;
    for (i = 0 ; i < SIZE_TNY_TEST ; ++i) {
        snprintf(buf, SIZE_TNY_TEST, "key -> %d", i);
        keys[i] = strdup(buf);
        CU_ASSERT(set->add(set, (void*)keys[i]) == true);
    }
    for (i = 0 ; i < SIZE_TNY_TEST ; ++i) {
        CU_ASSERT(set->find(set, (void*)keys[i]) == true);
        free(keys[i]);
    }

    HashSetDeinit(set);
}

void TestRemoveTxt()
{
    char buf[SIZE_TNY_TEST];
    char* keys[SIZE_TNY_TEST];
    HashSet* set = HashSetInit();
    set->set_hash(set, HashKey);
    set->set_compare(set, CompareKey);
    set->set_clean_key(set, CleanKey);

    int i;
    for (i = 0 ; i < SIZE_TNY_TEST ; ++i) {
        snprintf(buf, SIZE_TNY_TEST, "key -> %d", i);
        keys[i] = strdup(buf);
        set->add(set, (void*)keys[i]);
    }

    /* Remove the first half of the keys. */
    for (i = 0 ; i < SIZE_TNY_TEST >> 1 ; ++i)
        CU_ASSERT(set->remove(set, (void*)keys[i]) == true);

    /* Querying for the keys that are already removed should fail. */
    for (i = 0 ; i < SIZE_TNY_TEST >> 1 ; ++i) {
        snprintf(buf, SIZE_TNY_TEST, "key -> %d", i);
        CU_ASSERT(set->remove(set, (void*)buf) == false);
        CU_ASSERT(set->find(set, (void*)buf) == false);
    }

    /* Querying for the keys that still exist should success. */
    for (i = SIZE_TNY_TEST >> 1 ; i < SIZE_TNY_TEST ; ++i)
        CU_ASSERT(set->find(set, (void*)keys[i]) == true);

    HashSetDeinit(set);
}

void TestPutDupTxt()
{
    char buf[SIZE_TNY_TEST];
    char* keys[SIZE_TNY_TEST];
    HashSet* set = HashSetInit();
    set->set_hash(set, HashKey);
    set->set_compare(set, CompareKey);
    set->set_clean_key(set, CleanKey);

    int i;
    for (i = 0 ; i < SIZE_TNY_TEST ; ++i) {
        snprintf(buf, SIZE_TNY_TEST, "key -> %d", i);
        keys[i] = strdup(buf);
        set->add(set, (void*)keys[i]);
    }

    /* Insert the new set of keys with the same string content. */
    for (i = 0 ; i < SIZE_TNY_TEST ; ++i) {
        snprintf(buf, SIZE_TNY_TEST, "key -> %d", i);
        char* dup_key = strdup(buf);
        CU_ASSERT(set->add(set, (void*)dup_key) == true);
    }

    /* Now the test for the existing keys should be fine. */
    for (i = 0 ; i < SIZE_TNY_TEST ; ++i) {
        snprintf(buf, SIZE_TNY_TEST, "key -> %d", i);
        CU_ASSERT(set->find(set, (void*)buf) == true);
    }

    HashSetDeinit(set);
}

void TestBulkTxt()
{
    char buf[SIZE_TNY_TEST];
    char* keys[SIZE_MID_TEST];
    HashSet* set = HashSetInit();
    set->set_hash(set, HashKey);
    set->set_compare(set, CompareKey);
    set->set_clean_key(set, CleanKey);

    int i;
    for (i = 0 ; i < SIZE_MID_TEST ; ++i) {
        snprintf(buf, SIZE_MID_TEST, "key -> %d", i);
        keys[i] = strdup(buf);
        set->add(set, (void*)keys[i]);
    }

    /* Remove the first half of the keys. */
    for (i = 0 ; i < SIZE_MID_TEST >> 1 ; ++i)
        CU_ASSERT(set->remove(set, (void*)keys[i]) == true);

    /* Querying for the keys that are already removed should fail. */
    for (i = 0 ; i < SIZE_MID_TEST >> 1 ; ++i) {
        snprintf(buf, SIZE_MID_TEST, "key -> %d", i);
        CU_ASSERT(set->remove(set, (void*)buf) == false);
        CU_ASSERT(set->find(set, (void*)buf) == false);
    }

    /* Querying for the keys that still exist should success. */
    for (i = SIZE_MID_TEST >> 1 ; i < SIZE_MID_TEST ; ++i)
        CU_ASSERT(set->find(set, (void*)keys[i]) == true);

    HashSetDeinit(set);
}


/*-----------------------------------------------------------------------------*
 *             Unit tests relevant to set arithmetic operations                *
 *-----------------------------------------------------------------------------*/
void TestUnionOperation()
{
    char buf[SIZE_TNY_TEST];
    char* keys[SIZE_SML_TEST];
    int i;
    for (i = 0 ; i < SIZE_SML_TEST ; ++i) {
        snprintf(buf, SIZE_SML_TEST, "key -> %d", i);
        keys[i] = strdup(buf);
    }

    /* Prepare the first source set. */
    HashSet* set_lhs = HashSetInit();
    set_lhs->set_hash(set_lhs, HashKey);
    set_lhs->set_compare(set_lhs, CompareKey);
    set_lhs->set_clean_key(set_lhs, CleanKey);
    for (i = 0 ; i < SIZE_SML_TEST >> 1 ; ++i)
        set_lhs->add(set_lhs, keys[i]);

    /* Prepare the second source set. */
    HashSet* set_rhs = HashSetInit();
    set_rhs->set_hash(set_rhs, HashKey);
    set_rhs->set_compare(set_rhs, CompareKey);
    set_rhs->set_clean_key(set_rhs, CleanKey);
    for (i = SIZE_SML_TEST - 1 ; i >= SIZE_SML_TEST >> 1 ; --i)
        set_rhs->add(set_rhs, keys[i]);

    HashSet* result = HashSetUnion(set_lhs, set_rhs);
    CU_ASSERT(result != NULL);

    for (i = 0 ; i < SIZE_SML_TEST ; ++i)
        CU_ASSERT(result->find(result, keys[i]) == true);
    CU_ASSERT_EQUAL(result->size(result), SIZE_SML_TEST);

    HashSetDeinit(result);
    HashSetDeinit(set_lhs);
    HashSetDeinit(set_rhs);
}

void TestIntersectOperation()
{
    {
        char buf[SIZE_TNY_TEST];
        char* keys[SIZE_SML_TEST];
        int i;
        for (i = 0 ; i < SIZE_SML_TEST ; ++i) {
            snprintf(buf, SIZE_SML_TEST, "key -> %d", i);
            keys[i] = strdup(buf);
        }

        /* Prepare the first source set. */
        HashSet* set_lhs = HashSetInit();
        set_lhs->set_hash(set_lhs, HashKey);
        set_lhs->set_compare(set_lhs, CompareKey);
        set_lhs->set_clean_key(set_lhs, CleanKey);
        int bgn = 0;
        int end = SIZE_SML_TEST * 3 >> 2;
        for (i = 0 ; i < end ; ++i)
            set_lhs->add(set_lhs, strdup(keys[i]));

        /* Prepare the second source set. */
        HashSet* set_rhs = HashSetInit();
        set_rhs->set_hash(set_rhs, HashKey);
        set_rhs->set_compare(set_rhs, CompareKey);
        set_rhs->set_clean_key(set_rhs, CleanKey);
        bgn = SIZE_SML_TEST >> 2;
        end = SIZE_SML_TEST - 1;
        for (i = end ; i >= bgn ; --i)
            set_rhs->add(set_rhs, strdup(keys[i]));

        HashSet* result = HashSetIntersect(set_lhs, set_rhs);
        CU_ASSERT(result != NULL);

        /* Intersected part: 1/4 to 3/4 */
        bgn = 0;
        end = SIZE_SML_TEST >> 2;
        for (i = bgn ; i < end ; ++i)
            CU_ASSERT(result->find(result, keys[i]) == false);
        bgn = SIZE_SML_TEST >> 2;
        end = SIZE_SML_TEST * 3 >> 2;
        for (i = bgn ; i < end ; ++i)
            CU_ASSERT(result->find(result, keys[i]) == true);
        bgn = SIZE_SML_TEST * 3 >> 2;
        end = SIZE_SML_TEST;
        for (i = bgn ; i < end ; ++i)
            CU_ASSERT(result->find(result, keys[i]) == false);

        CU_ASSERT_EQUAL(result->size(result), SIZE_SML_TEST >> 1);
        HashSetDeinit(result);
        HashSetDeinit(set_lhs);
        HashSetDeinit(set_rhs);
        for (i = 0 ; i < SIZE_SML_TEST ; ++i)
            free(keys[i]);
    }
    {
        char buf[SIZE_TNY_TEST];
        char* keys[SIZE_SML_TEST];
        int i;
        for (i = 0 ; i < SIZE_SML_TEST ; ++i) {
            snprintf(buf, SIZE_SML_TEST, "key -> %d", i);
            keys[i] = strdup(buf);
        }

        /* Prepare the first source set. */
        HashSet* set_lhs = HashSetInit();
        set_lhs->set_hash(set_lhs, HashKey);
        set_lhs->set_compare(set_lhs, CompareKey);
        set_lhs->set_clean_key(set_lhs, CleanKey);
        int bgn = 0;
        int end = SIZE_SML_TEST >> 1;
        for (i = 0 ; i < end ; ++i)
            set_lhs->add(set_lhs, strdup(keys[i]));

        /* Prepare the second source set. */
        HashSet* set_rhs = HashSetInit();
        set_rhs->set_hash(set_rhs, HashKey);
        set_rhs->set_compare(set_rhs, CompareKey);
        set_rhs->set_clean_key(set_rhs, CleanKey);
        bgn = SIZE_SML_TEST >> 2;
        end = SIZE_SML_TEST - 1;
        for (i = end ; i >= bgn ; --i)
            set_rhs->add(set_rhs, strdup(keys[i]));

        HashSet* result = HashSetIntersect(set_lhs, set_rhs);
        CU_ASSERT(result != NULL);

        /* Interseted part: 1/4 to 4/2 */
        bgn = 0;
        end = SIZE_SML_TEST >> 2;
        for (i = bgn ; i < end ; ++i)
            CU_ASSERT(result->find(result, keys[i]) == false);
        bgn = SIZE_SML_TEST >> 2;
        end = SIZE_SML_TEST >> 1;
        for (i = bgn ; i < end ; ++i)
            CU_ASSERT(result->find(result, keys[i]) == true);
        bgn = SIZE_SML_TEST >> 1;
        end = SIZE_SML_TEST;
        for (i = bgn ; i < end ; ++i)
            CU_ASSERT(result->find(result, keys[i]) == false);

        CU_ASSERT_EQUAL(result->size(result), SIZE_SML_TEST >> 2);
        HashSetDeinit(result);
        HashSetDeinit(set_lhs);
        HashSetDeinit(set_rhs);

        for (i = 0 ; i < SIZE_SML_TEST ; ++i)
            free(keys[i]);
    }
}

void TestDifferenceOperation()
{
    char buf[SIZE_TNY_TEST];
    char* keys[SIZE_SML_TEST];
    int i;
    for (i = 0 ; i < SIZE_SML_TEST ; ++i) {
        snprintf(buf, SIZE_SML_TEST, "key -> %d", i);
        keys[i] = strdup(buf);
    }

    /* Prepare the first source set. */
    HashSet* set_lhs = HashSetInit();
    set_lhs->set_hash(set_lhs, HashKey);
    set_lhs->set_compare(set_lhs, CompareKey);
    set_lhs->set_clean_key(set_lhs, CleanKey);
    int bgn = 0;
    int end = SIZE_SML_TEST * 3 >> 2;
    for (i = 0 ; i < end ; ++i)
        set_lhs->add(set_lhs, strdup(keys[i]));

    /* Prepare the second source set. */
    HashSet* set_rhs = HashSetInit();
    set_rhs->set_hash(set_rhs, HashKey);
    set_rhs->set_compare(set_rhs, CompareKey);
    set_rhs->set_clean_key(set_rhs, CleanKey);
    bgn = SIZE_SML_TEST >> 2;
    end = SIZE_SML_TEST - 1;
    for (i = end ; i >= bgn ; --i)
        set_rhs->add(set_rhs, strdup(keys[i]));

    HashSet* set_lhs_only = HashSetDifference(set_lhs, set_rhs);
    CU_ASSERT(set_lhs_only != NULL);
    HashSet* set_rhs_only = HashSetDifference(set_rhs, set_lhs);
    CU_ASSERT(set_rhs_only != NULL);

    /* The "lhs_only" set contains the first quarter of elements and
       the "rhs_only" set contains the forth quarter of elements */
    bgn = 0;
    end = SIZE_SML_TEST >> 2;
    for (i = bgn ; i < end ; ++i) {
        CU_ASSERT(set_lhs_only->find(set_lhs_only, keys[i]) == true);
        CU_ASSERT(set_rhs_only->find(set_rhs_only, keys[i]) == false);
    }
    bgn = SIZE_SML_TEST >> 2;
    end = SIZE_SML_TEST * 3 >> 2;
    for (i = bgn ; i < end ; ++i) {
        CU_ASSERT(set_lhs_only->find(set_lhs_only, keys[i]) == false);
        CU_ASSERT(set_rhs_only->find(set_rhs_only, keys[i]) == false);
    }
    bgn = SIZE_SML_TEST * 3 >> 2;
    end = SIZE_SML_TEST;
    for (i = bgn ; i < end ; ++i) {
        CU_ASSERT(set_lhs_only->find(set_lhs_only, keys[i]) == false);
        CU_ASSERT(set_rhs_only->find(set_rhs_only, keys[i]) == true);
    }

    CU_ASSERT_EQUAL(set_lhs_only->size(set_lhs_only), SIZE_SML_TEST >> 2);
    CU_ASSERT_EQUAL(set_rhs_only->size(set_rhs_only), SIZE_SML_TEST >> 2);

    HashSetDeinit(set_lhs_only);
    HashSetDeinit(set_rhs_only);
    HashSetDeinit(set_lhs);
    HashSetDeinit(set_rhs);
    for (i = 0 ; i < SIZE_SML_TEST ; ++i)
        free(keys[i]);
}


/*-----------------------------------------------------------------------------*
 *                      The driver for HashSet unit test                       *
 *-----------------------------------------------------------------------------*/
bool AddSuite()
{
    {
        /* Verify the basic operations and the structural correctness. */
        CU_pSuite suite = CU_add_suite("Structure Verification", NULL, NULL);
        if (!suite)
            return false;

        CU_pTest unit = CU_add_test(suite, "Set New and Delete", TestNewDelete);
        if (!unit)
            return false;

        unit = CU_add_test(suite, "Numeric Key Put and Get", TestPutGetNum);
        if (!unit)
            return false;

        unit = CU_add_test(suite, "Numeric Key Remove", TestRemoveNum);
        if (!unit)
            return false;

        unit = CU_add_test(suite, "Set Iterator", TestIterateNum);
        if (!unit)
            return false;
    }
    {
        /* Test its robustness to maintain complex data types and large data set. */
        CU_pSuite suite = CU_add_suite("Complex Data Maintenance", NULL, NULL);
        if (!suite)
            return false;

        CU_pTest unit = CU_add_test(suite, "Object Key Put and Get", TestPutGetTxt);
        if (!unit)
            return false;

        unit = CU_add_test(suite, "Same Key Replacement", TestPutDupTxt);
        if (!unit)
            return false;

        unit = CU_add_test(suite, "Object Key Remove and Garbage Collection", TestRemoveTxt);
        if (!unit)
            return false;

        unit = CU_add_test(suite, "Large Amount Key Maintenance", TestBulkTxt);
        if (!unit)
            return false;
    }
    {
        /* Test set arithmetic operation. */
        CU_pSuite suite = CU_add_suite("Set Arithmetic", NULL, NULL);
        if (!suite)
            return false;

        CU_pTest unit = CU_add_test(suite, "Union", TestUnionOperation);
        if (!unit)
            return false;

        unit = CU_add_test(suite, "Intersection", TestIntersectOperation);
        if (!unit)
            return false;

        unit = CU_add_test(suite, "Difference", TestDifferenceOperation);
        if (!unit)
            return false;
    }
    return true;
}

int main()
{
    int rc = 0;

    if (CU_initialize_registry() != CUE_SUCCESS) {
        rc = CU_get_error();
        goto EXIT;
    }

    /* Register the test suites for HashSet verification. */
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
