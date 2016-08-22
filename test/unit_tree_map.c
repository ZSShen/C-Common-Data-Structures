#include "container/tree_map.h"
#include "CUnit/Util.h"
#include "CUnit/Basic.h"


static const int SIZE_TNY_TEST = 128;
static const int SIZE_SML_TEST = 512;
static const int SIZE_MID_TEST = 1024;
static const int SIZE_LGE_TEST = 4096;
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
    TreeMap* map;
    CU_ASSERT((map = TreeMapInit()) != NULL);
    TreeMapDeinit(map);

    /* Enlarge the map size to test the destructor. */
    CU_ASSERT((map = TreeMapInit()) != NULL);
    int i;
    for (i = 0 ; i < SIZE_SML_TEST ; ++i)
        CU_ASSERT(map->put(map, (void*)(intptr_t)i, (void*)(intptr_t)i) == true);
    for (i = SIZE_MID_TEST - 1; i >= SIZE_SML_TEST; --i)
        CU_ASSERT(map->put(map, (void*)(intptr_t)i, (void*)(intptr_t)i) == true);
    TreeMapDeinit(map);

    /* The trival trees which trigger the root maintenance of the destructor. */
    CU_ASSERT((map = TreeMapInit()) != NULL);
    CU_ASSERT(map->put(map, (void*)(intptr_t)0, (void*)(intptr_t)0) == true);
    TreeMapDeinit(map);

    CU_ASSERT((map = TreeMapInit()) != NULL);
    CU_ASSERT(map->put(map, (void*)(intptr_t)1, (void*)(intptr_t)1) == true);
    CU_ASSERT(map->put(map, (void*)(intptr_t)0, (void*)(intptr_t)0) == true);
    TreeMapDeinit(map);
}

