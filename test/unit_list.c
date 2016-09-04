#include "container/list.h"
#include "CUnit/Util.h"
#include "CUnit/Basic.h"


static const int SIZE_TXT_BUFF = 32;
static const int SIZE_TNY_TEST = 128;
static const int SIZE_SML_TEST = 512;
static const int SIZE_MID_TEST = 2048;


/*-----------------------------------------------------------------------------*
 *                      The utilities for resource clean                       *
 *-----------------------------------------------------------------------------*/
void CleanElement(void* element)
{
    free(element);
}


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
 *              Unit tests relevant to complex data maintenance                *
 *-----------------------------------------------------------------------------*/
void TestObjectInsert()
{
    char* nums[SIZE_MID_TEST];
    char buf[SIZE_TXT_BUFF];
    int idx = 0;
    while (idx < SIZE_MID_TEST) {
        snprintf(buf, SIZE_TXT_BUFF, "%d", idx);
        nums[idx] = strdup((const char*)buf);
        ++idx;
    }

    List* list = ListInit();
    list->set_clean(list, CleanElement);

    /* Push the elements ranging from 1536 to 2047 to the list tail. */
    idx = SIZE_MID_TEST - SIZE_SML_TEST;
    while (idx < SIZE_MID_TEST) {
        CU_ASSERT(list->push_back(list, strdup(nums[idx])) == true);
        ++idx;
    }

    /* Push the elements ranging from 0 to 511 to the list head. */
    idx = SIZE_SML_TEST - 1;
    while (idx >= 0) {
        CU_ASSERT(list->push_front(list, strdup(nums[idx])) == true);
        --idx;
    }

    /* Insert the elements ranging from 512 to 1535. */
    idx = SIZE_SML_TEST;
    int bnd = SIZE_MID_TEST - SIZE_SML_TEST;
    while (idx < bnd) {
        CU_ASSERT(list->insert(list, idx, strdup(nums[idx])) == true);
        ++idx;
    }

    /* Check the element sequence. */
    void* element;
    CU_ASSERT(list->get_front(list, &element) == true);
    CU_ASSERT(strcmp((char*)element, nums[0]) == 0);

    CU_ASSERT(list->get_back(list, &element) == true);
    CU_ASSERT(strcmp((char*)element, nums[SIZE_MID_TEST - 1]) == 0);

    idx = 0;
    while (idx < SIZE_MID_TEST) {
        CU_ASSERT(list->get_at(list, idx, &element) == true);
        CU_ASSERT(strcmp((char*)element, nums[idx]) == 0);
        ++idx;
    }

    ListDeinit(list);

    idx = 0;
    while (idx < SIZE_MID_TEST) {
        free(nums[idx]);
        ++idx;
    }
}

void TestObjectRemove()
{
    char* nums[SIZE_MID_TEST];
    char buf[SIZE_TXT_BUFF];
    int idx = 0;
    while (idx < SIZE_MID_TEST) {
        snprintf(buf, SIZE_TXT_BUFF, "%d", idx);
        nums[idx] = strdup((const char*)buf);
        ++idx;
    }

    List* list = ListInit();
    list->set_clean(list, CleanElement);

    /* Push the elements ranging from 0 to 2047. */
    idx = 0;
    while (idx < SIZE_MID_TEST) {
        CU_ASSERT(list->push_back(list, strdup(nums[idx])) == true);
        ++idx;
    }

    /* Pop the elements ranging from 0 to 511 at the list head. */
    idx = 0;
    while (idx < SIZE_SML_TEST) {
        CU_ASSERT(list->pop_front(list) == true);
        ++idx;
    }

    /* Pop the elements ranging from 1536 to 2047 at the list tail. */
    idx = 0;
    while (idx < SIZE_SML_TEST) {
        CU_ASSERT(list->pop_back(list) == true);
        ++idx;
    }

    /* Remove the elements ranging from 1024 to 1535. */
    idx = 0;
    while (idx < SIZE_SML_TEST) {
        CU_ASSERT(list->remove(list, SIZE_SML_TEST) == true);
        ++idx;
    }

    /* Check the element sequence. */
    void* element;
    CU_ASSERT(list->get_front(list, &element) == true);
    CU_ASSERT(strcmp((char*)element, nums[SIZE_SML_TEST]) == 0);

    CU_ASSERT(list->get_back(list, &element) == true);
    CU_ASSERT(strcmp((char*)element, nums[(SIZE_SML_TEST << 1) - 1]) == 0);

    idx = 0;
    while (idx < SIZE_SML_TEST) {
        CU_ASSERT(list->get_at(list, idx, &element) == true);
        CU_ASSERT(strcmp((char*)element, nums[idx + SIZE_SML_TEST]) == 0);
        ++idx;
    }

    ListDeinit(list);

    idx = 0;
    while (idx < SIZE_MID_TEST) {
        free(nums[idx]);
        ++idx;
    }
}

void TestObjectReplace()
{
    char* nums[SIZE_SML_TEST];
    char buf[SIZE_TXT_BUFF];
    int idx = 0;
    while (idx < SIZE_SML_TEST) {
        snprintf(buf, SIZE_TXT_BUFF, "%d", idx);
        nums[idx] = strdup((const char*)buf);
        ++idx;
    }

    List* list = ListInit();
    list->set_clean(list, CleanElement);

    /* Push the elements ranging from 0 to 511. */
    idx = 0;
    while (idx < SIZE_SML_TEST) {
        CU_ASSERT(list->push_back(list, strdup(nums[idx])) == true);
        ++idx;
    }

    /* Reverse the list via element replacement. */
    CU_ASSERT(list->set_front(list, strdup(nums[SIZE_SML_TEST - 1])) == true);
    CU_ASSERT(list->set_back(list, strdup((nums[0]))) == true);
    idx = 1;
    while (idx < SIZE_SML_TEST - 1) {
        CU_ASSERT(list->set_at(list, idx, strdup(nums[SIZE_SML_TEST - 1 - idx])) == true);
        ++idx;
    }

    /* Check the element sequence. */
    void* element;
    idx = 0;
    while (idx < SIZE_SML_TEST) {
        CU_ASSERT(list->get_at(list, idx, &element) == true);
        CU_ASSERT(strcmp((char*)element, nums[SIZE_SML_TEST - 1 - idx]) == 0);
        ++idx;
    }

    ListDeinit(list);

    idx = 0;
    while (idx < SIZE_SML_TEST) {
        free(nums[idx]);
        ++idx;
    }
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
    {
        CU_pSuite suite = CU_add_suite("Complex Data Maintenance", NULL, NULL);
        if (!suite)
            return false;

        CU_pTest unit = CU_add_test(suite, "Object Push and Insert", TestObjectInsert);
        if (!unit)
            return false;

        unit = CU_add_test(suite, "Object Pop and Remove", TestObjectRemove);
        if (!unit)
            return false;

        unit = CU_add_test(suite, "Object Replace", TestObjectReplace);
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
