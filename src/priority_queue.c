#include "queue/priority_queue.h"
#include "heap/binary_heap.h"


/*===========================================================================*
 *                  Hide the private data of the tree                        *
 *===========================================================================*/
struct _PrioQueueData {
    BinHeap *pHeap_;
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
int32_t _PrioQueueItemComp(Item itemSrc, Item itemTge);

/**
 * @brief The default item clean method.
 *
 * @param item          The designated item
 */
void _PrioQueueItemDestroy(Item item);


#define CHECK_INIT(self)                                                        \
            do {                                                                \
                if (!self)                                                      \
                    return ERR_NOINIT;                                          \
                if (!(self->pData))                                             \
                    return ERR_NOINIT;                                          \
                if (!(self->pData->pHeap_))                                     \
                    return ERR_NOINIT;                                          \
            } while (0);


/*===========================================================================*
 *         Implementation for the container supporting operations            *
 *===========================================================================*/
int32_t PrioQueueInit(PrioQueue **ppObj)
{
    *ppObj = (PrioQueue*)malloc(sizeof(PrioQueue));
    if (!(*ppObj))
        return ERR_NOMEM;
    PrioQueue *pObj = *ppObj;

    pObj->pData = (PrioQueueData*)malloc(sizeof(PrioQueueData));
    if (!(pObj->pData)) {
        free(*ppObj);
        *ppObj = NULL;
        return ERR_NOMEM;
    }
    PrioQueueData *pData = pObj->pData;

    int32_t iRtnCode = BinHeapInit(&(pData->pHeap_));
    if (iRtnCode != SUCC) {
        free(pObj->pData);
        free(*ppObj);
        *ppObj = NULL;
        return ERR_NOMEM;
    }

    pObj->push = PrioQueuePush;
    pObj->top = PrioQueueTop;
    pObj->pop = PrioQueuePop;
    pObj->size = PrioQueueSize;
    pObj->set_compare = PrioQueueSetCompare;
    pObj->set_destroy = PrioQueueSetDestroy;

    BinHeap *pHeap = pData->pHeap_;
    pHeap->set_destroy(pHeap, _PrioQueueItemDestroy);
    pHeap->set_compare(pHeap, _PrioQueueItemComp);

    return SUCC;
}

void PrioQueueDeinit(PrioQueue **ppObj, bool bClean)
{
    if (!(*ppObj))
        goto EXIT;

    PrioQueue *pObj = *ppObj;
    if (!(pObj->pData))
        goto FREE_QUEUE;

    PrioQueueData *pData = pObj->pData;
    if (!(pData->pHeap_))
        goto FREE_DATA;

    BinHeapDeinit(&(pData->pHeap_), bClean);

FREE_DATA:
    free(pObj->pData);
FREE_QUEUE:
    free(*ppObj);
EXIT:
    return;
}

int32_t PrioQueuePush(PrioQueue *self, Item item)
{
    CHECK_INIT(self);
    BinHeap *pHeap = self->pData->pHeap_;
    int32_t iRtnCode = pHeap->push(pHeap, item);
    return iRtnCode;
}

int32_t PrioQueuePop(PrioQueue *self, bool bClean)
{
    CHECK_INIT(self);
    BinHeap *pHeap = self->pData->pHeap_;
    int32_t iRtnCode = pHeap->pop(pHeap, bClean);
    return iRtnCode;
}

int32_t PrioQueueTop(PrioQueue *self, Item *pItem)
{
    CHECK_INIT(self);
    BinHeap *pHeap = self->pData->pHeap_;
    int32_t iRtnCode = pHeap->top(pHeap, pItem);
    return iRtnCode;
}

int32_t PrioQueueSize(PrioQueue *self)
{
    CHECK_INIT(self);
    BinHeap *pHeap = self->pData->pHeap_;
    int32_t iSize = pHeap->size(pHeap);
    return iSize;
}

int32_t PrioQueueSetCompare(PrioQueue *self, int32_t (*pFunc) (Item, Item))
{
    CHECK_INIT(self);
    BinHeap *pHeap = self->pData->pHeap_;
    int32_t iRtnCode = pHeap->set_compare(pHeap, pFunc);
    return iRtnCode;
}

int32_t PrioQueueSetDestroy(PrioQueue *self, void (*pFunc) (Item))
{
    CHECK_INIT(self);
    BinHeap *pHeap = self->pData->pHeap_;
    int32_t iRtnCode = pHeap->set_destroy(pHeap, pFunc);
    return iRtnCode;
}


/*===========================================================================*
 *               Implementation for internal operations                      *
 *===========================================================================*/
int32_t _PrioQueueItemComp(Item itemSrc, Item itemTge)
{
    if (itemSrc == itemTge)
        return 0;
    return (itemSrc > itemTge)? 1 : (-1);
}

void _PrioQueueItemDestroy(Item item) {}