void TestOrderRelation()
{
    TreeMap* map = TreeMapInit();

    /* Get the minimum and maximum keys from empty tree. */
    CU_ASSERT(map->minimum(map) == NULL);
    CU_ASSERT(map->maximum(map) == NULL);

    /* Test the correctness of insertion fixup. */
    CU_ASSERT(map->put(map, (void*)(intptr_t)10, (void*)(intptr_t)10) == true);
    CU_ASSERT(map->put(map, (void*)(intptr_t)15, (void*)(intptr_t)15) == true);
    CU_ASSERT(map->put(map, (void*)(intptr_t)20, (void*)(intptr_t)20) == true);
    CU_ASSERT(map->put(map, (void*)(intptr_t)25, (void*)(intptr_t)25) == true);
    CU_ASSERT(map->put(map, (void*)(intptr_t)22, (void*)(intptr_t)22) == true);
    CU_ASSERT(map->put(map, (void*)(intptr_t)9, (void*)(intptr_t)9) == true);
    CU_ASSERT(map->put(map, (void*)(intptr_t)6, (void*)(intptr_t)6) == true);
    CU_ASSERT(map->put(map, (void*)(intptr_t)1, (void*)(intptr_t)1) == true);
    CU_ASSERT(map->put(map, (void*)(intptr_t)4, (void*)(intptr_t)4) == true);
    CU_ASSERT(map->put(map, (void*)(intptr_t)7, (void*)(intptr_t)7) == true);

    /**
     * The vision of the test tree after maintaining the balanced tree property.
     *          9
     *        /   \
     *      4      15
     *     / \     / \
     *    1   6   10  22
     *         \      / \
     *          7   20   25
     */

    /* Check structure correctness. */
    Pair* ptr_pair = map->predecessor(map, (void*)(intptr_t)4);
    CU_ASSERT_EQUAL(1, (int)(intptr_t)ptr_pair->key);
    CU_ASSERT_EQUAL(1, (int)(intptr_t)ptr_pair->value);
    ptr_pair = map->successor(map, (void*)(intptr_t)4);
    CU_ASSERT_EQUAL(6, (int)(intptr_t)ptr_pair->key);
    CU_ASSERT_EQUAL(6, (int)(intptr_t)ptr_pair->value);

    ptr_pair = map->predecessor(map, (void*)(intptr_t)6);
    CU_ASSERT_EQUAL(4, (int)(intptr_t)ptr_pair->key);
    CU_ASSERT_EQUAL(4, (int)(intptr_t)ptr_pair->value);
    ptr_pair = map->successor(map, (void*)(intptr_t)6);
    CU_ASSERT_EQUAL(7, (int)(intptr_t)ptr_pair->key);
    CU_ASSERT_EQUAL(7, (int)(intptr_t)ptr_pair->value);

    ptr_pair = map->predecessor(map, (void*)(intptr_t)7);
    CU_ASSERT_EQUAL(6, (int)(intptr_t)ptr_pair->key);
    CU_ASSERT_EQUAL(6, (int)(intptr_t)ptr_pair->value);
    ptr_pair = map->successor(map, (void*)(intptr_t)7);
    CU_ASSERT_EQUAL(9, (int)(intptr_t)ptr_pair->key);
    CU_ASSERT_EQUAL(9, (int)(intptr_t)ptr_pair->value);

    ptr_pair = map->predecessor(map, (void*)(intptr_t)9);
    CU_ASSERT_EQUAL(7, (int)(intptr_t)ptr_pair->key);
    CU_ASSERT_EQUAL(7, (int)(intptr_t)ptr_pair->value);
    ptr_pair = map->successor(map, (void*)(intptr_t)9);
    CU_ASSERT_EQUAL(10, (int)(intptr_t)ptr_pair->key);
    CU_ASSERT_EQUAL(10, (int)(intptr_t)ptr_pair->value);

    ptr_pair = map->predecessor(map, (void*)(intptr_t)10);
    CU_ASSERT_EQUAL(9, (int)(intptr_t)ptr_pair->key);
    CU_ASSERT_EQUAL(9, (int)(intptr_t)ptr_pair->value);
    ptr_pair = map->successor(map, (void*)(intptr_t)10);
    CU_ASSERT_EQUAL(15, (int)(intptr_t)ptr_pair->key);
    CU_ASSERT_EQUAL(15, (int)(intptr_t)ptr_pair->value);

    ptr_pair = map->predecessor(map, (void*)(intptr_t)15);
    CU_ASSERT_EQUAL(10, (int)(intptr_t)ptr_pair->key);
    CU_ASSERT_EQUAL(10, (int)(intptr_t)ptr_pair->value);
    ptr_pair = map->successor(map, (void*)(intptr_t)15);
    CU_ASSERT_EQUAL(20, (int)(intptr_t)ptr_pair->key);
    CU_ASSERT_EQUAL(20, (int)(intptr_t)ptr_pair->value);

    ptr_pair = map->predecessor(map, (void*)(intptr_t)20);
    CU_ASSERT_EQUAL(15, (int)(intptr_t)ptr_pair->key);
    CU_ASSERT_EQUAL(15, (int)(intptr_t)ptr_pair->value);
    ptr_pair = map->successor(map, (void*)(intptr_t)20);
    CU_ASSERT_EQUAL(22, (int)(intptr_t)ptr_pair->key);
    CU_ASSERT_EQUAL(22, (int)(intptr_t)ptr_pair->value);

    ptr_pair = map->predecessor(map, (void*)(intptr_t)22);
    CU_ASSERT_EQUAL(20, (int)(intptr_t)ptr_pair->key);
    CU_ASSERT_EQUAL(20, (int)(intptr_t)ptr_pair->value);
    ptr_pair = map->successor(map, (void*)(intptr_t)22);
    CU_ASSERT_EQUAL(25, (int)(intptr_t)ptr_pair->key);
    CU_ASSERT_EQUAL(25, (int)(intptr_t)ptr_pair->value);

    /* Check the minimum and maximum key. */
    ptr_pair = map->minimum(map);
    CU_ASSERT_EQUAL(1, (int)(intptr_t)ptr_pair->key);
    CU_ASSERT_EQUAL(1, (int)(intptr_t)ptr_pair->value);
    CU_ASSERT(map->predecessor(map, ptr_pair->key) == NULL);

    ptr_pair = map->maximum(map);
    CU_ASSERT_EQUAL(25, (int)(intptr_t)ptr_pair->key);
    CU_ASSERT_EQUAL(25, (int)(intptr_t)ptr_pair->value);
    CU_ASSERT(map->successor(map, ptr_pair->key) == NULL);

    /* Get the predecessor and successor for the non-existing key. */
    CU_ASSERT(map->predecessor(map, (void*)(intptr_t)100) == NULL);
    CU_ASSERT(map->successor(map, (void*)(intptr_t)100) == NULL);

    /* Check the map size. */
    CU_ASSERT_EQUAL(map->size(map), 10);

    TreeMapDeinit(map);
}

