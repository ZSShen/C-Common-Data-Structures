#include "map/ordered_map.h"
#include "CUnit/Util.h"
#include "CUnit/Basic.h"


/*===========================================================================*
 *        Definition for the test cases of the simple key value pair         *
 *===========================================================================*/
int32_t SimpleCompare(Entry, Entry);
void SimpleDestroy(Entry);
void TestSimplePut();
void TestSimpleManipulate();


int32_t SuitePrimitive()
{
    CU_pSuite pSuite = CU_add_suite("Primitive Input", NULL, NULL);
    if (!pSuite)
        return ERR_NOMEM;

    CU_pTest pTest = CU_add_test(pSuite, "Basic key value pair insertion",
                     TestSimpleManipulate);
    if (!pTest)
        return ERR_NOMEM;

    return SUCC;
}


int32_t main()
{
    if (CU_initialize_registry() != CUE_SUCCESS)
        return CU_get_error();
    assert(CU_get_registry() != NULL);
    assert(!CU_is_test_running());

    /* Prepare the test suite for primitive input. */
    if (SuitePrimitive() != SUCC) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Launch all the tests. */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    return SUCC;
}


/*===========================================================================*
 *      Implementation for the test cases of the simple key value pair       *
 *===========================================================================*/
int32_t SimpleCompare(Entry entSrc, Entry entTge)
{
    Pair *pPairSrc = (Pair*)entSrc;
    Pair *pPairTge = (Pair*)entTge;
    if (pPairSrc->key == pPairTge->key)
        return 0;
    else {
        if (pPairSrc->key > pPairTge->key)
            return 1;
        else
            return -1;
    }
}

void SimpleDestroy(Entry ent) { free((Pair*)ent); }

void TestSimplePut(OrderedMap *pMap, Key key, Value value)
{
    Pair *pPair = (Pair*)malloc(sizeof(Pair));
    pPair->key = key;
    pPair->value = value;
    CU_ASSERT(pMap->put(pMap, (Entry)pPair) == SUCC);
}

void TestSimpleManipulate()
{
    OrderedMap *pMap;
    CU_ASSERT(OdrMapInit(&pMap) == SUCC);
    CU_ASSERT(pMap->set_compare(pMap, SimpleCompare) == SUCC);
    CU_ASSERT(pMap->set_destroy(pMap, SimpleDestroy) == SUCC);

    int32_t iIdx;
    #if __x86_64__
    for (iIdx = 0 ; iIdx < 100 ; iIdx++)
        TestSimplePut(pMap, (Key)(int64_t)iIdx, (Value)(int64_t)iIdx);

    #else
    for (iIdx = 0 ; iIdx < 100 ; iIdx++)
    TestSimplePut(pMap, (Key)iIdx, (Value)iIdx;

    #endif

    OdrMapDeinit(&pMap);
}


