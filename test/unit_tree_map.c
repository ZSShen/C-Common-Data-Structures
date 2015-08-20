#include "map/tree_map.h"
#include "CUnit/Util.h"
#include "CUnit/Basic.h"


/*------------------------------------------------------------*
 * Test Function Declaration for basic structure verification *
 *------------------------------------------------------------*/
int32_t AddBasicSuite();
void TestBasicInsert();
void TestBoundary();

void DestroyBasicPair(Pair*);
int32_t CompareBasicKey(Key, Key);


/*------------------------------------------------------------*
 *    Test Function Declaration for bulk data manipulation    *
 *------------------------------------------------------------*/
#define SIZE_MID_TEST       (10000)
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

int32_t AddBulkSuite();
void TestBulkManipulate();

int32_t PrepareTestData();
void ReleaseTestData();

void DestroyBulkPair(Pair*);
int32_t CompareBulkKey(Key, Key);


int32_t main()
{
    int32_t rc = SUCC;

    if (PrepareTestData() != SUCC)
        goto EXIT;

    if (CU_initialize_registry() != CUE_SUCCESS) {
        rc = CU_get_error();
        goto EXIT;
    }

    /* Register the test suite for basic structure verification. */
    if (AddBasicSuite() != SUCC) {
        rc = CU_get_error();
        goto CLEAN;
    }

    /* Register the test suite for bulk data manipulation. */
    if (AddBulkSuite() != SUCC) {
        rc = CU_get_error();
        goto CLEAN;
    }

    /* Launch all the tests. */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

CLEAN:
    CU_cleanup_registry();
    ReleaseTestData();
EXIT:
    return rc;
}


/*------------------------------------------------------------*
 *        Test Function Implementation for Basic Suite        *
 *------------------------------------------------------------*/
void DestroyBasicPair(Pair *pPair) { free(pPair); }

int32_t CompareBasicKey(Key keySrc, Key keyTge)
{
    if (keySrc == keyTge)
        return 0;
    return (keySrc > keyTge)? 1 : (-1);
}

int32_t AddBasicSuite()
{
    CU_pSuite pSuite = CU_add_suite("Basic Structure Verification", NULL, NULL);
    if (!pSuite)
        return ERR_REG;

    CU_pTest pTest = CU_add_test(pSuite, "Pair insertion and structure verification",
                     TestBasicInsert);
    if (!pTest)
        return ERR_REG;

    pTest = CU_add_test(pSuite, "Key search and boundary case handling",
            TestBoundary);
    if (!pTest)
        return ERR_REG;

    return SUCC;
}