void TestPutGetNum()
{
    srand(time(NULL));

    int elems[SIZE_SML_TEST];
    int i;
    for (i = 0 ; i < SIZE_SML_TEST ; ++i)
        elems[i] = i;

    for (i = 0 ; i < SIZE_SML_TEST ; ++i) {
        int src = rand() % SIZE_SML_TEST;
        int tge = rand() % SIZE_SML_TEST;
        int temp = elems[src];
        elems[src] = elems[tge];
        elems[tge] = temp;
    }

    TreeMap* map = TreeMapInit();
    for (i = 0 ; i < SIZE_SML_TEST ; ++i)
        map->put(map, (void*)(intptr_t)elems[i], (void*)(intptr_t)elems[i]);

    for (i = 1 ; i < SIZE_SML_TEST - 1 ; ++i) {
        void* value = map->get(map, (void*)(intptr_t)i);
        CU_ASSERT_EQUAL(i, (int)(intptr_t)value);
    }

    CU_ASSERT(map->get(map, (void*)(intptr_t)-1) == NULL);

    TreeMapDeinit(map);
}

void TestRemoveNum()
{
    srand(time(NULL));

    int elems[SIZE_LGE_TEST];
    int i;
    for (i = 0 ; i < SIZE_LGE_TEST ; ++i)
        elems[i] = i;

    for (i = 0 ; i < SIZE_MID_TEST ; ++i) {
        int src = rand() % SIZE_LGE_TEST;
        int tge = rand() % SIZE_LGE_TEST;
        int temp = elems[src];
        elems[src] = elems[tge];
        elems[tge] = temp;
    }

    TreeMap* map = TreeMapInit();
    for (i = 0 ; i < SIZE_LGE_TEST ; ++i)
        map->put(map, (void*)(intptr_t)elems[i], (void*)(intptr_t)elems[i]);

    /* Remove part of the key value pairs. */
    int bgn = 0;
    int end = SIZE_TNY_TEST;
    for (i = bgn ; i < end ; ++i)
        CU_ASSERT(map->remove(map, (void*)(intptr_t)i) == true);

    bgn = SIZE_LGE_TEST - 1;
    end = SIZE_LGE_TEST - SIZE_TNY_TEST;
    for (i = bgn ; i >= end ; --i)
        CU_ASSERT(map->remove(map, (void*)(intptr_t)i) == true);

    int divd = SIZE_LGE_TEST >> 1;
    bgn = divd - SIZE_TNY_TEST;
    end = divd + SIZE_TNY_TEST;
    for (i = bgn ; i <= end ; ++i)
        CU_ASSERT(map->remove(map, (void*)(intptr_t)i) == true);

    /* Querying for the keys that are already removed should fail. */
    bgn = 0;
    end = SIZE_TNY_TEST;
    for (i = bgn ; i < end ; ++i) {
        CU_ASSERT(map->remove(map, (void*)(intptr_t)i) == false);
        CU_ASSERT(map->find(map, (void*)(intptr_t)i) == false);
    }

    /* Querying for the keys that still exist should success. */
    bgn = SIZE_TNY_TEST;
    end = SIZE_SML_TEST - SIZE_TNY_TEST;
    for (i = bgn ; i < end ; ++i)
        CU_ASSERT(map->find(map, (void*)(intptr_t)i) == true);

    bgn = SIZE_SML_TEST + SIZE_TNY_TEST + 1;
    end = SIZE_MID_TEST - SIZE_TNY_TEST;
    for (i = bgn ; i < end ; ++i)
        CU_ASSERT(map->find(map, (void*)(intptr_t)i) == true);

    TreeMapDeinit(map);

    /* Test the trival tree handling. */
    map = TreeMapInit();
    map->put(map, (void*)(intptr_t)1, (void*)(intptr_t)1);
    CU_ASSERT(map->remove(map, (void*)(intptr_t)1) == true);
    CU_ASSERT(map->minimum(map) == NULL);
    CU_ASSERT(map->maximum(map) == NULL);
    CU_ASSERT(map->predecessor(map, (void*)(intptr_t)1) == NULL);
    CU_ASSERT(map->successor(map, (void*)(intptr_t)1) == NULL);

    map->put(map, (void*)(intptr_t)1, (void*)(intptr_t)1);
    map->put(map, (void*)(intptr_t)2, (void*)(intptr_t)2);
    CU_ASSERT(map->remove(map, (void*)(intptr_t)1) == true);

    Pair* ptr_pair = map->maximum(map);
    CU_ASSERT_EQUAL(ptr_pair->key, (void*)(intptr_t)2);
    CU_ASSERT_EQUAL(ptr_pair->value, (void*)(intptr_t)2);

    ptr_pair = map->minimum(map);
    CU_ASSERT_EQUAL(ptr_pair->key, (void*)(intptr_t)2);
    CU_ASSERT_EQUAL(ptr_pair->value, (void*)(intptr_t)2);

    CU_ASSERT(map->predecessor(map, (void*)(intptr_t)2) == NULL);
    CU_ASSERT(map->successor(map, (void*)(intptr_t)2) == NULL);

    TreeMapDeinit(map);
}

