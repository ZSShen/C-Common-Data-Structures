#include "container/vector.h"
#include "CUnit/Util.h"
#include "CUnit/Basic.h"


typedef struct Tuple_ {
    int32_t iMajor;
    int32_t iMinor;
} Tuple;


void TestPrimPushBack();
void TestPrimInsert();
void TestPrimSet();
void TestPrimPopBack();
void TestPrimDelete();
void TestPrimResize();

void DestroyObject(Item);
int32_t CompareObject(const void*, const void*);
void TestSort();
void TestIterate();


int32_t SuitePrimitive()
{
    CU_pSuite pSuite = CU_add_suite("Primitive Type Input", NULL, NULL);
    if (!pSuite)
        return ERR_NOMEM;

    CU_pTest pTest = CU_add_test(pSuite, "Item insertion via push_back().",
                     TestPrimPushBack);
    if (!pTest)
        return ERR_NOMEM;

    pTest = CU_add_test(pSuite, "Item insertion via insert().", TestPrimInsert);
    if (!pTest)
        return ERR_NOMEM;

    pTest = CU_add_test(pSuite, "Item replacement via set().", TestPrimSet);
    if (!pTest)
        return ERR_NOMEM;

    pTest = CU_add_test(pSuite, "Item deletion via pop_back().", TestPrimPopBack);
    if (!pTest)
        return ERR_NOMEM;

    pTest = CU_add_test(pSuite, "Item deletion via remove().", TestPrimDelete);
    if (!pTest)
        return ERR_NOMEM;

    pTest = CU_add_test(pSuite, "Storage reallocation.", TestPrimResize);
    if (!pTest)
        return ERR_NOMEM;

    pTest = CU_add_test(pSuite, "Item sorting.", TestSort);
    if (!pTest)
        return ERR_NOMEM;

    pTest = CU_add_test(pSuite, "Vector iteration.", TestIterate);
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


void TestPrimPushBack()
{
    Vector *pVec;
    CU_ASSERT(VectorInit(&pVec, 0) == SUCC);

    /* Append the items. */
    CU_ASSERT(pVec->push_back(pVec, (Item)1) == SUCC);
    CU_ASSERT(pVec->push_back(pVec, (Item)2) == SUCC);
    CU_ASSERT(pVec->push_back(pVec, (Item)3) == SUCC);
    CU_ASSERT(pVec->push_back(pVec, (Item)4) == SUCC);

    /* Check item insertion sequence. */
    Item item;
    CU_ASSERT(pVec->get(pVec, &item, 0) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)1);
    CU_ASSERT(pVec->get(pVec, &item, 1) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)2);
    CU_ASSERT(pVec->get(pVec, &item, 2) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)3);
    CU_ASSERT(pVec->get(pVec, &item, 3) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)4);

    /* Check vector storage. */
    CU_ASSERT_EQUAL(pVec->size(pVec), 4);
    CU_ASSERT_EQUAL(pVec->capacity(pVec), 4);

    /* Check illegal indexing. */
    CU_ASSERT(pVec->get(pVec, &item, -1) == ERR_IDX);
    CU_ASSERT(pVec->get(pVec, &item, 4) == ERR_IDX);

    VectorDeinit(&pVec);
}

void TestPrimInsert()
{
    Vector *pVec;
    CU_ASSERT(VectorInit(&pVec, 0) == SUCC);

    /* Append the items. */
    CU_ASSERT(pVec->insert(pVec, (Item)3, 0) == SUCC);
    CU_ASSERT(pVec->insert(pVec, (Item)4, 1) == SUCC);

    /* Trigger the shift of trailing items. */
    CU_ASSERT(pVec->insert(pVec, (Item)1, 0) == SUCC);
    CU_ASSERT(pVec->insert(pVec, (Item)2, 1) == SUCC);

    /* Check item insertion sequence. */
    Item item;
    CU_ASSERT(pVec->get(pVec, &item, 0) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)1);
    CU_ASSERT(pVec->get(pVec, &item, 1) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)2);
    CU_ASSERT(pVec->get(pVec, &item, 2) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)3);
    CU_ASSERT(pVec->get(pVec, &item, 3) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)4);

    /* Check vector storage. */
    CU_ASSERT_EQUAL(pVec->size(pVec), 4);
    CU_ASSERT_EQUAL(pVec->capacity(pVec), 4);

    /* Check illegal indexing. */
    CU_ASSERT(pVec->insert(pVec, (Item)-1, -1) == ERR_IDX);
    CU_ASSERT(pVec->insert(pVec, (Item)-1, 5) == ERR_IDX);

    VectorDeinit(&pVec);
}

