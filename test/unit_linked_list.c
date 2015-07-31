#include "list/linked_list.h"
#include "CUnit/Util.h"
#include "CUnit/Basic.h"


typedef struct Employ_ {
    int32_t iId;
    char *szName;
} Employ;

#define SIZE_NAME_BUF (4)


/* The function to register primitive test suites for algorithm verification. */
void TestPrimPushFront();
void TestPrimPushBack();
void TestPrimInsertForward();
void TestPrimInsertBackward();
void TestPrimPopFront();
void TestPrimPopBack();
void TestPrimDeleteForward();
void TestPrimDeleteBackward();
void TestPrimSet();
void TestIterate();

/* The function to register non-primitive test suites for resource management
   verification. */
void TestNonPrimPop();
void TestNonPrimDelete();
void TestNonPrimSet();
void NonPrimDestroy(Item);

/* The function to register common test suites. */
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

    pTest = CU_add_test(pSuite, "List Iteration", TestIterate);
    if (!pTest)
        return ERR_NOMEM;

    pTest = CU_add_test(pSuite, "Boundary test", TestBoundary);
    if (!pTest)
        return ERR_NOMEM;

    return SUCC;
}

int32_t SuiteNonPrimitive()
{
    CU_pSuite pSuite = CU_add_suite("NonPrimitive Type Input", NULL, NULL);
    if (!pSuite)
        return ERR_NOMEM;

    char *szMsg = "Item removal via sequence of pop_front() and pop_back().";
    CU_pTest pTest = CU_add_test(pSuite, szMsg, TestNonPrimPop);
    if (!pTest)
        return ERR_NOMEM;

    szMsg = "Item removal via sequence of delete().";
    pTest = CU_add_test(pSuite, szMsg, TestNonPrimDelete);
    if (!pTest)
        return ERR_NOMEM;

    szMsg = "Item replacement via sequence of set_at(), set_front(), and set_back().";
    pTest = CU_add_test(pSuite, szMsg, TestNonPrimSet);
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

    /* Prepare the test suite for non-primitive input. */
    if (SuiteNonPrimitive() != SUCC) {
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
    LinkedList *pList;
    CU_ASSERT(ListInit(&pList) == SUCC);

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

    ListDeinit(&pList);
}

void TestPrimPushBack()
{
    LinkedList *pList;
    CU_ASSERT(ListInit(&pList) == SUCC);

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

    ListDeinit(&pList);
}

void TestPrimInsertForward()
{
    LinkedList *pList;
    CU_ASSERT(ListInit(&pList) == SUCC);

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

    ListDeinit(&pList);
}

void TestPrimInsertBackward()
{
    LinkedList *pList;
    CU_ASSERT(ListInit(&pList) == SUCC);

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

    ListDeinit(&pList);
}

void TestPrimPopFront()
{
    LinkedList *pList;
    CU_ASSERT(ListInit(&pList) == SUCC);

    /* Prepare initial items. */
    CU_ASSERT(pList->push_back(pList, (Item)1) == SUCC);
    CU_ASSERT(pList->push_back(pList, (Item)2) == SUCC);
    CU_ASSERT(pList->push_back(pList, (Item)3) == SUCC);

    /* Pop items from the head and check the remaining item sequence. */
    Item item;
    CU_ASSERT(pList->pop_front(pList) == SUCC);
    CU_ASSERT(pList->get_front(pList, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)2);
    CU_ASSERT(pList->get_back(pList, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)3);
    CU_ASSERT(pList->get_at(pList, &item, 0) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)2);
    CU_ASSERT(pList->get_at(pList, &item, -1) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)3);

    CU_ASSERT(pList->pop_front(pList) == SUCC);
    CU_ASSERT(pList->get_front(pList, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)3);
    CU_ASSERT(pList->get_back(pList, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)3);
    CU_ASSERT(pList->get_at(pList, &item, 0) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)3);
    CU_ASSERT(pList->get_at(pList, &item, -1) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)3);

    CU_ASSERT(pList->pop_front(pList) == SUCC);
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

    ListDeinit(&pList);
}

