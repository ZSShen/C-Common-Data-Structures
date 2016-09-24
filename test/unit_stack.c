#include "container/stack.h"
#include "CUnit/Util.h"
#include "CUnit/Basic.h"


static const int SIZE_SML_TEST = 64;

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
    Stack* stack;
    CU_ASSERT((stack = StackInit()) != NULL);

    /* Enlarge the stack size to test the destructor. */
    int i;
    for (i = 0 ; i < SIZE_SML_TEST ; ++i)
        CU_ASSERT(stack->push(stack, (void*)(intptr_t)i) == true);

    StackDeinit(stack);
}

void TestOrderNumerics()
{
    Stack* stack = StackInit();

    /* Boundary check. */
    void *element;
    CU_ASSERT(stack->pop(stack) == false);
    CU_ASSERT(stack->top(stack, &element) == false);

    int i;
    for (i = 0 ; i < SIZE_SML_TEST ; ++i)
        CU_ASSERT(stack->push(stack, (void*)(intptr_t)i) == true);
    CU_ASSERT_EQUAL(stack->size(stack), SIZE_SML_TEST);

    /* Sequentially retrieve the stack top element and remove it. */
    for (i = SIZE_SML_TEST - 1 ; i >= 0 ; --i) {
        CU_ASSERT(stack->top(stack, &element) == true);
        CU_ASSERT_EQUAL((int)(intptr_t)element, i);
        CU_ASSERT(stack->pop(stack) == true);
    }
    CU_ASSERT(stack->pop(stack) == false);
    CU_ASSERT_EQUAL(stack->size(stack), 0);

    StackDeinit(stack);
}


/*-----------------------------------------------------------------------------*
 *              Unit tests relevant to complex data maintenance                *
 *-----------------------------------------------------------------------------*/
void TestOrderObjects()
{
    Stack* stack = StackInit();
    stack->set_clean(stack, CleanObject);

    int i;
    for (i = 0 ; i < SIZE_SML_TEST ; ++i) {
        Tuple* tuple = (Tuple*)malloc(sizeof(Tuple));
        tuple->first = i;
        tuple->second = -i;
        CU_ASSERT(stack->push(stack, tuple) == true);
    }
    CU_ASSERT_EQUAL(stack->size(stack), SIZE_SML_TEST);

    /* Sequentially retrieve the stack top element and remove it. */
    for (i = SIZE_SML_TEST - 1 ; i >= 0 ; --i) {
        void* element;
        CU_ASSERT(stack->top(stack, &element) == true);
        CU_ASSERT_EQUAL(((Tuple*)element)->first, i);
        CU_ASSERT(stack->pop(stack) == true);
    }
    CU_ASSERT(stack->pop(stack) == false);
    CU_ASSERT_EQUAL(stack->size(stack), 0);

    for (i = 0 ; i < SIZE_SML_TEST >> 1 ; ++i) {
        Tuple* tuple = (Tuple*)malloc(sizeof(Tuple));
        tuple->first = i;
        tuple->second = -i;
        CU_ASSERT(stack->push(stack, tuple) == true);
    }

    StackDeinit(stack);
}


/*-----------------------------------------------------------------------------*
 *                       The driver for Stack unit test                        *
 *-----------------------------------------------------------------------------*/
bool AddSuite()
{
    {
        CU_pSuite suite = CU_add_suite("Structure Verification", NULL, NULL);
        if (!suite)
            return false;

        CU_pTest unit = CU_add_test(suite, "Stack New and Delete", TestNewDelete);
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

    /* Register the test suite for Stack structure verification. */
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
