#include "vector.h"
#include "CUnit/Util.h"
#include "CUnit/Basic.h"


/*===========================================================================*
 *        Definition for the test cases of the primitive data suite          *
 *===========================================================================*/
void TestPrimPushBack();
void TestPrimInsert();


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


/*===========================================================================*
 *      Implementation for the test cases of the primitive data suite        *
 *===========================================================================*/
void TestPrimPushBack()
{
    Vector *pVec;
    CU_ASSERT(VectorInit(&pVec) == SUCCESS);

    /* Append the items. */
    CU_ASSERT(pVec->push_back(pVec, (Item)1) == SUCCESS);
    CU_ASSERT(pVec->push_back(pVec, (Item)2) == SUCCESS);
    CU_ASSERT(pVec->push_back(pVec, (Item)3) == SUCCESS);
    CU_ASSERT(pVec->push_back(pVec, (Item)4) == SUCCESS);

    /* Check the item insertion sequence. */
    Item item;
    CU_ASSERT(pVec->get(pVec, &item, 0) == SUCCESS);
    CU_ASSERT_EQUAL(item, (Item)1);
    CU_ASSERT(pVec->get(pVec, &item, 1) == SUCCESS);
    CU_ASSERT_EQUAL(item, (Item)2);
    CU_ASSERT(pVec->get(pVec, &item, 2) == SUCCESS);
    CU_ASSERT_EQUAL(item, (Item)3);
    CU_ASSERT(pVec->get(pVec, &item, 3) == SUCCESS);
    CU_ASSERT_EQUAL(item, (Item)4);

    /* Check the vector storage. */
    CU_ASSERT_EQUAL(pVec->size(pVec), 4);
    CU_ASSERT_EQUAL(pVec->capacity(pVec), 4);

    /* Check for the illegal indexing. */
    CU_ASSERT(pVec->get(pVec, &item, -1) == FAIL_OUT_OF_RANGE);
    CU_ASSERT(pVec->get(pVec, &item, 4) == FAIL_OUT_OF_RANGE);

    VectorDeinit(&pVec);
}

void TestPrimInsert()
{
    Vector *pVec;
    CU_ASSERT(VectorInit(&pVec) == SUCCESS);

    /* Check the same effect as push_back(). */
    CU_ASSERT(pVec->insert(pVec, (Item)3, 0) == SUCCESS);
    CU_ASSERT(pVec->insert(pVec, (Item)4, 1) == SUCCESS);

    /* Check the shift for trailing items. */
    CU_ASSERT(pVec->insert(pVec, (Item)1, 0) == SUCCESS);
    CU_ASSERT(pVec->insert(pVec, (Item)2, 1) == SUCCESS);

    /* Check the item insertion sequence. */
    Item item;
    CU_ASSERT(pVec->get(pVec, &item, 0) == SUCCESS);
    CU_ASSERT_EQUAL(item, (Item)1);
    CU_ASSERT(pVec->get(pVec, &item, 1) == SUCCESS);
    CU_ASSERT_EQUAL(item, (Item)2);
    CU_ASSERT(pVec->get(pVec, &item, 2) == SUCCESS);
    CU_ASSERT_EQUAL(item, (Item)3);
    CU_ASSERT(pVec->get(pVec, &item, 3) == SUCCESS);
    CU_ASSERT_EQUAL(item, (Item)4);

    /* Check the vector storage. */
    CU_ASSERT_EQUAL(pVec->size(pVec), 4);
    CU_ASSERT_EQUAL(pVec->capacity(pVec), 4);

    /* Check for the illegal indexing. */
    CU_ASSERT(pVec->insert(pVec, (Item)-1, -1) == FAIL_OUT_OF_RANGE);
    CU_ASSERT(pVec->insert(pVec, (Item)-1, 5) == FAIL_OUT_OF_RANGE);

    VectorDeinit(&pVec);
}

