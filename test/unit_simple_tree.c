#include "tree/simple_tree.h"
#include "CUnit/Util.h"
#include "CUnit/Basic.h"


/* Test suite to manipulate primitive type input. */
void TestPrimInsertAndOrder();
void TestPrimDeleteAndOrder();
void TestPrimSearch();


/* Test suite to manipulate non-primitive type input. */
typedef struct Employ_ {
    char *szName;
    int iId;
    int iLevel;
} Employ;

#define SIZE_NAME_BUF (3)

int32_t ItemCompare(Item, Item);
void ItemDestroy(Item);
void TestNonPrimInsertAndOrder();
void TestNonPrimDeleteAndOrder();
void TestNonPrimSearch();


int32_t SuitePrimitive()
{
    CU_pSuite pSuite = CU_add_suite("Primitive Type Input", NULL, NULL);
    if (!pSuite)
        return ERR_NOMEM;

    CU_pTest pTest = CU_add_test(pSuite, "Item insertion and structure verification",
                     TestPrimInsertAndOrder);
    if (!pTest)
        return ERR_NOMEM;

    pTest = CU_add_test(pSuite, "Item deletion and structure verification",
            TestPrimDeleteAndOrder);
    if (!pTest)
        return ERR_NOMEM;

    pTest = CU_add_test(pSuite, "Item search", TestPrimSearch);
    if (!pTest)
        return ERR_NOMEM;

    return SUCC;
}

int32_t SuiteNonPrimitive()
{
    CU_pSuite pSuite = CU_add_suite("Non-Primitive Type Input", NULL, NULL);
    if (!pSuite)
        return ERR_NOMEM;

    CU_pTest pTest = CU_add_test(pSuite, "Item insertion and structure verification",
                     TestNonPrimInsertAndOrder);
    if (!pTest)
        return ERR_NOMEM;

    pTest = CU_add_test(pSuite, "Item deletion and structure verification",
            TestNonPrimDeleteAndOrder);
    if (!pTest)
        return ERR_NOMEM;
    /*
    pTest = CU_add_test(pSuite, "Item search", TestNonPrimSearch);
    if (!pTest)
        return ERR_NOMEM;
    */

    return SUCC;
}


