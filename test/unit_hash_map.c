#include "map/hash_map.h"
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


/* The key value pair clean methods. */
void SimpleDestroy(Entry);
void AdvancedDestroy(Entry);

/* The functions to prepare and release test data. */
int32_t PrepareTestData();
void ReleaseTestData();

/* The functions to register simple and advanced test suites. */
int32_t AddSimpleSuite();
int32_t AddAdvancedSuite();
void TestSimpleManipulate();
void TestBoundaryManipulate();
void TestAdvancedManipulate();

/* The functions to assist map operatoin testing. */
void TestSimplePut(HashMap*, int32_t);
void TestSimpleGetSucc(HashMap*, int32_t);
void TestSimpleGetFail(HashMap*, int32_t);
void TestSimpleErrorKeySize(HashMap*, int32_t);
void TestAdvancedPut(HashMap*, int32_t);


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
    CU_pSuite pSuite = CU_add_suite("Simple key value pair", NULL, NULL);
    if (!pSuite)
        return ERR_NOMEM;

    char *szMsg = "Data manipulation with sequence of put(), get(), and remove() operations.";
    CU_pTest pTest = CU_add_test(pSuite, szMsg, TestSimpleManipulate);
    if (!pTest)
        return ERR_NOMEM;

    pTest = CU_add_test(pSuite, "Boundary test", TestBoundaryManipulate);
    if (!pTest)
        return ERR_NOMEM;

    return SUCC;
}

int32_t AddAdvancedSuite()
{
    CU_pSuite pSuite = CU_add_suite("Advanced key value pair", NULL, NULL);
    if (!pSuite)
        return ERR_NOMEM;

    char *szMsg = "Data manipulation with sequence of put(), get(), and remove() operations.";
    CU_pTest pTest = CU_add_test(pSuite, szMsg, TestAdvancedManipulate);
    if (!pTest)
        return ERR_NOMEM;

    return SUCC;
}


void SimpleDestroy(Entry ent)
{
    Pair *pPair = (Pair *)ent;
    free((int32_t *)pPair->key);
    free((int32_t *)pPair->value);
    free((Pair*)ent);
}


void TestSimplePut(HashMap *pMap, int32_t iIdx)
{
    Pair *pPair = (Pair*)malloc(sizeof(Pair));
    if (!pPair)
        return;

    int32_t *key = (int32_t *)malloc(sizeof(int32_t));
    *key = aNum[iIdx];
    pPair->key = (Key)key;
    int32_t *value = (int32_t *)malloc(sizeof(int32_t));
    *value = aNum[iIdx];
    pPair->value = (Value)value;
    CU_ASSERT(pMap->put(pMap, (Entry)pPair, sizeof(int32_t)) == SUCC);
}

void TestSimpleGetSucc(HashMap *pMap, int32_t iIdx)
{
    int32_t key = aNum[iIdx];
    int32_t valuePred = aNum[iIdx];
    int32_t *valueRetv;
    CU_ASSERT(pMap->get(pMap, (Key)&key, sizeof(int32_t), (Value *)&valueRetv) == SUCC);
    CU_ASSERT_EQUAL(*valueRetv, valuePred);
}

void TestSimpleGetFail(HashMap *pMap, int32_t iIdx)
{
    int32_t key = aNum[iIdx];
    Value valueRetv;
    CU_ASSERT(pMap->get(pMap, (Key)&key, sizeof(int32_t), &valueRetv) 
            == ERR_NODATA);
    CU_ASSERT_EQUAL(valueRetv, NULL);
    CU_ASSERT(pMap->get(pMap, (Key)&key, sizeof(int32_t), NULL) == ERR_GET);
}

void TestSimpleErrorKeySize(HashMap *pMap, int32_t iIdx)
{
    int32_t key = aNum[iIdx];
    Value valueRetv;
    CU_ASSERT(pMap->get(pMap, (Key)&key, 0, &valueRetv) 
            == ERR_KEYSIZE);
    CU_ASSERT(pMap->remove(pMap, (Key)&key, 0) == ERR_KEYSIZE);
    CU_ASSERT(pMap->put(pMap, NULL, 0) == ERR_KEYSIZE);
}

void TestSimpleManipulate()
{
    HashMap *pMap;
    CU_ASSERT(HashMapInit(&pMap, 0) == SUCC);
    CU_ASSERT(pMap->set_destroy(pMap, SimpleDestroy) == SUCC);
    CU_ASSERT_EQUAL(pMap->size(pMap), 0);

    int32_t iIdx;
    for (iIdx = 0 ; iIdx < SIZE_MID_TEST ; iIdx++)
        TestSimplePut(pMap, iIdx);
    for (iIdx = 0 ; iIdx < SIZE_MID_TEST/2 ; iIdx++)
        TestSimpleGetSucc(pMap, iIdx);
    CU_ASSERT_EQUAL(pMap->size(pMap), SIZE_MID_TEST);

    int32_t key;
    for (iIdx = SIZE_MID_TEST/2 ; iIdx < SIZE_MID_TEST ; iIdx++) {
        key = aNum[iIdx];
        CU_ASSERT(pMap->remove(pMap, (Key)&key, sizeof(int32_t)) == SUCC);
    }
    for (iIdx = SIZE_MID_TEST/2 ; iIdx < SIZE_MID_TEST ; iIdx++)
        TestSimpleGetFail(pMap, iIdx);
    for (iIdx = 0 ; iIdx < SIZE_MID_TEST/2 ; iIdx++)
        TestSimpleGetSucc(pMap, iIdx);
    CU_ASSERT_EQUAL(pMap->size(pMap), SIZE_MID_TEST/2);

    TestSimpleErrorKeySize(pMap, 0);

    HashMapDeinit(&pMap);
}

