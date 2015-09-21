#include "container/stack.h"
#include "CUnit/Util.h"
#include "CUnit/Basic.h"


#define SIZE_DATA           (100)
#define MASK_YEAR           (100)
#define MASK_LEVEL          (200)

typedef struct Employ_ {
    int8_t cYear;
    int8_t cLevel;
    int32_t iId;
} Employ;


int32_t AddSuite();
void TestManipulate();
void TestBoundary();
void DestroyItem(Item);


int32_t main()
{
    int32_t rc = SUCC;

    if (CU_initialize_registry() != CUE_SUCCESS) {
        rc = CU_get_error();
        goto EXIT;
    }

    /* Register the test suite for stack structure verification. */
    if (AddSuite() != SUCC) {
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

int32_t AddSuite()
{
    int32_t rc = SUCC;

    CU_pSuite pSuite = CU_add_suite("Structure Verification", NULL, NULL);
    if (!pSuite) {
        rc = ERR_REG;
        goto EXIT;
    }

    char *szMsg = "Combination with insertion, deletion, and order checking.";
    CU_pTest pTest = CU_add_test(pSuite, szMsg, TestManipulate);
    if (!pTest)
        rc = ERR_REG;

    pTest = CU_add_test(pSuite, "Boundary Testing", TestBoundary);
    if (!pTest)
        rc = ERR_REG;

EXIT:
    return rc;
}

void DestroyItem(Item item)
{
    free((Employ*)item);
}

void TestManipulate()
{
    Stack *pStack;
    CU_ASSERT(StackInit(&pStack) == SUCC);
    CU_ASSERT(pStack->set_destroy(pStack, DestroyItem) == SUCC);

    /* Insert the initial data set. */
    int32_t iIdx;
    for (iIdx = 0 ; iIdx < SIZE_DATA ; iIdx++) {
        Employ *pEmploy = (Employ*)malloc(sizeof(Employ));
        pEmploy->cYear = iIdx % MASK_YEAR;
        pEmploy->cLevel = iIdx % MASK_LEVEL;
        pEmploy->iId = iIdx;
        CU_ASSERT(pStack->push(pStack, (Item)pEmploy) == SUCC);
    }
    CU_ASSERT_EQUAL(pStack->size(pStack), SIZE_DATA);

    /* Sequentially retrieve the stack top data and delete it. */
    for (iIdx = SIZE_DATA - 1 ; iIdx >= SIZE_DATA >> 1 ; iIdx--) {
        Item item;
        CU_ASSERT(pStack->top(pStack, &item) == SUCC);
        CU_ASSERT_EQUAL(((Employ*)item)->iId, iIdx);
        CU_ASSERT(pStack->pop(pStack) == SUCC);
    }
    CU_ASSERT_EQUAL(pStack->size(pStack), 0);

    /* The rest data will be cleaned by the destructor. */
    StackDeinit(&pStack);
}

void TestBoundary()
{
    Stack *pStack;
    CU_ASSERT(StackInit(&pStack) == SUCC);
    CU_ASSERT(pStack->set_destroy(pStack, DestroyItem) == SUCC);

    /* Pop the empty stack. */
    CU_ASSERT(pStack->pop(pStack) == ERR_IDX);

    /* Retrieve the items from the empty stack. */
    Item item;
    CU_ASSERT(pStack->top(pStack, &item) == ERR_IDX);
    CU_ASSERT_EQUAL(item, NULL);

    /* Insert the initial data set. */
    int iIdx;
    for (iIdx = 0 ; iIdx < SIZE_DATA ; iIdx++) {
        Employ *pEmploy = (Employ*)malloc(sizeof(Employ));
        pEmploy->cYear = iIdx % MASK_YEAR;
        pEmploy->cLevel = iIdx % MASK_LEVEL;
        pEmploy->iId = iIdx;
        CU_ASSERT(pStack->push(pStack, (Item)pEmploy) == SUCC);
    }

    /* Delete all the data. */
    for (iIdx = 0 ; iIdx < SIZE_DATA ; iIdx++)
        CU_ASSERT(pStack->pop(pStack) == SUCC);

    CU_ASSERT(pStack->pop(pStack) == ERR_IDX);
    CU_ASSERT(pStack->top(pStack, &item) == ERR_IDX);
    CU_ASSERT_EQUAL(item, NULL);
    CU_ASSERT_EQUAL(pStack->size(pStack), 0);

    /* Invalid parameter to retrieve stack data. */
    CU_ASSERT(pStack->top(pStack, NULL) == ERR_GET);

    StackDeinit(&pStack);
}