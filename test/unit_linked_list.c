#include "container/linked_list.h"
#include "CUnit/Util.h"
#include "CUnit/Basic.h"


static const int SIZE_TNY_TEST = 128;
static const int SIZE_SML_TEST = 512;
static const int SIZE_MID_TEST = 2048;

/*-----------------------------------------------------------------------------*
 *                      The utilities for resource clean                       *
 *-----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------*
 *            Unit tests relevant to basic structure verification              *
 *-----------------------------------------------------------------------------*/
void TestNewDelete()
{
    List* list;
    CU_ASSERT((list = ListInit()) != NULL);

    /* Enlarge the list size to test the destructor. */
    unsigned i;
    for (i = 0 ; i < SIZE_SML_TEST ; ++i)
        CU_ASSERT(list->push_back(list, (void*)(intptr_t)i) == true);

    ListDeinit(list);
}

void TestPushFrontAndBack()
{
    {
        List* list = ListInit();

        void* element;
        CU_ASSERT(list->get_front(list, &element) == false);
        CU_ASSERT(list->get_back(list, &element) == false);
        CU_ASSERT(list->get_at(list, 0, &element) == false);

        /* Push elements to the list head. */
        CU_ASSERT(list->push_front(list, (void*)(intptr_t)4) == true);
        CU_ASSERT(list->push_front(list, (void*)(intptr_t)3) == true);
        CU_ASSERT(list->push_front(list, (void*)(intptr_t)2) == true);
        CU_ASSERT(list->push_front(list, (void*)(intptr_t)1) == true);

        /* Check element insertion sequence. */
        CU_ASSERT(list->get_front(list, &element) == true);
        CU_ASSERT_EQUAL(element, (void*)(intptr_t)1);
        CU_ASSERT(list->get_back(list, &element) == true);
        CU_ASSERT_EQUAL(element, (void*)(intptr_t)4);

        CU_ASSERT(list->get_at(list, 0, &element) == true);
        CU_ASSERT_EQUAL(element, (void*)(intptr_t)1);
        CU_ASSERT(list->get_at(list, 1, &element) == true);
        CU_ASSERT_EQUAL(element, (void*)(intptr_t)2);
        CU_ASSERT(list->get_at(list, 2, &element) == true);
        CU_ASSERT_EQUAL(element, (void*)(intptr_t)3);
        CU_ASSERT(list->get_at(list, 3, &element) == true);
        CU_ASSERT_EQUAL(element, (void*)(intptr_t)4);

        CU_ASSERT_EQUAL(list->size(list), 4);
        ListDeinit(list);
    }
    {
        List* list = ListInit();

        /* Push elements to the list tail. */
        CU_ASSERT(list->push_back(list, (void*)(intptr_t)1) == true);
        CU_ASSERT(list->push_back(list, (void*)(intptr_t)2) == true);
        CU_ASSERT(list->push_back(list, (void*)(intptr_t)3) == true);
        CU_ASSERT(list->push_back(list, (void*)(intptr_t)4) == true);

        void* element;
        CU_ASSERT(list->get_front(list, &element) == true);
        CU_ASSERT_EQUAL(element, (void*)(intptr_t)1);
        CU_ASSERT(list->get_back(list, &element) == true);
        CU_ASSERT_EQUAL(element, (void*)(intptr_t)4);

        CU_ASSERT(list->get_at(list, 0, &element) == true);
        CU_ASSERT_EQUAL(element, (void*)(intptr_t)1);
        CU_ASSERT(list->get_at(list, 1, &element) == true);
        CU_ASSERT_EQUAL(element, (void*)(intptr_t)2);
        CU_ASSERT(list->get_at(list, 2, &element) == true);
        CU_ASSERT_EQUAL(element, (void*)(intptr_t)3);
        CU_ASSERT(list->get_at(list, 3, &element) == true);
        CU_ASSERT_EQUAL(element, (void*)(intptr_t)4);

        CU_ASSERT_EQUAL(list->size(list), 4);
        ListDeinit(list);
    }
}