void TestBasicInsert()
{
    TreeMap *pMap;
    CU_ASSERT(TreeMapInit(&pMap) == SUCC);

    CU_ASSERT(pMap->set_compare(pMap, CompareBasicKey) == SUCC);
    CU_ASSERT(pMap->set_destroy(pMap, DestroyBasicPair) == SUCC);

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
    Pair *pPair = (Pair*)malloc(sizeof(Pair));
    pPair->key = (Key)10; pPair->value = 0;
    CU_ASSERT(pMap->put(pMap, pPair) == SUCC);

    pPair = (Pair*)malloc(sizeof(Pair));
    pPair->key = (Key)15; pPair->value = 0;
    CU_ASSERT(pMap->put(pMap, pPair) == SUCC);

    pPair = (Pair*)malloc(sizeof(Pair));
    pPair->key = (Key)20; pPair->value = 0;
    CU_ASSERT(pMap->put(pMap, pPair) == SUCC);

    pPair = (Pair*)malloc(sizeof(Pair));
    pPair->key = (Key)25; pPair->value = 0;
    CU_ASSERT(pMap->put(pMap, pPair) == SUCC);

    pPair = (Pair*)malloc(sizeof(Pair));
    pPair->key = (Key)22; pPair->value = 0;
    CU_ASSERT(pMap->put(pMap, pPair) == SUCC);

    pPair = (Pair*)malloc(sizeof(Pair));
    pPair->key = (Key)9; pPair->value = 0;
    CU_ASSERT(pMap->put(pMap, pPair) == SUCC);

    pPair = (Pair*)malloc(sizeof(Pair));
    pPair->key = (Key)6; pPair->value = 0;
    CU_ASSERT(pMap->put(pMap, pPair) == SUCC);

    pPair = (Pair*)malloc(sizeof(Pair));
    pPair->key = (Key)1; pPair->value = 0;
    CU_ASSERT(pMap->put(pMap, pPair) == SUCC);

    pPair = (Pair*)malloc(sizeof(Pair));
    pPair->key = (Key)4; pPair->value = 0;
    CU_ASSERT(pMap->put(pMap, pPair) == SUCC);

    pPair = (Pair*)malloc(sizeof(Pair));
    pPair->key = (Key)7; pPair->value = 0;
    CU_ASSERT(pMap->put(pMap, pPair) == SUCC);

    /* Check structure correctness. */
    CU_ASSERT(pMap->predecessor(pMap, (Key)4, &pPair) == SUCC);
    CU_ASSERT_EQUAL(pPair->key, (Key)1);
    CU_ASSERT(pMap->successor(pMap, (Key)4, &pPair) == SUCC);
    CU_ASSERT_EQUAL(pPair->key, (Key)6);

    CU_ASSERT(pMap->predecessor(pMap, (Key)6, &pPair) == SUCC);
    CU_ASSERT_EQUAL(pPair->key, (Key)4);
    CU_ASSERT(pMap->successor(pMap, (Key)6, &pPair) == SUCC);
    CU_ASSERT_EQUAL(pPair->key, (Key)7);

    CU_ASSERT(pMap->predecessor(pMap, (Key)7, &pPair) == SUCC);
    CU_ASSERT_EQUAL(pPair->key, (Key)6);
    CU_ASSERT(pMap->successor(pMap, (Key)7, &pPair) == SUCC);
    CU_ASSERT_EQUAL(pPair->key, (Key)9);

    CU_ASSERT(pMap->predecessor(pMap, (Key)9, &pPair) == SUCC);
    CU_ASSERT_EQUAL(pPair->key, (Key)7);
    CU_ASSERT(pMap->successor(pMap, (Key)9, &pPair) == SUCC);
    CU_ASSERT_EQUAL(pPair->key, (Key)10);

    CU_ASSERT(pMap->predecessor(pMap, (Key)10, &pPair) == SUCC);
    CU_ASSERT_EQUAL(pPair->key, (Key)9);
    CU_ASSERT(pMap->successor(pMap, (Key)10, &pPair) == SUCC);
    CU_ASSERT_EQUAL(pPair->key, (Key)15);

    CU_ASSERT(pMap->predecessor(pMap, (Key)15, &pPair) == SUCC);
    CU_ASSERT_EQUAL(pPair->key, (Key)10);
    CU_ASSERT(pMap->successor(pMap, (Key)15, &pPair) == SUCC);
    CU_ASSERT_EQUAL(pPair->key, (Key)20);

    CU_ASSERT(pMap->predecessor(pMap, (Key)20, &pPair) == SUCC);
    CU_ASSERT_EQUAL(pPair->key, (Key)15);
    CU_ASSERT(pMap->successor(pMap, (Key)20, &pPair) == SUCC);
    CU_ASSERT_EQUAL(pPair->key, (Key)22);

    CU_ASSERT(pMap->predecessor(pMap, (Key)22, &pPair) == SUCC);
    CU_ASSERT_EQUAL(pPair->key, (Key)20);
    CU_ASSERT(pMap->successor(pMap, (Key)22, &pPair) == SUCC);
    CU_ASSERT_EQUAL(pPair->key, (Key)25);

    /* Check the minimum and maximum item. */
    CU_ASSERT(pMap->minimum(pMap, &pPair) == SUCC);
    CU_ASSERT_EQUAL(pPair->key, (Key)1);
    CU_ASSERT(pMap->maximum(pMap, &pPair) == SUCC);
    CU_ASSERT_EQUAL(pPair->key, (Key)25);

    /* Check the map size. */
    CU_ASSERT_EQUAL(pMap->size(pMap), 10);

    TreeMapDeinit(&pMap);
}

