#include "map/tree_map.h"
#include "tree/red_black_tree.h"


/*===========================================================================*
 *                  Hide the private data of the tree                        *
 *===========================================================================*/
struct _TreeMapData {
    RedBlackTree *pTree_;
    int32_t (*pCompare_) (Entry, Entry);
    void (*pDestroy_) (Entry);
};


/*===========================================================================*
 *                  Definition for internal operations                       *
 *===========================================================================*/
/**
 * @brief The default order comparison method for a key value pair.
 *
 * @param entSrc         The pointer to the source pair
 * @param entTge         The pointer to the target pair
 *
 * @retval 1             The source pair has the larger order
 * @retval 0             Both the pairs have the same order
 * @retval -1            The source pair has the smaller order
 */
int32_t _TreeMapCompare(Entry entSrc, Entry entTge);

/**
 * @brief The default resource clean method for a key value pair.
 *
 * @param ent            The pointer to the designated key value pair
 */
void _TreeMapDestroy(Entry ent);


#define CHECK_INIT(self)                                                        \
            do {                                                                \
                if (!self)                                                      \
                    return ERR_NOINIT;                                          \
                if (!(self->pData))                                             \
                    return ERR_NOINIT;                                          \
                if (!(self->pData->pTree_))                                     \
                    return ERR_NOINIT;                                          \
            } while (0);


/*===========================================================================*
 *         Implementation for the container supporting operations            *
 *===========================================================================*/
int32_t TreeMapInit(TreeMap **ppObj)
{
    *ppObj = (TreeMap*)malloc(sizeof(TreeMap));
    if (!(*ppObj))
        return ERR_NOMEM;
    TreeMap *pObj = *ppObj;

    pObj->pData = (TreeMapData*)malloc(sizeof(TreeMapData));
    if (!(pObj->pData)) {
        free(*ppObj);
        *ppObj = NULL;
        return ERR_NOMEM;
    }
    TreeMapData *pData = pObj->pData;

    int32_t iRtnCode = RBTreeInit(&(pData->pTree_));
    if (iRtnCode != SUCC) {
        free(pObj->pData);
        free(*ppObj);
        *ppObj = NULL;
        return ERR_NOMEM;
    }

    pObj->put = TreeMapPut;
    pObj->get = TreeMapGet;
    pObj->remove = TreeMapRemove;
    pObj->size = TreeMapSize;
    pObj->set_compare = TreeMapSetCompare;
    pObj->set_destroy = TreeMapSetDestroy;

    RedBlackTree *pTree = pData->pTree_;
    pTree->set_compare(pTree, _TreeMapCompare);
    pTree->set_destroy(pTree, _TreeMapDestroy);

    return SUCC;
}

void TreeMapDeinit(TreeMap **ppObj)
{
    if (!(*ppObj))
        goto EXIT;

    TreeMap *pObj = *ppObj;
    if (!(pObj->pData))
        goto FREE_MAP;

    TreeMapData *pData = pObj->pData;
    if (!(pData->pTree_))
        goto FREE_DATA;

    RBTreeDeinit(&(pData->pTree_));

FREE_DATA:
    free(pObj->pData);
FREE_MAP:
    free(*ppObj);
    *ppObj = NULL;
EXIT:
    return;
}

int32_t TreeMapPut(TreeMap *self, Entry ent)
{
    CHECK_INIT(self);
    RedBlackTree *pTree = self->pData->pTree_;
    int32_t iRtnCode = pTree->insert(pTree, (Item)ent);
    return iRtnCode;
}

int32_t TreeMapGet(TreeMap *self, Key key, Value *pValue)
{
    if (!pValue)
        return ERR_GET;
    *pValue = NULL;
    CHECK_INIT(self);

    RedBlackTree *pTree = self->pData->pTree_;
    Pair pairIn = {key, NULL};
    Pair *pPairOut;
    int32_t iRtnCode = pTree->search(pTree, (Entry)&pairIn, (Entry*)&pPairOut);
    if (iRtnCode == SUCC)
        *pValue = pPairOut->value;
    return iRtnCode;
}

int32_t TreeMapRemove(TreeMap *self, Key key)
{
    CHECK_INIT(self);
    RedBlackTree *pTree = self->pData->pTree_;
    Pair pair = {key, NULL};
    int32_t iRtnCode = pTree->delete(pTree, (Entry)&pair);
    return iRtnCode;
}

int32_t TreeMapSize(TreeMap *self)
{
    CHECK_INIT(self);
    RedBlackTree *pTree = self->pData->pTree_;
    int32_t iSize = pTree->size(pTree);
    return iSize;
}

int32_t TreeMapSetCompare(TreeMap *self, int32_t (*pFunc) (Entry, Entry))
{
    CHECK_INIT(self);
    RedBlackTree *pTree = self->pData->pTree_;
    pTree->set_compare(pTree, pFunc);
    return SUCC;
}

int32_t TreeMapSetDestroy(TreeMap *self, void (*pFunc) (Entry))
{
    CHECK_INIT(self);
    RedBlackTree *pTree = self->pData->pTree_;
    pTree->set_destroy(pTree, pFunc);
    return SUCC;
}


/*===========================================================================*
 *               Implementation for internal operations                      *
 *===========================================================================*/
int32_t _TreeMapCompare(Entry entSrc, Entry entTge)
{
    Pair *pPairSrc = (Pair*)entSrc;
    Pair *pPairTge = (Pair*)entTge;
    if (pPairSrc->key == pPairTge->key)
        return 0;
    return (pPairSrc->key > pPairTge->key)? 1 : (-1);
}

void _TreeMapDestroy(Entry ent)
{
    free((Pair*)ent);
    return;
}
