#include "container/vector.h"
#include "CUnit/Util.h"
#include "CUnit/Basic.h"


/*------------------------------------------------------------*
 *    Test Function Declaration for Structure Verification    *
 *------------------------------------------------------------*/
static const int DEFAULT_CAPACITY = 512;
static const int SIZE_TNY_TEST = 128;
static const int SIZE_SML_TEST = 512;
static const int SIZE_MID_TEST = 2048;

typedef struct Tuple_ {
    int first;
    int second;
} Tuple;


/*-----------------------------------------------------------------------------*
 *                      The utilities for resource clean                       *
 *-----------------------------------------------------------------------------*/
void CleanElement(void* element)
{
    free(element);
}

int SortElement(const void* lhs, const void* rhs)
{
    Tuple* tpl_lhs = *((Tuple**)lhs);
    Tuple* tpl_rhs = *((Tuple**)rhs);
    if (tpl_lhs->first == tpl_rhs->first)
        return 0;
    return (tpl_lhs->first > tpl_rhs->first)? 1 : -1;
}

/*-----------------------------------------------------------------------------*
 *            Unit tests relevant to basic structure verification              *
 *-----------------------------------------------------------------------------*/
void TestNewDelete()
{
    Vector* vector;
    CU_ASSERT((vector = VectorInit(DEFAULT_CAPACITY)) != NULL);

    /* Enlarge the vector size to test the destructor. */
    unsigned i;
    for (i = 0 ; i < SIZE_SML_TEST ; ++i)
        CU_ASSERT(vector->push_back(vector, (void*)(intptr_t)i) == true);

    VectorDeinit(vector);
}

void TestResize()
{
    Vector* vector = VectorInit(DEFAULT_CAPACITY);

    /* Implicitly trigger the resize function. */
    unsigned i;
    for (i = 0 ; i < SIZE_MID_TEST ; ++i)
       vector->push_back(vector, (void*)(intptr_t)i);

    /* Explicitly trigger the resize function. */
    CU_ASSERT(vector->resize(vector, SIZE_MID_TEST << 1) == true);
    CU_ASSERT(vector->resize(vector, SIZE_TNY_TEST) == true);
    VectorDeinit(vector);

    /* Test the garbage collection of resize function. */
    vector = VectorInit(DEFAULT_CAPACITY);
    vector->set_clean(vector, CleanElement);
    for (i = 0 ; i < SIZE_MID_TEST ; ++i) {
        Tuple* tuple = (Tuple*)malloc(sizeof(Tuple));
        tuple->first = i;
        tuple->second = i;
        vector->push_back(vector, tuple);
    }
    CU_ASSERT(vector->resize(vector, SIZE_TNY_TEST) == true);
    VectorDeinit(vector);
}

void TestPushAndInsert()
{
    Vector* vector = VectorInit(DEFAULT_CAPACITY);
    vector->set_clean(vector, CleanElement);
    void* dummy;

    /* Push elements ranging from SML to (SML * 2) to the vector tail. */
    unsigned num = SIZE_SML_TEST;
    unsigned i;
    for (i = 0 ; i < SIZE_SML_TEST ; ++i) {
        Tuple* tuple = (Tuple*)malloc(sizeof(Tuple));
        tuple->first = num;
        tuple->second = num;
        CU_ASSERT(vector->push_back(vector, tuple) == true);
        ++num;
    }
    num = SIZE_SML_TEST;
    for (i = 0 ; i < SIZE_SML_TEST ; ++i) {
        void* tuple;
        CU_ASSERT(vector->get(vector, i, &tuple) == true);
        CU_ASSERT_EQUAL(((Tuple*)tuple)->first, num);
        ++num;
    }
    CU_ASSERT(vector->get(vector, SIZE_SML_TEST << 1, &dummy) == false);
    CU_ASSERT(vector->get(vector, SIZE_MID_TEST, &dummy) == false);

    /* Insert elements ranging from 0 to SML to the vector head. */
    num = SIZE_SML_TEST - 1;
    for (i = 0 ; i < SIZE_SML_TEST ; ++i) {
        Tuple* tuple = (Tuple*)malloc(sizeof(Tuple));
        tuple->first = num;
        tuple->second = num;
        CU_ASSERT(vector->insert(vector, 0, tuple) == true);
        --num;
    }
    for (i = 0 ; i < SIZE_SML_TEST ; ++i) {
        void* tuple;
        CU_ASSERT(vector->get(vector, i, &tuple) == true);
        CU_ASSERT_EQUAL(((Tuple*)tuple)->first, i);
    }
    CU_ASSERT(vector->insert(vector, SIZE_MID_TEST, dummy) == false);

    /* Insert elements ranging from (SML * 2) to (SML * 3) to the vector tail. */
    num = SIZE_SML_TEST << 1;
    for (i = 0 ; i < SIZE_SML_TEST ; ++i) {
        Tuple* tuple = (Tuple*)malloc(sizeof(Tuple));
        tuple->first = num;
        tuple->second = num;
        CU_ASSERT(vector->insert(vector, num, tuple) == true);
        ++num;
    }
    for (i = 0 ; i < SIZE_SML_TEST * 3 ; ++i) {
        void* tuple;
        CU_ASSERT(vector->get(vector, i, &tuple) == true);
        CU_ASSERT_EQUAL(((Tuple*)tuple)->first, i);
    }

    CU_ASSERT_EQUAL(vector->size(vector), SIZE_SML_TEST * 3);
    CU_ASSERT_EQUAL(vector->capacity(vector), SIZE_MID_TEST);

    VectorDeinit(vector);
}