void TestBoundaryManipulate()
{
    HashMap *pMap;
    CU_ASSERT(HashMapInit(&pMap, 0) == SUCC);
    CU_ASSERT(pMap->set_destroy(pMap, SimpleDestroy) == SUCC);

    /* Get the unexisting key value pair. */
    int32_t iIdx;
    for (iIdx = 0 ; iIdx < SIZE_MID_TEST/2 ; iIdx++)
        TestSimpleGetFail(pMap, iIdx);

    int32_t key;
    /* Remove the unexisting key value pair. */
    for (iIdx = 0 ; iIdx < SIZE_MID_TEST/2 ; iIdx++) {
        key = aNum[iIdx];
        CU_ASSERT(pMap->remove(pMap, (Key)&key, sizeof(int32_t)) == ERR_NODATA);
    }
    for (iIdx = 0 ; iIdx < SIZE_MID_TEST/2 ; iIdx++)
        TestSimplePut(pMap, iIdx);

    /* Get the unexisting key value pair. */
    for (iIdx = SIZE_MID_TEST/2 ; iIdx < SIZE_MID_TEST ; iIdx++)
        TestSimpleGetFail(pMap, iIdx);

    /* Remove the unexisting key value pair. */
    for (iIdx = SIZE_MID_TEST/2 ; iIdx < SIZE_MID_TEST ; iIdx++) {
        key = aNum[iIdx];
        CU_ASSERT(pMap->remove(pMap, (Key)&key, sizeof(int32_t)) == ERR_NODATA);
    }
    HashMapDeinit(&pMap);
}

void AdvancedDestroy(Entry ent)
{
    Pair *pPair = (Pair*)ent;
    free((Employ*)pPair->value);
    free(pPair);
}

void TestAdvancedPut(HashMap *pMap, int32_t iIdx)
{
    Pair *pPair = (Pair*)malloc(sizeof(Pair));
    if (!pPair)
        return;

    Employ *pEmploy = (Employ*)malloc(sizeof(Employ));
    if (!pEmploy)
        return;
    pEmploy->cYear = aNum[iIdx] / MASK_YEAR;
    pEmploy->cLevel = aNum[iIdx] / MASK_LEVEL;
    pEmploy->iId = aNum[iIdx];

    pPair->key = aName[iIdx];
    pPair->value = (Value)pEmploy;

    CU_ASSERT(pMap->put(pMap, (Entry)pPair, strlen(aName[iIdx]) + 1) == SUCC);
}

void TestAdvancedGetSucc(HashMap *pMap, int32_t iIdx)
{
    Key key = aName[iIdx];
    Employ valuePred = {aNum[iIdx]/MASK_YEAR, aNum[iIdx]/MASK_LEVEL, aNum[iIdx]};
    Value valueRetv;

    CU_ASSERT(pMap->get(pMap, key, strlen(aName[iIdx]) + 1, &valueRetv) 
            == SUCC);
    CU_ASSERT_EQUAL(valuePred.cYear, ((Employ*)valueRetv)->cYear);
    CU_ASSERT_EQUAL(valuePred.cLevel, ((Employ*)valueRetv)->cLevel);
    CU_ASSERT_EQUAL(valuePred.iId, ((Employ*)valueRetv)->iId);
}

void TestAdvancedGetFail(HashMap *pMap, int32_t iIdx)
{
    Key key = aName[iIdx];
    Value valueRetv;

    CU_ASSERT(pMap->get(pMap, key, strlen(aName[iIdx]) + 1, &valueRetv) == ERR_NODATA);
    CU_ASSERT_EQUAL(valueRetv, NULL);
}

void TestAdvancedManipulate()
{
    HashMap *pMap;
    CU_ASSERT(HashMapInit(&pMap, 0) == SUCC);
    CU_ASSERT(pMap->set_destroy(pMap, AdvancedDestroy) == SUCC);
    CU_ASSERT_EQUAL(pMap->size(pMap), 0);

    int32_t iIdx;
    for (iIdx = 0 ; iIdx < SIZE_MID_TEST ; iIdx++)
        TestAdvancedPut(pMap, iIdx);
    for (iIdx = 0 ; iIdx < SIZE_MID_TEST/2 ; iIdx++)
        TestAdvancedGetSucc(pMap, iIdx);
    CU_ASSERT_EQUAL(pMap->size(pMap), SIZE_MID_TEST);

    for (iIdx = SIZE_MID_TEST/2 ; iIdx < SIZE_MID_TEST ; iIdx++)
        CU_ASSERT(pMap->remove(pMap, (Key)aName[iIdx], strlen(aName[iIdx]) + 1) == SUCC);
    for (iIdx = SIZE_MID_TEST/2 ; iIdx < SIZE_MID_TEST ; iIdx++)
        TestAdvancedGetFail(pMap, iIdx);
    for (iIdx = 0 ; iIdx < SIZE_MID_TEST/2 ; iIdx++)
        TestAdvancedGetSucc(pMap, iIdx);
    CU_ASSERT_EQUAL(pMap->size(pMap), SIZE_MID_TEST/2);

    HashMapDeinit(&pMap);
}