void TestInsert()
{
    List* list = ListInit();

    /* Insert elements to the specified indexes. */
    CU_ASSERT(list->insert(list, 1, (void*)(intptr_t)1) == false);

    CU_ASSERT(list->insert(list, 0, (void*)(intptr_t)1) == true);
    CU_ASSERT(list->insert(list, 1, (void*)(intptr_t)4) == true);
    CU_ASSERT(list->insert(list, 1, (void*)(intptr_t)2) == true);
    CU_ASSERT(list->insert(list, 2, (void*)(intptr_t)3) == true);
    CU_ASSERT(list->insert(list, 0, (void*)(intptr_t)0) == true);

    /* Check element insertion sequence. */
    void* element;
    CU_ASSERT(list->get_front(list, &element) == true);
    CU_ASSERT_EQUAL(element, (void*)(intptr_t)0);
    CU_ASSERT(list->get_back(list, &element) == true);
    CU_ASSERT_EQUAL(element, (void*)(intptr_t)4);

    CU_ASSERT(list->get_at(list, 0, &element) == true);
    CU_ASSERT_EQUAL(element, (void*)(intptr_t)0);
    CU_ASSERT(list->get_at(list, 1, &element) == true);
    CU_ASSERT_EQUAL(element, (void*)(intptr_t)1);
    CU_ASSERT(list->get_at(list, 2, &element) == true);
    CU_ASSERT_EQUAL(element, (void*)(intptr_t)2);
    CU_ASSERT(list->get_at(list, 3, &element) == true);
    CU_ASSERT_EQUAL(element, (void*)(intptr_t)3);
    CU_ASSERT(list->get_at(list, 4, &element) == true);
    CU_ASSERT_EQUAL(element, (void*)(intptr_t)4);

    CU_ASSERT_EQUAL(list->size(list), 5);
    ListDeinit(list);
}