int32_t main()
{
    if (CU_initialize_registry() != CUE_SUCCESS)
        return CU_get_error();
    assert(CU_get_registry() != NULL);
    assert(!CU_is_test_running());

    /* Prepare the test suites for primitive and non-primitive input. */
    if (SuitePrimitive() != SUCC) {
        CU_cleanup_registry();
        return CU_get_error();
    }
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


void TestPrimInsertAndOrder()
{
    SimpleTree *pTree;
    CU_ASSERT(SimTreeInit(&pTree) == SUCC);

    /**
     * The vision of the test tree.
     *         (4)
     *        /   \
     *     (2)     (6)
     *     / \     / \
     *   (1) (3) (5) (7)
     */
    CU_ASSERT(pTree->insert(pTree, (Item)4) == SUCC);
    CU_ASSERT(pTree->insert(pTree, (Item)2) == SUCC);
    CU_ASSERT(pTree->insert(pTree, (Item)3) == SUCC);
    CU_ASSERT(pTree->insert(pTree, (Item)1) == SUCC);
    CU_ASSERT(pTree->insert(pTree, (Item)6) == SUCC);
    CU_ASSERT(pTree->insert(pTree, (Item)5) == SUCC);
    CU_ASSERT(pTree->insert(pTree, (Item)7) == SUCC);

    /* Check structure correctness. */
    Item item;
    CU_ASSERT(pTree->predecessor(pTree, (Item)2, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)1);
    CU_ASSERT(pTree->successor(pTree, (Item)2, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)3);

    CU_ASSERT(pTree->predecessor(pTree, (Item)3, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)2);
    CU_ASSERT(pTree->successor(pTree, (Item)3, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)4);

    CU_ASSERT(pTree->predecessor(pTree, (Item)5, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)4);
    CU_ASSERT(pTree->successor(pTree, (Item)5, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)6);

    CU_ASSERT(pTree->predecessor(pTree, (Item)4, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)3);
    CU_ASSERT(pTree->successor(pTree, (Item)4, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)5);

    /* Check the minimum and maximum item. */
    CU_ASSERT(pTree->minimum(pTree, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)1);
    CU_ASSERT(pTree->maximum(pTree, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)7);

    /* Check container size. */
    CU_ASSERT_EQUAL(pTree->size(pTree), 7);

    SimTreeDeinit(&pTree);
}

void TestPrimDeleteAndOrder()
{
    SimpleTree *pTree;
    CU_ASSERT(SimTreeInit(&pTree) == SUCC);

    /**
     * The vision of the test tree.
     *         (4)
     *        /   \
     *     (2)     (7)
     *     / \     / \
     *   (1) (3) (5) (8)
     *   /         \
     * (0)         (6)
     */
    CU_ASSERT(pTree->insert(pTree, (Item)4) == SUCC);
    CU_ASSERT(pTree->insert(pTree, (Item)2) == SUCC);
    CU_ASSERT(pTree->insert(pTree, (Item)3) == SUCC);
    CU_ASSERT(pTree->insert(pTree, (Item)1) == SUCC);
    CU_ASSERT(pTree->insert(pTree, (Item)0) == SUCC);
    CU_ASSERT(pTree->insert(pTree, (Item)7) == SUCC);
    CU_ASSERT(pTree->insert(pTree, (Item)5) == SUCC);
    CU_ASSERT(pTree->insert(pTree, (Item)6) == SUCC);
    CU_ASSERT(pTree->insert(pTree, (Item)8) == SUCC);

    /* Check structure correctness. */
    CU_ASSERT(pTree->delete(pTree, (Item)4) == SUCC);
    /**
     * The new structure of the trimmed tree.
     *         (5)
     *        /   \
     *      /\    (7)
     *     /  \   / \
     *    /____\(6) (8)
     */
    Item item;
    CU_ASSERT(pTree->predecessor(pTree, (Item)7, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)6);
    CU_ASSERT(pTree->predecessor(pTree, (Item)5, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)3);
    CU_ASSERT(pTree->successor(pTree, (Item)5, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)6);

    CU_ASSERT(pTree->delete(pTree, (Item)5) == SUCC);
    CU_ASSERT(pTree->predecessor(pTree, (Item)7, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)6);
    CU_ASSERT(pTree->predecessor(pTree, (Item)6, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)3);
    CU_ASSERT(pTree->successor(pTree, (Item)6, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)7);

    CU_ASSERT(pTree->delete(pTree, (Item)6) == SUCC);
    /**
     * The new structure of the trimmed tree.
     *         (7)
     *        /   \
     *      /\    (8)
     *     /  \
     *    /____\
     */
    CU_ASSERT(pTree->predecessor(pTree, (Item)7, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)3);
    CU_ASSERT(pTree->successor(pTree, (Item)7, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)8);

    CU_ASSERT(pTree->delete(pTree, (Item)8) == SUCC);

    CU_ASSERT(pTree->delete(pTree, (Item)7) == SUCC);
    /**
     * The new structure of the trimmed tree.
     *        (2)
     *       /   \
     *     (1)   (3)
     *     /
     *   (0)
     */
    CU_ASSERT(pTree->maximum(pTree, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)3);
    CU_ASSERT(pTree->minimum(pTree, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)0);

    CU_ASSERT(pTree->delete(pTree, (Item)1) == SUCC);
    /**
     * The new structure of the trimmed tree.
     *        (2)
     *       /   \
     *     (0)   (3)
     */
    CU_ASSERT(pTree->predecessor(pTree, (Item)2, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)0);
    CU_ASSERT(pTree->successor(pTree, (Item)0, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)2);

    /* Check the container size. */
    CU_ASSERT_EQUAL(pTree->size(pTree), 3);

    SimTreeDeinit(&pTree);
}

void TestPrimSearch()
{
    SimpleTree *pTree;
    CU_ASSERT(SimTreeInit(&pTree) == SUCC);

    /* Search for the empty tree. */
    Item item;
    CU_ASSERT(pTree->search(pTree, (Item)0, &item) == ERR_NODATA);
    CU_ASSERT_EQUAL(item, NULL);

    /* Search for the real data. */
    CU_ASSERT(pTree->insert(pTree, (Item)1) == SUCC);
    CU_ASSERT(pTree->insert(pTree, (Item)0) == SUCC);
    CU_ASSERT(pTree->search(pTree, (Item)0, &item) == SUCC);
    CU_ASSERT_EQUAL(item, (Item)0);

    CU_ASSERT(pTree->delete(pTree, (Item)0) == SUCC);
    CU_ASSERT(pTree->search(pTree, (Item)0, &item) == ERR_NODATA);
    CU_ASSERT_EQUAL(item, NULL);

    SimTreeDeinit(&pTree);
}


int32_t ItemCompare(Item itemSrc, Item itemTge)
{
    Employ *pEmpSrc = (Employ*)itemSrc;
    Employ *pEmpTge = (Employ*)itemTge;

    int32_t iOrder = strcmp(pEmpSrc->szName, pEmpTge->szName);
    if (iOrder == 0)
        return 0;
    return (iOrder > 0)? 1 : (-1);
}

void ItemDestroy(Item item)
{
    Employ *pEmp = (Employ*)item;
    free(pEmp->szName);
    free(pEmp);
}

void TestNonPrimInsertAndOrder()
{
    SimpleTree *pTree;
    CU_ASSERT(SimTreeInit(&pTree) == SUCC);

    CU_ASSERT(pTree->set_destroy(pTree, ItemDestroy) == SUCC);
    CU_ASSERT(pTree->set_compare(pTree, ItemCompare) == SUCC);

    /* Insert the items "de", "bc", "cd", "ab", "fg", "ef", "gh". */
    Employ* aEmp[7] = {NULL};

    Employ *pEmp = (Employ*)malloc(sizeof(Employ));
    pEmp->szName = (char*)malloc(sizeof(char) * SIZE_NAME_BUF);
    pEmp->szName[0] = 'd';
    pEmp->szName[1] = 'e';
    pEmp->szName[2] = 0;
    pEmp->iId = 4;
    pEmp->iLevel = 4;
    aEmp[3] = pEmp;
    CU_ASSERT(pTree->insert(pTree, (Item)pEmp) == SUCC);

    pEmp = (Employ*)malloc(sizeof(Employ));
    pEmp->szName = (char*)malloc(sizeof(char) * SIZE_NAME_BUF);
    pEmp->szName[0] = 'b';
    pEmp->szName[1] = 'c';
    pEmp->szName[2] = 0;
    pEmp->iId = 2;
    pEmp->iLevel = 2;
    aEmp[1] = pEmp;
    CU_ASSERT(pTree->insert(pTree, (Item)pEmp) == SUCC);

    pEmp = (Employ*)malloc(sizeof(Employ));
    pEmp->szName = (char*)malloc(sizeof(char) * SIZE_NAME_BUF);
    pEmp->szName[0] = 'c';
    pEmp->szName[1] = 'd';
    pEmp->szName[2] = 0;
    pEmp->iId = 3;
    pEmp->iLevel = 3;
    aEmp[2] = pEmp;
    CU_ASSERT(pTree->insert(pTree, (Item)pEmp) == SUCC);

    pEmp = (Employ*)malloc(sizeof(Employ));
    pEmp->szName = (char*)malloc(sizeof(char) * SIZE_NAME_BUF);
    pEmp->szName[0] = 'a';
    pEmp->szName[1] = 'b';
    pEmp->szName[2] = 0;
    pEmp->iId = 1;
    pEmp->iLevel = 1;
    aEmp[0] = pEmp;
    CU_ASSERT(pTree->insert(pTree, (Item)pEmp) == SUCC);

    pEmp = (Employ*)malloc(sizeof(Employ));
    pEmp->szName = (char*)malloc(sizeof(char) * SIZE_NAME_BUF);
    pEmp->szName[0] = 'f';
    pEmp->szName[1] = 'g';
    pEmp->szName[2] = 0;
    pEmp->iId = 6;
    pEmp->iLevel = 6;
    aEmp[5] = pEmp;
    CU_ASSERT(pTree->insert(pTree, (Item)pEmp) == SUCC);

    pEmp = (Employ*)malloc(sizeof(Employ));
    pEmp->szName = (char*)malloc(sizeof(char) * SIZE_NAME_BUF);
    pEmp->szName[0] = 'e';
    pEmp->szName[1] = 'f';
    pEmp->szName[2] = 0;
    pEmp->iId = 5;
    pEmp->iLevel = 5;
    aEmp[4] = pEmp;
    CU_ASSERT(pTree->insert(pTree, (Item)pEmp) == SUCC);

    pEmp = (Employ*)malloc(sizeof(Employ));
    pEmp->szName = (char*)malloc(sizeof(char) * SIZE_NAME_BUF);
    pEmp->szName[0] = 'g';
    pEmp->szName[1] = 'h';
    pEmp->szName[2] = 0;
    pEmp->iId = 7;
    pEmp->iLevel = 7;
    aEmp[6] = pEmp;
    CU_ASSERT(pTree->insert(pTree, (Item)pEmp) == SUCC);

    /**
     * The vision of the test tree.
     *         (de)
     *        /    \
     *     (bc)    (fg)
     *     / \     / \
     *    /   \   /   \
     *  (ab) (cd)(ef) (gh)
     */

    /* Check structure correctness. */
    Item item;
    Employ emp = {"bc\0", 2, 2};
    CU_ASSERT(pTree->predecessor(pTree, (Item)&emp, &item) == SUCC);
    CU_ASSERT_EQUAL(aEmp[0], item);
    CU_ASSERT(pTree->successor(pTree, (Item)&emp, &item) == SUCC);
    CU_ASSERT_EQUAL(aEmp[2], item);

    emp.szName = "cd\0";
    emp.iId = 3;
    emp.iLevel = 3;
    CU_ASSERT(pTree->predecessor(pTree, (Item)&emp, &item) == SUCC);
    CU_ASSERT_EQUAL(aEmp[1], item);
    CU_ASSERT(pTree->successor(pTree, (Item)&emp, &item) == SUCC);
    CU_ASSERT_EQUAL(aEmp[3], item);

    emp.szName = "ef\0";
    emp.iId = 5;
    emp.iLevel = 5;
    CU_ASSERT(pTree->predecessor(pTree, (Item)&emp, &item) == SUCC);
    CU_ASSERT_EQUAL(aEmp[3], item);
    CU_ASSERT(pTree->successor(pTree, (Item)&emp, &item) == SUCC);
    CU_ASSERT_EQUAL(aEmp[5], item);

    emp.szName = "de\0";
    emp.iId = 4;
    emp.iLevel = 4;
    CU_ASSERT(pTree->predecessor(pTree, (Item)&emp, &item) == SUCC);
    CU_ASSERT_EQUAL(aEmp[2], item);
    CU_ASSERT(pTree->successor(pTree, (Item)&emp, &item) == SUCC);
    CU_ASSERT_EQUAL(aEmp[4], item);

    /* Check the minimum and maximum item. */
    CU_ASSERT(pTree->minimum(pTree, &item) == SUCC);
    CU_ASSERT_EQUAL(item, aEmp[0]);
    CU_ASSERT(pTree->maximum(pTree, &item) == SUCC);
    CU_ASSERT_EQUAL(item, aEmp[6]);

    /* Check container size. */
    CU_ASSERT_EQUAL(pTree->size(pTree), 7);

    SimTreeDeinit(&pTree);
}

void TestNonPrimDeleteAndOrder()
{
    SimpleTree *pTree;
    CU_ASSERT(SimTreeInit(&pTree) == SUCC);

    CU_ASSERT(pTree->set_destroy(pTree, ItemDestroy) == SUCC);
    CU_ASSERT(pTree->set_compare(pTree, ItemCompare) == SUCC);

    /* Insert the items "ef", "cd", "de", "bc", "ab", "hi", "fg", "gh", "ij". */
    Employ* aEmp[9] = {NULL};

    Employ *pEmp = (Employ*)malloc(sizeof(Employ));
    pEmp->szName = (char*)malloc(sizeof(char) * SIZE_NAME_BUF);
    pEmp->szName[0] = 'e';
    pEmp->szName[1] = 'f';
    pEmp->szName[2] = 0;
    pEmp->iId = 4;
    pEmp->iLevel = 4;
    aEmp[4] = pEmp;
    CU_ASSERT(pTree->insert(pTree, (Item)pEmp) == SUCC);

    pEmp = (Employ*)malloc(sizeof(Employ));
    pEmp->szName = (char*)malloc(sizeof(char) * SIZE_NAME_BUF);
    pEmp->szName[0] = 'c';
    pEmp->szName[1] = 'd';
    pEmp->szName[2] = 0;
    pEmp->iId = 2;
    pEmp->iLevel = 2;
    aEmp[2] = pEmp;
    CU_ASSERT(pTree->insert(pTree, (Item)pEmp) == SUCC);

    pEmp = (Employ*)malloc(sizeof(Employ));
    pEmp->szName = (char*)malloc(sizeof(char) * SIZE_NAME_BUF);
    pEmp->szName[0] = 'd';
    pEmp->szName[1] = 'e';
    pEmp->szName[2] = 0;
    pEmp->iId = 3;
    pEmp->iLevel = 3;
    aEmp[3] = pEmp;
    CU_ASSERT(pTree->insert(pTree, (Item)pEmp) == SUCC);

    pEmp = (Employ*)malloc(sizeof(Employ));
    pEmp->szName = (char*)malloc(sizeof(char) * SIZE_NAME_BUF);
    pEmp->szName[0] = 'b';
    pEmp->szName[1] = 'c';
    pEmp->szName[2] = 0;
    pEmp->iId = 1;
    pEmp->iLevel = 1;
    aEmp[1] = pEmp;
    CU_ASSERT(pTree->insert(pTree, (Item)pEmp) == SUCC);

    pEmp = (Employ*)malloc(sizeof(Employ));
    pEmp->szName = (char*)malloc(sizeof(char) * SIZE_NAME_BUF);
    pEmp->szName[0] = 'a';
    pEmp->szName[1] = 'b';
    pEmp->szName[2] = 0;
    pEmp->iId = 0;
    pEmp->iLevel = 0;
    aEmp[0] = pEmp;
    CU_ASSERT(pTree->insert(pTree, (Item)pEmp) == SUCC);

    pEmp = (Employ*)malloc(sizeof(Employ));
    pEmp->szName = (char*)malloc(sizeof(char) * SIZE_NAME_BUF);
    pEmp->szName[0] = 'h';
    pEmp->szName[1] = 'i';
    pEmp->szName[2] = 0;
    pEmp->iId = 7;
    pEmp->iLevel = 7;
    aEmp[7] = pEmp;
    CU_ASSERT(pTree->insert(pTree, (Item)pEmp) == SUCC);

    pEmp = (Employ*)malloc(sizeof(Employ));
    pEmp->szName = (char*)malloc(sizeof(char) * SIZE_NAME_BUF);
    pEmp->szName[0] = 'f';
    pEmp->szName[1] = 'g';
    pEmp->szName[2] = 0;
    pEmp->iId = 5;
    pEmp->iLevel = 5;
    aEmp[5] = pEmp;
    CU_ASSERT(pTree->insert(pTree, (Item)pEmp) == SUCC);

    pEmp = (Employ*)malloc(sizeof(Employ));
    pEmp->szName = (char*)malloc(sizeof(char) * SIZE_NAME_BUF);
    pEmp->szName[0] = 'g';
    pEmp->szName[1] = 'h';
    pEmp->szName[2] = 0;
    pEmp->iId = 6;
    pEmp->iLevel = 6;
    aEmp[6] = pEmp;
    CU_ASSERT(pTree->insert(pTree, (Item)pEmp) == SUCC);

    pEmp = (Employ*)malloc(sizeof(Employ));
    pEmp->szName = (char*)malloc(sizeof(char) * SIZE_NAME_BUF);
    pEmp->szName[0] = 'i';
    pEmp->szName[1] = 'j';
    pEmp->szName[2] = 0;
    pEmp->iId = 8;
    pEmp->iLevel = 8;
    aEmp[8] = pEmp;
    CU_ASSERT(pTree->insert(pTree, (Item)pEmp) == SUCC);

    /**
     * The vision of the test tree.
     *            (ef)
     *           /    \
     *        (cd)     (hi)
     *        / \      / \
     *       /   \    /   \
     *    (bc) (de) (fg)   (ij)
     *     /          \
     *  (ab)          (gh)
     */

    /* Delete "ef". */
    Employ emp = {"ef\0", 4, 4};
    CU_ASSERT(pTree->delete(pTree, (Item)&emp) == SUCC);
    /**
     * The new structure of the trimmed tree.
     *         (fg)
     *        /    \
     *      /\     (hi)
     *     /  \    /  \
     *    /____\(gh)  (ij)
     */
    Item item;
    emp.szName = "hi\0";
    emp.iId = 7;
    emp.iLevel = 7;
    CU_ASSERT(pTree->predecessor(pTree, (Item)&emp, &item) == SUCC);
    CU_ASSERT_EQUAL(item, aEmp[6]);
    emp.szName = "fg\0";
    emp.iId = 5;
    emp.iLevel = 5;
    CU_ASSERT(pTree->predecessor(pTree, (Item)&emp, &item) == SUCC);
    CU_ASSERT_EQUAL(item, aEmp[3]);
    CU_ASSERT(pTree->successor(pTree, (Item)&emp, &item) == SUCC);
    CU_ASSERT_EQUAL(item, aEmp[6]);

    /* Delete "fg". */
    CU_ASSERT(pTree->delete(pTree, (Item)&emp) == SUCC);
    emp.szName = "hi\0";
    emp.iId = 7;
    emp.iLevel = 7;
    CU_ASSERT(pTree->predecessor(pTree, (Item)&emp, &item) == SUCC);
    CU_ASSERT_EQUAL(item, aEmp[6]);
    emp.szName = "gh\0";
    emp.iId = 6;
    emp.iLevel = 6;
    CU_ASSERT(pTree->predecessor(pTree, (Item)&emp, &item) == SUCC);
    CU_ASSERT_EQUAL(item, aEmp[3]);
    CU_ASSERT(pTree->successor(pTree, (Item)&emp, &item) == SUCC);
    CU_ASSERT_EQUAL(item, aEmp[7]);

    /* Delete "gh". */
    CU_ASSERT(pTree->delete(pTree, (Item)&emp) == SUCC);
    /**
     * The new structure of the trimmed tree.
     *         (hi)
     *        /    \
     *      /\     (ij)
     *     /  \
     *    /____\
     */
    emp.szName = "hi\0";
    emp.iId = 7;
    emp.iLevel = 7;
    CU_ASSERT(pTree->predecessor(pTree, (Item)&emp, &item) == SUCC);
    CU_ASSERT_EQUAL(item, aEmp[3]);
    CU_ASSERT(pTree->successor(pTree, (Item)&emp, &item) == SUCC);
    CU_ASSERT_EQUAL(item, aEmp[8]);

    /* Delete "ij". */
    emp.szName = "ij\0";
    emp.iId = 8;
    emp.iLevel = 8;
    CU_ASSERT(pTree->delete(pTree, (Item)&emp) == SUCC);

    /* Delete "hi". */
    emp.szName = "hi\0";
    emp.iId = 7;
    emp.iLevel = 7;
    CU_ASSERT(pTree->delete(pTree, (Item)&emp) == SUCC);
    /**
     * The new structure of the trimmed tree.
     *        (cd)
     *       /    \
     *    (bc)    (de)
     *    /
     *  (ab)
     */
    CU_ASSERT(pTree->maximum(pTree, &item) == SUCC);
    CU_ASSERT_EQUAL(item, aEmp[3]);
    CU_ASSERT(pTree->minimum(pTree, &item) == SUCC);
    CU_ASSERT_EQUAL(item, aEmp[0]);

    /* Delete "bc". */
    emp.szName = "bc\0";
    emp.iId = 1;
    emp.iLevel = 1;
    CU_ASSERT(pTree->delete(pTree, (Item)&emp) == SUCC);
    /**
     * The new structure of the trimmed tree.
     *        (cd)
     *       /    \
     *    (ab)    (de)
     */
    emp.szName = "cd\0";
    emp.iId = 2;
    emp.iLevel = 2;
    CU_ASSERT(pTree->predecessor(pTree, (Item)&emp, &item) == SUCC);
    CU_ASSERT_EQUAL(item, aEmp[0]);
    emp.szName = "ab\0";
    emp.iId = 0;
    emp.iLevel = 0;
    CU_ASSERT(pTree->successor(pTree, (Item)&emp, &item) == SUCC);
    CU_ASSERT_EQUAL(item, aEmp[2]);

    /* Check the container size. */
    CU_ASSERT_EQUAL(pTree->size(pTree), 3);

    SimTreeDeinit(&pTree);
}

//void TestNonPrimSearch();