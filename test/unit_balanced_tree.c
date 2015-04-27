#include "tree/balanced_tree.h"
#include "CUnit/Util.h"
#include "CUnit/Basic.h"


/*===========================================================================*
 *        Definition for the test cases of the primitive input suite         *
 *===========================================================================*/
void TestPrimInsertAndOrder();


int32_t SuitePrimitive()
{
    CU_pSuite pSuite = CU_add_suite("Primitive Input", NULL, NULL);
    if (!pSuite)
        return ERR_NOMEM;

    CU_pTest pTest = CU_add_test(pSuite, "Item insertion and structure verification",
                     TestPrimInsertAndOrder);
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
void TestPrimInsertAndOrder()
{
    BalancedTree *pTree;
    CU_ASSERT(BalTreeInit(&pTree) == SUCC);

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
    CU_ASSERT(pTree->insert(pTree, (Item)10) == SUCC);
    CU_ASSERT(pTree->insert(pTree, (Item)15) == SUCC);
    CU_ASSERT(pTree->insert(pTree, (Item)20) == SUCC);
    CU_ASSERT(pTree->insert(pTree, (Item)25) == SUCC);
    CU_ASSERT(pTree->insert(pTree, (Item)22) == SUCC);
    CU_ASSERT(pTree->insert(pTree, (Item)9) == SUCC);
    CU_ASSERT(pTree->insert(pTree, (Item)6) == SUCC);
    CU_ASSERT(pTree->insert(pTree, (Item)1) == SUCC);
    CU_ASSERT(pTree->insert(pTree, (Item)4) == SUCC);
    CU_ASSERT(pTree->insert(pTree, (Item)7) == SUCC);

    /* Check the item insertion and structure invariant. */
    Item item;
    CU_ASSERT(pTree->predecessor(pTree, (Item)4, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)1);
    CU_ASSERT(pTree->successor(pTree, (Item)4, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)6);

    CU_ASSERT(pTree->predecessor(pTree, (Item)6, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)4);
    CU_ASSERT(pTree->successor(pTree, (Item)6, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)7);

    CU_ASSERT(pTree->predecessor(pTree, (Item)7, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)6);
    CU_ASSERT(pTree->successor(pTree, (Item)7, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)9);

    CU_ASSERT(pTree->predecessor(pTree, (Item)9, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)7);
    CU_ASSERT(pTree->successor(pTree, (Item)9, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)10);

    CU_ASSERT(pTree->predecessor(pTree, (Item)10, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)9);
    CU_ASSERT(pTree->successor(pTree, (Item)10, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)15);

    CU_ASSERT(pTree->predecessor(pTree, (Item)15, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)10);
    CU_ASSERT(pTree->successor(pTree, (Item)15, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)20);

    CU_ASSERT(pTree->predecessor(pTree, (Item)20, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)15);
    CU_ASSERT(pTree->successor(pTree, (Item)20, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)22);

    CU_ASSERT(pTree->predecessor(pTree, (Item)22, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)20);
    CU_ASSERT(pTree->successor(pTree, (Item)22, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)25);

    /* Check the minimum and maximum item. */
    CU_ASSERT(pTree->minimum(pTree, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)1);
    CU_ASSERT(pTree->maximum(pTree, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)25);

    /* Check the container size. */
    CU_ASSERT_EQUAL(pTree->size(pTree), 10);

    BalTreeDeinit(&pTree);
}

