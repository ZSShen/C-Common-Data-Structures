#include "tree/simple_tree.h"
#include "CUnit/Util.h"
#include "CUnit/Basic.h"


/*===========================================================================*
 *        Definition for the test cases of the primitive input suite         *
 *===========================================================================*/
void TestPrimInsertAndOrder();
void TestPrimDeleteAndOrder();
void TestPrimSearch();


int32_t SuitePrimitive()
{
    CU_pSuite pSuite = CU_add_suite("Primitive Input", NULL, NULL);
    if (!pSuite)
        return ERR_NOMEM;

    CU_pTest pTest = CU_add_test(pSuite, "Item insertion and structure verification",
                     TestPrimInsertAndOrder);
    if (!pTest)
        return ERR_NOMEM;

    pTest = CU_add_test(pSuite, "Item deletion and structure verification",
            TestPrimDeleteAndOrder);
    if (!pTest)
        return ERR_NOMEM;

    pTest = CU_add_test(pSuite, "Item search", TestPrimSearch);
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
    CU_ASSERT(pTree->insert(pTree, (Item)4) == SUCC);
    CU_ASSERT(pTree->insert(pTree, (Item)2) == SUCC);
    CU_ASSERT(pTree->insert(pTree, (Item)3) == SUCC);
    CU_ASSERT(pTree->insert(pTree, (Item)1) == SUCC);
    CU_ASSERT(pTree->insert(pTree, (Item)6) == SUCC);
    CU_ASSERT(pTree->insert(pTree, (Item)5) == SUCC);
    CU_ASSERT(pTree->insert(pTree, (Item)7) == SUCC);

    /* Check the item insertion and structure invariant. */
    Item item;
    CU_ASSERT(pTree->predecessor(pTree, (Item)2, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)1);
    CU_ASSERT(pTree->successor(pTree, (Item)2, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)3);

    CU_ASSERT(pTree->predecessor(pTree, (Item)3, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)2);
    CU_ASSERT(pTree->successor(pTree, (Item)3, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)4);

    CU_ASSERT(pTree->predecessor(pTree, (Item)5, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)4);
    CU_ASSERT(pTree->successor(pTree, (Item)5, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)6);

    CU_ASSERT(pTree->predecessor(pTree, (Item)4, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)3);
    CU_ASSERT(pTree->successor(pTree, (Item)4, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)5);

    /* Check the minimum and maximum item. */
    CU_ASSERT(pTree->minimum(pTree, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)1);
    CU_ASSERT(pTree->maximum(pTree, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)7);

    /* Check the container size. */
    CU_ASSERT_EQUAL(pTree->size(pTree), 7);

    SimTreeDeinit(&pTree);
}

void TestPrimDeleteAndOrder()
{
    SimpleTree *pTree;
    CU_ASSERT(SimTreeInit(&pTree) == SUCC);

    /**
     * The vision of the test tree.
     *         (4)
     *        /   \
     *     (2)     (7)
     *     / \     / \
     *   (1) (3) (5) (8)
     *   /         \
     * (0)         (6)
     */
    CU_ASSERT(pTree->insert(pTree, (Item)4) == SUCC);
    CU_ASSERT(pTree->insert(pTree, (Item)2) == SUCC);
    CU_ASSERT(pTree->insert(pTree, (Item)3) == SUCC);
    CU_ASSERT(pTree->insert(pTree, (Item)1) == SUCC);
    CU_ASSERT(pTree->insert(pTree, (Item)0) == SUCC);
    CU_ASSERT(pTree->insert(pTree, (Item)7) == SUCC);
    CU_ASSERT(pTree->insert(pTree, (Item)5) == SUCC);
    CU_ASSERT(pTree->insert(pTree, (Item)6) == SUCC);
    CU_ASSERT(pTree->insert(pTree, (Item)8) == SUCC);

    /* Check the item deletion and structure invariant. */
    CU_ASSERT(pTree->delete(pTree, (Item)4) == SUCC);
    /**
     * The new structure of the trimmed tree.
     *         (5)
     *        /   \
     *      /\    (7)
     *     /  \   / \
     *    /____\(6) (8)
     */
    Item item;
    CU_ASSERT(pTree->predecessor(pTree, (Item)7, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)6);
    CU_ASSERT(pTree->predecessor(pTree, (Item)5, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)3);
    CU_ASSERT(pTree->successor(pTree, (Item)5, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)6);

    CU_ASSERT(pTree->delete(pTree, (Item)5) == SUCC);
    CU_ASSERT(pTree->predecessor(pTree, (Item)7, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)6);
    CU_ASSERT(pTree->predecessor(pTree, (Item)6, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)3);
    CU_ASSERT(pTree->successor(pTree, (Item)6, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)7);

    CU_ASSERT(pTree->delete(pTree, (Item)6) == SUCC);
    /**
     * The new structure of the trimmed tree.
     *         (7)
     *        /   \
     *      /\    (8)
     *     /  \
     *    /____\
     */
    CU_ASSERT(pTree->predecessor(pTree, (Item)7, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)3);
    CU_ASSERT(pTree->successor(pTree, (Item)7, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)8);

    CU_ASSERT(pTree->delete(pTree, (Item)8) == SUCC);

    CU_ASSERT(pTree->delete(pTree, (Item)7) == SUCC);
    /**
     * The new structure of the trimmed tree.
     *        (2)
     *       /   \
     *     (1)   (3)
     *     /
     *   (0)
     */
    CU_ASSERT(pTree->maximum(pTree, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)3);
    CU_ASSERT(pTree->minimum(pTree, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)0);

    CU_ASSERT(pTree->delete(pTree, (Item)1) == SUCC);
    /**
     * The new structure of the trimmed tree.
     *        (2)
     *       /   \
     *     (0)   (3)
     */
    CU_ASSERT(pTree->predecessor(pTree, (Item)2, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)0);
    CU_ASSERT(pTree->successor(pTree, (Item)0, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)2);

    /* Check the container size. */
    CU_ASSERT_EQUAL(pTree->size(pTree), 3);

    SimTreeDeinit(&pTree);
}

void TestPrimSearch()
{
    SimpleTree *pTree;
    CU_ASSERT(SimTreeInit(&pTree) == SUCC);

    /* Search for the empty tree. */
    Item item;
    CU_ASSERT(pTree->search(pTree, (Item)0, &item) == ERR_NODATA);
    CU_ASSERT_EQUAL(item, NULL);

    /* Search for the real data. */
    CU_ASSERT(pTree->insert(pTree, (Item)1) == SUCC);
    CU_ASSERT(pTree->insert(pTree, (Item)0) == SUCC);
    CU_ASSERT(pTree->search(pTree, (Item)0, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)0);

    CU_ASSERT(pTree->delete(pTree, (Item)0) == SUCC);
    CU_ASSERT(pTree->search(pTree, (Item)0, &item) == ERR_NODATA);
    CU_ASSERT_EQUAL(item, NULL);

    SimTreeDeinit(&pTree);
}