void TestPopFrontAndBack()
{
    {
        List* list = ListInit();

        CU_ASSERT(list->pop_front(list) == false);

        /* Prepare the initial elements. */
        CU_ASSERT(list->push_back(list, (void*)(intptr_t)1) == true);
        CU_ASSERT(list->push_back(list, (void*)(intptr_t)2) == true);
        CU_ASSERT(list->push_back(list, (void*)(intptr_t)3) == true);
        CU_ASSERT(list->push_back(list, (void*)(intptr_t)4) == true);

        /* Pop elements from the list head and check the remaining element sequence. */
        void* element;
        CU_ASSERT(list->pop_front(list) == true);
        CU_ASSERT(list->get_front(list, &element) == true);
        CU_ASSERT_EQUAL(element, (void*)(intptr_t)2);
        CU_ASSERT(list->get_back(list, &element) == true);
        CU_ASSERT_EQUAL(element, (void*)(intptr_t)4);
        CU_ASSERT(list->get_at(list, 0, &element) == true);
        CU_ASSERT_EQUAL(element, (void*)(intptr_t)2);
        CU_ASSERT(list->get_at(list, 1, &element) == true);
        CU_ASSERT_EQUAL(element, (void*)(intptr_t)3);
        CU_ASSERT(list->get_at(list, 2, &element) == true);
        CU_ASSERT_EQUAL(element, (void*)(intptr_t)4);

        CU_ASSERT(list->pop_front(list) == true);
        CU_ASSERT(list->get_front(list, &element) == true);
        CU_ASSERT_EQUAL(element, (void*)(intptr_t)3);
        CU_ASSERT(list->get_back(list, &element) == true);
        CU_ASSERT_EQUAL(element, (void*)(intptr_t)4);
        CU_ASSERT(list->get_at(list, 0, &element) == true);
        CU_ASSERT_EQUAL(element, (void*)(intptr_t)3);
        CU_ASSERT(list->get_at(list, 1, &element) == true);
        CU_ASSERT_EQUAL(element, (void*)(intptr_t)4);

        CU_ASSERT(list->pop_front(list) == true);
        CU_ASSERT(list->get_front(list, &element) == true);
        CU_ASSERT_EQUAL(element, (void*)(intptr_t)4);
        CU_ASSERT(list->get_back(list, &element) == true);
        CU_ASSERT_EQUAL(element, (void*)(intptr_t)4);
        CU_ASSERT(list->get_at(list, 0, &element) == true);
        CU_ASSERT_EQUAL(element, (void*)(intptr_t)4);

        CU_ASSERT_EQUAL(list->size(list), 1);

        CU_ASSERT(list->pop_front(list) == true);
        CU_ASSERT(list->get_front(list, &element) == false);
        CU_ASSERT(list->get_back(list, &element) == false);
        CU_ASSERT(list->get_at(list, 0, &element) == false);

        CU_ASSERT(list->pop_front(list) == false);

        /* Insert the element again to check if the list is well handled in
           the previous test. */
        CU_ASSERT(list->push_back(list, (void*)(intptr_t)777) == true);
        CU_ASSERT(list->get_front(list, &element) == true);
        CU_ASSERT_EQUAL(element, (void*)(intptr_t)777);
        CU_ASSERT(list->get_back(list, &element) == true);
        CU_ASSERT_EQUAL(element, (void*)(intptr_t)777);
        CU_ASSERT_EQUAL(list->size(list), 1);

        ListDeinit(list);
    }
    {
        List* list = ListInit();

        CU_ASSERT(list->pop_back(list) == false);

        /* Prepare the initial elements. */
        CU_ASSERT(list->push_back(list, (void*)(intptr_t)1) == true);
        CU_ASSERT(list->push_back(list, (void*)(intptr_t)2) == true);
        CU_ASSERT(list->push_back(list, (void*)(intptr_t)3) == true);
        CU_ASSERT(list->push_back(list, (void*)(intptr_t)4) == true);

        /* Pop elements from the list tail and check the remaining element sequence. */
        void* element;
        CU_ASSERT(list->pop_back(list) == true);
        CU_ASSERT(list->get_front(list, &element) == true);
        CU_ASSERT_EQUAL(element, (void*)(intptr_t)1);
        CU_ASSERT(list->get_back(list, &element) == true);
        CU_ASSERT_EQUAL(element, (void*)(intptr_t)3);
        CU_ASSERT(list->get_at(list, 0, &element) == true);
        CU_ASSERT_EQUAL(element, (void*)(intptr_t)1);
        CU_ASSERT(list->get_at(list, 1, &element) == true);
        CU_ASSERT_EQUAL(element, (void*)(intptr_t)2);
        CU_ASSERT(list->get_at(list, 2, &element) == true);
        CU_ASSERT_EQUAL(element, (void*)(intptr_t)3);

        CU_ASSERT(list->pop_back(list) == true);
        CU_ASSERT(list->get_front(list, &element) == true);
        CU_ASSERT_EQUAL(element, (void*)(intptr_t)1);
        CU_ASSERT(list->get_back(list, &element) == true);
        CU_ASSERT_EQUAL(element, (void*)(intptr_t)2);
        CU_ASSERT(list->get_at(list, 0, &element) == true);
        CU_ASSERT_EQUAL(element, (void*)(intptr_t)1);
        CU_ASSERT(list->get_at(list, 1, &element) == true);
        CU_ASSERT_EQUAL(element, (void*)(intptr_t)2);

        CU_ASSERT(list->pop_back(list) == true);
        CU_ASSERT(list->get_front(list, &element) == true);
        CU_ASSERT_EQUAL(element, (void*)(intptr_t)1);
        CU_ASSERT(list->get_back(list, &element) == true);
        CU_ASSERT_EQUAL(element, (void*)(intptr_t)1);
        CU_ASSERT(list->get_at(list, 0, &element) == true);
        CU_ASSERT_EQUAL(element, (void*)(intptr_t)1);

        CU_ASSERT_EQUAL(list->size(list), 1);

        CU_ASSERT(list->pop_back(list) == true);
        CU_ASSERT(list->get_front(list, &element) == false);
        CU_ASSERT(list->get_back(list, &element) == false);
        CU_ASSERT(list->get_at(list, 0, &element) == false);

        CU_ASSERT(list->pop_back(list) == false);

        /* Insert the element again to check if the list is well handled in
           the previous test. */
        CU_ASSERT(list->push_back(list, (void*)(intptr_t)777) == true);
        CU_ASSERT(list->get_front(list, &element) == true);
        CU_ASSERT_EQUAL(element, (void*)(intptr_t)777);
        CU_ASSERT(list->get_back(list, &element) == true);
        CU_ASSERT_EQUAL(element, (void*)(intptr_t)777);
        CU_ASSERT_EQUAL(list->size(list), 1);

        ListDeinit(list);
    }
}

