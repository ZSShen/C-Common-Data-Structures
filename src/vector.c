#include "array/vector.h"


/*===========================================================================*
 *                       The container private data                          *
 *===========================================================================*/
struct _VectorData {
    int32_t iSize_;
    int32_t iCapacity_;
    Item *aItem_;
    void (*pDestroy_) (Item);
};

#define DEFAULT_CAPACITY    (1)


/*===========================================================================*
 *                  Definition for internal operations                       *
 *===========================================================================*/
/**
 * @brief The default item clean method.
 *
 * @param item          The designated item
 */
void _VectorItemDestroy(Item item);

/**
 * @brief Change the capacity of the internal array.
 *
 * This function resizes the internal array. If the new capacity is smaller than
 * the old one, the trailing items will be removed. Also, if user defined
 * destroy func is set , it also runs the resource clean method for the removed
 * items.
 *
 * @param pData         The pointer to the private data
 * @param iSizeNew      The designated size
 *
 * @retval SUCC
 * @retval ERR_NOMEM    Insufficient memory for array expansion
 *
 * @note The designated capacity should greater than zero.
 */
int32_t _VectorReisze(VectorData *pData, int32_t iSizeNew);


#define CHECK_INIT(self)                                                        \
            do {                                                                \
                if (!self)                                                      \
                    return ERR_NOINIT;                                          \
                if (!(self->pData))                                             \
                    return ERR_NOINIT;                                          \
            } while (0);


/*===========================================================================*
 *         Implementation for the container supporting operations            *
 *===========================================================================*/
int32_t VectorInit(Vector **ppObj)
{
    Vector *pObj;
    *ppObj = (Vector*)malloc(sizeof(Vector));
    if (!(*ppObj))
        return ERR_NOMEM;
    pObj = *ppObj;

    VectorData *pData;
    pObj->pData = (VectorData*)malloc(sizeof(VectorData));
    if (!(pObj->pData)) {
        free(*ppObj);
        *ppObj = NULL;
        return ERR_NOMEM;
    }
    pData = pObj->pData;

    pData->aItem_ = (Item*)malloc(sizeof(Item) * DEFAULT_CAPACITY);
    if (!(pData->aItem_)) {
        free(pObj->pData);
        free(*ppObj);
        *ppObj = NULL;
        return ERR_NOMEM;
    }
    pData->iSize_ = 0;
    pData->iCapacity_ = DEFAULT_CAPACITY;
    pData->pDestroy_ = _VectorItemDestroy;

    pObj->push_back = VectorPushBack;
    pObj->pop_back = VectorPopBack;
    pObj->insert = VectorInsert;
    pObj->delete = VectorDelete;
    pObj->resize = VectorResize;
    pObj->size = VectorSize;
    pObj->capacity = VectorCapacity;
    pObj->set = VectorSet;
    pObj->get = VectorGet;
    pObj->set_destroy = VectorSetDestroy;

    return SUCC;
}

void VectorDeinit(Vector **ppObj)
{
    if (!(*ppObj))
        goto EXIT;
    VectorData *pData = (*ppObj)->pData;
    if (!pData)
        goto FREE_VECTOR;
    Item *aItem = pData->aItem_;
    if (!aItem)
        goto FREE_INTERNAL;

    int32_t iIdx;
    for (iIdx = 0 ; iIdx < pData->iSize_ ; iIdx++)
        pData->pDestroy_(aItem[iIdx]);

    free(pData->aItem_);
FREE_INTERNAL:
    free((*ppObj)->pData);
FREE_VECTOR:
    free(*ppObj);
    *ppObj = NULL;
EXIT:
    return;
}

int32_t VectorPushBack(Vector *self, Item item)
{
    CHECK_INIT(self);
    VectorData *pData = self->pData;

    /* If the internal array is full, extend it to double capacity. */
    if (pData->iSize_ == pData->iCapacity_) {
        int iRtnCode = _VectorReisze(pData, pData->iCapacity_ * 2);
        if (iRtnCode != SUCC)
            return iRtnCode;
    }

    pData->aItem_[pData->iSize_] = item;
    pData->iSize_++;
    return SUCC;
}

