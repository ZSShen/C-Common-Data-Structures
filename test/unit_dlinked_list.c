#include "list/dlinked_list.h"
#include "CUnit/Util.h"
#include "CUnit/Basic.h"


/*===========================================================================*
 *              Test suite to manipulate primitive type input                *
 *===========================================================================*/
void TestPrimPushFront();
void TestPrimPushBack();
void TestPrimInsertForward();
void TestPrimInsertBackward();
void TestPrimPopFront();
void TestPrimPopBack();
void TestPrimDeleteForward();
void TestPrimDeleteBackward();
void TestPrimSet();

void TestBoundary();
void TestReverse();


int32_t SuitePrimitive()
{
    CU_pSuite pSuite = CU_add_suite("Primitive Type Input", NULL, NULL);
    if (!pSuite)
        return ERR_NOMEM;

    CU_pTest pTest = CU_add_test(pSuite, "Item insertion via push_front()", TestPrimPushFront);
    if (!pTest)
        return ERR_NOMEM;

    pTest = CU_add_test(pSuite, "Item insertion via push_back()", TestPrimPushBack);
    if (!pTest)
        return ERR_NOMEM;

    pTest = CU_add_test(pSuite, "Item insertion via insert() with forward indexing",
            TestPrimInsertForward);
    if (!pTest)
        return ERR_NOMEM;

    pTest = CU_add_test(pSuite, "Item insertion via insert() with backward indexing",
            TestPrimInsertBackward);
    if (!pTest)
        return ERR_NOMEM;

    pTest = CU_add_test(pSuite, "Item removal via pop_front()", TestPrimPopFront);
    if (!pTest)
        return ERR_NOMEM;

    pTest = CU_add_test(pSuite, "Item removal via pop_back()", TestPrimPopBack);
    if (!pTest)
        return ERR_NOMEM;

    pTest = CU_add_test(pSuite, "Item removal via delete() with forward indexing",
            TestPrimDeleteForward);
    if (!pTest)
        return ERR_NOMEM;

    pTest = CU_add_test(pSuite, "Item removal via delete() with backward indexing",
            TestPrimDeleteBackward);
    if (!pTest)
        return ERR_NOMEM;

    pTest = CU_add_test(pSuite, "Item replacement", TestPrimSet);
    if (!pTest)
        return ERR_NOMEM;

    pTest = CU_add_test(pSuite, "List reversing", TestReverse);
    if (!pTest)
        return ERR_NOMEM;

    pTest = CU_add_test(pSuite, "Boundary test", TestBoundary);
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
void TestPrimPushFront()
{
    DLinkedList *pList;
    CU_ASSERT(DListInit(&pList) == SUCC);

    /* Prepend items. */
    CU_ASSERT(pList->push_front(pList, (Item)4) == SUCC);
    CU_ASSERT(pList->push_front(pList, (Item)3) == SUCC);
    CU_ASSERT(pList->push_front(pList, (Item)2) == SUCC);
    CU_ASSERT(pList->push_front(pList, (Item)1) == SUCC);

    /* Check item insertion sequence. */
    Item item;
    CU_ASSERT(pList->get_front(pList, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)1);
    CU_ASSERT(pList->get_back(pList, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)4);

    CU_ASSERT(pList->get_at(pList, &item, 0) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)1);
    CU_ASSERT(pList->get_at(pList, &item, 1) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)2);
    CU_ASSERT(pList->get_at(pList, &item, 2) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)3);
    CU_ASSERT(pList->get_at(pList, &item, 3) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)4);

    CU_ASSERT(pList->get_at(pList, &item, -4) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)1);
    CU_ASSERT(pList->get_at(pList, &item, -3) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)2);
    CU_ASSERT(pList->get_at(pList, &item, -2) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)3);
    CU_ASSERT(pList->get_at(pList, &item, -1) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)4);

    /* Check list size. */
    CU_ASSERT_EQUAL(pList->size(pList), 4);

    DListDeinit(&pList, true);
}

