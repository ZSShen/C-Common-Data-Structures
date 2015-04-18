#include "simple_tree.h"
#include "CUnit/Util.h"
#include "CUnit/Basic.h"


/*===========================================================================*
 *        Definition for the test cases of the primitive input suite         *
 *===========================================================================*/
void TestPrimInsert();


int32_t SuitePrimitive()
{
    CU_pSuite pSuite = CU_add_suite("Primitive Input", NULL, NULL);
    if (!pSuite)
        return ERR_NOMEM;

    CU_pTest pTest = CU_add_test(pSuite, "Basic item insertion", TestPrimInsert);
    if (!pTest)
        return ERR_NOMEM;

    return SUCC;
}


int32_t main()
{
    if (CU_initialize_registry() != CUE_SUCCESS)
        return CU_get_error();
    assert(CU_get_registry() != NULL);
    assert(!CU_is_test_running());

    /* Prepare the test suite for primitive input. */
    if (SuitePrimitive() != SUCC) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Launch all the tests. */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    return SUCC;
}


/*===========================================================================*
 *      Implementation for the test cases of the primitive input suite       *
 *===========================================================================*/
void TestPrimInsert()
{
    SimpleTree *pTree;
    CU_ASSERT(SimTreeInit(&pTree) == SUCC);

    /**
     * The vision of the test tree.
     *         (4)
     *        /   \
     *     (2)     (6)
     *     / \     / \
     *   (1) (3) (5) (7)
     */
    /* Insert the data to form a complete tree. */
    CU_ASSERT(pTree->insert(pTree, (Item)4) == SUCC);
    CU_ASSERT(pTree->insert(pTree, (Item)2) == SUCC);
    CU_ASSERT(pTree->insert(pTree, (Item)3) == SUCC);
    CU_ASSERT(pTree->insert(pTree, (Item)1) == SUCC);
    CU_ASSERT(pTree->insert(pTree, (Item)6) == SUCC);
    CU_ASSERT(pTree->insert(pTree, (Item)5) == SUCC);
    CU_ASSERT(pTree->insert(pTree, (Item)7) == SUCC);

    Item item;

    SimTreeDeinit(&pTree);
}
