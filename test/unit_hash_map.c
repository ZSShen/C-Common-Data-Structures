#include "container/hash_map.h"
#include "math/hash.h"
#include "CUnit/Util.h"
#include "CUnit/Basic.h"


/*------------------------------------------------------------*
 *    Test Function Declaration for Structure Verification    *
 *------------------------------------------------------------*/
static const int COUNT_ITER = 1000;
static const int SIZE_TNY_TEST = 128;
static const int SIZE_SML_TEST = 512;
static const int SIZE_MID_TEST = 1024;
static const int SIZE_MID_STR = 32;

static const int RANGE_CHAR = 26;
static const int BASE_CHAR = 97;

static const int MASK_YEAR = 50;
static const int MASK_LEVEL = 100;

typedef struct Employ_ {
    int year;
    int level;
    int id;
} Employ;


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

void CleanValue(void* value)
{
    free(value);
}


/*-----------------------------------------------------------------------------*
 *            Unit tests relevant to basic structure verification              *
 *-----------------------------------------------------------------------------*/
void TestNewDelete()
{
    HashMap* map;
    CU_ASSERT((map = HashMapInit()) != NULL);

    /* Enlarge the map size to test the destructor. */
    int i;
    for (i = 0 ; i < SIZE_SML_TEST ; ++i)
        CU_ASSERT(map->put(map, (void*)(intptr_t)i, (void*)(intptr_t)i) == true);

    HashMapDeinit(map);
}

void TestPutGetNum()
{
    HashMap* map = HashMapInit();
    int i;
    for (i = 0 ; i < SIZE_TNY_TEST ; ++i)
        map->put(map, (void*)(intptr_t)i, (void*)(intptr_t)i);

    for (i = 0 ; i < SIZE_TNY_TEST ; ++i) {
        CU_ASSERT(map->find(map, (void*)(intptr_t)i) == true);
        int val = (int)(intptr_t)map->get(map, (void*)(intptr_t)i);
        CU_ASSERT_EQUAL(i, val);
    }
    HashMapDeinit(map);
}

void TestRemoveNum()
{
    HashMap* map = HashMapInit();

    int i;
    for (i = 0 ; i < SIZE_TNY_TEST ; ++i)
        map->put(map, (void*)(intptr_t)i, (void*)(intptr_t)i);

    /* Remove the first half of the key value pairs. */
    for (i = 0 ; i < SIZE_TNY_TEST >> 1 ; ++i)
        CU_ASSERT(map->remove(map, (void*)(intptr_t)i) == true);

    /* Querying for the keys that are already removed should fail. */
    for (i = 0 ; i < SIZE_TNY_TEST >> 1 ; ++i) {
        CU_ASSERT(map->remove(map, (void*)(intptr_t)i) == false);
        CU_ASSERT(map->find(map, (void*)(intptr_t)i) == false);
    }

    /* Querying for the keys that still exist should success. */
    for (i = SIZE_TNY_TEST >> 1 ; i < SIZE_TNY_TEST ; ++i)
        CU_ASSERT(map->find(map, (void*)(intptr_t)i) == true);

    CU_ASSERT_EQUAL(map->size(map), SIZE_TNY_TEST >> 1);

    HashMapDeinit(map);
}

void TestIterateNum()
{
    HashMap* map = HashMapInit();

    int i;
    for (i = 0 ; i < SIZE_TNY_TEST ; ++i)
        map->put(map, (void*)(intptr_t)i, (void*)(intptr_t)i);

    /* Since the data size won't trigger re-hashing, we can predict the
       data distribution. */
    i = 0;
    Pair* ptr_pair;
    map->first(map);
    while ((ptr_pair = map->next(map)) != NULL) {
        CU_ASSERT_EQUAL(i, (int)(intptr_t)ptr_pair->key);
        ++i;
    }
    CU_ASSERT(map->next(map) == NULL);

    /* The previous iteration should not change the structure layout. */
    i = 0;
    map->first(map);
    while ((ptr_pair = map->next(map)) != NULL) {
        CU_ASSERT_EQUAL(i, (int)(intptr_t)ptr_pair->key);
        ++i;
    }

    HashMapDeinit(map);
}

void TestPutGetTxt()
{
    char buf[SIZE_TNY_TEST];
    char* keys[SIZE_TNY_TEST];
    HashMap* map = HashMapInit();
    map->set_hash(map, HashKey);
    map->set_compare(map, CompareKey);

    int i;
    for (i = 0 ; i < SIZE_TNY_TEST ; ++i) {
        snprintf(buf, SIZE_TNY_TEST, "key -> %d", i);
        keys[i] = strdup(buf);
        map->put(map, (void*)keys[i], (void*)(intptr_t)i);
    }

    for (i = 0 ; i < SIZE_TNY_TEST ; ++i) {
        CU_ASSERT(map->find(map, (void*)keys[i]) == true);
        int val = (int)(intptr_t)map->get(map, (void*)keys[i]);
        CU_ASSERT_EQUAL(i, val);
        free(keys[i]);
    }

    HashMapDeinit(map);
}

