#include "vector.h"
#include "CUnit/Util.h"
#include "CUnit/Basic.h"


void TestPrimPushBack()
{
    Vector *pVec;
    CU_ASSERT(VectorInit(&pVec) == SUCCESS);

    CU_ASSERT(pVec->push_back(pVec, (Item)1) == SUCCESS);
    CU_ASSERT(pVec->push_back(pVec, (Item)2) == SUCCESS);
    CU_ASSERT(pVec->push_back(pVec, (Item)3) == SUCCESS);
    CU_ASSERT(pVec->push_back(pVec, (Item)4) == SUCCESS);

    CU_ASSERT_EQUAL(pVec->at(pVec, 0), (Item)1);
    CU_ASSERT_EQUAL(pVec->at(pVec, 1), (Item)2);
    CU_ASSERT_EQUAL(pVec->at(pVec, 2), (Item)3);
    CU_ASSERT_EQUAL(pVec->at(pVec, 3), (Item)4);

    VectorDeinit(&pVec);
}

void TestPrimInsert()
{
    Vector *pVec;
    CU_ASSERT(VectorInit(&pVec) == SUCCESS);

    CU_ASSERT(pVec->insert(pVec, (Item)0, 0) == FAIL_OUT_OF_RANGE);
    CU_ASSERT(pVec->push_back(pVec, (Item)4) == SUCCESS);
    CU_ASSERT(pVec->insert(pVec, (Item)2, 0) == SUCCESS);
    CU_ASSERT(pVec->insert(pVec, (Item)3, 1) == SUCCESS);
    CU_ASSERT(pVec->insert(pVec, (Item)1, 0) == SUCCESS);

    CU_ASSERT_EQUAL(pVec->at(pVec, 0), (Item)1);
    CU_ASSERT_EQUAL(pVec->at(pVec, 1), (Item)2);
    CU_ASSERT_EQUAL(pVec->at(pVec, 2), (Item)3);
    CU_ASSERT_EQUAL(pVec->at(pVec, 3), (Item)4);

    VectorDeinit(&pVec);
}

void TestPrimResize()
{
    Vector *pVec;
    CU_ASSERT(VectorInit(&pVec) == SUCCESS);



    VectorDeinit(&pVec);
}

int32_t SuitePrimitive()
{
    CU_pSuite pSuite = CU_add_suite("Primitive Input", NULL, NULL);
    if (!pSuite)
        return FAIL_NO_MEMORY;

    CU_pTest pTest = CU_add_test(pSuite, "Basic push_back()", TestPrimPushBack);
    if (!pTest)
        return FAIL_NO_MEMORY;

    pTest = CU_add_test(pSuite, "Basic insert()", TestPrimInsert);
    if (!pTest)
        return FAIL_NO_MEMORY;

    return SUCCESS;
}


int32_t main()
{
    if (CU_initialize_registry() != CUE_SUCCESS)
        return CU_get_error();
    assert(CU_get_registry() != NULL);
    assert(!CU_is_test_running());

    /* Prepare the test suite for primitive input. */
    if (SuitePrimitive() != SUCCESS) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Launch all the tests. */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    return SUCCESS;
}