void TestRemove()
{
    List* list = ListInit();

    /* Prepare the initial elements. */
    CU_ASSERT(list->push_back(list, (void*)(intptr_t)1) == true);
    CU_ASSERT(list->push_back(list, (void*)(intptr_t)2) == true);
    CU_ASSERT(list->push_back(list, (void*)(intptr_t)3) == true);
    CU_ASSERT(list->push_back(list, (void*)(intptr_t)4) == true);
    CU_ASSERT(list->push_back(list, (void*)(intptr_t)5) == true);

    /* Remove elements from the specified index and check the remaining
       element sequence. */
    void* element;
    CU_ASSERT(list->remove(list, 0) == true);
    CU_ASSERT(list->get_front(list, &element) == true);
    CU_ASSERT_EQUAL(element, (void*)(intptr_t)2);
    CU_ASSERT(list->get_back(list, &element) == true);
    CU_ASSERT_EQUAL(element, (void*)(intptr_t)5);
    CU_ASSERT(list->get_at(list, 0, &element) == true);
    CU_ASSERT_EQUAL(element, (void*)(intptr_t)2);
    CU_ASSERT(list->get_at(list, 1, &element) == true);
    CU_ASSERT_EQUAL(element, (void*)(intptr_t)3);
    CU_ASSERT(list->get_at(list, 2, &element) == true);
    CU_ASSERT_EQUAL(element, (void*)(intptr_t)4);
    CU_ASSERT(list->get_at(list, 3, &element) == true);
    CU_ASSERT_EQUAL(element, (void*)(intptr_t)5);

    CU_ASSERT(list->remove(list, 1) == true);
    CU_ASSERT(list->get_front(list, &element) == true);
    CU_ASSERT_EQUAL(element, (void*)(intptr_t)2);
    CU_ASSERT(list->get_back(list, &element) == true);
    CU_ASSERT_EQUAL(element, (void*)(intptr_t)5);
    CU_ASSERT(list->get_at(list, 0, &element) == true);
    CU_ASSERT_EQUAL(element, (void*)(intptr_t)2);
    CU_ASSERT(list->get_at(list, 1, &element) == true);
    CU_ASSERT_EQUAL(element, (void*)(intptr_t)4);
    CU_ASSERT(list->get_at(list, 2, &element) == true);
    CU_ASSERT_EQUAL(element, (void*)(intptr_t)5);

    CU_ASSERT(list->remove(list, 2) == true);
    CU_ASSERT(list->get_front(list, &element) == true);
    CU_ASSERT_EQUAL(element, (void*)(intptr_t)2);
    CU_ASSERT(list->get_back(list, &element) == true);
    CU_ASSERT_EQUAL(element, (void*)(intptr_t)4);
    CU_ASSERT(list->get_at(list, 0, &element) == true);
    CU_ASSERT_EQUAL(element, (void*)(intptr_t)2);
    CU_ASSERT(list->get_at(list, 1, &element) == true);
    CU_ASSERT_EQUAL(element, (void*)(intptr_t)4);

    CU_ASSERT(list->remove(list, 1) == true);
    CU_ASSERT(list->get_front(list, &element) == true);
    CU_ASSERT_EQUAL(element, (void*)(intptr_t)2);
    CU_ASSERT(list->get_back(list, &element) == true);
    CU_ASSERT_EQUAL(element, (void*)(intptr_t)2);
    CU_ASSERT(list->get_at(list, 0, &element) == true);
    CU_ASSERT_EQUAL(element, (void*)(intptr_t)2);

    CU_ASSERT_EQUAL(list->size(list), 1);

    CU_ASSERT(list->remove(list, 0) == true);
    CU_ASSERT(list->remove(list, 0) == false);
    CU_ASSERT(list->get_front(list, &element) == false);
    CU_ASSERT(list->get_back(list, &element) == false);
    CU_ASSERT(list->get_at(list, 0, &element) == false);

    /* Insert the element again to check if the list is well handled in
       the previous test. */
    CU_ASSERT(list->push_back(list, (void*)(intptr_t)777) == true);
    CU_ASSERT(list->get_front(list, &element) == true);
    CU_ASSERT_EQUAL(element, (void*)(intptr_t)777);
    CU_ASSERT(list->get_back(list, &element) == true);
    CU_ASSERT_EQUAL(element, (void*)(intptr_t)777);
    CU_ASSERT_EQUAL(list->size(list), 1);

    ListDeinit(list);
}