void TestPrimSet()
{
    Vector *pVec;
    CU_ASSERT(VectorInit(&pVec, 0) == SUCC);

    CU_ASSERT(pVec->push_back(pVec, (Item)0) == SUCC);
    CU_ASSERT(pVec->push_back(pVec, (Item)1) == SUCC);

    /* Replace the existing items. */
    CU_ASSERT(pVec->set(pVec, (Item)2, 0) == SUCC);
    CU_ASSERT(pVec->set(pVec, (Item)3, 1) == SUCC);

    Item item;
    CU_ASSERT(pVec->get(pVec, &item, 0) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)2);
    CU_ASSERT(pVec->get(pVec, &item, 1) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)3);

    /* Check illegal indexing. */
    CU_ASSERT(pVec->set(pVec, (Item)-1, -1) == ERR_IDX);
    CU_ASSERT(pVec->set(pVec, (Item)-1, 2) == ERR_IDX);

    VectorDeinit(&pVec);
}

void TestPrimPopBack()
{
    Vector *pVec;
    CU_ASSERT(VectorInit(&pVec, 0) == SUCC);

    CU_ASSERT(pVec->push_back(pVec, (Item)0) == SUCC);
    CU_ASSERT(pVec->push_back(pVec, (Item)1) == SUCC);

    /* Pop all the items. */
    CU_ASSERT(pVec->pop_back(pVec) == SUCC);
    CU_ASSERT(pVec->pop_back(pVec) == SUCC);

    /* Check vector storage. */
    CU_ASSERT_EQUAL(pVec->size(pVec), 0);
    CU_ASSERT_EQUAL(pVec->capacity(pVec), 2);

    /* Check illegal pop. */
    CU_ASSERT(pVec->pop_back(pVec) == ERR_IDX);

    VectorDeinit(&pVec);
}

void TestPrimDelete()
{
    Vector *pVec;
    CU_ASSERT(VectorInit(&pVec, 0) == SUCC);

    CU_ASSERT(pVec->push_back(pVec, (Item)0) == SUCC);
    CU_ASSERT(pVec->push_back(pVec, (Item)1) == SUCC);
    CU_ASSERT(pVec->push_back(pVec, (Item)2) == SUCC);
    CU_ASSERT(pVec->push_back(pVec, (Item)3) == SUCC);

    /* Delete the head and tail. */
    CU_ASSERT(pVec->remove(pVec, 3) == SUCC);
    CU_ASSERT(pVec->remove(pVec, 0) == SUCC);

    /* Check item shifting sequence. */
    Item item;
    CU_ASSERT(pVec->get(pVec, &item, 0) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)1);
    CU_ASSERT(pVec->get(pVec, &item, 1) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)2);

    /* Check vector storage. */
    CU_ASSERT_EQUAL(pVec->size(pVec), 2);
    CU_ASSERT_EQUAL(pVec->capacity(pVec), 4);

    /* Check illegal deletion. */
    CU_ASSERT(pVec->remove(pVec, -1) == ERR_IDX);
    CU_ASSERT(pVec->remove(pVec, 2) == ERR_IDX);

    VectorDeinit(&pVec);
}

void TestPrimResize()
{
    Vector *pVec;
    CU_ASSERT(VectorInit(&pVec, 0) == SUCC);

    /* Expand the capacity. */
    CU_ASSERT(pVec->resize(pVec, 4) == SUCC);
    CU_ASSERT_EQUAL(pVec->size(pVec), 0);
    CU_ASSERT_EQUAL(pVec->capacity(pVec), 4);

    /* Fill the half of the storage. */
    CU_ASSERT(pVec->push_back(pVec, (Item)0) == SUCC);
    CU_ASSERT(pVec->push_back(pVec, (Item)1) == SUCC);
    CU_ASSERT_EQUAL(pVec->size(pVec), 2);
    CU_ASSERT_EQUAL(pVec->capacity(pVec), 4);

    /* Shrink the storage. */
    CU_ASSERT(pVec->resize(pVec, 3) == SUCC);
    CU_ASSERT_EQUAL(pVec->size(pVec), 2);
    CU_ASSERT_EQUAL(pVec->capacity(pVec), 3);

    /* Shrink the storage again and trim the trailing items. */
    CU_ASSERT(pVec->resize(pVec, 1) == SUCC);
    Item item;
    CU_ASSERT(pVec->get(pVec, &item, 0) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)0);
    CU_ASSERT_EQUAL(pVec->size(pVec), 1);
    CU_ASSERT_EQUAL(pVec->capacity(pVec), 1);

    VectorDeinit(&pVec);
}

