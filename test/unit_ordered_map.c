#include "map/ordered_map.h"
#include "CUnit/Util.h"
#include "CUnit/Basic.h"


#define SIZE_MID_TEST     1000
int32_t aData[SIZE_MID_TEST];


/*===========================================================================*
 *        Definition for the test cases of the simple key value pair         *
 *===========================================================================*/
int32_t SimpleCompare(Entry, Entry);
void SimpleDestroy(Entry);
void TestPut();
void TestSimpleGetSucc();
void TestSimpleGetFail();
void TestSimpleManipulate();


int32_t SuitePrimitive()
{
    /* Prepare the testing dataset. */
    srand(time(NULL));
    int32_t idxFst = 0, idxSnd;
    while (idxFst < SIZE_MID_TEST) {
        int32_t iRand;
        bool bDup = true;
        do {
            iRand = rand() % SIZE_MID_TEST;
            for (idxSnd = 0 ; idxSnd < idxFst ; idxSnd++) {
                if (iRand == aData[idxSnd])
                    break;
            }
            if (idxSnd == idxFst)
                bDup = false;
        } while (bDup);
        aData[idxFst] = iRand;
        idxFst++;
    }

    CU_pSuite pSuite = CU_add_suite("Primitive Input", NULL, NULL);
    if (!pSuite)
        return ERR_NOMEM;

    CU_pTest pTest = CU_add_test(pSuite, "Basic key value pair manipulation",
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

void TestPut(OrderedMap *pMap, Key key, Value value)
{
    Pair *pPair = (Pair*)malloc(sizeof(Pair));
    if (!pPair)
        return;
    pPair->key = key;
    pPair->value = value;
    CU_ASSERT(pMap->put(pMap, (Entry)pPair) == SUCC);
}

void TestSimpleGetSucc(OrderedMap *pMap, Key key, Value valuePred)
{
    Value valueRetv;
    CU_ASSERT(pMap->get(pMap, key, &valueRetv) == SUCC);
    CU_ASSERT_EQUAL(valueRetv, valuePred);
}

void TestSimpleGetFail(OrderedMap *pMap, Key key, Value valuePred)
{
    Value valueRetv;
    CU_ASSERT(pMap->get(pMap, key, &valueRetv) == ERR_NODATA);
    CU_ASSERT_EQUAL(valueRetv, NULL);
}


void TestSimpleManipulate()
{
    OrderedMap *pMap;
    CU_ASSERT(OdrMapInit(&pMap) == SUCC);
    CU_ASSERT(pMap->set_compare(pMap, SimpleCompare) == SUCC);
    CU_ASSERT(pMap->set_destroy(pMap, SimpleDestroy) == SUCC);
    CU_ASSERT_EQUAL(pMap->size(pMap), 0);

    int32_t iIdx;
    #if __x86_64__
        for (iIdx = 0 ; iIdx < SIZE_MID_TEST ; iIdx++)
            TestPut(pMap, (Key)(int64_t)aData[iIdx], (Value)(int64_t)aData[iIdx]);
        for (iIdx = 0 ; iIdx < SIZE_MID_TEST/2 ; iIdx++)
            TestSimpleGetSucc(pMap, (Key)(int64_t)aData[iIdx], (Value)(int64_t)aData[iIdx]);
        CU_ASSERT_EQUAL(pMap->size(pMap), SIZE_MID_TEST);

        for (iIdx = SIZE_MID_TEST/2 ; iIdx < SIZE_MID_TEST ; iIdx++)
            CU_ASSERT(pMap->remove(pMap, (Key)(int64_t)aData[iIdx]) == SUCC);
        for (iIdx = SIZE_MID_TEST/2 ; iIdx < SIZE_MID_TEST ; iIdx++)
            TestSimpleGetFail(pMap, (Key)(int64_t)aData[iIdx], (Value)(int64_t)aData[iIdx]);
        for (iIdx = 0 ; iIdx < SIZE_MID_TEST/2 ; iIdx++)
            TestSimpleGetSucc(pMap, (Key)(int64_t)aData[iIdx], (Value)(int64_t)aData[iIdx]);
        CU_ASSERT_EQUAL(pMap->size(pMap), SIZE_MID_TEST/2);

    #else
        for (iIdx = 0 ; iIdx < SIZE_MID_TEST ; iIdx++)
            TestPut(pMap, (Key)aData[iIdx], (Value)aData[iIdx]);
        for (iIdx = 0 ; iIdx < SIZE_MID_TEST/2 ; iIdx++)
            TestSimpleGetSucc(pMap, (Key)aData[iIdx], (Value)aData[iIdx]);
        CU_ASSERT_EQUAL(pMap->size(pMap), SIZE_MID_TEST);

        for (iIdx = SIZE_MID_TEST/2 ; iIdx < SIZE_MID_TEST ; iIdx++)
            CU_ASSERT(pMap->remove(pMap, (Key)aData[iIdx]) == SUCC);
        for (iIdx = SIZE_MID_TEST/2 ; iIdx < SIZE_MID_TEST ; iIdx++)
            TestSimpleGetFail(pMap, (Key)aData[iIdx], (Value)aData[iIdx]);
        for (iIdx = 0 ; iIdx < SIZE_MID_TEST/2 ; iIdx++)
            TestSimpleGetSucc(pMap, (Key)aData[iIdx], (Value)aData[iIdx]);
        CU_ASSERT_EQUAL(pMap->size(pMap), SIZE_MID_TEST/2);
    #endif

    OdrMapDeinit(&pMap);
}