int32_t VectorInsert(Vector *self, Item item, int32_t iIdx)
{
    CHECK_INIT(self);
    VectorData *pData = self->pData;

    /* Check for illegal index. */
    if ((iIdx < 0) || (iIdx > pData->iSize_))
        return ERR_IDX;

    /* If the internal array is full, extend it to double capacity. */
    if (pData->iSize_ == pData->iCapacity_) {
        int iRtnCode = _VectorReisze(pData, pData->iCapacity_ * 2);
        if (iRtnCode != SUCC)
            return iRtnCode;
    }

    /* Shift the trailing items if necessary. */
    Item *aItem = pData->aItem_;
    int32_t iShftSize = pData->iSize_ - iIdx;
    if (iShftSize > 0)
        memmove(aItem + iIdx + 1, aItem + iIdx, sizeof(Item) * iShftSize);
    aItem[iIdx] = item;
    pData->iSize_++;

    return SUCC;
}

int32_t VectorPopBack(Vector *self)
{
    CHECK_INIT(self);
    VectorData *pData = self->pData;

    if (pData->iSize_ == 0)
        return ERR_IDX;

    pData->iSize_--;
    Item item = pData->aItem_[pData->iSize_];
    pData->pDestroy_(item);
    return SUCC;
}

int32_t VectorDelete(Vector *self, int32_t iIdx)
{
    CHECK_INIT(self);
    VectorData *pData = self->pData;

    /* Check for illegal index. */
    if ((iIdx < 0) || (iIdx >= pData->iSize_))
        return ERR_IDX;

    Item *aItem = pData->aItem_;

    pData->pDestroy_(aItem[iIdx]);

    /* Shift the trailing items if necessary. */
    int32_t iShftSize = pData->iSize_ - iIdx - 1;
    if (iShftSize > 0)
        memmove(aItem + iIdx, aItem + iIdx + 1, sizeof(Item) *iShftSize);
    pData->iSize_--;

    return SUCC;
}

int32_t VectorResize(Vector *self, int32_t iSize)
{
    CHECK_INIT(self);
    return _VectorReisze(self->pData, iSize);
}

int32_t VectorSize(Vector *self)
{
    CHECK_INIT(self);
    return self->pData->iSize_;
}

int32_t VectorCapacity(Vector *self)
{
    CHECK_INIT(self);
    return self->pData->iCapacity_;
}

int32_t VectorSet(Vector *self, Item item, int32_t iIdx)
{
    CHECK_INIT(self);
    VectorData *pData = self->pData;

    /* Check for illegal index. */
    if ((iIdx < 0) || (iIdx >= pData->iSize_))
        return ERR_IDX;

    Item *aItem = pData->aItem_;
    pData->pDestroy_(aItem[iIdx]);

    aItem[iIdx] = item;
    return SUCC;
}

int32_t VectorGet(Vector *self, Item *pItem, int32_t iIdx)
{
    CHECK_INIT(self);
    VectorData *pData = self->pData;

    /* Check for illegal index. */
    if ((iIdx < 0) || (iIdx >= pData->iSize_))
        return ERR_IDX;

    *pItem = pData->aItem_[iIdx];
    return SUCC;
}

int32_t VectorSetDestroy(Vector *self, void (*pFunc) (Item))
{
    CHECK_INIT(self);
    self->pData->pDestroy_ = pFunc;
    return SUCC;
}


/*===========================================================================*
 *               Implementation for internal operations                      *
 *===========================================================================*/
void _VectorItemDestroy(Item item) {}

int32_t _VectorReisze(VectorData *pData, int32_t iSizeNew)
{
    /* Remove the trailing items if the new capacity is smaller than the old size.
       Clean the resource hold by the removed items if user defined destroy is
       set. */
    if ((iSizeNew < pData->iSize_)) {
        int32_t iIdx = iSizeNew;
        while (iIdx < pData->iSize_) {
            pData->pDestroy_(pData->aItem_[iIdx]);
            iIdx++;
        }
        pData->iSize_ = iSizeNew;
    }

    Item *aItemNew = (Item*)realloc(pData->aItem_, iSizeNew * sizeof(Item));
    if (aItemNew) {
        pData->aItem_ = aItemNew;
        pData->iCapacity_ = iSizeNew;
    }
    return (aItemNew)? SUCC : ERR_NOMEM;
}