void TestReplace()
{
    List* list = ListInit();

    CU_ASSERT(list->set_front(list, (void*)(intptr_t)0) == false);
    CU_ASSERT(list->set_back(list, (void*)(intptr_t)0) == false);
    CU_ASSERT(list->set_at(list, 0, (void*)(intptr_t)0) == false);

    /* Prepare the initial elements. */
    CU_ASSERT(list->push_back(list, (void*)(intptr_t)1) == true);
    CU_ASSERT(list->push_back(list, (void*)(intptr_t)2) == true);
    CU_ASSERT(list->push_back(list, (void*)(intptr_t)3) == true);
    CU_ASSERT(list->push_back(list, (void*)(intptr_t)4) == true);

    /* Replace the element residing at the list head and check the result. */
    void* element;
    CU_ASSERT(list->set_front(list, (void*)(intptr_t)-1) == true);
    CU_ASSERT(list->get_front(list, &element) == true);
    CU_ASSERT_EQUAL(element, (void*)(intptr_t)-1);

    /* Replace the element residing at the list tail and check the result. */
    CU_ASSERT(list->set_back(list, (void*)(intptr_t)-3) == true);
    CU_ASSERT(list->get_back(list, &element) == true);
    CU_ASSERT_EQUAL(element, (void*)(intptr_t)-3);

    /* Replace the elements residing at the specified indexes and check the result. */
    CU_ASSERT(list->set_at(list, 0, (void*)(intptr_t)10) == true);
    CU_ASSERT(list->get_at(list, 0, &element) == true);
    CU_ASSERT_EQUAL(element, (void*)(intptr_t)10);

    CU_ASSERT(list->set_at(list, 1, (void*)(intptr_t)20) == true);
    CU_ASSERT(list->get_at(list, 1, &element) == true);
    CU_ASSERT_EQUAL(element, (void*)(intptr_t)20);

    CU_ASSERT(list->set_at(list, 2, (void*)(intptr_t)30) == true);
    CU_ASSERT(list->get_at(list, 2, &element) == true);
    CU_ASSERT_EQUAL(element, (void*)(intptr_t)30);

    CU_ASSERT(list->set_at(list, 3, (void*)(intptr_t)40) == true);
    CU_ASSERT(list->get_at(list, 3, &element) == true);
    CU_ASSERT_EQUAL(element, (void*)(intptr_t)40);

    CU_ASSERT(list->set_at(list, 4, (void*)(intptr_t)0) == false);

    ListDeinit(list);
}

