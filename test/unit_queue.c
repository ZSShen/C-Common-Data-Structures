#include "container/queue.h"
#include "CUnit/Util.h"
#include "CUnit/Basic.h"


static const int SIZE_SML_TEST = 32;

typedef struct Tuple_ {
    int first;
    int second;
} Tuple;


/*-----------------------------------------------------------------------------*
 *                  The utilities for element resource clean                   *
 *-----------------------------------------------------------------------------*/
void CleanObject(void* element)
{
    free(element);
}


/*-----------------------------------------------------------------------------*
 *            Unit tests relevant to basic structure verification              *
 *-----------------------------------------------------------------------------*/
void TestNewDelete()
{
    Queue* queue;
    CU_ASSERT((queue = QueueInit()) != NULL);

    /* Enlarge the queue size to test the destructor. */
    int i;
    for (i = 0 ; i < SIZE_SML_TEST ; ++i)
        CU_ASSERT(queue->push(queue, (void*)(intptr_t)i) == true);

    QueueDeinit(queue);
}

void TestOrderNumerics()
{
    Queue* queue = QueueInit();

    /* Boundary check. */
    void *element;
    CU_ASSERT(queue->pop(queue) == false);
    CU_ASSERT(queue->front(queue, &element) == false);
    CU_ASSERT(queue->back(queue, &element) == false);

    int i;
    for (i = 0 ; i < SIZE_SML_TEST ; ++i)
        CU_ASSERT(queue->push(queue, (void*)(intptr_t)i) == true);
    CU_ASSERT_EQUAL(queue->size(queue), SIZE_SML_TEST);

    /* Remove the elements ranging from 0 to 15. */
    for (i = 0 ; i < SIZE_SML_TEST >> 1 ; ++i)
        CU_ASSERT(queue->pop(queue) == true);

    /* Re-push the removed elements. */
    for (i = 0 ; i < SIZE_SML_TEST >> 1 ; ++i)
        CU_ASSERT(queue->push(queue, (void*)(intptr_t)i) == true);

    /* Check the queue order. */
    CU_ASSERT(queue->front(queue, &element) == true);
    CU_ASSERT_EQUAL((int)(intptr_t)element, SIZE_SML_TEST >> 1);
    CU_ASSERT(queue->back(queue, &element) == true);
    CU_ASSERT_EQUAL((int)(intptr_t)element, (SIZE_SML_TEST >> 1) - 1);

    /* Push another set of elements. */
    for (i = SIZE_SML_TEST ; i < SIZE_SML_TEST << 1 ; ++i)
        CU_ASSERT(queue->push(queue, (void*)(intptr_t)i) == true);

    /* Remove the elements ranging from 16 to 31. */
    for (i = 0 ; i < SIZE_SML_TEST >> 1 ; ++i)
        CU_ASSERT(queue->pop(queue) == true);

    /* Check the queue order. */
    CU_ASSERT(queue->front(queue, &element) == true);
    CU_ASSERT_EQUAL((int)(intptr_t)element, 0);
    CU_ASSERT(queue->back(queue, &element) == true);
    CU_ASSERT_EQUAL((int)(intptr_t)element, (SIZE_SML_TEST << 1) - 1);

    QueueDeinit(queue);
}


/*-----------------------------------------------------------------------------*
 *              Unit tests relevant to complex data maintenance                *
 *-----------------------------------------------------------------------------*/
void TestOrderObjects()
{
    Queue* queue = QueueInit();
    queue->set_clean(queue, CleanObject);

    int i;
    for (i = 0 ; i < SIZE_SML_TEST ; ++i) {
        Tuple* tuple = (Tuple*)malloc(sizeof(Tuple));
        tuple->first = i;
        tuple->second = -i;
        CU_ASSERT(queue->push(queue, tuple) == true);
    }
    CU_ASSERT_EQUAL(queue->size(queue), SIZE_SML_TEST);

    /* Remove the elements ranging from 0 to 15. */
    for (i = 0 ; i < SIZE_SML_TEST >> 1 ; ++i)
        CU_ASSERT(queue->pop(queue) == true);

    /* Re-push the removed elements. */
    for (i = 0 ; i < SIZE_SML_TEST >> 1 ; ++i) {
        Tuple* tuple = (Tuple*)malloc(sizeof(Tuple));
        tuple->first = i;
        tuple->second = -i;
        CU_ASSERT(queue->push(queue, tuple) == true);
    }

    /* Check the queue order. */
    void* element;
    CU_ASSERT(queue->front(queue, &element) == true);
    CU_ASSERT_EQUAL(((Tuple*)element)->first, SIZE_SML_TEST >> 1);
    CU_ASSERT(queue->back(queue, &element) == true);
    CU_ASSERT_EQUAL(((Tuple*)element)->first, (SIZE_SML_TEST >> 1) - 1);

    /* Push another set of elements. */
    for (i = SIZE_SML_TEST ; i < SIZE_SML_TEST << 1 ; ++i) {
        Tuple* tuple = (Tuple*)malloc(sizeof(Tuple));
        tuple->first = i;
        tuple->second = -i;
        CU_ASSERT(queue->push(queue, tuple) == true);
    }

    /* Remove the elements ranging from 16 to 31. */
    for (i = 0 ; i < SIZE_SML_TEST >> 1 ; ++i)
        CU_ASSERT(queue->pop(queue) == true);

    /* Check the queue order. */
    CU_ASSERT(queue->front(queue, &element) == true);
    CU_ASSERT_EQUAL(((Tuple*)element)->first, 0);
    CU_ASSERT(queue->back(queue, &element) == true);
    CU_ASSERT_EQUAL(((Tuple*)element)->first, (SIZE_SML_TEST << 1) - 1);

    QueueDeinit(queue);
}


/*-----------------------------------------------------------------------------*
 *                       The driver for Queue unit test                        *
 *-----------------------------------------------------------------------------*/
bool AddSuite()
{
    {
        CU_pSuite suite = CU_add_suite("Structure Verification", NULL, NULL);
        if (!suite)
            return false;

        CU_pTest unit = CU_add_test(suite, "Queue New and Delete", TestNewDelete);
        if (!unit)
            return false;

        unit = CU_add_test(suite, "Numerics Push, Pop, and Get", TestOrderNumerics);
        if (!unit)
            return false;
    }
    {
        CU_pSuite suite = CU_add_suite("Complex Data Maintenance", NULL, NULL);
        if (!suite)
            return false;

        CU_pTest unit = CU_add_test(suite, "Objects Push, Pop, and Get", TestOrderObjects);
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

    /* Register the test suite for Queue structure verification. */
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