void TestPrimPopBack()
{
    LinkedList *pList;
    CU_ASSERT(ListInit(&pList) == SUCC);

    /* Prepare initial items. */
    CU_ASSERT(pList->push_back(pList, (Item)1) == SUCC);
    CU_ASSERT(pList->push_back(pList, (Item)2) == SUCC);
    CU_ASSERT(pList->push_back(pList, (Item)3) == SUCC);

    /* Pop items from the tail and check the remaining item sequence. */
    Item item;
    CU_ASSERT(pList->pop_back(pList) == SUCC);
    CU_ASSERT(pList->get_front(pList, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)1);
    CU_ASSERT(pList->get_back(pList, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)2);
    CU_ASSERT(pList->get_at(pList, &item, 1) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)2);
    CU_ASSERT(pList->get_at(pList, &item, -1) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)2);

    CU_ASSERT(pList->pop_back(pList) == SUCC);
    CU_ASSERT(pList->get_front(pList, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)1);
    CU_ASSERT(pList->get_back(pList, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)1);
    CU_ASSERT(pList->get_at(pList, &item, 0) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)1);
    CU_ASSERT(pList->get_at(pList, &item, -1) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)1);

    CU_ASSERT(pList->pop_front(pList) == SUCC);
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

    ListDeinit(&pList);
}

void TestPrimDeleteForward()
{
    LinkedList *pList;
    CU_ASSERT(ListInit(&pList) == SUCC);

    /* Prepare initial items. */
    CU_ASSERT(pList->push_back(pList, (Item)1) == SUCC);
    CU_ASSERT(pList->push_back(pList, (Item)2) == SUCC);
    CU_ASSERT(pList->push_back(pList, (Item)3) == SUCC);
    CU_ASSERT(pList->push_back(pList, (Item)4) == SUCC);
    CU_ASSERT(pList->push_back(pList, (Item)5) == SUCC);

    /* Remove items from the designated index and check the remaining
       item sequence. */
    Item item;
    CU_ASSERT(pList->delete(pList, 0) == SUCC);
    CU_ASSERT(pList->get_front(pList, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)2);
    CU_ASSERT(pList->get_at(pList, &item, 0) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)2);
    CU_ASSERT(pList->get_at(pList, &item, -1) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)5);

    CU_ASSERT(pList->delete(pList, 1) == SUCC);
    CU_ASSERT(pList->get_at(pList, &item, 0) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)2);
    CU_ASSERT(pList->get_at(pList, &item, 1) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)4);
    CU_ASSERT(pList->get_at(pList, &item, -2) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)4);
    CU_ASSERT(pList->get_at(pList, &item, -3) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)2);

    CU_ASSERT(pList->delete(pList, 2) == SUCC);
    CU_ASSERT(pList->get_back(pList, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)4);
    CU_ASSERT(pList->get_at(pList, &item, 1) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)4);
    CU_ASSERT(pList->get_at(pList, &item, -1) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)4);

    CU_ASSERT(pList->delete(pList, 0) == SUCC);
    CU_ASSERT(pList->delete(pList, 0) == SUCC);
    CU_ASSERT(pList->get_at(pList, &item, 0) == ERR_IDX);

    /* Re-insert the item again to check if the list is well handled in
       the previous test. */
    CU_ASSERT(pList->push_back(pList, (Item)777) == SUCC);
    CU_ASSERT(pList->get_front(pList, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)777);
    CU_ASSERT(pList->get_back(pList, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)777);
    CU_ASSERT_EQUAL(pList->size(pList), 1);

    ListDeinit(&pList);
}

