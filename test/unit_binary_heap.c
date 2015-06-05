#include "heap/binary_heap.h"
#include "CUnit/Util.h"
#include "CUnit/Basic.h"


#define SIZE_MID_TEST       (1000)

int32_t aPrim[SIZE_MID_TEST];


/* The functions to prepare and release test data. */
int32_t PrepareTestData();
void ReleaseTestData();


/* The item comparison methods. */
int32_t CompPrimMax(Item, Item);


/* The item clean methods. */


/* The functions to register simple and advanced test suites. */
int32_t AddSimpleSuite();
void TestSimpleManipulate();


int32_t main()
{
    int32_t iRtnCode = SUCC;

    iRtnCode = PrepareTestData();
    if (iRtnCode != SUCC)
        goto EXIT;

    if (CU_initialize_registry() != CUE_SUCCESS)
        return CU_get_error();

    /* Prepare the test suite for primitive item manipulation. */
    if (AddSimpleSuite() != SUCC) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Launch all the tests. */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
CLEAN:
    ReleaseTestData();
EXIT:
    return iRtnCode;
}


int32_t PrepareTestData()
{
    int32_t iRtnCode = SUCC;

    int32_t idx = 0;
    while (idx < SIZE_MID_TEST)
        aPrim[idx++] = idx;

    return iRtnCode;
}


void ReleaseTestData() {}


int32_t AddSimpleSuite()
{
    CU_pSuite pSuite = CU_add_suite("Simple Test Suite", NULL, NULL);
    if (!pSuite)
        return ERR_NOMEM;

    CU_pTest pTest = CU_add_test(pSuite, "Primitive item manipulation",
                     TestSimpleManipulate);
    if (!pTest)
        return ERR_NOMEM;

    return SUCC;
}


void TestSimpleManipulate()
{
    BinHeap *pHeap;
    CU_ASSERT(BinHeapInit(&pHeap) == SUCC);

    /* Build the maximum heap. */
    CU_ASSERT(pHeap->set_compare(pHeap, CompPrimMax) == SUCC);

    /* Push items into the heap. */
    int32_t idx = 0;
    Item item;
    while (idx < SIZE_MID_TEST) {
        #if __x86_64__
        CU_ASSERT(pHeap->push(pHeap, (Item)(int64_t)aPrim[idx]) == SUCC);
        CU_ASSERT(pHeap->top(pHeap, &item) == SUCC);
        CU_ASSERT_EQUAL(item, (Item)(int64_t)aPrim[idx]);
        #else
        CU_ASSERT(pHeap->push(pHeap, (Item)aPrim[idx]) == SUCC);
        CU_ASSERT(pHeap->top(pHeap, &item) == SUCC);
        CU_ASSERT_EQUAL(item, aPrim[idx]);
        #endif
        idx++;
    }

    BinHeapDeinit(&pHeap);
}


int32_t CompPrimMax(Item itemSrc, Item itemTge)
{
    if (itemSrc == itemTge)
        return 0;
    return (itemSrc > itemTge)? 1 : (-1);
}
