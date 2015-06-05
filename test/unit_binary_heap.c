#include "heap/binary_heap.h"
#include "CUnit/Util.h"
#include "CUnit/Basic.h"


/* The functions to register simple and advanced test suites. */
int32_t AddSimpleSuite();
void TestSimpleManipulate();


int32_t main()
{
    if (CU_initialize_registry() != CUE_SUCCESS)
        return CU_get_error();
    assert(CU_get_registry() != NULL);
    assert(!CU_is_test_running());

    /* Prepare the test suite for primitive item manipulation. */
    if (AddSimpleSuite() != SUCC) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Launch all the tests. */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    return SUCC;
}

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

    BinHeapDeinit(&pHeap);
}