void TestPrimDeleteBackward()
{
    LinkedList *pList;
    CU_ASSERT(ListInit(&pList) == SUCC);

    /* Prepare initial items. */
    CU_ASSERT(pList->push_back(pList, (Item)1) == SUCC);
    CU_ASSERT(pList->push_back(pList, (Item)2) == SUCC);
    CU_ASSERT(pList->push_back(pList, (Item)3) == SUCC);
    CU_ASSERT(pList->push_back(pList, (Item)4) == SUCC);
    CU_ASSERT(pList->push_back(pList, (Item)5) == SUCC);

    /* Remove items from the designated index and check the remaining
       item sequence. */
    Item item;
    CU_ASSERT(pList->delete(pList, -5) == SUCC);
    CU_ASSERT(pList->get_front(pList, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)2);
    CU_ASSERT(pList->get_at(pList, &item, 0) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)2);
    CU_ASSERT(pList->get_at(pList, &item, -1) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)5);

    CU_ASSERT(pList->delete(pList, -3) == SUCC);
    CU_ASSERT(pList->get_at(pList, &item, 0) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)2);
    CU_ASSERT(pList->get_at(pList, &item, 1) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)4);
    CU_ASSERT(pList->get_at(pList, &item, -2) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)4);
    CU_ASSERT(pList->get_at(pList, &item, -3) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)2);

    CU_ASSERT(pList->delete(pList, -1) == SUCC);
    CU_ASSERT(pList->get_back(pList, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)4);
    CU_ASSERT(pList->get_at(pList, &item, 1) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)4);
    CU_ASSERT(pList->get_at(pList, &item, -1) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)4);

    CU_ASSERT(pList->delete(pList, 0) == SUCC);
    CU_ASSERT(pList->delete(pList, 0) == SUCC);
    CU_ASSERT(pList->get_at(pList, &item, 0) == ERR_IDX);

    /* Re-insert the item again to check if the list is well handled in
       the previous test. */
    CU_ASSERT(pList->push_back(pList, (Item)777) == SUCC);
    CU_ASSERT(pList->get_front(pList, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)777);
    CU_ASSERT(pList->get_back(pList, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)777);
    CU_ASSERT_EQUAL(pList->size(pList), 1);

    ListDeinit(&pList);
}

void TestPrimSet()
{
    LinkedList *pList;
    CU_ASSERT(ListInit(&pList) == SUCC);

    /* Prepare the initial items. */
    CU_ASSERT(pList->push_back(pList, (Item)1) == SUCC);
    CU_ASSERT(pList->push_back(pList, (Item)2) == SUCC);
    CU_ASSERT(pList->push_back(pList, (Item)3) == SUCC);

    /* Replace item at the head and check the result. */
    Item item;
    CU_ASSERT(pList->set_front(pList, (Item)-1) == SUCC);
    CU_ASSERT(pList->get_front(pList, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)-1);

    /* Replace item at the tail and check the result. */
    CU_ASSERT(pList->set_back(pList, (Item)-3) == SUCC);
    CU_ASSERT(pList->get_back(pList, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)-3);

    /* Replace item at the designated index and check the result. */
    CU_ASSERT(pList->set_at(pList, (Item)1, 0) == SUCC);
    CU_ASSERT(pList->get_at(pList, &item, 0) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)1);
    CU_ASSERT(pList->set_at(pList, (Item)-1, -3) == SUCC);
    CU_ASSERT(pList->get_at(pList, &item, -3) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)-1);
    CU_ASSERT(pList->set_at(pList, (Item)3, 2) == SUCC);
    CU_ASSERT(pList->get_at(pList, &item, 2) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)3);
    CU_ASSERT(pList->set_at(pList, (Item)-3, -1) == SUCC);
    CU_ASSERT(pList->get_at(pList, &item, -1) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)-3);
    CU_ASSERT(pList->set_at(pList, (Item)-2, 1) == SUCC);
    CU_ASSERT(pList->get_at(pList, &item, 1) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)-2);
    CU_ASSERT(pList->set_at(pList, (Item)2, -2) == SUCC);
    CU_ASSERT(pList->get_at(pList, &item, -2) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)2);

    ListDeinit(&pList);
}

void TestIterate()
{
    LinkedList *pList;
    CU_ASSERT(ListInit(&pList) == SUCC);

    /* Push the initial items. */
    CU_ASSERT(pList->push_back(pList, (Item)1) == SUCC);
    CU_ASSERT(pList->push_back(pList, (Item)2) == SUCC);
    CU_ASSERT(pList->push_back(pList, (Item)3) == SUCC);
    CU_ASSERT(pList->push_back(pList, (Item)4) == SUCC);

    /* Iterate through the list items. */
    Item item;
    int32_t iIdx = 1;
    CU_ASSERT(pList->iterate(pList, true, NULL) == SUCC);
    while (pList->iterate(pList, false, &item) != END) {
        CU_ASSERT_EQUAL(iIdx, (int32_t)(long)item);
        iIdx++;
    }

    /* Reversely iterate through the list items. */
    iIdx = 4;
    CU_ASSERT(pList->reverse_iterate(pList, true, NULL) == SUCC);
    while (pList->reverse_iterate(pList, false, &item) != END) {
        CU_ASSERT_EQUAL(iIdx, (int32_t)(long)item);
        iIdx--;
    }

    ListDeinit(&pList);
}