void TestPrimPushBack()
{
    DLinkedList *pList;
    CU_ASSERT(DListInit(&pList) == SUCC);

    /* Prepend items. */
    CU_ASSERT(pList->push_back(pList, (Item)1) == SUCC);
    CU_ASSERT(pList->push_back(pList, (Item)2) == SUCC);
    CU_ASSERT(pList->push_back(pList, (Item)3) == SUCC);
    CU_ASSERT(pList->push_back(pList, (Item)4) == SUCC);

    /* Check item insertion sequence. */
    Item item;
    CU_ASSERT(pList->get_front(pList, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)1);
    CU_ASSERT(pList->get_back(pList, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)4);

    CU_ASSERT(pList->get_at(pList, &item, 0) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)1);
    CU_ASSERT(pList->get_at(pList, &item, 1) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)2);
    CU_ASSERT(pList->get_at(pList, &item, 2) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)3);
    CU_ASSERT(pList->get_at(pList, &item, 3) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)4);

    CU_ASSERT(pList->get_at(pList, &item, -4) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)1);
    CU_ASSERT(pList->get_at(pList, &item, -3) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)2);
    CU_ASSERT(pList->get_at(pList, &item, -2) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)3);
    CU_ASSERT(pList->get_at(pList, &item, -1) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)4);

    /* Check list size. */
    CU_ASSERT_EQUAL(pList->size(pList), 4);

    DListDeinit(&pList, true);
}

void TestPrimInsertForward()
{
    DLinkedList *pList;
    CU_ASSERT(DListInit(&pList) == SUCC);

    /* Insert items. */
    CU_ASSERT(pList->insert(pList, (Item)1, 0) == SUCC);
    CU_ASSERT(pList->insert(pList, (Item)4, 1) == SUCC);
    CU_ASSERT(pList->insert(pList, (Item)2, 1) == SUCC);
    CU_ASSERT(pList->insert(pList, (Item)3, 2) == SUCC);
    CU_ASSERT(pList->insert(pList, (Item)0, 0) == SUCC);

    /* Check item insertion sequence. */
    Item item;
    CU_ASSERT(pList->get_front(pList, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)0);
    CU_ASSERT(pList->get_back(pList, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)4);

    CU_ASSERT(pList->get_at(pList, &item, 0) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)0);
    CU_ASSERT(pList->get_at(pList, &item, 1) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)1);
    CU_ASSERT(pList->get_at(pList, &item, 2) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)2);
    CU_ASSERT(pList->get_at(pList, &item, 3) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)3);
    CU_ASSERT(pList->get_at(pList, &item, 4) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)4);

    CU_ASSERT(pList->get_at(pList, &item, -5) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)0);
    CU_ASSERT(pList->get_at(pList, &item, -4) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)1);
    CU_ASSERT(pList->get_at(pList, &item, -3) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)2);
    CU_ASSERT(pList->get_at(pList, &item, -2) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)3);
    CU_ASSERT(pList->get_at(pList, &item, -1) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)4);

    /* Check list size. */
    CU_ASSERT_EQUAL(pList->size(pList), 5);

    DListDeinit(&pList, true);
}

void TestPrimInsertBackward()
{
    DLinkedList *pList;
    CU_ASSERT(DListInit(&pList) == SUCC);

    /* Insert items. */
    CU_ASSERT(pList->insert(pList, (Item)4, 0) == SUCC);
    CU_ASSERT(pList->insert(pList, (Item)1, -1) == SUCC);
    CU_ASSERT(pList->insert(pList, (Item)3, -1) == SUCC);
    CU_ASSERT(pList->insert(pList, (Item)2, -2) == SUCC);
    CU_ASSERT(pList->insert(pList, (Item)0, -4) == SUCC);

    /* Check item insertion sequence. */
    Item item;
    CU_ASSERT(pList->get_front(pList, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)0);
    CU_ASSERT(pList->get_back(pList, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)4);

    CU_ASSERT(pList->get_at(pList, &item, 0) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)0);
    CU_ASSERT(pList->get_at(pList, &item, 1) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)1);
    CU_ASSERT(pList->get_at(pList, &item, 2) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)2);
    CU_ASSERT(pList->get_at(pList, &item, 3) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)3);
    CU_ASSERT(pList->get_at(pList, &item, 4) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)4);

    CU_ASSERT(pList->get_at(pList, &item, -5) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)0);
    CU_ASSERT(pList->get_at(pList, &item, -4) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)1);
    CU_ASSERT(pList->get_at(pList, &item, -3) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)2);
    CU_ASSERT(pList->get_at(pList, &item, -2) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)3);
    CU_ASSERT(pList->get_at(pList, &item, -1) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)4);

    /* Check list size. */
    CU_ASSERT_EQUAL(pList->size(pList), 5);

    DListDeinit(&pList, true);
}

