#include "container/priority_queue.h"
#include "CUnit/Util.h"
#include "CUnit/Basic.h"


static const int SIZE_SML_TEST = 512;

typedef struct Tuple_ {
    int first;
    int second;
} Tuple;


/*-----------------------------------------------------------------------------*
 *          The utilities for element comparison and resource clean            *
 *-----------------------------------------------------------------------------*/
/* Let the integer with minimum value reside at the top position. */
int CompareNumerics(const void* lhs, const void* rhs)
{
    if ((intptr_t)lhs == (intptr_t)rhs)
        return 0;
    return ((intptr_t)lhs >= (intptr_t)rhs)? 1 : (-1);
}

/* Let the object with maximum value reside at the top position. */
int CompareObjects(const void* lhs, const void* rhs)
{
    Tuple* tpl_lhs = (Tuple*)lhs;
    Tuple* tpl_rhs = (Tuple*)rhs;
    if (tpl_lhs->first == tpl_rhs->first)
        return 0;
    return (tpl_lhs->first > tpl_rhs->first)? (-1) : 1;
}

void CleanObject(void* element)
{
    free(element);
}

/*-----------------------------------------------------------------------------*
 *            Unit tests relevant to basic structure verification              *
 *-----------------------------------------------------------------------------*/
void TestNewDelete()
{
    PriorityQueue* queue;
    CU_ASSERT((queue = PriorityQueueInit()) != NULL);

    /* Enlarge the queue size to test the destructor. */
    int i;
    for (i = 0 ; i < SIZE_SML_TEST ; ++i)
        CU_ASSERT(queue->push(queue, (void*)(intptr_t)i) == true);

    PriorityQueueDeinit(queue);
}

void TestOrderNumerics()
{
    srand(time(NULL));

    int elems[SIZE_SML_TEST];
    int i;
    for (i = 0 ; i < SIZE_SML_TEST ; ++i)
        elems[i] = i;
    for (i = 0 ; i < SIZE_SML_TEST ; ++i) {
        int src = rand() % SIZE_SML_TEST;
        int tge = rand() % SIZE_SML_TEST;
        int elem = elems[src];
        elems[src] = elems[tge];
        elems[tge] = elem;
    }

    PriorityQueue* queue = PriorityQueueInit();

    void* elem;
    CU_ASSERT(queue->top(queue, &elem) == false);

    for (i = 0 ; i < SIZE_SML_TEST ; ++i)
        CU_ASSERT(queue->push(queue, (void*)(intptr_t)elems[i]) == true);
    CU_ASSERT_EQUAL(queue->size(queue), SIZE_SML_TEST);

    for (i = 0 ; i < SIZE_SML_TEST ; ++i) {
        CU_ASSERT(queue->top(queue, &elem) == true);
        CU_ASSERT_EQUAL((int)(intptr_t)elem, i);
        CU_ASSERT(queue->pop(queue) == true);
    }
    CU_ASSERT(queue->top(queue, &elem) == false);
    CU_ASSERT(queue->pop(queue) == false);
    CU_ASSERT_EQUAL(queue->size(queue), 0);

    PriorityQueueDeinit(queue);
}

void TestOrderObjects()
{
    srand(time(NULL));

    void* tuples[SIZE_SML_TEST];
    int i;
    for (i = 0 ; i < SIZE_SML_TEST ; ++i) {
        Tuple* tuple = (Tuple*)malloc(sizeof(Tuple));
        tuple->first = i;
        tuple->second = i;
        tuples[i] = tuple;
    }
    for (i = 0 ; i < SIZE_SML_TEST ; ++i) {
        int src = rand() % SIZE_SML_TEST;
        int tge = rand() % SIZE_SML_TEST;
        void* tuple = tuples[src];
        tuples[src] = tuples[tge];
        tuples[tge] = tuple;
    }

    PriorityQueue* queue = PriorityQueueInit();
    queue->set_compare(queue, CompareObjects);
    queue->set_clean(queue, CleanObject);

    void* elem;
    CU_ASSERT(queue->top(queue, &elem) == false);

    for (i = 0 ; i < SIZE_SML_TEST ; ++i)
        CU_ASSERT(queue->push(queue, tuples[i]) == true);
    CU_ASSERT_EQUAL(queue->size(queue), SIZE_SML_TEST);

    for (i = SIZE_SML_TEST - 1 ; i >= 0 ; --i) {
        CU_ASSERT(queue->top(queue, &elem) == true);
        CU_ASSERT_EQUAL(((Tuple*)elem)->first, i);
        CU_ASSERT(queue->pop(queue) == true);
    }
    CU_ASSERT(queue->top(queue, &elem) == false);
    CU_ASSERT(queue->pop(queue) == false);
    CU_ASSERT_EQUAL(queue->size(queue), 0);

    PriorityQueueDeinit(queue);
}

/*-----------------------------------------------------------------------------*
 *                      The driver for PriorityQueue unit test                        *
 *-----------------------------------------------------------------------------*/
bool AddSuite()
{
    {
        CU_pSuite suite = CU_add_suite("Structure Verification", NULL, NULL);
        if (!suite)
            return false;

        CU_pTest unit = CU_add_test(suite, "New and Delete", TestNewDelete);
        if (!unit)
            return false;

        unit = CU_add_test(suite, "Integer Push, Pop, and Get", TestOrderNumerics);
        if (!unit)
            return false;
    }
    {
        CU_pSuite suite = CU_add_suite("Complex Data Maintenance", NULL, NULL);
        if (!suite)
            return false;

        CU_pTest unit = CU_add_test(suite, "Object Push, Pop, and Get", TestOrderObjects);
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

    /* Register the test suite for map structure verification. */
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
