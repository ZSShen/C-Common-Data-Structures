#include "heap/binary_heap.h"
#include "array/vector.h"


/*===========================================================================*
 *                  Hide the private data of the tree                        *
 *===========================================================================*/
struct _BinHeapData {
    Vector *pVector_;
    int32_t (*pCompare_) (Item, Item);
    int32_t (*pDestroy_) (Item);
};


/*===========================================================================*
 *                  Definition for internal operations                       *
 *===========================================================================*/
/**
 * @brief The default order comparison method for a pair of items.
 *
 * @param itemSrc       The source item
 * @param itemTge       The target item
 *
 * @retval 1            The source item has the larger order
 * @retval 0            Both the items have the same order
 * @retval -1           The source item has the smaller order
 */
int32_t _BinHeapCompare(Item itemSrc, Item itemTge);

/**
 * @brief The default resource clean method for an item.
 *
 * @param item          The designated item
 */
void _BinHeapDestroy(Item item);


/*===========================================================================*
 *         Implementation for the container supporting operations            *
 *===========================================================================*/
int32_t BinHeapInit(BinHeap **ppObj)
{
    *ppObj = (BinHeap*)malloc(sizeof(BinHeap));
    if (!(*ppObj))
        return ERR_NOMEM;
    BinHeap *pObj = *ppObj;

    pObj->pData = (BinHeapData*)malloc(sizeof(BinHeapData));
    if (!(pObj->pData)) {
        free(*ppObj);
        *ppObj = NULL;
        return ERR_NOMEM;
    }
    BinHeapData *pData = pObj->pData;

    int32_t iRtnCode = VectorInit(&(pData->pVector_));
    if (iRtnCode != SUCC) {
        free(pObj->pData);
        free(*ppObj);
        *ppObj = NULL;
        return ERR_NOMEM;
    }

    pObj->push = BinHeapPush;
    pObj->top = BinHeapTop;
    pObj->pop = BinHeapPop;
    pObj->size = BinHeapSize;
    pObj->set_compare = BinHeapSetCompare;
    pObj->set_destroy = BinHeapSetDestroy;

    Vector *pVector = pData->pVector_;
    pVector->set_destroy(pVector, _BinHeapDestroy);

    return SUCC;
}

void BinHeapDeinit(BinHeap **ppObj)
{
    if (!(*ppObj))
        goto EXIT;

    BinHeap *pObj = *ppObj;
    if (!(pObj->pData))
        goto FREE_HEAP;

    BinHeapData *pData = pObj->pData;
    if (!(pData->pVector_))
        goto FREE_DATA;

    VectorDeinit(&(pData->pVector_));

FREE_DATA:
    free(pObj->pData);
FREE_HEAP:
    free(*ppObj);
EXIT:
    return;
}

int32_t BinHeapPush(BinHeap *self, Item item)
{
    return SUCC;
}

int32_t BinHeapPop(BinHeap *self)
{
    return SUCC;
}

int32_t BinHeapTop(BinHeap *self, Item *pItem)
{
    return SUCC;
}

int32_t BinHeapSize(BinHeap *self)
{
    return 0;
}

int32_t BinHeapSetCompare(BinHeap *self, int32_t (*pFunc) (Item, Item))
{
    return SUCC;
}

int32_t BinHeapSetDestroy(BinHeap *self, void (*pFunc) (Item))
{
    return SUCC;
}


/*===========================================================================*
 *               Implementation for internal operations                      *
 *===========================================================================*/
int32_t _BinHeapCompare(Item itemSrc, Item itemTge)
{
    if (itemSrc == itemTge)
        return 0;
    return (itemSrc > itemTge)? 1 : (-1);
}

void _BinHeapDestroy(Item item) {}