void TestReverse()
{
    List* list = ListInit();

    /* Prepare the initial elements. */
    CU_ASSERT(list->push_back(list, (void*)(intptr_t)1) == true);
    CU_ASSERT(list->push_back(list, (void*)(intptr_t)2) == true);
    CU_ASSERT(list->push_back(list, (void*)(intptr_t)3) == true);
    CU_ASSERT(list->push_back(list, (void*)(intptr_t)4) == true);

    list->reverse(list);

    /* Check the reversed element sequence. */
    void* element;
    CU_ASSERT(list->get_at(list, 0, &element) == true);
    CU_ASSERT_EQUAL(element, (void*)(intptr_t)4);
    CU_ASSERT(list->get_at(list, 1, &element) == true);
    CU_ASSERT_EQUAL(element, (void*)(intptr_t)3);
    CU_ASSERT(list->get_at(list, 2, &element) == true);
    CU_ASSERT_EQUAL(element, (void*)(intptr_t)2);
    CU_ASSERT(list->get_at(list, 3, &element) == true);
    CU_ASSERT_EQUAL(element, (void*)(intptr_t)1);

    ListDeinit(list);
}

void TestIterator()
{
    List* list = ListInit();

    /* Prepare the initial elements. */
    CU_ASSERT(list->push_back(list, (void*)(intptr_t)1) == true);
    CU_ASSERT(list->push_back(list, (void*)(intptr_t)2) == true);
    CU_ASSERT(list->push_back(list, (void*)(intptr_t)3) == true);
    CU_ASSERT(list->push_back(list, (void*)(intptr_t)4) == true);

    /* Iterate through the list. */
    void* element;
    int check = 1;

    list->first(list, false);
    while (list->next(list, &element)) {
        CU_ASSERT_EQUAL((int)(intptr_t)element, check);
        ++check;
    }

    /* Iterate through the list in reversed order. */
    check = 4;
    list->first(list, true);
    while (list->reverse_next(list, &element)) {
        CU_ASSERT_EQUAL((int)(intptr_t)element, check);
        --check;
    }

    ListDeinit(list);
}

/*-----------------------------------------------------------------------------*
 *                      The driver for List unit test                        *
 *-----------------------------------------------------------------------------*/
bool AddSuite()
{
    {
        CU_pSuite suite = CU_add_suite("Structure Verification", NULL, NULL);
        if (!suite)
            return false;

        CU_pTest unit = CU_add_test(suite, "List New and Delete", TestNewDelete);
        if (!unit)
            return false;

        unit = CU_add_test(suite, "Element Push Front and Back", TestPushFrontAndBack);
        if (!unit)
            return false;

        unit = CU_add_test(suite, "Element Insert", TestInsert);
        if (!unit)
            return false;

        unit = CU_add_test(suite, "Element Pop Front and Back", TestPopFrontAndBack);
        if (!unit)
            return false;

        unit = CU_add_test(suite, "Element Remove", TestRemove);
        if (!unit)
            return false;

        unit = CU_add_test(suite, "Element Replace", TestReplace);
        if (!unit)
            return false;

        unit = CU_add_test(suite, "List Reverse", TestReverse);
        if (!unit)
            return false;

        unit = CU_add_test(suite, "List Iterator", TestIterator);
        if (!unit)
            return false;
    }

    return true;
}

int main()
{
    int rc = 0;

    if (CU_initialize_registry() != CUE_SUCCESS) {
        rc = CU_get_error();
        goto EXIT;
    }

    /* Register the test suites to verify List functionalities. */
    if (AddSuite() == false) {
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
