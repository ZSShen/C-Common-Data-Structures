#include "container/hash_set.h"
#include "math/hash.h"
#include "CUnit/Util.h"
#include "CUnit/Basic.h"


/*------------------------------------------------------------*
 *    Test Function Declaration for Structure Verification    *
 *------------------------------------------------------------*/
#define COUNT_ITER          (1000)
#define SIZE_MID_TEST       (10000)
#define SIZE_MID_STR        (17)
#define RANGE_CHAR          (26)
#define BASE_CHAR           (97)
#define MASK_YEAR           (50)
#define MASK_LEVEL          (100)


char* aName[SIZE_MID_TEST];

int32_t AddSuite();
void TestManipulate();
void TestIterator();
void TestUnionOperation();
void TestIntersectOperation();
void TestDifferenceOperation();

int32_t PrepareTestData();
void ReleaseTestData();


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

    /* Register the test suite for set structure verification. */
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

    pTest = CU_add_test(pSuite, "Set iterator.", TestIterator);
    if (!pTest)
        rc = ERR_REG;

    pTest = CU_add_test(pSuite, "Set Union Operation.", TestUnionOperation);
    if (!pTest)
        rc = ERR_REG;

    pTest = CU_add_test(pSuite, "Set Intersection Operation.", TestIntersectOperation);
    if (!pTest)
        rc = ERR_REG;

    pTest = CU_add_test(pSuite, "Set Difference Operation.", TestDifferenceOperation);
    if (!pTest)
        rc = ERR_REG;

EXIT:
    return rc;
}

void TestManipulate()
{
    HashSet *pSet;
    CU_ASSERT(HashSetInit(&pSet) == SUCC);

    /* Insert the full data set. */
    int32_t iIdx;
    for (iIdx = 0 ; iIdx < SIZE_MID_TEST ; iIdx++)
        CU_ASSERT(pSet->add(pSet, (Key)aName[iIdx], SIZE_MID_STR) == SUCC);

    /* Search for the first half of the data. It should succeed. */
    for (iIdx = 0 ; iIdx < SIZE_MID_TEST / 2 ; iIdx++)
        CU_ASSERT(pSet->find(pSet, (Key)aName[iIdx], SIZE_MID_STR) == SUCC);

    CU_ASSERT_EQUAL(pSet->size(pSet), SIZE_MID_TEST);

    /* Delete the second half of the data. */
    for (iIdx = SIZE_MID_TEST / 2 ; iIdx < SIZE_MID_TEST ; iIdx++)
        CU_ASSERT(pSet->remove(pSet, (Key)aName[iIdx], SIZE_MID_STR) == SUCC);

    /* Search for the second half of the data. It should fail. */
    for (iIdx = SIZE_MID_TEST / 2 ; iIdx < SIZE_MID_TEST ; iIdx++)
        CU_ASSERT(pSet->find(pSet, (Key)aName[iIdx], SIZE_MID_STR) == NOKEY);

    /* But the searching for the first half should not be affected. */
    for (iIdx = 0 ; iIdx < SIZE_MID_TEST / 2 ; iIdx++)
        CU_ASSERT(pSet->find(pSet, (Key)aName[iIdx], SIZE_MID_STR) == SUCC);

    /* Delete the already deleted second half of the data. It should fail. */
    for (iIdx = SIZE_MID_TEST / 2 ; iIdx < SIZE_MID_TEST ; iIdx++)
        CU_ASSERT(pSet->remove(pSet, (Key)aName[iIdx], SIZE_MID_STR) == ERR_NODATA);

    CU_ASSERT_EQUAL(pSet->size(pSet), SIZE_MID_TEST / 2);

    HashSetDeinit(&pSet);
}

void TestIterator()
{
    HashSet *pSet;
    CU_ASSERT(HashSetInit(&pSet) == SUCC);

    /* Insert the initial data set. */
    int32_t iIdx;
    for (iIdx = COUNT_ITER / 2 ; iIdx > 0 ; iIdx--)
        CU_ASSERT(pSet->add(pSet, (Key)aName[iIdx], SIZE_MID_STR) == SUCC);

    for (iIdx = COUNT_ITER / 2 + 1 ; iIdx <= COUNT_ITER ; iIdx++)
        CU_ASSERT(pSet->add(pSet, (Key)aName[iIdx], SIZE_MID_STR) == SUCC);

    Key key;
    CU_ASSERT(pSet->iterate(pSet, true, NULL) == SUCC);
    while (pSet->iterate(pSet, false, &key) == CONTINUE);
    CU_ASSERT_EQUAL(key, NULL);
    CU_ASSERT(pSet->iterate(pSet, false, &key) == END);
    CU_ASSERT_EQUAL(key, NULL);

    HashSetDeinit(&pSet);
}

void TestUnionOperation()
{
    HashSet *pSrcFst;
    CU_ASSERT(HashSetInit(&pSrcFst) == SUCC);
    int32_t iIdx;
    for (iIdx = 0 ; iIdx < COUNT_ITER / 2 ; iIdx++)
        CU_ASSERT(pSrcFst->add(pSrcFst, (Key)aName[iIdx], SIZE_MID_STR) == SUCC);

    HashSet *pSrcSnd;
    CU_ASSERT(HashSetInit(&pSrcSnd) == SUCC);
    for (iIdx = COUNT_ITER - 1 ; iIdx >= COUNT_ITER / 2 ; iIdx--)
        CU_ASSERT(pSrcSnd->add(pSrcSnd, (Key)aName[iIdx], SIZE_MID_STR) == SUCC);

    HashSet *pInter;
    CU_ASSERT(HashSetUnion(pSrcFst, pSrcSnd, &pInter) == SUCC);

    for (iIdx = 0 ; iIdx < COUNT_ITER ; iIdx++)
        CU_ASSERT(pInter->find(pInter, (Key)aName[iIdx], SIZE_MID_STR) == SUCC);
    CU_ASSERT_EQUAL(pInter->size(pInter), COUNT_ITER);

    HashSetDeinit(&pSrcFst);
    HashSetDeinit(&pSrcSnd);
    HashSetDeinit(&pInter);
}