void TestBoundary()
{
    TreeMap *pMap;
    CU_ASSERT(TreeMapInit(&pMap) == SUCC);
    CU_ASSERT(pMap->set_compare(pMap, CompareBasicKey) == SUCC);
    CU_ASSERT(pMap->set_destroy(pMap, DestroyBasicPair) == SUCC);
    CU_ASSERT_EQUAL(pMap->size(pMap), 0);

    /* Search data from the empty map. */
    Value value;
    CU_ASSERT(pMap->get(pMap, (Key)0, &value) == ERR_NODATA);
    CU_ASSERT_EQUAL(value, NULL);
    CU_ASSERT(pMap->find(pMap, (Key)0) == NOKEY);

    /* Search data from the non-empty map. */
    Pair *pPair = (Pair*)malloc(sizeof(Pair));
    pPair->key = (Key)1; pPair->value = (Value)100;
    CU_ASSERT(pMap->put(pMap, pPair) == SUCC);

    pPair = (Pair*)malloc(sizeof(Pair));
    pPair->key = (Key)0; pPair->value = (Value)200;
    CU_ASSERT(pMap->put(pMap, pPair) == SUCC);

    CU_ASSERT(pMap->get(pMap, (Key)0, &value) == SUCC);
    CU_ASSERT_EQUAL(value, (Value)200);
    CU_ASSERT(pMap->find(pMap, (Key)0) == SUCC);

    CU_ASSERT(pMap->remove(pMap, (Key)0) == SUCC);

    CU_ASSERT(pMap->get(pMap, (Key)0, &value) == ERR_NODATA);
    CU_ASSERT_EQUAL(value, NULL);
    CU_ASSERT(pMap->find(pMap, (Key)0) == NOKEY);

    /* Test boundary cases. */
    CU_ASSERT(pMap->predecessor(pMap, (Key)0, &pPair) == ERR_NODATA);
    CU_ASSERT(pMap->successor(pMap, (Key)0, &pPair) == ERR_NODATA);
    CU_ASSERT(pMap->predecessor(pMap, (Key)1, &pPair) == ERR_NODATA);
    CU_ASSERT(pMap->successor(pMap, (Key)1, &pPair) == ERR_NODATA);

    CU_ASSERT(pMap->remove(pMap, (Key)1) == SUCC);
    CU_ASSERT(pMap->maximum(pMap, &pPair) == ERR_IDX);
    CU_ASSERT(pMap->minimum(pMap, &pPair) == ERR_IDX);
    CU_ASSERT(pMap->remove(pMap, (Key)1) == ERR_NODATA);

    TreeMapDeinit(&pMap);
}

/*------------------------------------------------------------*
 *        Test Function Implementation for Bulk Suite         *
 *------------------------------------------------------------*/
int32_t CompareBulkKey(Key keySrc, Key keyTge)
{
    char *nameSrc = (char*)keySrc;
    char *nameTge = (char*)keyTge;

    int32_t iOrder = strcmp(nameSrc, nameTge);
    if (iOrder == 0)
        return 0;
    return (iOrder > 0)? 1 : (-1);
}

