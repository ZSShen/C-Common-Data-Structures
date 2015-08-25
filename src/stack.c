#include "container/stack.h"


/*===========================================================================*
 *                        The container private data                         *
 *===========================================================================*/
struct _StackData {
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
int32_t StackInit(Stack **ppObj)
{
    *ppObj = (Stack*)malloc(sizeof(Stack));
    if (!(*ppObj))
        return ERR_NOMEM;
    Stack *pObj = *ppObj;

    pObj->pData = (StackData*)malloc(sizeof(StackData));
    if (!(pObj->pData)) {
        free(*ppObj);
        *ppObj = NULL;
        return ERR_NOMEM;
    }
    StackData *pData = pObj->pData;

    pData->aItem_ = (Item*)malloc(sizeof(Item) * DEFAULT_CAPACITY);
    if (!(pData->aItem_)) {
        free(pObj->pData);
        free(*ppObj);
        *ppObj = NULL;
        return ERR_NOMEM;
    }
    pData->iSize_ = 0;
    pData->iCapacity_ = DEFAULT_CAPACITY;
    pData->pDestroy_ = NULL;

    pObj->push = StackPush;
    pObj->pop = StackPop;
    pObj->top = StackTop;
    pObj->size = StackSize;
    pObj->set_destroy = StackSetDestroy;

    return SUCC;
}

void StackDeinit(Stack **ppObj)
{
    if (!(*ppObj))
        goto EXIT;

    Stack *pObj = *ppObj;
    if (!(pObj->pData))
        goto FREE_QUEUE;

    StackData *pData = pObj->pData;
    if (!(pData->aItem_))
        goto FREE_DATA;

    if (!(pData->pDestroy_))
        goto FREE_ARRAY;

	int32_t iIdx = 0;
	while (iIdx < pData->iSize_) {
		pData->pDestroy_(pData->aItem_[iIdx]);
		iIdx++;
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

int32_t StackPush(Stack *self, Item item)
{
    CHECK_INIT(self);
    StackData *pData = self->pData;

    /* If the array is full, extend it to double capacity. */
    if (pData->iSize_ == pData->iCapacity_) {
        int32_t iCapaNew = pData->iCapacity_ << 1;
        Item *aItemNew = (Item*)realloc(pData->aItem_, iCapaNew * sizeof(Item));
        if (!aItemNew)
            return ERR_NOMEM;
        pData->aItem_ = aItemNew;
        pData->iCapacity_ = iCapaNew;
    }

    /* Insert the item to the tail of the array. */
    pData->aItem_[pData->iSize_++] = item;

    return SUCC;
}

int32_t StackPop(Stack *self)
{
    CHECK_INIT(self);
    StackData *pData = self->pData;
    if (pData->iSize_ == 0)
        return ERR_IDX;

    /* Delete the item from the tail of the array. */
    if (pData->pDestroy_)
        pData->pDestroy_(pData->aItem_[pData->iSize_ - 1]);
    pData->iSize_--;

    return SUCC;
}

int32_t StackTop(Stack *self, Item *pItem)
{
    CHECK_INIT(self);
    if (!pItem)
        return ERR_GET;

    StackData *pData = self->pData;
    if (pData->iSize_ == 0) {
        *pItem = NULL;
        return ERR_IDX;
    }

    *pItem = pData->aItem_[pData->iSize_ - 1];
    return SUCC;
}

int32_t StackSize(Stack *self)
{
    CHECK_INIT(self);
    return self->pData->iSize_;
}

int32_t StackSetDestroy(Stack *self, void (*pFunc) (Item))
{
    CHECK_INIT(self);
    self->pData->pDestroy_ = pFunc;
    return SUCC;
}
