#include "map/ordered_map.h"
#include "tree/balanced_tree.h"


/*===========================================================================*
 *                  Hide the private data of the tree                        *
 *===========================================================================*/
struct _OMapData {
    BalancedTree *pTree_;
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
int32_t _OdrMapCompare(Entry entSrc, Entry entTge);

/**
 * @brief The default resource clean method for a key value pair.
 *
 * @param ent            The pointer to the designated key value pair
 */
void _OdrMapDestroy(Entry ent);


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
int32_t OdrMapInit(OrderedMap **ppObj)
{
    *ppObj = (OrderedMap*)malloc(sizeof(OrderedMap));
    if (!(*ppObj))
        return ERR_NOMEM;
    OrderedMap *pObj = *ppObj;

    pObj->pData = (OMapData*)malloc(sizeof(OrderedMap));
    if (!(pObj->pData)) {
        free(*ppObj);
        *ppObj = NULL;
        return ERR_NOMEM;
    }
    OMapData *pData = pObj->pData;

    int32_t iRtnCode = BalTreeInit(&(pData->pTree_));
    if (!(pData->pTree_)) {
        free(pObj->pData);
        free(*ppObj);
        *ppObj = NULL;
        return ERR_NOMEM;
    }

    pObj->put = OdrMapPut;
    pObj->get = OdrMapGet;
    pObj->remove = OdrMapRemove;
    pObj->size = OdrMapSize;
    pObj->set_compare = OdrMapSetCompare;
    pObj->set_destroy = OdrMapSetDestroy;

    BalancedTree *pTree = pData->pTree_;
    pTree->set_compare(pTree, _OdrMapCompare);
    pTree->set_destroy(pTree, _OdrMapDestroy);

    return SUCC;
}

void OdrMapDeinit(OrderedMap **ppObj)
{
    if (!(*ppObj))
        goto EXIT;

    OrderedMap *pObj = *ppObj;
    if (!(pObj->pData))
        goto FREE_MAP;

    OMapData *pData = pObj->pData;
    if (!(pData->pTree_))
        goto FREE_DATA;

    BalTreeDeinit(&(pData->pTree_));

FREE_DATA:
    free(pObj->pData);
FREE_MAP:
    free(*ppObj);
    *ppObj = NULL;
EXIT:
    return;
}


int32_t OdrMapPut(OrderedMap *self, Entry ent)
{
    CHECK_INIT(self);
    BalancedTree *pTree = self->pData->pTree_;
    int32_t iRtnCode = pTree->insert(pTree, (Item)ent);
    return iRtnCode;
}

int32_t OdrMapGet(OrderedMap *self, Key key, Value *pValue)
{
    return SUCC;
}

int32_t OdrMapRemove(OrderedMap *self, Key key)
{
    return SUCC;
}

int32_t OdrMapSize(OrderedMap *self)
{
    CHECK_INIT(self);
    BalancedTree *pTree = self->pData->pTree_;
    int32_t iSize = pTree->size(pTree);
    return iSize;
}

int32_t OdrMapSetCompare(OrderedMap *self, int32_t (*pFunc) (Entry, Entry))
{
    CHECK_INIT(self);
    BalancedTree *pTree = self->pData->pTree_;
    pTree->set_compare(pTree, pFunc);
    return SUCC;
}

int32_t OdrMapSetDestroy(OrderedMap *self, void (*pFunc) (Entry))
{
    CHECK_INIT(self);
    BalancedTree *pTree = self->pData->pTree_;
    pTree->set_destroy(pTree, pFunc);
    return SUCC;
}


/*===========================================================================*
 *               Implementation for internal operations                      *
 *===========================================================================*/
int32_t _OdrMapCompare(Entry entSrc, Entry entTge)
{
    Pair *pPairSrc = (Pair*)entSrc;
    Pair *pPairTge = (Pair*)entTge;
    if (pPairSrc->key == pPairTge->key)
        return 0;
    else {
        if (pPairSrc->key > pPairTge->key)
            return 1;
        else
            return -1;
    }
}

void _OdrMapDestroy(Entry ent)
{
    free((Pair*)ent);
    return;
}