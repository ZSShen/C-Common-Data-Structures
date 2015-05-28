#include "map/ordered_map.h"
#include "CUnit/Util.h"
#include "CUnit/Basic.h"


#define SIZE_MID_TEST       (1000)
#define SIZE_TINY_STR       (4)
#define RANGE_CHAR          (26)
#define BASE_CHAR           (97)
#define MASK_YEAR           (100)
#define MASK_LEVEL          (200)

typedef struct Employ_ {
    int8_t cYear;
    int8_t cLevel;
    int32_t iId;
} Employ;

int32_t aNum[SIZE_MID_TEST];
char* aName[SIZE_MID_TEST];


/* The key value pair comparison methods.*/
int32_t SimpleCompare(Entry, Entry);
int32_t AdvancedCompare(Entry, Entry);

/* The key value pair clean methods. */
void SimpleDestroy(Entry);
void AdvancedDestroy(Entry);

/* The functions to prepare and release the test data. */
int32_t PrepareTestData();
void ReleaseTestData();

/* The functions to register simple and advanced test suites. */
int32_t AddSimpleSuite();
int32_t AddAdvancedSuite();
void TestSimpleManipulate();
void TestAdvancedManipulate();

/* The functions to assist map operatoin testing. */
void TestSimplePut(OrderedMap*, int32_t);
void TestSimpleGetSucc(OrderedMap*, int32_t);
void TestSimpleGetFail(OrderedMap*, int32_t);


int32_t main()
{
    int32_t iRtnCode = SUCC;

    iRtnCode = PrepareTestData();
    if (iRtnCode != SUCC)
        goto EXIT;

    if (CU_initialize_registry() != CUE_SUCCESS) {
        iRtnCode = CU_get_error();
        goto EXIT;
    }

    /* Register the test suite for basic key value pairs. */
    if (AddSimpleSuite() != SUCC) {
        iRtnCode = CU_get_error();
        goto CLEAN;
    }

    /* Register the test suite for advanced key value pairs. */
    if (AddAdvancedSuite() != SUCC) {
        iRtnCode = CU_get_error();
        goto CLEAN;
    }

    /* Launch all the tests. */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

CLEAN:
    CU_cleanup_registry();
EXIT:
    ReleaseTestData();
    return iRtnCode;
}


int32_t PrepareTestData()
{
    srand(time(NULL));
    int32_t idxFst = 0, idxSnd;
    for (idxFst = 0 ; idxFst < SIZE_MID_TEST ; idxFst++)
        aName[idxFst] = NULL;

    int32_t iRand;
    char szNew[SIZE_TINY_STR];
    idxFst = 0;
    while (idxFst < SIZE_MID_TEST) {
        bool bDup = true;

        /* For the array of strings. */
        do {
            int32_t iOfst;
            char cToken;
            szNew[SIZE_TINY_STR - 1] = 0;
            for (iOfst = 0 ; iOfst < SIZE_TINY_STR - 1; iOfst++) {
                cToken = BASE_CHAR + rand() % RANGE_CHAR;
                szNew[iOfst] = cToken;
            }

            for (idxSnd = 0 ; idxSnd < idxFst ; idxSnd++) {
                if (strcmp(szNew, aName[idxSnd]) == 0)
                    break;
            }
            if (idxSnd == idxFst)
                bDup = false;
        } while (bDup);

        aName[idxFst] = (char*)malloc(SIZE_TINY_STR * sizeof(char));
        if (!(aName[idxFst]))
            return ERR_NOMEM;
        strcpy(aName[idxFst], szNew);
        aName[idxFst][SIZE_TINY_STR - 1] = 0;

        /* For the array of integers. */
        bDup = true;
        do {
            iRand = rand() % SIZE_MID_TEST;
            for (idxSnd = 0 ; idxSnd < idxFst ; idxSnd++) {
                if (iRand == aNum[idxSnd])
                    break;
            }
            if (idxSnd == idxFst)
                bDup = false;
        } while (bDup);
        aNum[idxFst] = iRand;

        idxFst++;
    }

    return SUCC;
}

void ReleaseTestData()
{
    int32_t iIdx;
    for (iIdx = 0 ; iIdx < SIZE_MID_TEST ; iIdx++) {
        if (aName[iIdx])
            free(aName[iIdx]);
    }
}


