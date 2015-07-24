#include "cds.h"


typedef struct Object_ {
    int32_t iId;
    char *szName;
} Object;

#define SIZE_NAME_BUF (3)


void DestroyObject(Item item)
{
    Object *pObj = (Object*)item;
    free(pObj->szName);
    free(pObj);
}

int32_t CompareObject(Item itemSrc, Item itemTge)
{
    Object *pObjSrc = (Object*)itemSrc;
    Object *pObjTge = (Object*)itemTge;
    int32_t iOrder = strcmp(pObjSrc->szName, pObjTge->szName);
    if (iOrder == 0)
        return 0;
    return (iOrder > 0)? 1 : (-1);
}

int main()
{
    RedBlackTree *pTree;

    int32_t rc = RBTreeInit(&pTree);
    if (rc != SUCC)
        return rc;

    /* You should specify how to compare your items. */
    pTree->set_compare(pTree, CompareObject);

    /* If you plan to delegate the resource clean task to the DS, please set the
       custom clean method. */
    pTree->set_destroy(pTree, DestroyObject);

    /* Insert items into the tree. */
    Object *pObj = (Object*)malloc(sizeof(Object));
    pObj->szName = (char*)malloc(sizeof(char) * SIZE_NAME_BUF);
    pObj->iId = 1;
    pObj->szName[0] = 'b'; pObj->szName[1] = 'c'; pObj->szName[2] = 0;
    pTree->insert(pTree, (Item)pObj);

    pObj = (Object*)malloc(sizeof(Object));
    pObj->szName = (char*)malloc(sizeof(char) * SIZE_NAME_BUF);
    pObj->iId = 2;
    pObj->szName[0] = 'c'; pObj->szName[1] = 'd'; pObj->szName[2] = 0;
    pTree->insert(pTree, (Item)pObj);

    pObj = (Object*)malloc(sizeof(Object));
    pObj->szName = (char*)malloc(sizeof(char) * SIZE_NAME_BUF);
    pObj->iId = 0;
    pObj->szName[0] = 'a'; pObj->szName[1] = 'b'; pObj->szName[2] = 0;
    pTree->insert(pTree, (Item)pObj);

    /* Delete items from the tree. */
    Object obj = {1, "bc\0"};
    rc = pTree->delete(pTree, (Item)&obj);
    assert(rc == SUCC);
    rc = pTree->delete(pTree, (Item)&obj);
    assert(rc == ERR_NODATA);

    /* Search items from the tree. */
    obj.iId = 2; obj.szName = "cd\0";
    Item item;
    rc = pTree->search(pTree, (Item)&obj, &item);
    assert(rc == SUCC);
    assert(strcmp(((Object*)item)->szName, "cd\0") == 0);

    obj.iId = 1; obj.szName = "bc\0";
    rc = pTree->search(pTree, (Item)&obj, &item);
    assert(rc == ERR_NODATA);

    /* Resolve the tree structure. */
    pTree->minimum(pTree, &item);
    assert(strcmp(((Object*)item)->szName, "ab\0") == 0);

    pTree->maximum(pTree, &item);
    assert(strcmp(((Object*)item)->szName, "cd\0") == 0);

    obj.iId = 2; obj.szName = "cd\0";
    pTree->predecessor(pTree, (Item)&obj, &item);
    assert(strcmp(((Object*)item)->szName, "ab\0") == 0);

    rc = pTree->successor(pTree, (Item)&obj, &item);
    assert(rc == ERR_NODATA);

    obj.iId = 0; obj.szName = "ab\0";
    pTree->successor(pTree, (Item)&obj, &item);
    assert(strcmp(((Object*)item)->szName, "cd\0") == 0);

    rc = pTree->predecessor(pTree, (Item)&obj, &item);
    assert(rc == ERR_NODATA);

    /* Check the tree size. */
    assert(pTree->size(pTree) == 2);

    /* You should deinitialize the DS after all the relevant tasks. */
    RBTreeDeinit(&pTree);

    return SUCC;
}
