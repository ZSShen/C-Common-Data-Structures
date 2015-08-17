#include "container/priority_queue.h"


/*===========================================================================*
 *                  Hide the private data of the tree                        *
 *===========================================================================*/
struct _PriorityQueueData {
    int32_t iSize_;
    int32_t iCapacity_;
    Item *aItem_;
    int32_t (*pCompare_) (Item, Item);
    void (*pDestroy_) (Item);
};

#define DEFAULT_CAPACITY        (32)
#define PARENT(idx)             (idx >> 1)
#define LEFT(idx)               (idx << 1)
#define RIGHT(idx)              ((idx << 1) + 1)


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
int32_t _PriorityQueueItemComp(Item itemSrc, Item itemTge);


#define CHECK_INIT(self)                                                        \
            do {                                                                \
                if (!self)                                                      \
                    return ERR_NOINIT;                                          \
                if (!(self->pData))                                             \
                    return ERR_NOINIT;                                          \
            } while (0);


/*===========================================================================*
 *               Implementation for the exported operations                  *
 *===========================================================================*/
int32_t PriorityQueueInit(PriorityQueue **ppObj)
{
    *ppObj = (PriorityQueue*)malloc(sizeof(PriorityQueue));
    if (!(*ppObj))
        return ERR_NOMEM;
    PriorityQueue *pObj = *ppObj;

    pObj->pData = (PriorityQueueData*)malloc(sizeof(PriorityQueueData));
    if (!(pObj->pData)) {
        free(*ppObj);
        *ppObj = NULL;
        return ERR_NOMEM;
    }
    PriorityQueueData *pData = pObj->pData;

    pData->aItem_ = (Item*)malloc(sizeof(Item) * DEFAULT_CAPACITY);
    if (!(pData->aItem_)) {
        free(pObj->pData);
        free(*ppObj);
        *ppObj = NULL;
        return ERR_NOMEM;
    }
    pData->iSize_ = 0;
    pData->iCapacity_ = DEFAULT_CAPACITY;
    pData->pCompare_ = _PriorityQueueItemComp;
    pData->pDestroy_ = NULL;

    pObj->push = PriorityQueuePush;
    pObj->top = PriorityQueueTop;
    pObj->pop = PriorityQueuePop;
    pObj->size = PriorityQueueSize;
    pObj->set_compare = PriorityQueueSetCompare;
    pObj->set_destroy = PriorityQueueSetDestroy;

    return SUCC;
}

void PriorityQueueDeinit(PriorityQueue **ppObj)
{
    if (!(*ppObj))
        goto EXIT;

    PriorityQueue *pObj = *ppObj;
    if (!(pObj->pData))
        goto FREE_QUEUE;

    PriorityQueueData *pData = pObj->pData;
    if (!(pData->aItem_))
        goto FREE_DATA;

    if (!(pData->pDestroy_))
        goto FREE_ARRAY;

    Item *aItem = pData->aItem_;
    int iIdx;
    for (iIdx = 0 ; iIdx < pData->iSize_ ; iIdx++)
        pData->pDestroy_(aItem[iIdx]);

FREE_ARRAY:
    free(pData->aItem_);
FREE_DATA:
    free(pObj->pData);
FREE_QUEUE:
    free(*ppObj);
EXIT:
    return;
}

int32_t PriorityQueuePush(PriorityQueue *self, Item item)
{
    CHECK_INIT(self);
    PriorityQueueData *pData = self->pData;

    /* If the heap is full, extend it to double capacity. */
    Item *aItem = pData->aItem_;
    if (pData->iSize_ == pData->iCapacity_) {
        int32_t iCapaNew = pData->iCapacity_ << 1;
        Item *aItemNew = (Item*)realloc(aItem, iCapaNew * sizeof(Item));
        if (!aItemNew)
            return ERR_NOMEM;
        aItem = pData->aItem_ = aItemNew;
        pData->iCapacity_ = iCapaNew;
    }

    /* Push the item to the bottom of the heap. */
    aItem[pData->iSize_] = item;
    pData->iSize_++;

    /* Adjust the heap structure. */
    int32_t iIdxCurr = pData->iSize_;
    int32_t iIdxParent;
    int32_t iOrder;
    while (iIdxCurr > 1) {
        iIdxParent = PARENT(iIdxCurr);
        iOrder = pData->pCompare_(aItem[iIdxCurr - 1], aItem[iIdxParent - 1]);
        if (iOrder <= 0)
            break;
        item = aItem[iIdxCurr - 1];
        aItem[iIdxCurr - 1] = aItem[iIdxParent - 1];
        aItem[iIdxParent - 1] = item;
        iIdxCurr = iIdxParent;
    }

    return SUCC;
}

int32_t PriorityQueuePop(PriorityQueue *self)
{
    CHECK_INIT(self);
    PriorityQueueData *pData = self->pData;

    /* Remove the heap top item. */
    if (pData->pDestroy_)
        pData->pDestroy_(pData->aItem_[0]);
    pData->aItem_[0] = pData->aItem_[pData->iSize_ - 1];
    pData->iSize_--;

    /* Adjust the heap structure. */
    Item *aItem = pData->aItem_;
    int32_t iSize = pData->iSize_;
    int32_t iIdxCurr = 1;
    int32_t iIdxChildL, iIdxChildR, iIdxNext;
    int32_t iOrder;
    do {
        iIdxChildL = LEFT(iIdxCurr);
        if (iIdxChildL > iSize)
            break;

        iIdxNext = iIdxCurr;
        iOrder = pData->pCompare_(aItem[iIdxChildL - 1], aItem[iIdxNext - 1]);
        if (iOrder > 0)
            iIdxNext = iIdxChildL;

        iIdxChildR = RIGHT(iIdxCurr);
        if (iIdxChildR <= iSize) {
            iOrder = pData->pCompare_(aItem[iIdxChildR - 1], aItem[iIdxNext - 1]);
            if (iOrder > 0)
                iIdxNext = iIdxChildR;
        }

        if (iIdxNext == iIdxCurr)
            break;
        Item item = aItem[iIdxNext - 1];
        aItem[iIdxNext - 1] = aItem[iIdxCurr - 1];
        aItem[iIdxCurr - 1] = item;
        iIdxCurr = iIdxNext;
    } while (true);

    return SUCC;
}

int32_t PriorityQueueTop(PriorityQueue *self, Item *pItem)
{
    CHECK_INIT(self);
    if (!pItem)
        return ERR_GET;
    if (self->pData->iSize_ == 0)
        return ERR_IDX;
    *pItem = self->pData->aItem_[0];
    return SUCC;
}

int32_t PriorityQueueSize(PriorityQueue *self)
{
    CHECK_INIT(self);
    return self->pData->iSize_;
}

int32_t PriorityQueueSetCompare(PriorityQueue *self, int32_t (*pFunc) (Item, Item))
{
    CHECK_INIT(self);
    self->pData->pCompare_ = pFunc;
    return SUCC;
}

int32_t PriorityQueueSetDestroy(PriorityQueue *self, void (*pFunc) (Item))
{
    CHECK_INIT(self);
    self->pData->pDestroy_ = pFunc;
    return SUCC;
}


/*===========================================================================*
 *               Implementation for internal operations                      *
 *===========================================================================*/
int32_t _PriorityQueueItemComp(Item itemSrc, Item itemTge)
{
    if (itemSrc == itemTge)
        return 0;
    return (itemSrc > itemTge)? 1 : (-1);
}
