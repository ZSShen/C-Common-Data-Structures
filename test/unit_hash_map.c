#include "container/hash_map.h"
#include "math/hash.h"
#include "CUnit/Util.h"
#include "CUnit/Basic.h"


/*------------------------------------------------------------*
 *    Test Function Declaration for Structure Verification    *
 *------------------------------------------------------------*/
#define COUNT_ITER          (1000)
#define SIZE_MID_TEST       (10000)
#define SIZE_MID_STR        (32)
#define RANGE_CHAR          (26)
#define BASE_CHAR           (97)
#define MASK_YEAR           (50)
#define MASK_LEVEL          (100)

typedef struct Employ_ {
    int8_t cYear;
    int8_t cLevel;
    int32_t iId;
} Employ;

char* aName[SIZE_MID_TEST];

int32_t AddSuite();
void TestManipulate();
void TestIterator();

int32_t PrepareTestData();
void ReleaseTestData();
void DestroyPair(Pair*);


int32_t main()
{
    int32_t rc = SUCC;

    rc = PrepareTestData();
    if (rc != SUCC)
        goto EXIT;

    if (CU_initialize_registry() != CUE_SUCCESS) {
        rc = CU_get_error();
        goto EXIT;
    }

    /* Register the test suite for map structure verification. */
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
    ReleaseTestData();
    return rc;
}


/*------------------------------------------------------------*
 *  Test Function Implementation for Structure Verification   *
 *------------------------------------------------------------*/
void DestroyPair(Pair *pPair)
{
    free((Employ*)pPair->value);
    free(pPair);
}

