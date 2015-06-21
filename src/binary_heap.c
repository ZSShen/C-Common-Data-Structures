#include "heap/binary_heap.h"
#include "array/vector.h"


/*===========================================================================*
 *                  Hide the private data of the tree                        *
 *===========================================================================*/
struct _BinHeapData {
    Vector *pVector_;
    int32_t (*pCompare_) (Item, Item);
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
int32_t _BinHeapItemComp(Item itemSrc, Item itemTge);

/**
 * @brief The default item clean method.
 *
 * @param item          The designated item
 */
void _BinHeapItemDestroy(Item item);


#define PARENT(idx)             (idx >> 1)
#define LEFT(idx)               (idx << 1)
#define RIGHT(idx)              ((idx << 1) + 1)


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
    pVector->set_destroy(pVector, _BinHeapItemDestroy);
    pData->pCompare_ = _BinHeapItemComp;

    return SUCC;
}

void BinHeapDeinit(BinHeap **ppObj, bool bClean)
{
    if (!(*ppObj))
        goto EXIT;

    BinHeap *pObj = *ppObj;
    if (!(pObj->pData))
        goto FREE_HEAP;

    BinHeapData *pData = pObj->pData;
    if (!(pData->pVector_))
        goto FREE_DATA;

    VectorDeinit(&(pData->pVector_), bClean);

FREE_DATA:
    free(pObj->pData);
FREE_HEAP:
    free(*ppObj);
EXIT:
    return;
}

int32_t BinHeapPush(BinHeap *self, Item item)
{
    if (!self)
        return ERR_NOINIT;
    BinHeapData *pData = self->pData;

    /* First, push item to the bottom of the heap. */
    Vector *pVector = pData->pVector_;
    int32_t iRtnCode = pVector->push_back(pVector, item);
    if (iRtnCode != SUCC)
        return iRtnCode;

    /* Second, adjust the heap structure. */
    int32_t iIdxCurr = pVector->size(pVector);
    int32_t iIdxParent;
    int32_t iOrder;
    Item itemCurr, itemParent;
    while (iIdxCurr > 1) {
        iIdxParent = PARENT(iIdxCurr);
        pVector->get(pVector, &itemCurr, iIdxCurr - 1);
        pVector->get(pVector, &itemParent, iIdxParent - 1);
        iOrder = pData->pCompare_(itemCurr, itemParent);
        if (iOrder <= 0)
            break;
        pVector->set(pVector, itemCurr, iIdxParent - 1, false);
        pVector->set(pVector, itemParent, iIdxCurr - 1, false);
        iIdxCurr = iIdxParent;
    }

    return SUCC;
}

int32_t BinHeapPop(BinHeap *self, bool bClean)
{
    if (!self)
        return ERR_NOINIT;
    BinHeapData *pData = self->pData;

    /* First, replace the top item with the one stored at the bottom. */
    Vector *pVector = pData->pVector_;
    int32_t iSize = pVector->size(pVector);
    Item itemTop, itemCurr;
    pVector->get(pVector, &itemTop, 0);
    pVector->get(pVector, &itemCurr, iSize - 1);
    pVector->set(pVector, itemTop, iSize - 1, false);
    pVector->set(pVector, itemCurr, 0, false);
    pVector->pop_back(pVector, bClean);

    /* Second, adjust the heap structure. */
    iSize--;
    int32_t iIdxCurr = 1;
    int32_t iIdxChildL, iIdxChildR, iIdxNext;
    int32_t iOrder;
    Item itemChildL, itemChildR, itemNext;
    do {
        iIdxChildL = LEFT(iIdxCurr);
        if (iIdxChildL > iSize)
            break;

        iIdxNext = iIdxCurr;
        pVector->get(pVector, &itemCurr, iIdxCurr - 1);
        pVector->get(pVector, &itemChildL, iIdxChildL - 1);
        iOrder = pData->pCompare_(itemChildL, itemCurr);
        if (iOrder > 0)
            iIdxNext = iIdxChildL;

        iIdxChildR = RIGHT(iIdxCurr);
        if (iIdxChildR <= iSize) {
            pVector->get(pVector, &itemNext, iIdxNext - 1);
            pVector->get(pVector, &itemChildR, iIdxChildR - 1);
            iOrder = pData->pCompare_(itemChildR, itemNext);
            if (iOrder > 0)
                iIdxNext = iIdxChildR;
        }

        if (iIdxNext == iIdxCurr)
            break;

        pVector->get(pVector, &itemNext, iIdxNext - 1);
        pVector->set(pVector, itemCurr, iIdxNext - 1, false);
        pVector->set(pVector, itemNext, iIdxCurr - 1, false);
        iIdxCurr = iIdxNext;
    } while (1);

    return SUCC;
}

int32_t BinHeapTop(BinHeap *self, Item *pItem)
{
    if (!self)
        return ERR_NOINIT;
    Vector *pVector = self->pData->pVector_;
    int32_t iRtnCode = pVector->get(pVector, pItem, 0);
    return iRtnCode;
}

int32_t BinHeapSize(BinHeap *self)
{
    if (!self)
        return ERR_NOINIT;
    Vector *pVector = self->pData->pVector_;
    int32_t iSize = pVector->size(pVector);
    return iSize;
}

int32_t BinHeapSetCompare(BinHeap *self, int32_t (*pFunc) (Item, Item))
{
    if (!self)
        return ERR_NOINIT;
    self->pData->pCompare_ = pFunc;
    return SUCC;
}

int32_t BinHeapSetDestroy(BinHeap *self, void (*pFunc) (Item))
{
    if (!self)
        return ERR_NOINIT;
    Vector *pVector = self->pData->pVector_;
    pVector->set_destroy(pVector, pFunc);
    return SUCC;
}


/*===========================================================================*
 *               Implementation for internal operations                      *
 *===========================================================================*/
int32_t _BinHeapItemComp(Item itemSrc, Item itemTge)
{
    if (itemSrc == itemTge)
        return 0;
    return (itemSrc > itemTge)? 1 : (-1);
}

void _BinHeapItemDestroy(Item item) {}