void NonPrimDestroy(Item item)
{
    Employ *pEmploy = (Employ*)item;
    free(pEmploy->szName);
    free(pEmploy);
}

void TestNonPrimPop()
{
    LinkedList *pList;
    CU_ASSERT(ListInit(&pList) == SUCC);
    CU_ASSERT(pList->set_destroy(pList, NonPrimDestroy) == SUCC);

    /* Prepare initial items. */
    Employ *pEmploy = (Employ*)malloc(sizeof(Employ));
    pEmploy->iId = 1;
    pEmploy->szName = (char*)malloc(sizeof(char) * SIZE_NAME_BUF);
    pEmploy->szName[0] = 'a';
    pEmploy->szName[1] = 'b';
    pEmploy->szName[2] = 'c';
    pEmploy->szName[3] = 0;
    CU_ASSERT(pList->push_back(pList, (Item)pEmploy) == SUCC);

    pEmploy = (Employ*)malloc(sizeof(Employ));
    pEmploy->iId = 2;
    pEmploy->szName = (char*)malloc(sizeof(char) * SIZE_NAME_BUF);
    pEmploy->szName[0] = 'd';
    pEmploy->szName[1] = 'e';
    pEmploy->szName[2] = 'f';
    pEmploy->szName[3] = 0;
    CU_ASSERT(pList->push_back(pList, (Item)pEmploy) == SUCC);

    pEmploy = (Employ*)malloc(sizeof(Employ));
    pEmploy->iId = 3;
    pEmploy->szName = (char*)malloc(sizeof(char) * SIZE_NAME_BUF);
    pEmploy->szName[0] = 'g';
    pEmploy->szName[1] = 'h';
    pEmploy->szName[2] = 'i';
    pEmploy->szName[3] = 0;
    CU_ASSERT(pList->push_back(pList, (Item)pEmploy) == SUCC);

    pEmploy = (Employ*)malloc(sizeof(Employ));
    pEmploy->iId = 4;
    pEmploy->szName = (char*)malloc(sizeof(char) * SIZE_NAME_BUF);
    pEmploy->szName[0] = 'j';
    pEmploy->szName[1] = 'k';
    pEmploy->szName[2] = 'l';
    pEmploy->szName[3] = 0;
    CU_ASSERT(pList->push_back(pList, (Item)pEmploy) == SUCC);

    /* Pop item from the head and check the remaining item sequence. */
    Item item;
    CU_ASSERT(pList->pop_front(pList) == SUCC);
    CU_ASSERT(pList->get_front(pList, &item) == SUCC);
    CU_ASSERT(strcmp(((Employ*)item)->szName, "def") == 0);

    CU_ASSERT(pList->get_back(pList, &item) == SUCC);
    CU_ASSERT(strcmp(((Employ*)item)->szName, "jkl") == 0);

    CU_ASSERT(pList->get_at(pList, &item, 0) == SUCC);
    CU_ASSERT(strcmp(((Employ*)item)->szName, "def") == 0);

    CU_ASSERT(pList->get_at(pList, &item, -1) == SUCC);
    CU_ASSERT(strcmp(((Employ*)item)->szName, "jkl") == 0) ;

    /* Pop item from the tail and check the remaining item sequence. */
    CU_ASSERT(pList->pop_back(pList) == SUCC);
    CU_ASSERT(pList->get_front(pList, &item) == SUCC);
    CU_ASSERT(strcmp(((Employ*)item)->szName, "def") == 0);

    CU_ASSERT(pList->get_back(pList, &item) == SUCC);
    CU_ASSERT(strcmp(((Employ*)item)->szName, "ghi") == 0);

    CU_ASSERT(pList->get_at(pList, &item, 0) == SUCC);
    CU_ASSERT(strcmp(((Employ*)item)->szName, "def") == 0);

    CU_ASSERT(pList->get_at(pList, &item, -1) == SUCC);
    CU_ASSERT(strcmp(((Employ*)item)->szName, "ghi") == 0) ;

    /* Check the resource management at list head. */
    CU_ASSERT(pList->pop_front(pList) == SUCC);
    CU_ASSERT(pList->pop_front(pList) == SUCC);

    pEmploy = (Employ*)malloc(sizeof(Employ));
    pEmploy->iId = 1;
    pEmploy->szName = (char*)malloc(sizeof(char) * SIZE_NAME_BUF);
    pEmploy->szName[0] = 'a';
    pEmploy->szName[1] = 'b';
    pEmploy->szName[2] = 'c';
    pEmploy->szName[3] = 0;
    CU_ASSERT(pList->push_back(pList, (Item)pEmploy) == SUCC);

    CU_ASSERT(pList->pop_back(pList) == SUCC);

    pEmploy = (Employ*)malloc(sizeof(Employ));
    pEmploy->iId = 1;
    pEmploy->szName = (char*)malloc(sizeof(char) * SIZE_NAME_BUF);
    pEmploy->szName[0] = 'a';
    pEmploy->szName[1] = 'b';
    pEmploy->szName[2] = 'c';
    pEmploy->szName[3] = 0;
    CU_ASSERT(pList->push_back(pList, (Item)pEmploy) == SUCC);

    ListDeinit(&pList);
}