void TestPopAndRemove()
{
    Vector* vector = VectorInit(DEFAULT_CAPACITY);
    vector->set_clean(vector, CleanElement);
    void* dummy;

    CU_ASSERT(vector->pop_back(vector) == false);

    unsigned i;
    for (i = 0 ; i < SIZE_MID_TEST ; ++i) {
        Tuple* tuple = (Tuple*)malloc(sizeof(Tuple));
        tuple->first = i;
        tuple->second = i;
        vector->push_back(vector, tuple);
    }

    /* Pop elements ranging from (SML * 3) to MID from the vector tail. */
    for (i = 0 ; i < SIZE_SML_TEST ; ++i)
        CU_ASSERT(vector->pop_back(vector) == true);
    CU_ASSERT(vector->get(vector, SIZE_SML_TEST * 3, &dummy) == false);
    CU_ASSERT(vector->get(vector, SIZE_MID_TEST, &dummy) == false);

    /* Remove elements ranging from (SML * 2) to (SML * 3) from the vector tail. */
    for (i = SIZE_SML_TEST * 3 - 1 ; i >= SIZE_SML_TEST << 1 ; --i)
        CU_ASSERT(vector->remove(vector, i) == true);
    CU_ASSERT(vector->get(vector, SIZE_SML_TEST << 1, &dummy) == false);
    CU_ASSERT(vector->remove(vector, SIZE_SML_TEST << 1) == false);
    CU_ASSERT(vector->remove(vector, SIZE_MID_TEST) == false);

    /* Remove elements ranging from 0 to SML from the vector head. */
    for (i = 0 ; i < SIZE_SML_TEST ; ++i)
        CU_ASSERT(vector->remove(vector, 0) == true);
    CU_ASSERT(vector->get(vector, SIZE_SML_TEST, &dummy) == false);

    unsigned num = SIZE_SML_TEST;
    for (i = 0 ; i < SIZE_SML_TEST ; ++i) {
        void* tuple;
        CU_ASSERT(vector->get(vector, i, &tuple) == true);
        CU_ASSERT_EQUAL(((Tuple*)tuple)->first, num);
        ++num;
    }

    CU_ASSERT_EQUAL(vector->size(vector), SIZE_SML_TEST);
    CU_ASSERT_EQUAL(vector->capacity(vector), SIZE_MID_TEST);

    VectorDeinit(vector);
}