int32_t AddSimpleSuite()
{
    CU_pSuite pSuite = CU_add_suite("Basic Test Suite", NULL, NULL);
    if (!pSuite)
        return ERR_NOMEM;

    CU_pTest pTest = CU_add_test(pSuite, "Basic key value pair manipulation",
                     TestSimpleManipulate);
    if (!pTest)
        return ERR_NOMEM;

    return SUCC;
}

int32_t AddAdvancedSuite()
{
    CU_pSuite pSuite = CU_add_suite("Advanced Test Suite", NULL, NULL);
    if (!pSuite)
        return ERR_NOMEM;

    CU_pTest pTest = CU_add_test(pSuite, "Advanced key value pair manipulation",
                     TestAdvancedManipulate);
    if (!pTest)
        return ERR_NOMEM;

    return SUCC;
}


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

void SimpleDestroy(Entry ent)
{
    free((Pair*)ent);
}


void TestSimplePut(OrderedMap *pMap, int32_t iIdx)
{
    Pair *pPair = (Pair*)malloc(sizeof(Pair));
    if (!pPair)
        return;

    #if __x86_64__
        pPair->key = (Key)(int64_t)aNum[iIdx];
        pPair->value = (Value)(int64_t)aNum[iIdx];
    #else
        pPair->key = (Key)aNum[iIdx];
        pPair->value = (Value)aNum[iIdx];
    #endif
    CU_ASSERT(pMap->put(pMap, (Entry)pPair) == SUCC);
}

void TestSimpleGetSucc(OrderedMap *pMap, int32_t iIdx)
{
    Key key;
    Value valuePred, valueRetv;
    #if __x86_64__
        key = (Key)(int64_t)aNum[iIdx];
        valuePred = (Value)(int64_t)aNum[iIdx];
    #else
        key = (Key)aNum[iIdx];
        valuePred = (Value)aNum[iIdx];
    #endif
    CU_ASSERT(pMap->get(pMap, key, &valueRetv) == SUCC);
    CU_ASSERT_EQUAL(valueRetv, valuePred);
}

void TestSimpleGetFail(OrderedMap *pMap, int32_t iIdx)
{
    Key key;
    Value valueRetv;
    #if __x86_64__
        key = (Key)(int64_t)aNum[iIdx];
    #else
        key = (Key)aNum[iIdx];
    #endif
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
    for (iIdx = 0 ; iIdx < SIZE_MID_TEST ; iIdx++)
        TestSimplePut(pMap, iIdx);
    for (iIdx = 0 ; iIdx < SIZE_MID_TEST/2 ; iIdx++)
        TestSimpleGetSucc(pMap, iIdx);
    CU_ASSERT_EQUAL(pMap->size(pMap), SIZE_MID_TEST);

    for (iIdx = SIZE_MID_TEST/2 ; iIdx < SIZE_MID_TEST ; iIdx++)
        #if __x86_64__
        CU_ASSERT(pMap->remove(pMap, (Key)(int64_t)aNum[iIdx]) == SUCC);
        #else
        CU_ASSERT(pMap->remove(pMap, (Key)aNum[iIdx]) == SUCC);
        #endif
    for (iIdx = SIZE_MID_TEST/2 ; iIdx < SIZE_MID_TEST ; iIdx++)
        TestSimpleGetFail(pMap, iIdx);
    for (iIdx = 0 ; iIdx < SIZE_MID_TEST/2 ; iIdx++)
        TestSimpleGetSucc(pMap, iIdx);
    CU_ASSERT_EQUAL(pMap->size(pMap), SIZE_MID_TEST/2);

    OdrMapDeinit(&pMap);
}


int32_t AdvancedCompare(Entry entSrc, Entry entTge)
{
    Pair *pPairSrc = (Pair*)entSrc;
    Pair *pPairTge = (Pair*)entTge;
    int32_t iOrder = strcmp(pPairSrc->key, pPairTge->key);
    if (iOrder != 0)
        return (iOrder > 0)? 1 : (-1);
    return 0;
}

void AdvancedDestroy(Entry ent)
{
    Pair *pPair = (Pair*)ent;
    free((Employ*)pPair->value);
    free(pPair);
}

void TestAdvancedManipulate()
{
    OrderedMap *pMap;
    CU_ASSERT(OdrMapInit(&pMap) == SUCC);
    CU_ASSERT(pMap->set_compare(pMap, AdvancedCompare) == SUCC);
    CU_ASSERT(pMap->set_destroy(pMap, AdvancedDestroy) == SUCC);
    CU_ASSERT_EQUAL(pMap->size(pMap), 0);

    OdrMapDeinit(&pMap);
}