void TestPrimPopFront()
{
    DLinkedList *pList;
    CU_ASSERT(DListInit(&pList) == SUCC);

    /* Prepare initial items. */
    CU_ASSERT(pList->push_back(pList, (Item)1) == SUCC);
    CU_ASSERT(pList->push_back(pList, (Item)2) == SUCC);
    CU_ASSERT(pList->push_back(pList, (Item)3) == SUCC);

    /* Pop items from the head and check the remaining item sequence. */
    Item item;
    CU_ASSERT(pList->pop_front(pList, true) == SUCC);
    CU_ASSERT(pList->get_front(pList, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)2);
    CU_ASSERT(pList->get_back(pList, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)3);
    CU_ASSERT(pList->get_at(pList, &item, 0) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)2);
    CU_ASSERT(pList->get_at(pList, &item, -1) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)3);

    CU_ASSERT(pList->pop_front(pList, true) == SUCC);
    CU_ASSERT(pList->get_front(pList, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)3);
    CU_ASSERT(pList->get_back(pList, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)3);
    CU_ASSERT(pList->get_at(pList, &item, 0) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)3);
    CU_ASSERT(pList->get_at(pList, &item, -1) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)3);

    CU_ASSERT(pList->pop_front(pList, true) == SUCC);
    CU_ASSERT(pList->get_front(pList, &item) == ERR_IDX);
    CU_ASSERT(pList->get_back(pList, &item) == ERR_IDX);

    /* Re-insert the item again to check if the list is well handled in
       the previous test. */
    CU_ASSERT(pList->push_back(pList, (Item)777) == SUCC);
    CU_ASSERT(pList->get_front(pList, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)777);
    CU_ASSERT(pList->get_back(pList, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)777);
    CU_ASSERT_EQUAL(pList->size(pList), 1);

    DListDeinit(&pList, true);
}

void TestPrimPopBack()
{
    DLinkedList *pList;
    CU_ASSERT(DListInit(&pList) == SUCC);

    /* Prepare initial items. */
    CU_ASSERT(pList->push_back(pList, (Item)1) == SUCC);
    CU_ASSERT(pList->push_back(pList, (Item)2) == SUCC);
    CU_ASSERT(pList->push_back(pList, (Item)3) == SUCC);

    /* Pop items from the tail and check the remaining item sequence. */
    Item item;
    CU_ASSERT(pList->pop_back(pList, true) == SUCC);
    CU_ASSERT(pList->get_front(pList, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)1);
    CU_ASSERT(pList->get_back(pList, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)2);
    CU_ASSERT(pList->get_at(pList, &item, 1) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)2);
    CU_ASSERT(pList->get_at(pList, &item, -1) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)2);

    CU_ASSERT(pList->pop_back(pList, true) == SUCC);
    CU_ASSERT(pList->get_front(pList, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)1);
    CU_ASSERT(pList->get_back(pList, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)1);
    CU_ASSERT(pList->get_at(pList, &item, 0) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)1);
    CU_ASSERT(pList->get_at(pList, &item, -1) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)1);

    CU_ASSERT(pList->pop_front(pList, true) == SUCC);
    CU_ASSERT(pList->get_front(pList, &item) == ERR_IDX);
    CU_ASSERT(pList->get_back(pList, &item) == ERR_IDX);

    /* Re-insert the item again to check if the list is well handled in
       the previous test. */
    CU_ASSERT(pList->push_back(pList, (Item)777) == SUCC);
    CU_ASSERT(pList->get_front(pList, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)777);
    CU_ASSERT(pList->get_back(pList, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)777);
    CU_ASSERT_EQUAL(pList->size(pList), 1);

    DListDeinit(&pList, true);
}