void TestReplace()
{
    Vector* vector = VectorInit(DEFAULT_CAPACITY);
    vector->set_clean(vector, CleanElement);
    void* dummy;

    unsigned i;
    for (i = 0 ; i < SIZE_SML_TEST ; ++i) {
        Tuple* tuple = (Tuple*)malloc(sizeof(Tuple));
        tuple->first = i;
        tuple->second = i;
        vector->push_back(vector, tuple);
    }
    CU_ASSERT(vector->set(vector, SIZE_SML_TEST, dummy) == false);

    unsigned num = SIZE_SML_TEST;
    for (i = 0 ; i < SIZE_SML_TEST ; ++i) {
        Tuple* tuple = (Tuple*)malloc(sizeof(Tuple));
        tuple->first = num;
        tuple->second = num;
        CU_ASSERT(vector->set(vector, i, tuple) == true);
        ++num;
    }
    num = SIZE_SML_TEST;
    for (i = 0 ; i < SIZE_SML_TEST ; ++i) {
        void* tuple;
        vector->get(vector, i, &tuple);
        CU_ASSERT_EQUAL(((Tuple*)tuple)->first, num);
        ++num;
    }

    CU_ASSERT_EQUAL(vector->size(vector), SIZE_SML_TEST);
    CU_ASSERT_EQUAL(vector->capacity(vector), SIZE_SML_TEST);

    VectorDeinit(vector);
}

void TestIterator()
{
    Vector* vector = VectorInit(DEFAULT_CAPACITY);
    vector->set_clean(vector, CleanElement);

    unsigned i;
    for (i = 0 ; i < SIZE_SML_TEST ; ++i) {
        Tuple* tuple = (Tuple*)malloc(sizeof(Tuple));
        tuple->first = i;
        tuple->second = i;
        vector->push_back(vector, tuple);
    }

    void* tuple;
    i = 0;
    vector->first(vector, false);
    while (vector->next(vector, &tuple)) {
        CU_ASSERT_EQUAL(((Tuple*)tuple)->first, i);
        ++i;
    }

    i = SIZE_SML_TEST - 1;
    vector->first(vector, true);
    while (vector->reverse_next(vector, &tuple)) {
        CU_ASSERT_EQUAL(((Tuple*)tuple)->first, i);
        --i;
    }

    VectorDeinit(vector);
}

void TestSort()
{
    srand(time(NULL));

    void* tuples[SIZE_SML_TEST];
    unsigned i;
    for (i = 0 ; i < SIZE_SML_TEST ; ++i) {
        Tuple* tuple = (Tuple*)malloc(sizeof(Tuple));
        tuple->first = i;
        tuple->second = i;
        tuples[i] = tuple;
    }
    for (i = 0 ; i < SIZE_MID_TEST ; ++i) {
        int src = rand() % SIZE_SML_TEST;
        int tge = rand() % SIZE_SML_TEST;
        void* tuple = tuples[src];
        tuples[src] = tuples[tge];
        tuples[tge] = tuple;
    }

    Vector* vector = VectorInit(DEFAULT_CAPACITY);
    vector->set_clean(vector, CleanElement);

    for (i = 0 ; i < SIZE_SML_TEST ; ++i)
        vector->push_back(vector, tuples[i]);

    vector->sort(vector, SortElement);
    for (i = 0 ; i < SIZE_SML_TEST ; ++i) {
        void* tuple;
        vector->get(vector, i, &tuple);
        CU_ASSERT_EQUAL(((Tuple*)tuple)->first, i);
    }

    VectorDeinit(vector);
}

/*-----------------------------------------------------------------------------*
 *                      The driver for Vector unit test                        *
 *-----------------------------------------------------------------------------*/
bool AddSuite()
{
    CU_pSuite suite = CU_add_suite("Structure Verification", NULL, NULL);
    if (!suite)
        return false;

    CU_pTest unit = CU_add_test(suite, "New and Delete", TestNewDelete);
    if (!unit)
        return false;

    unit = CU_add_test(suite, "Resize", TestResize);
    if (!unit)
        return false;

    unit = CU_add_test(suite, "Element Push and Insert", TestPushAndInsert);
    if (!unit)
        return false;

    unit = CU_add_test(suite, "Element Pop and Remove", TestPopAndRemove);
    if (!unit)
        return false;

    unit = CU_add_test(suite, "Element Replace", TestReplace);
    if (!unit)
        return false;

    unit = CU_add_test(suite, "Iterator", TestIterator);
    if (!unit)
        return false;

    unit = CU_add_test(suite, "Sort", TestSort);
    if (!unit)
        return false;

    return true;
}

int32_t main()
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