void DestroyBulkPair(Pair *pPair)
{
    free((Employ*)pPair->value);
    free(pPair);
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

int32_t AddBulkSuite()
{
    int32_t rc = SUCC;

    CU_pSuite pSuite = CU_add_suite("Bulk date manipulation", NULL, NULL);
    if (!pSuite) {
        rc = ERR_REG;
        goto EXIT;
    }

    char *szMsg = "Combination with insertion, deletion, and searching.";
    CU_pTest pTest = CU_add_test(pSuite, szMsg, TestBulkManipulate);
    if (!pTest)
        rc = ERR_REG;

EXIT:
    return rc;
}

void TestBulkManipulate()
{
    TreeMap *pMap;
    CU_ASSERT(TreeMapInit(&pMap) == SUCC);
    CU_ASSERT(pMap->set_compare(pMap, CompareBulkKey) == SUCC);
    CU_ASSERT(pMap->set_destroy(pMap, DestroyBulkPair) == SUCC);

    /* Insert the full data. */
    int32_t iIdx;
    for (iIdx = 0 ; iIdx < SIZE_MID_TEST ; iIdx++) {
        Pair *pPair = (Pair*)malloc(sizeof(Pair));
        Employ *pEmploy = (Employ*)malloc(sizeof(Employ));

        pEmploy->cYear = aNum[iIdx] / MASK_YEAR;
        pEmploy->cLevel = aNum[iIdx] / MASK_LEVEL;
        pEmploy->iId = aNum[iIdx];

        pPair->key = aName[iIdx];
        pPair->value = (Value)pEmploy;
        CU_ASSERT(pMap->put(pMap, pPair) == SUCC);
    }

    /* Search and Retrieve the first half of the data. It should succeed. */
    for (iIdx = 0 ; iIdx < SIZE_MID_TEST/2 ; iIdx++) {
        Value valueRetv;
        CU_ASSERT(pMap->find(pMap, aName[iIdx]) == SUCC);
        CU_ASSERT(pMap->get(pMap, aName[iIdx], &valueRetv) == SUCC);
        CU_ASSERT_EQUAL(aNum[iIdx]/MASK_YEAR, ((Employ*)valueRetv)->cYear);
        CU_ASSERT_EQUAL(aNum[iIdx]/MASK_LEVEL, ((Employ*)valueRetv)->cLevel);
        CU_ASSERT_EQUAL(aNum[iIdx], ((Employ*)valueRetv)->iId);
    }

    CU_ASSERT_EQUAL(pMap->size(pMap), SIZE_MID_TEST);

    /* Delete the second half of the data.  */
    for (iIdx = SIZE_MID_TEST/2 ; iIdx < SIZE_MID_TEST ; iIdx++)
        CU_ASSERT(pMap->remove(pMap, (Key)aName[iIdx]) == SUCC);

    /* Search and Retrieve the second half of the data. It should fail. */
    for (iIdx = SIZE_MID_TEST/2 ; iIdx < SIZE_MID_TEST ; iIdx++) {
        Value valueRetv;
        CU_ASSERT(pMap->find(pMap, aName[iIdx]) == NOKEY);
        CU_ASSERT(pMap->get(pMap, aName[iIdx], &valueRetv) == ERR_NODATA);
        CU_ASSERT_EQUAL(valueRetv, NULL);
    }

    /* But the searching for the first half should not be affected. */
    for (iIdx = 0 ; iIdx < SIZE_MID_TEST/2 ; iIdx++) {
        Value valueRetv;
        CU_ASSERT(pMap->find(pMap, aName[iIdx]) == SUCC);
        CU_ASSERT(pMap->get(pMap, aName[iIdx], &valueRetv) == SUCC);
        CU_ASSERT_EQUAL(aNum[iIdx]/MASK_YEAR, ((Employ*)valueRetv)->cYear);
        CU_ASSERT_EQUAL(aNum[iIdx]/MASK_LEVEL, ((Employ*)valueRetv)->cLevel);
        CU_ASSERT_EQUAL(aNum[iIdx], ((Employ*)valueRetv)->iId);
    }

    /* Delete the already deleted second half of the data. It should fail. */
    for (iIdx = SIZE_MID_TEST/2 ; iIdx < SIZE_MID_TEST ; iIdx++)
        CU_ASSERT(pMap->remove(pMap, (Key)aName[iIdx]) == ERR_NODATA);

    CU_ASSERT_EQUAL(pMap->size(pMap), SIZE_MID_TEST/2);

    TreeMapDeinit(&pMap);
}