void TestPrimDeleteForward()
{
    DLinkedList *pList;
    CU_ASSERT(DListInit(&pList) == SUCC);

    /* Prepare initial items. */
    CU_ASSERT(pList->push_back(pList, (Item)1) == SUCC);
    CU_ASSERT(pList->push_back(pList, (Item)2) == SUCC);
    CU_ASSERT(pList->push_back(pList, (Item)3) == SUCC);
    CU_ASSERT(pList->push_back(pList, (Item)4) == SUCC);
    CU_ASSERT(pList->push_back(pList, (Item)5) == SUCC);

    /* Remove items from the designated index and check the remaining
       item sequence. */
    Item item;
    CU_ASSERT(pList->delete(pList, 0, true) == SUCC);
    CU_ASSERT(pList->get_front(pList, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)2);
    CU_ASSERT(pList->get_at(pList, &item, 0) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)2);
    CU_ASSERT(pList->get_at(pList, &item, -1) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)5);

    CU_ASSERT(pList->delete(pList, 1, true) == SUCC);
    CU_ASSERT(pList->get_at(pList, &item, 0) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)2);
    CU_ASSERT(pList->get_at(pList, &item, 1) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)4);
    CU_ASSERT(pList->get_at(pList, &item, -2) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)4);
    CU_ASSERT(pList->get_at(pList, &item, -3) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)2);

    CU_ASSERT(pList->delete(pList, 2, true) == SUCC);
    CU_ASSERT(pList->get_back(pList, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)4);
    CU_ASSERT(pList->get_at(pList, &item, 1) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)4);
    CU_ASSERT(pList->get_at(pList, &item, -1) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)4);

    CU_ASSERT(pList->delete(pList, 0, true) == SUCC);
    CU_ASSERT(pList->delete(pList, 0, true) == SUCC);
    CU_ASSERT(pList->get_at(pList, &item, 0) == ERR_IDX);

    /* Re-insert the item again to check if the list is well handled in
       the previous test. */
    CU_ASSERT(pList->push_back(pList, (Item)777) == SUCC);
    CU_ASSERT(pList->get_front(pList, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)777);
    CU_ASSERT(pList->get_back(pList, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)777);
    CU_ASSERT_EQUAL(pList->size(pList), 1);

    DListDeinit(&pList, true);
}

void TestPrimDeleteBackward()
{
    DLinkedList *pList;
    CU_ASSERT(DListInit(&pList) == SUCC);

    /* Prepare initial items. */
    CU_ASSERT(pList->push_back(pList, (Item)1) == SUCC);
    CU_ASSERT(pList->push_back(pList, (Item)2) == SUCC);
    CU_ASSERT(pList->push_back(pList, (Item)3) == SUCC);
    CU_ASSERT(pList->push_back(pList, (Item)4) == SUCC);
    CU_ASSERT(pList->push_back(pList, (Item)5) == SUCC);

    /* Remove items from the designated index and check the remaining
       item sequence. */
    Item item;
    CU_ASSERT(pList->delete(pList, -5, true) == SUCC);
    CU_ASSERT(pList->get_front(pList, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)2);
    CU_ASSERT(pList->get_at(pList, &item, 0) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)2);
    CU_ASSERT(pList->get_at(pList, &item, -1) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)5);

    CU_ASSERT(pList->delete(pList, -3, true) == SUCC);
    CU_ASSERT(pList->get_at(pList, &item, 0) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)2);
    CU_ASSERT(pList->get_at(pList, &item, 1) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)4);
    CU_ASSERT(pList->get_at(pList, &item, -2) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)4);
    CU_ASSERT(pList->get_at(pList, &item, -3) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)2);

    CU_ASSERT(pList->delete(pList, -1, true) == SUCC);
    CU_ASSERT(pList->get_back(pList, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)4);
    CU_ASSERT(pList->get_at(pList, &item, 1) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)4);
    CU_ASSERT(pList->get_at(pList, &item, -1) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)4);

    CU_ASSERT(pList->delete(pList, 0, true) == SUCC);
    CU_ASSERT(pList->delete(pList, 0, true) == SUCC);
    CU_ASSERT(pList->get_at(pList, &item, 0) == ERR_IDX);

    /* Re-insert the item again to check if the list is well handled in
       the previous test. */
    CU_ASSERT(pList->push_back(pList, (Item)777) == SUCC);
    CU_ASSERT(pList->get_front(pList, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)777);
    CU_ASSERT(pList->get_back(pList, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)777);
    CU_ASSERT_EQUAL(pList->size(pList), 1);

    DListDeinit(&pList, true);
}

