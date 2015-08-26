#include "container/queue.h"


/*===========================================================================*
 *                        The container private data                         *
 *===========================================================================*/
struct _QueueData {
    int32_t iFront_;
    int32_t iBack_;
    int32_t iSize_;
    int32_t iCapacity_;
    Item *aItem_;
    void (*pDestroy_) (Item);
};

#define DEFAULT_CAPACITY        (32)

#define CHECK_INIT(self)                                                        \
            do {                                                                \
                if (!self)                                                      \
                    return ERR_NOINIT;                                          \
                if (!(self->pData))                                             \
                    return ERR_NOINIT;                                          \
                if (!(self->pData->aItem_))                                     \
                    return ERR_NOINIT;                                          \
            } while (0);


/*===========================================================================*
 *               Implementation for the exported operations                  *
 *===========================================================================*/
int32_t QueueInit(Queue **ppObj)
{
    *ppObj = (Queue*)malloc(sizeof(Queue));
    if (!(*ppObj))
        return ERR_NOMEM;
    Queue *pObj = *ppObj;

    pObj->pData = (QueueData*)malloc(sizeof(QueueData));
    if (!(pObj->pData)) {
        free(*ppObj);
        *ppObj = NULL;
        return ERR_NOMEM;
    }
    QueueData *pData = pObj->pData;

    pData->aItem_ = (Item*)malloc(sizeof(Item) * DEFAULT_CAPACITY);
    if (!(pData->aItem_)) {
        free(pObj->pData);
        free(*ppObj);
        *ppObj = NULL;
        return ERR_NOMEM;
    }
    pData->iFront_ = 0;
    pData->iBack_ = 0;
    pData->iSize_ = 0;
    pData->iCapacity_ = DEFAULT_CAPACITY;
    pData->pDestroy_ = NULL;

    pObj->push = QueuePush;
    pObj->pop = QueuePop;
    pObj->front = QueueFront;
    pObj->back = QueueBack;
    pObj->size = QueueSize;
    pObj->set_destroy = QueueSetDestroy;

    return SUCC;
}

void QueueDeinit(Queue **ppObj)
{
    if (!(*ppObj))
        goto EXIT;

    Queue *pObj = *ppObj;
    if (!(pObj->pData))
        goto FREE_QUEUE;

    QueueData *pData = pObj->pData;
    if (!(pData->aItem_))
        goto FREE_DATA;

    if (!(pData->pDestroy_))
        goto FREE_ARRAY;

    Item *aItem = pData->aItem_;
    int32_t iSize = pData->iSize_;
    int32_t iCapacity = pData->iCapacity_;
    int32_t iFront = pData->iFront_;
    while (iSize > 0) {
        if (iFront == iCapacity)
            iFront = 0;
        pData->pDestroy_(aItem[iFront]);
        iFront++;
        iSize--;
    }

FREE_ARRAY:
    free(pData->aItem_);
FREE_DATA:
    free(pObj->pData);
FREE_QUEUE:
    free(*ppObj);
EXIT:
    return;
}

int32_t QueuePush(Queue *self, Item item)
{
    CHECK_INIT(self);
    QueueData *pData = self->pData;

    /* If the array is full, extend it to double capacity. */
    if (pData->iSize_ == pData->iCapacity_) {
        int32_t iCapaNew = pData->iCapacity_ << 1;
        Item *aItemNew = (Item*)realloc(pData->aItem_, iCapaNew * sizeof(Item));
        if (!aItemNew)
            return ERR_NOMEM;
        pData->aItem_ = aItemNew;
        pData->iCapacity_ = iCapaNew;

        /* If back index is smaller than front index, we should migrate the
           circularly pushed items to the newly allocated space. */
        if (pData->iBack_ <= pData->iFront_) {
            memmove(pData->aItem_ + pData->iSize_, pData->aItem_,
                    sizeof(Item) * pData->iBack_);
            pData->iBack_ += pData->iSize_;
        }
    }

    /* Insert the item to the tail of the array. */
    pData->aItem_[pData->iBack_] = item;
    pData->iBack_++;
    if (pData->iBack_ == pData->iCapacity_)
        pData->iBack_ = 0;
    pData->iSize_++;

    return SUCC;
}

int32_t QueuePop(Queue *self)
{
    CHECK_INIT(self);
    QueueData *pData = self->pData;
    if (pData->iSize_ == 0)
        return ERR_IDX;

    /* Delete the item from the head of the array. */
    if (pData->pDestroy_)
        pData->pDestroy_(pData->aItem_[pData->iFront_]);
    pData->iFront_++;
    if (pData->iFront_ == pData->iCapacity_)
        pData->iFront_ = 0;
    pData->iSize_--;

    return SUCC;
}

int32_t QueueFront(Queue *self, Item *pItem)
{
    CHECK_INIT(self);
    if (!pItem)
        return ERR_GET;

    QueueData *pData = self->pData;
    if (pData->iSize_ == 0) {
        *pItem = NULL;
        return ERR_IDX;
    }

    *pItem = pData->aItem_[pData->iFront_];
    return SUCC;
}

int32_t QueueBack(Queue *self, Item *pItem)
{
    CHECK_INIT(self);
    if (!pItem)
        return ERR_GET;

    QueueData *pData = self->pData;
    if (pData->iSize_ == 0) {
        *pItem = NULL;
        return ERR_IDX;
    }

    if (pData->iBack_ == 0)
        *pItem = pData->aItem_[pData->iCapacity_ - 1];
    else
        *pItem = pData->aItem_[pData->iBack_ - 1];
    return SUCC;
}

int32_t QueueSize(Queue *self)
{
    CHECK_INIT(self);
    return self->pData->iSize_;
}

int32_t QueueSetDestroy(Queue *self, void (*pFunc) (Item))
{
    CHECK_INIT(self);
    self->pData->pDestroy_ = pFunc;
    return SUCC;
}