void TestIterate()
{
    srand(time(NULL));

    int elems[SIZE_SML_TEST];
    int i;
    for (i = 0 ; i < SIZE_SML_TEST ; ++i)
        elems[i] = i;

    for (i = 0 ; i < SIZE_SML_TEST ; ++i) {
        int src = rand() % SIZE_SML_TEST;
        int tge = rand() % SIZE_SML_TEST;
        int temp = elems[src];
        elems[src] = elems[tge];
        elems[tge] = temp;
    }

    TreeMap* map = TreeMapInit();
    for (i = 0 ; i < SIZE_SML_TEST ; ++i)
        map->put(map, (void*)(intptr_t)elems[i], (void*)(intptr_t)elems[i]);

    map->first(map);
    i = 0;
    Pair* ptr_pair;
    while ((ptr_pair = map->next(map)) != NULL) {
        CU_ASSERT_EQUAL(i, (int)(intptr_t)ptr_pair->key);
        CU_ASSERT_EQUAL(i, (int)(intptr_t)ptr_pair->value);
        ++i;
    }
    CU_ASSERT(map->next(map) == NULL);

    /* The previous iteration should not change the structure layout. */
    map->first(map);
    i = 0;
    while ((ptr_pair = map->next(map)) != NULL) {
        CU_ASSERT_EQUAL(i, (int)(intptr_t)ptr_pair->key);
        CU_ASSERT_EQUAL(i, (int)(intptr_t)ptr_pair->value);
        ++i;
    }

    TreeMapDeinit(map);
}

void TestReverseIterate()
{
    srand(time(NULL));

    int elems[SIZE_SML_TEST];
    int i;
    for (i = 0 ; i < SIZE_SML_TEST ; ++i)
        elems[i] = i;

    for (i = 0 ; i < SIZE_SML_TEST ; ++i) {
        int src = rand() % SIZE_SML_TEST;
        int tge = rand() % SIZE_SML_TEST;
        int temp = elems[src];
        elems[src] = elems[tge];
        elems[tge] = temp;
    }

    TreeMap* map = TreeMapInit();
    for (i = 0 ; i < SIZE_SML_TEST ; ++i)
        map->put(map, (void*)(intptr_t)elems[i], (void*)(intptr_t)elems[i]);

    map->first(map);
    i = SIZE_SML_TEST - 1;
    Pair* ptr_pair;
    while ((ptr_pair = map->reverse_next(map)) != NULL) {
        CU_ASSERT_EQUAL(i, (int)(intptr_t)ptr_pair->key);
        CU_ASSERT_EQUAL(i, (int)(intptr_t)ptr_pair->value);
        --i;
    }
    CU_ASSERT(map->next(map) == NULL);

    /* The previous iteration should not change the structure layout. */
    map->first(map);
    i = SIZE_SML_TEST - 1;
    while ((ptr_pair = map->reverse_next(map)) != NULL) {
        CU_ASSERT_EQUAL(i, (int)(intptr_t)ptr_pair->key);
        CU_ASSERT_EQUAL(i, (int)(intptr_t)ptr_pair->value);
        --i;
    }

    TreeMapDeinit(map);
}


/*-----------------------------------------------------------------------------*
 *                      The driver for TreeMap unit test                       *
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

        unit = CU_add_test(suite, "Order Relation", TestOrderRelation);
        if (!unit)
            return false;

        unit = CU_add_test(suite, "Numerics Put and Get", TestPutGetNum);
        if (!unit)
            return false;

        unit = CU_add_test(suite, "Numerics Remove", TestRemoveNum);
        if (!unit)
            return false;

        unit = CU_add_test(suite, "Iterator", TestIterate);
        if (!unit)
            return false;

        unit = CU_add_test(suite, "Reverse Iterator", TestReverseIterate);
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