void TestIntersectOperation()
{
    HashSet *pSrcFst;
    CU_ASSERT(HashSetInit(&pSrcFst) == SUCC);
    int32_t iBgn = 0;
    int32_t iEnd = COUNT_ITER * 3 / 4;
    int32_t iIdx;
    for (iIdx = 0 ; iIdx < iEnd ; iIdx++) {
        CU_ASSERT(pSrcFst->add(pSrcFst, (Key)aName[iIdx], SIZE_MID_STR) == SUCC);
    }

    HashSet *pSrcSnd;
    CU_ASSERT(HashSetInit(&pSrcSnd) == SUCC);
    iBgn = COUNT_ITER / 4;
    iEnd = COUNT_ITER - 1;
    for (iIdx = iEnd ; iIdx >= iBgn ; iIdx--)
        CU_ASSERT(pSrcSnd->add(pSrcSnd, (Key)aName[iIdx], SIZE_MID_STR) == SUCC);

    HashSet *pInter;
    CU_ASSERT(HashSetIntersect(pSrcFst, pSrcSnd, &pInter) == SUCC);

    iBgn = 0;
    iEnd = COUNT_ITER / 4;
    for (iIdx = iBgn ; iIdx < iEnd ; iIdx++)
        CU_ASSERT(pInter->find(pInter, (Key)aName[iIdx], SIZE_MID_STR) == NOKEY);
    iBgn = COUNT_ITER / 4;
    iEnd = COUNT_ITER * 3 / 4;
    for (iIdx = iBgn ; iIdx < iEnd ; iIdx++)
        CU_ASSERT(pInter->find(pInter, (Key)aName[iIdx], SIZE_MID_STR) == SUCC);
    iBgn = COUNT_ITER * 3 / 4;
    iEnd = COUNT_ITER;
    for (iIdx = iBgn ; iIdx < iEnd ; iIdx++)
        CU_ASSERT(pInter->find(pInter, (Key)aName[iIdx], SIZE_MID_STR) == NOKEY);

    CU_ASSERT_EQUAL(pInter->size(pInter), COUNT_ITER / 2);

    HashSetDeinit(&pSrcFst);
    HashSetDeinit(&pSrcSnd);
    HashSetDeinit(&pInter);
}

void TestDifferenceOperation()
{
    HashSet *pSrcFst;
    CU_ASSERT(HashSetInit(&pSrcFst) == SUCC);
    int32_t iBgn = 0;
    int32_t iEnd = COUNT_ITER * 3 / 4;
    int32_t iIdx;
    for (iIdx = 0 ; iIdx < iEnd ; iIdx++) {
        CU_ASSERT(pSrcFst->add(pSrcFst, (Key)aName[iIdx], SIZE_MID_STR) == SUCC);
    }

    HashSet *pSrcSnd;
    CU_ASSERT(HashSetInit(&pSrcSnd) == SUCC);
    iBgn = COUNT_ITER / 4;
    iEnd = COUNT_ITER - 1;
    for (iIdx = iEnd ; iIdx >= iBgn ; iIdx--)
        CU_ASSERT(pSrcSnd->add(pSrcSnd, (Key)aName[iIdx], SIZE_MID_STR) == SUCC);

    HashSet *pFstOnly;
    CU_ASSERT(HashSetDifference(pSrcFst, pSrcSnd, &pFstOnly) == SUCC);
    HashSet *pSndOnly;
    CU_ASSERT(HashSetDifference(pSrcSnd, pSrcFst, &pSndOnly) == SUCC);

    iBgn = 0;
    iEnd = COUNT_ITER / 4;
    for (iIdx = iBgn ; iIdx < iEnd ; iIdx++) {
        CU_ASSERT(pFstOnly->find(pFstOnly, (Key)aName[iIdx], SIZE_MID_STR) == SUCC);
        CU_ASSERT(pSndOnly->find(pSndOnly, (Key)aName[iIdx], SIZE_MID_STR) == NOKEY);
    }
    iBgn = COUNT_ITER / 4;
    iEnd = COUNT_ITER * 3 / 4;
    for (iIdx = iBgn ; iIdx < iEnd ; iIdx++) {
        CU_ASSERT(pFstOnly->find(pFstOnly, (Key)aName[iIdx], SIZE_MID_STR) == NOKEY);
        CU_ASSERT(pSndOnly->find(pSndOnly, (Key)aName[iIdx], SIZE_MID_STR) == NOKEY);
    }
    iBgn = COUNT_ITER * 3 / 4;
    iEnd = COUNT_ITER;
    for (iIdx = iBgn ; iIdx < iEnd ; iIdx++) {
        CU_ASSERT(pFstOnly->find(pFstOnly, (Key)aName[iIdx], SIZE_MID_STR) == NOKEY);
        CU_ASSERT(pSndOnly->find(pSndOnly, (Key)aName[iIdx], SIZE_MID_STR) == SUCC);
    }

    CU_ASSERT_EQUAL(pFstOnly->size(pFstOnly), COUNT_ITER / 4);
    CU_ASSERT_EQUAL(pSndOnly->size(pSndOnly), COUNT_ITER / 4);

    HashSetDeinit(&pSrcFst);
    HashSetDeinit(&pSrcSnd);
    HashSetDeinit(&pFstOnly);
    HashSetDeinit(&pSndOnly);
}