void TestNonPrimDelete()
{
    LinkedList *pList;
    CU_ASSERT(ListInit(&pList) == SUCC);
    CU_ASSERT(pList->set_destroy(pList, NonPrimDestroy) == SUCC);

    /* Prepare initial items. */
    Employ *pEmploy = (Employ*)malloc(sizeof(Employ));
    pEmploy->iId = 1;
    pEmploy->szName = (char*)malloc(sizeof(char) * SIZE_NAME_BUF);
    pEmploy->szName[0] = 'a';
    pEmploy->szName[1] = 'b';
    pEmploy->szName[2] = 'c';
    pEmploy->szName[3] = 0;
    CU_ASSERT(pList->push_back(pList, (Item)pEmploy) == SUCC);

    pEmploy = (Employ*)malloc(sizeof(Employ));
    pEmploy->iId = 2;
    pEmploy->szName = (char*)malloc(sizeof(char) * SIZE_NAME_BUF);
    pEmploy->szName[0] = 'd';
    pEmploy->szName[1] = 'e';
    pEmploy->szName[2] = 'f';
    pEmploy->szName[3] = 0;
    CU_ASSERT(pList->push_back(pList, (Item)pEmploy) == SUCC);

    pEmploy = (Employ*)malloc(sizeof(Employ));
    pEmploy->iId = 3;
    pEmploy->szName = (char*)malloc(sizeof(char) * SIZE_NAME_BUF);
    pEmploy->szName[0] = 'g';
    pEmploy->szName[1] = 'h';
    pEmploy->szName[2] = 'i';
    pEmploy->szName[3] = 0;
    CU_ASSERT(pList->push_back(pList, (Item)pEmploy) == SUCC);

    pEmploy = (Employ*)malloc(sizeof(Employ));
    pEmploy->iId = 4;
    pEmploy->szName = (char*)malloc(sizeof(char) * SIZE_NAME_BUF);
    pEmploy->szName[0] = 'j';
    pEmploy->szName[1] = 'k';
    pEmploy->szName[2] = 'l';
    pEmploy->szName[3] = 0;
    CU_ASSERT(pList->push_back(pList, (Item)pEmploy) == SUCC);

    pEmploy = (Employ*)malloc(sizeof(Employ));
    pEmploy->iId = 5;
    pEmploy->szName = (char*)malloc(sizeof(char) * SIZE_NAME_BUF);
    pEmploy->szName[0] = 'm';
    pEmploy->szName[1] = 'n';
    pEmploy->szName[2] = 'o';
    pEmploy->szName[3] = 0;
    CU_ASSERT(pList->push_back(pList, (Item)pEmploy) == SUCC);

    /* Remove items from the designated index and check the remaining
       item sequence. */
    Item item;
    CU_ASSERT(pList->delete(pList, 0) == SUCC);
    CU_ASSERT(pList->get_front(pList, &item) == SUCC);
    CU_ASSERT(strcmp(((Employ*)item)->szName, "def") == 0);
    CU_ASSERT(pList->get_at(pList, &item, 0) == SUCC);
    CU_ASSERT(strcmp(((Employ*)item)->szName, "def") == 0);
    CU_ASSERT(pList->get_back(pList, &item) == SUCC);
    CU_ASSERT(strcmp(((Employ*)item)->szName, "mno") == 0);
    CU_ASSERT(pList->get_at(pList, &item, -1) == SUCC);
    CU_ASSERT(strcmp(((Employ*)item)->szName, "mno") == 0);

    CU_ASSERT(pList->delete(pList, 1) == SUCC);
    CU_ASSERT(pList->get_at(pList, &item, 0) == SUCC);
    CU_ASSERT(strcmp(((Employ*)item)->szName, "def") == 0);
    CU_ASSERT(pList->get_at(pList, &item, 1) == SUCC);
    CU_ASSERT(strcmp(((Employ*)item)->szName, "jkl") == 0);
    CU_ASSERT(pList->get_at(pList, &item, -2) == SUCC);
    CU_ASSERT(strcmp(((Employ*)item)->szName, "jkl") == 0);
    CU_ASSERT(pList->get_at(pList, &item, -3) == SUCC);
    CU_ASSERT(strcmp(((Employ*)item)->szName, "def") == 0);

    CU_ASSERT(pList->delete(pList, 2) == SUCC);
    CU_ASSERT(pList->get_at(pList, &item, 0) == SUCC);
    CU_ASSERT(strcmp(((Employ*)item)->szName, "def") == 0);
    CU_ASSERT(pList->get_at(pList, &item, -1) == SUCC);
    CU_ASSERT(strcmp(((Employ*)item)->szName, "jkl") == 0);

    ListDeinit(&pList);
}