void TestPrimSet()
{
    DLinkedList *pList;
    CU_ASSERT(DListInit(&pList) == SUCC);

    /* Prepare the initial items. */
    CU_ASSERT(pList->push_back(pList, (Item)1) == SUCC);
    CU_ASSERT(pList->push_back(pList, (Item)2) == SUCC);
    CU_ASSERT(pList->push_back(pList, (Item)3) == SUCC);

    /* Replace item at the head and check the result. */
    Item item;
    CU_ASSERT(pList->set_front(pList, (Item)-1, true) == SUCC);
    CU_ASSERT(pList->get_front(pList, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)-1);

    /* Replace item at the tail and check the result. */
    CU_ASSERT(pList->set_back(pList, (Item)-3, true) == SUCC);
    CU_ASSERT(pList->get_back(pList, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)-3);

    /* Replace item at the designated index and check the result. */
    CU_ASSERT(pList->set_at(pList, (Item)1, 0, true) == SUCC);
    CU_ASSERT(pList->get_at(pList, &item, 0) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)1);
    CU_ASSERT(pList->set_at(pList, (Item)-1, -3, true) == SUCC);
    CU_ASSERT(pList->get_at(pList, &item, -3) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)-1);
    CU_ASSERT(pList->set_at(pList, (Item)3, 2, true) == SUCC);
    CU_ASSERT(pList->get_at(pList, &item, 2) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)3);
    CU_ASSERT(pList->set_at(pList, (Item)-3, -1, true) == SUCC);
    CU_ASSERT(pList->get_at(pList, &item, -1) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)-3);
    CU_ASSERT(pList->set_at(pList, (Item)-2, 1, true) == SUCC);
    CU_ASSERT(pList->get_at(pList, &item, 1) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)-2);
    CU_ASSERT(pList->set_at(pList, (Item)2, -2, true) == SUCC);
    CU_ASSERT(pList->get_at(pList, &item, -2) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)2);

    DListDeinit(&pList, true);
}

void TestReverse()
{
    DLinkedList *pList;
    CU_ASSERT(DListInit(&pList) == SUCC);

    /* Prepare initial list. (1) */
    CU_ASSERT(pList->push_back(pList, (Item)1) == SUCC);

    /* Reverse and check the item sequence. */
    Item item;
    pList->reverse(pList);
    CU_ASSERT(pList->get_front(pList, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)1);
    CU_ASSERT(pList->get_back(pList, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)1);

    /* Extend the list. (1)<-->(2) */
    CU_ASSERT(pList->push_back(pList, (Item)2) == SUCC);
    pList->reverse(pList);
    CU_ASSERT(pList->get_at(pList, &item, 0) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)2);
    CU_ASSERT(pList->get_at(pList, &item, 1) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)1);

    /* Extend the list. (3)<-->(2)<-->(1) */
    CU_ASSERT(pList->push_front(pList, (Item)3) == SUCC);
    pList->reverse(pList);
    CU_ASSERT(pList->get_at(pList, &item, 0) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)1);
    CU_ASSERT(pList->get_at(pList, &item, 1) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)2);
    CU_ASSERT(pList->get_at(pList, &item, 2) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)3);

    /* Extend the list. (1)<-->(2)<-->(3)<-->(4) */
    CU_ASSERT(pList->push_back(pList, (Item)4) == SUCC);
    pList->reverse(pList);
    CU_ASSERT(pList->get_at(pList, &item, 0) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)4);
    CU_ASSERT(pList->get_at(pList, &item, 1) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)3);
    CU_ASSERT(pList->get_at(pList, &item, 2) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)2);
    CU_ASSERT(pList->get_at(pList, &item, 3) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)1);

    DListDeinit(&pList, true);
}

void TestBoundary()
{
    DLinkedList *pList;
    CU_ASSERT(DListInit(&pList) == SUCC);

    /* Initial boundary test. */
    CU_ASSERT(pList->pop_front(pList, true) == ERR_IDX);
    CU_ASSERT(pList->pop_back(pList, true) == ERR_IDX);

    CU_ASSERT(pList->delete(pList, 0, true) == ERR_IDX);
    CU_ASSERT(pList->delete(pList, -1, true) == ERR_IDX);
    CU_ASSERT(pList->insert(pList, (Item)1, 1) == ERR_IDX);
    CU_ASSERT(pList->insert(pList, (Item)1, -1) == ERR_IDX);

    CU_ASSERT(pList->set_front(pList, (Item)1, true) == ERR_IDX);
    CU_ASSERT(pList->set_back(pList, (Item)1, true) == ERR_IDX);
    CU_ASSERT(pList->set_at(pList, (Item)1, 0, true) == ERR_IDX);
    CU_ASSERT(pList->set_at(pList, (Item)1, -1, true) == ERR_IDX);

    Item item;
    CU_ASSERT(pList->get_front(pList, &item) == ERR_IDX);
    CU_ASSERT_EQUAL(item, NULL);
    CU_ASSERT(pList->get_back(pList, &item) == ERR_IDX);
    CU_ASSERT_EQUAL(item, NULL);
    CU_ASSERT(pList->get_at(pList, &item, 0) == ERR_IDX);
    CU_ASSERT_EQUAL(item, NULL);
    CU_ASSERT(pList->get_at(pList, &item, -1) == ERR_IDX);
    CU_ASSERT_EQUAL(item, NULL);

    DListDeinit(&pList, true);
}