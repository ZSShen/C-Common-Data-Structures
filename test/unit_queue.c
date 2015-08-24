#include "container/queue.h"
#include "CUnit/Util.h"
#include "CUnit/Basic.h"


#define SIZE_SMALL_TEST     (32)
#define MASK_YEAR           (100)
#define MASK_LEVEL          (200)

typedef struct Employ_ {
    int8_t cYear;
    int8_t cLevel;
    int32_t iId;
} Employ;


int32_t AddSuite();
void TestManipulate();
void TestBoundary();
void DestroyItem(Item);


int32_t main()
{
    int32_t rc = SUCC;

    if (CU_initialize_registry() != CUE_SUCCESS) {
        rc = CU_get_error();
        goto EXIT;
    }

    /* Register the test suite for queue structure verification. */
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
    return rc;
}

int32_t AddSuite()
{
    int32_t rc = SUCC;

    CU_pSuite pSuite = CU_add_suite("Structure Verification", NULL, NULL);
    if (!pSuite) {
        rc = ERR_REG;
        goto EXIT;
    }

    char *szMsg = "Combination with insertion, deletion, and order checking.";
    CU_pTest pTest = CU_add_test(pSuite, szMsg, TestManipulate);
    if (!pTest)
        rc = ERR_REG;

    pTest = CU_add_test(pSuite, "Boundary Testing", TestBoundary);
    if (!pTest)
        rc = ERR_REG;

EXIT:
    return rc;
}

void DestroyItem(Item item)
{
    free((Employ*)item);
}

void TestManipulate()
{
    Queue *pQueue;
    CU_ASSERT(QueueInit(&pQueue) == SUCC);
    CU_ASSERT(pQueue->set_destroy(pQueue, DestroyItem) == SUCC);

    /* Insert the initial data set. */
    int32_t iIdx;
    for (iIdx = 0 ; iIdx < SIZE_SMALL_TEST ; iIdx++) {
        Employ *pEmploy = (Employ*)malloc(sizeof(Employ));
        pEmploy->cYear = iIdx % MASK_YEAR;
        pEmploy->cLevel = iIdx % MASK_LEVEL;
        pEmploy->iId = iIdx;
        CU_ASSERT(pQueue->push(pQueue, (Item)pEmploy) == SUCC);
    }
    CU_ASSERT_EQUAL(pQueue->size(pQueue), SIZE_SMALL_TEST);

    /* Delete the first half of the data set. */
    for (iIdx = 0 ; iIdx < SIZE_SMALL_TEST / 2 ; iIdx++)
        CU_ASSERT(pQueue->pop(pQueue) == SUCC);

    /* Reinsert the deleted data set. */
    for (iIdx = 0 ; iIdx < SIZE_SMALL_TEST / 2 ; iIdx++) {
        Employ *pEmploy = (Employ*)malloc(sizeof(Employ));
        pEmploy->cYear = iIdx % MASK_YEAR;
        pEmploy->cLevel = iIdx % MASK_LEVEL;
        pEmploy->iId = iIdx;
        CU_ASSERT(pQueue->push(pQueue, (Item)pEmploy) == SUCC);
    }

    /* Check the queue order. */
    Item item;
    CU_ASSERT(pQueue->front(pQueue, &item) == SUCC);
    CU_ASSERT_EQUAL(SIZE_SMALL_TEST / 2, ((Employ*)item)->iId);
    CU_ASSERT(pQueue->back(pQueue, &item) == SUCC);
    CU_ASSERT_EQUAL(SIZE_SMALL_TEST / 2 - 1, ((Employ*)item)->iId);

    /* Insert another data set. */
    for (iIdx = SIZE_SMALL_TEST ; iIdx < SIZE_SMALL_TEST * 2 ; iIdx++) {
        Employ *pEmploy = (Employ*)malloc(sizeof(Employ));
        pEmploy->cYear = iIdx % MASK_YEAR;
        pEmploy->cLevel = iIdx % MASK_LEVEL;
        pEmploy->iId = iIdx;
        CU_ASSERT(pQueue->push(pQueue, (Item)pEmploy) == SUCC);
    }

    /* Delete the second quarter of the data set. */
    for (iIdx = 0 ; iIdx < SIZE_SMALL_TEST / 2 ; iIdx++)
        CU_ASSERT(pQueue->pop(pQueue) == SUCC);

    /* Check the queue order. */
    CU_ASSERT(pQueue->front(pQueue, &item) == SUCC);
    CU_ASSERT_EQUAL(0, ((Employ*)item)->iId);
    CU_ASSERT(pQueue->back(pQueue, &item) == SUCC);
    CU_ASSERT_EQUAL(SIZE_SMALL_TEST * 2 - 1, ((Employ*)item)->iId);

    QueueDeinit(&pQueue);
}

void TestBoundary()
{
    Queue *pQueue;
    CU_ASSERT(QueueInit(&pQueue) == SUCC);
    CU_ASSERT(pQueue->set_destroy(pQueue, DestroyItem) == SUCC);

    /* Pop the empty queue. */
    CU_ASSERT(pQueue->pop(pQueue) == ERR_IDX);

    /* Retrieve the items from the empty queue. */
    Item item;
    CU_ASSERT(pQueue->front(pQueue, &item) == ERR_IDX);
    CU_ASSERT_EQUAL(item, NULL);
    CU_ASSERT(pQueue->back(pQueue, &item) == ERR_IDX);
    CU_ASSERT_EQUAL(item, NULL);

    /* Insert the initial data set. */
    int iIdx;
    for (iIdx = 0 ; iIdx < SIZE_SMALL_TEST ; iIdx++) {
        Employ *pEmploy = (Employ*)malloc(sizeof(Employ));
        pEmploy->cYear = iIdx % MASK_YEAR;
        pEmploy->cLevel = iIdx % MASK_LEVEL;
        pEmploy->iId = iIdx;
        CU_ASSERT(pQueue->push(pQueue, (Item)pEmploy) == SUCC);
    }

    /* Delete all the data. */
    for (iIdx = 0 ; iIdx < SIZE_SMALL_TEST ; iIdx++)
        CU_ASSERT(pQueue->pop(pQueue) == SUCC);

    CU_ASSERT(pQueue->pop(pQueue) == ERR_IDX);
    CU_ASSERT(pQueue->front(pQueue, &item) == ERR_IDX);
    CU_ASSERT_EQUAL(item, NULL);
    CU_ASSERT(pQueue->back(pQueue, &item) == ERR_IDX);
    CU_ASSERT_EQUAL(item, NULL);
    CU_ASSERT_EQUAL(pQueue->size(pQueue), 0);

    /* Invalid parameter to retrieve queue data. */
    CU_ASSERT(pQueue->front(pQueue, NULL) == ERR_GET);
    CU_ASSERT(pQueue->back(pQueue, NULL) == ERR_GET);

    QueueDeinit(&pQueue);
}