void TestNonPrimSet()
{
    LinkedList *pList;
    CU_ASSERT(ListInit(&pList) == SUCC);
    CU_ASSERT(pList->set_destroy(pList, NonPrimDestroy) == SUCC);

    /* Prepare initial items. */
    Employ *pEmploy = (Employ*)malloc(sizeof(Employ));
    pEmploy->iId = 1;
    pEmploy->szName = (char*)malloc(sizeof(char) * SIZE_NAME_BUF);
    pEmploy->szName[0] = 'a';
    pEmploy->szName[1] = 'b';
    pEmploy->szName[2] = 'c';
    pEmploy->szName[3] = 0;
    CU_ASSERT(pList->push_back(pList, (Item)pEmploy) == SUCC);

    pEmploy = (Employ*)malloc(sizeof(Employ));
    pEmploy->iId = 2;
    pEmploy->szName = (char*)malloc(sizeof(char) * SIZE_NAME_BUF);
    pEmploy->szName[0] = 'd';
    pEmploy->szName[1] = 'e';
    pEmploy->szName[2] = 'f';
    pEmploy->szName[3] = 0;
    CU_ASSERT(pList->push_back(pList, (Item)pEmploy) == SUCC);

    pEmploy = (Employ*)malloc(sizeof(Employ));
    pEmploy->iId = 3;
    pEmploy->szName = (char*)malloc(sizeof(char) * SIZE_NAME_BUF);
    pEmploy->szName[0] = 'g';
    pEmploy->szName[1] = 'h';
    pEmploy->szName[2] = 'i';
    pEmploy->szName[3] = 0;
    CU_ASSERT(pList->push_back(pList, (Item)pEmploy) == SUCC);

    /* Replace item at the head and check the result. */
    Item item;
    pEmploy = (Employ*)malloc(sizeof(Employ));
    pEmploy->iId = 4;
    pEmploy->szName = (char*)malloc(sizeof(char) * SIZE_NAME_BUF);
    pEmploy->szName[0] = 'j';
    pEmploy->szName[1] = 'k';
    pEmploy->szName[2] = 'l';
    pEmploy->szName[3] = 0;
    CU_ASSERT(pList->set_front(pList, (Item)pEmploy) == SUCC);
    CU_ASSERT(pList->get_front(pList, &item) == SUCC);
    CU_ASSERT(strcmp(((Employ*)item)->szName, "jkl") == 0);

    /* Replace item at the tail and check the result. */
    pEmploy = (Employ*)malloc(sizeof(Employ));
    pEmploy->iId = 5;
    pEmploy->szName = (char*)malloc(sizeof(char) * SIZE_NAME_BUF);
    pEmploy->szName[0] = 'm';
    pEmploy->szName[1] = 'n';
    pEmploy->szName[2] = 'o';
    pEmploy->szName[3] = 0;
    CU_ASSERT(pList->set_back(pList, (Item)pEmploy) == SUCC);
    CU_ASSERT(pList->get_back(pList, &item) == SUCC);
    CU_ASSERT(strcmp(((Employ*)item)->szName, "mno") == 0);

    /* Replace item at the designated index and check the result. */
    pEmploy = (Employ*)malloc(sizeof(Employ));
    pEmploy->iId = 6;
    pEmploy->szName = (char*)malloc(sizeof(char) * SIZE_NAME_BUF);
    pEmploy->szName[0] = 'p';
    pEmploy->szName[1] = 'q';
    pEmploy->szName[2] = 'r';
    pEmploy->szName[3] = 0;
    CU_ASSERT(pList->set_at(pList, (Item)pEmploy, 0) == SUCC);
    CU_ASSERT(pList->get_at(pList, &item, 0) == SUCC);
    CU_ASSERT(strcmp(((Employ*)item)->szName, "pqr") == 0);

    pEmploy = (Employ*)malloc(sizeof(Employ));
    pEmploy->iId = 7;
    pEmploy->szName = (char*)malloc(sizeof(char) * SIZE_NAME_BUF);
    pEmploy->szName[0] = 's';
    pEmploy->szName[1] = 't';
    pEmploy->szName[2] = 'u';
    pEmploy->szName[3] = 0;
    CU_ASSERT(pList->set_at(pList, (Item)pEmploy, -3) == SUCC);
    CU_ASSERT(pList->get_at(pList, &item, -3) == SUCC);
    CU_ASSERT(strcmp(((Employ*)item)->szName, "stu") == 0);

    ListDeinit(&pList);
}