void DestroyObject(Item item)
{
    free((Tuple*)item);
}

int32_t CompareObject(const void *ppSrc, const void *ppTge)
{
    Tuple *TupleSrc = *((Tuple**)ppSrc);
    Tuple *TupleTge = *((Tuple**)ppTge);
    if (TupleSrc->iMajor == TupleTge->iMajor)
        return 0;
    return (TupleSrc->iMajor > TupleTge->iMajor)? 1 : (-1);
}

void TestSort()
{
    Vector *pVec;
    CU_ASSERT(VectorInit(&pVec, 0) == SUCC);
    CU_ASSERT(pVec->set_destroy(pVec, DestroyObject) == SUCC);

    /* Push the initial items. */
    Tuple *tuple = (Tuple*)malloc(sizeof(Tuple));
    tuple->iMajor = 3; tuple->iMinor = 3;
    CU_ASSERT(pVec->push_back(pVec, (Item)tuple) == SUCC);

    tuple = (Tuple*)malloc(sizeof(Tuple));
    tuple->iMajor = 1; tuple->iMinor = 1;
    CU_ASSERT(pVec->push_back(pVec, (Item)tuple) == SUCC);

    tuple = (Tuple*)malloc(sizeof(Tuple));
    tuple->iMajor = 2; tuple->iMinor = 2;
    CU_ASSERT(pVec->push_back(pVec, (Item)tuple) == SUCC);

    /* Sort the items. */
    CU_ASSERT(pVec->sort(pVec, CompareObject) == SUCC);

    /* Check the item order. */
    Item item;
    CU_ASSERT(pVec->get(pVec, &item, 0) == SUCC);
    CU_ASSERT_EQUAL(1, ((Tuple*)item)->iMajor);

    CU_ASSERT(pVec->get(pVec, &item, 1) == SUCC);
    CU_ASSERT_EQUAL(2, ((Tuple*)item)->iMajor);

    CU_ASSERT(pVec->get(pVec, &item, 2) == SUCC);
    CU_ASSERT_EQUAL(3, ((Tuple*)item)->iMajor);

    VectorDeinit(&pVec);
}

void TestIterate()
{
    Vector *pVec;
    CU_ASSERT(VectorInit(&pVec, 0) == SUCC);
    CU_ASSERT(pVec->set_destroy(pVec, DestroyObject) == SUCC);

    /* Iterate through empty vector. */
    Item item;
    CU_ASSERT(pVec->iterate(pVec, true, NULL) == SUCC);
    while (pVec->iterate(pVec, false, &item) != END);
    CU_ASSERT_EQUAL(item, NULL);
    CU_ASSERT(pVec->reverse_iterate(pVec, true, NULL) == SUCC);
    while (pVec->reverse_iterate(pVec, false, &item) != END);
    CU_ASSERT_EQUAL(item, NULL);

    /* Push the initial items. */
    Tuple *tuple = (Tuple*)malloc(sizeof(Tuple));
    tuple->iMajor = 3; tuple->iMinor = 3;
    CU_ASSERT(pVec->push_back(pVec, (Item)tuple) == SUCC);

    tuple = (Tuple*)malloc(sizeof(Tuple));
    tuple->iMajor = 2; tuple->iMinor = 2;
    CU_ASSERT(pVec->push_back(pVec, (Item)tuple) == SUCC);

    tuple = (Tuple*)malloc(sizeof(Tuple));
    tuple->iMajor = 1; tuple->iMinor = 1;
    CU_ASSERT(pVec->push_back(pVec, (Item)tuple) == SUCC);

    /* Iterate through the vector items. */
    int32_t iSum = 0;
    int32_t iIdx = 3;
    CU_ASSERT(pVec->iterate(pVec, true, NULL) == SUCC);
    while (pVec->iterate(pVec, false, &item) != END) {
        int32_t iMajor = ((Tuple*)item)->iMajor;
        CU_ASSERT_EQUAL(iIdx, iMajor);
        iSum += iMajor;
        iIdx--;
    }
    CU_ASSERT_EQUAL(iSum, 6);

    /* Reversely iterate through the vector items. */
    iSum = 0;
    iIdx = 1;
    CU_ASSERT(pVec->reverse_iterate(pVec, true, NULL) == SUCC);
    while (pVec->reverse_iterate(pVec, false, &item) != END) {
        int32_t iMajor = ((Tuple*)item)->iMajor;
        CU_ASSERT_EQUAL(iIdx, iMajor);
        iSum += iMajor;
        iIdx++;
    }
    CU_ASSERT_EQUAL(iSum, 6);

    VectorDeinit(&pVec);
}