int32_t PrepareTestData()
{
    srand(time(NULL));

    int32_t iIdx;
    for (iIdx = 0 ; iIdx < SIZE_MID_TEST ; iIdx++)
        aName[iIdx] = NULL;

    for (iIdx = 0 ; iIdx < SIZE_MID_TEST ; iIdx++) {
        aName[iIdx] = (char*)malloc(sizeof(char) * (SIZE_MID_STR + 1));
        if (!aName[iIdx])
            return ERR_NOMEM;
        int iOfst;
        for (iOfst = 0 ; iOfst < SIZE_MID_STR ; iOfst++) {
            char cToken = BASE_CHAR + rand() % RANGE_CHAR;
            aName[iIdx][iOfst] = cToken;
        }
        aName[iIdx][SIZE_MID_STR] = 0;
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

int32_t AddSuite()
{
    int32_t rc = SUCC;

    CU_pSuite pSuite = CU_add_suite("Structure Verification", NULL, NULL);
    if (!pSuite) {
        rc = ERR_REG;
        goto EXIT;
    }

    char *szMsg = "Combination with insertion, deletion, and searching.";
    CU_pTest pTest = CU_add_test(pSuite, szMsg, TestManipulate);
    if (!pTest)
        rc = ERR_REG;

    pTest = CU_add_test(pSuite, "Map iterator.", TestIterator);
    if (!pTest)
        rc = ERR_REG;

EXIT:
    return rc;
}

void TestManipulate()
{
    HashMap *pMap;
    CU_ASSERT(HashMapInit(&pMap) == SUCC);
    CU_ASSERT(pMap->set_destroy(pMap, DestroyPair) == SUCC);

    /* Insert the full data. */
    int32_t iIdx;
    for (iIdx = 0 ; iIdx < SIZE_MID_TEST ; iIdx++) {
        Pair *pPair = (Pair*)malloc(sizeof(Pair));
        Employ *pEmploy = (Employ*)malloc(sizeof(Employ));

        pEmploy->cYear = iIdx % MASK_YEAR;
        pEmploy->cLevel = iIdx % MASK_LEVEL;
        pEmploy->iId = iIdx;

        pPair->key = aName[iIdx];
        pPair->value = (Value)pEmploy;
        CU_ASSERT(pMap->put(pMap, pPair, SIZE_MID_STR) == SUCC);
    }

    /* Search and Retrieve the first half of the data. It should succeed. */
    for (iIdx = 0 ; iIdx < SIZE_MID_TEST / 2 ; iIdx++) {
        Value valueRetv;
        CU_ASSERT(pMap->find(pMap, aName[iIdx], SIZE_MID_STR) == SUCC);
        CU_ASSERT(pMap->get(pMap, aName[iIdx], SIZE_MID_STR, &valueRetv) == SUCC);
        CU_ASSERT_EQUAL(iIdx, ((Employ*)valueRetv)->iId);
    }

    CU_ASSERT_EQUAL(pMap->size(pMap), SIZE_MID_TEST);

    /* Delete the second half of the data.  */
    for (iIdx = SIZE_MID_TEST / 2 ; iIdx < SIZE_MID_TEST ; iIdx++)
        CU_ASSERT(pMap->remove(pMap, (Key)aName[iIdx], SIZE_MID_STR) == SUCC);

    /* Search and Retrieve the second half of the data. It should fail. */
    for (iIdx = SIZE_MID_TEST / 2 ; iIdx < SIZE_MID_TEST ; iIdx++) {
        Value valueRetv;
        CU_ASSERT(pMap->find(pMap, aName[iIdx], SIZE_MID_STR) == NOKEY);
        CU_ASSERT(pMap->get(pMap, aName[iIdx], SIZE_MID_STR, &valueRetv) == ERR_NODATA);
        CU_ASSERT_EQUAL(valueRetv, NULL);
    }

    /* But the searching for the first half should not be affected. */
    for (iIdx = 0 ; iIdx < SIZE_MID_TEST / 2 ; iIdx++) {
        Value valueRetv;
        CU_ASSERT(pMap->find(pMap, aName[iIdx], SIZE_MID_STR) == SUCC);
        CU_ASSERT(pMap->get(pMap, aName[iIdx], SIZE_MID_STR, &valueRetv) == SUCC);
        CU_ASSERT_EQUAL(iIdx, ((Employ*)valueRetv)->iId);
    }

    /* Delete the already deleted second half of the data. It should fail. */
    for (iIdx = SIZE_MID_TEST / 2 ; iIdx < SIZE_MID_TEST ; iIdx++)
        CU_ASSERT(pMap->remove(pMap, (Key)aName[iIdx], SIZE_MID_STR) == ERR_NODATA);

    CU_ASSERT_EQUAL(pMap->size(pMap), SIZE_MID_TEST/2);

    HashMapDeinit(&pMap);
}


void TestIterator()
{
    HashMap *pMap;
    CU_ASSERT(HashMapInit(&pMap) == SUCC);
    CU_ASSERT(pMap->set_destroy(pMap, DestroyPair) == SUCC);
    CU_ASSERT_EQUAL(pMap->size(pMap), 0);

    /* Insert the test data. */
    Pair *pPair;
    int32_t iIdx;
    for (iIdx = COUNT_ITER / 2 ; iIdx > 0 ; iIdx--) {
        Pair *pPair = (Pair*)malloc(sizeof(Pair));
        Employ *pEmploy = (Employ*)malloc(sizeof(Employ));

        pEmploy->cYear = iIdx % MASK_YEAR;
        pEmploy->cLevel = iIdx % MASK_LEVEL;
        pEmploy->iId = iIdx;

        pPair->key = aName[iIdx];
        pPair->value = (Value)pEmploy;
        CU_ASSERT(pMap->put(pMap, pPair, SIZE_MID_STR) == SUCC);
    }
    for (iIdx = COUNT_ITER / 2 + 1 ; iIdx <= COUNT_ITER ; iIdx++) {
        Pair *pPair = (Pair*)malloc(sizeof(Pair));
        Employ *pEmploy = (Employ*)malloc(sizeof(Employ));

        pEmploy->cYear = iIdx % MASK_YEAR;
        pEmploy->cLevel = iIdx % MASK_LEVEL;
        pEmploy->iId = iIdx;

        pPair->key = aName[iIdx];
        pPair->value = (Value)pEmploy;
        CU_ASSERT(pMap->put(pMap, pPair, SIZE_MID_STR) == SUCC);
    }

    /* Iterate through the map. */
    CU_ASSERT(pMap->iterate(pMap, true, NULL) == SUCC);
    while (pMap->iterate(pMap, false, &pPair) == CONTINUE);
    CU_ASSERT_EQUAL(pPair, NULL);

    CU_ASSERT(pMap->iterate(pMap, false, &pPair) == END);
    CU_ASSERT_EQUAL(pPair, NULL);

    HashMapDeinit(&pMap);
}