void TestReverse()
{
    LinkedList *pList;
    CU_ASSERT(ListInit(&pList) == SUCC);

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

    ListDeinit(&pList);
}

void TestBoundary()
{
    LinkedList *pList;
    CU_ASSERT(ListInit(&pList) == SUCC);

    /* Initial boundary test. */
    CU_ASSERT(pList->pop_front(pList) == ERR_IDX);
    CU_ASSERT(pList->pop_back(pList) == ERR_IDX);

    CU_ASSERT(pList->delete(pList, 0) == ERR_IDX);
    CU_ASSERT(pList->delete(pList, -1) == ERR_IDX);
    CU_ASSERT(pList->insert(pList, (Item)1, 1) == ERR_IDX);
    CU_ASSERT(pList->insert(pList, (Item)1, -1) == ERR_IDX);

    CU_ASSERT(pList->set_front(pList, (Item)1) == ERR_IDX);
    CU_ASSERT(pList->set_back(pList, (Item)1) == ERR_IDX);
    CU_ASSERT(pList->set_at(pList, (Item)1, 0) == ERR_IDX);
    CU_ASSERT(pList->set_at(pList, (Item)1, -1) == ERR_IDX);

    Item item;
    CU_ASSERT(pList->get_front(pList, &item) == ERR_IDX);
    CU_ASSERT_EQUAL(item, NULL);
    CU_ASSERT(pList->get_back(pList, &item) == ERR_IDX);
    CU_ASSERT_EQUAL(item, NULL);
    CU_ASSERT(pList->get_at(pList, &item, 0) == ERR_IDX);
    CU_ASSERT_EQUAL(item, NULL);
    CU_ASSERT(pList->get_at(pList, &item, -1) == ERR_IDX);
    CU_ASSERT_EQUAL(item, NULL);

    ListDeinit(&pList);
}