void TestRemoveTxt()
{
    char buf[SIZE_TNY_TEST];
    char* keys[SIZE_TNY_TEST];
    HashMap* map = HashMapInit();
    map->set_hash(map, HashKey);
    map->set_compare(map, CompareKey);
    map->set_clean_key(map, CleanKey);
    map->set_clean_value(map, CleanValue);

    int i;
    for (i = 0 ; i < SIZE_TNY_TEST ; ++i) {
        snprintf(buf, SIZE_TNY_TEST, "key -> %d", i);
        keys[i] = strdup(buf);
        Employ* employ = (Employ*)malloc(sizeof(Employ));
        employ->year = i;
        employ->level = i;
        employ->id = i;
        map->put(map, (void*)keys[i], (void*)employ);
    }

    /* Remove the first half of the key value pairs. */
    for (i = 0 ; i < SIZE_TNY_TEST >> 1 ; ++i)
        CU_ASSERT(map->remove(map, (void*)keys[i]) == true);

    /* Querying for the keys that are already removed should fail. */
    for (i = 0 ; i < SIZE_TNY_TEST >> 1 ; ++i) {
        snprintf(buf, SIZE_TNY_TEST, "key -> %d", i);
        CU_ASSERT(map->remove(map, (void*)buf) == false);
        CU_ASSERT(map->find(map, (void*)buf) == false);
    }

    /* Querying for the keys that still exist should success. */
    for (i = SIZE_TNY_TEST >> 1 ; i < SIZE_TNY_TEST ; ++i)
        CU_ASSERT(map->find(map, (void*)keys[i]) == true);

    HashMapDeinit(map);
}

void TestPutDupText()
{
    char buf[SIZE_TNY_TEST];
    char* keys[SIZE_TNY_TEST];
    HashMap* map = HashMapInit();
    map->set_hash(map, HashKey);
    map->set_compare(map, CompareKey);
    map->set_clean_key(map, CleanKey);
    map->set_clean_value(map, CleanValue);

    int i;
    for (i = 0 ; i < SIZE_TNY_TEST ; ++i) {
        snprintf(buf, SIZE_TNY_TEST, "key -> %d", i);
        keys[i] = strdup(buf);
        Employ* employ = (Employ*)malloc(sizeof(Employ));
        employ->year = i;
        employ->level = i;
        employ->id = i;
        map->put(map, (void*)keys[i], (void*)employ);
    }

    /* Insert the new key value pairs with the same key set. */
    for (i = 0 ; i < SIZE_TNY_TEST ; ++i) {
        snprintf(buf, SIZE_TNY_TEST, "key -> %d", i);
        keys[i] = strdup(buf);
        Employ* employ = (Employ*)malloc(sizeof(Employ));
        employ->year = SIZE_TNY_TEST - i;
        employ->level = SIZE_TNY_TEST - i;
        employ->id = SIZE_TNY_TEST - i;
        CU_ASSERT(map->put(map, (void*)keys[i], (void*)employ) == true);
    }

    /* Now the values of the existing pairs should be replaced. */
    for (i = 0 ; i < SIZE_TNY_TEST ; ++i) {
        Employ* employ = map->get(map, (void*)keys[i]);
        CU_ASSERT_EQUAL(SIZE_TNY_TEST - i, employ->year);
        CU_ASSERT_EQUAL(SIZE_TNY_TEST - i, employ->level);
        CU_ASSERT_EQUAL(SIZE_TNY_TEST - i, employ->id);
    }

    HashMapDeinit(map);
}


/*-----------------------------------------------------------------------------*
 *                      The driver for HashMap unit test                       *
 *-----------------------------------------------------------------------------*/
bool AddSuite()
{
    {
        /* Verify the basic operations and the structural correctness. */
        CU_pSuite suite = CU_add_suite("Structure Verification", NULL, NULL);
        if (!suite)
            return false;

        CU_pTest unit = CU_add_test(suite, "New and Delete", TestNewDelete);
        if (!unit)
            return false;

        unit = CU_add_test(suite, "Numerics Put and Get", TestPutGetNum);
        if (!unit)
            return false;

        unit = CU_add_test(suite, "Numerics Remove", TestRemoveNum);
        if (!unit)
            return false;

        unit = CU_add_test(suite, "Numerics Iterator", TestIterateNum);
        if (!unit)
            return false;
    }
    {
        /* Test its robustness to maintain complex data types and large data set. */
        CU_pSuite suite = CU_add_suite("Complex Data Maintenance", NULL, NULL);
        if (!suite)
            return false;

        CU_pTest unit = CU_add_test(suite, "Text Put and Get", TestPutGetTxt);
        if (!unit)
            return false;

        unit = CU_add_test(suite, "Pair Replacement", TestPutDupText);
        if (!unit)
            return false;

        unit = CU_add_test(suite, "Text Remove and Garbage Collection", TestRemoveTxt);
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

    /* Register the test suite for map structure verification. */
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

