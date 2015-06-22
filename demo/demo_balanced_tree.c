#include "cds.h"


/* The example to manipulate primitive type input. */
void usage_primitive();


int main()
{
    usage_primitive();

    return 0;
}


void usage_primitive()
{
    BalancedTree *pTree;

    int32_t rc = BalTreeInit(&pTree);
    if (rc != SUCC)
        return;

    /* Insert items into the tree. */
    pTree->insert(pTree, (Item)2, true);
    pTree->insert(pTree, (Item)3, true);
    pTree->insert(pTree, (Item)1, true);

    /* Delete items from the tree. */
    rc = pTree->delete(pTree, (Item)2, true);
    assert(rc == SUCC);
    rc = pTree->delete(pTree, (Item)2, true);
    assert(rc == ERR_NODATA);

    /* Search items from the tree. */
    Item item;
    rc = pTree->search(pTree, (Item)3, &item);
    assert(rc == SUCC);
    rc = pTree->search(pTree, (Item)2, &item);
    assert(rc == ERR_NODATA);

    /* Resolve the tree structures. */
    pTree->minimum(pTree, &item);
    assert(item == (Item)1);
    pTree->maximum(pTree, &item);
    assert(item == (Item)3);

    pTree->predecessor(pTree, (Item)3, &item);
    assert(item == (Item)1);
    rc = pTree->successor(pTree, (Item)3, &item);
    assert(rc == ERR_NODATA);

    pTree->successor(pTree, (Item)1, &item);
    assert(item == (Item)3);
    rc = pTree->predecessor(pTree, (Item)1, &item);
    assert(rc == ERR_NODATA);

    assert(pTree->size(pTree) == 2);

    BalTreeDeinit(&pTree, true);
    return;
}
