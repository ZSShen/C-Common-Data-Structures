#include "vector.h"


/*===========================================================================*
 *                       The container private data                          *
 *===========================================================================*/
struct _VectorData {
    int32_t iSize_;
    int32_t iCapacity_;
    Item *aItem_;
    void (*pDestroy_)(Item);
};

#define DEFAULT_CAPACITY    (1)


/*===========================================================================*
 *                  Definition for internal operations                       *
 *===========================================================================*/
/**
 * This function is the default item resource clean strategy.
 *
 * @param item          The requested item.
 */
void _VectorItemDestroy(Item item);

/**
 * This function resizes the internal array with the proper space reallocation.
 * If the new size is smaller than the old one, the trailing items will be
 * cleaned.
 *
 * It will fail when:
 *   (1) Insufficient memory space for array expansion.
 *
 * @param pData         The pointer to the private data.
 * @param iSizeNew      The designated size;
 *
 * @return              SUCCESS
 *                      FAIL_NO_MEMORY;
 */
int32_t _VectorReisze(VectorData *pData, int32_t iSizeNew);


/*===========================================================================*
 *         Implementation for the container supporting operations            *
 *===========================================================================*/
int32_t VectorInit(Vector **ppObj)
{
    Vector *pObj;
    *ppObj = (Vector*)malloc(sizeof(Vector));
    if (!(*ppObj))
        return FAIL_NO_MEMORY;
    pObj = *ppObj;

    VectorData *pData;
    pObj->pData = (VectorData*)malloc(sizeof(VectorData));
    if (!(pObj->pData)) {
        free(*ppObj);
        *ppObj = NULL;
        return FAIL_NO_MEMORY;
    }
    pData = pObj->pData;

    pData->aItem_ = (Item*)malloc(sizeof(Item) * DEFAULT_CAPACITY);
    if (!(pData->aItem_)) {
        free(pObj->pData);
        free(*ppObj);
        *ppObj = NULL;
        return FAIL_NO_MEMORY;
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

    return SUCCESS;
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
    VectorData *pData = self->pData;

    /* If the internal array is full, expand it to double capacity. */
    if (pData->iSize_ == pData->iCapacity_) {
        int iRtnCode = _VectorReisze(pData, pData->iCapacity_ * 2);
        if (iRtnCode != SUCCESS)
            return iRtnCode;
    }

    pData->aItem_[pData->iSize_] = item;
    pData->iSize_++;
    return SUCCESS;
}

int32_t VectorInsert(Vector *self, Item item, int32_t iIdx)
{
    VectorData *pData = self->pData;

    /* Check for illegal index. */
    if ((iIdx < 0) || (iIdx > pData->iSize_))
        return FAIL_OUT_OF_RANGE;

    /* If the internal array is full, expand it to double capacity. */
    if (pData->iSize_ == pData->iCapacity_) {
        int iRtnCode = _VectorReisze(pData, pData->iCapacity_ * 2);
        if (iRtnCode != SUCCESS)
            return iRtnCode;
    }

    /* Shift the trailing items if necessary. */
    Item *aItem = pData->aItem_;
    int32_t iShftSize = pData->iSize_ - iIdx;
    if (iShftSize > 0)
        memmove(aItem + iIdx + 1, aItem + iIdx, sizeof(Item) * iShftSize);
    aItem[iIdx] = item;
    pData->iSize_++;

    return SUCCESS;
}

int32_t VectorPopBack(Vector *self)
{
    VectorData *pData = self->pData;

    if (pData->iSize_ == 0)
        return FAIL_OUT_OF_RANGE;

    pData->iSize_--;
    Item item = pData->aItem_[pData->iSize_];
    pData->pDestroy_(item);
    return SUCCESS;
}

int32_t VectorDelete(Vector *self, int32_t iIdx)
{
    VectorData *pData = self->pData;

    /* Check for illegal index. */
    if ((iIdx < 0) || (iIdx >= pData->iSize_))
        return FAIL_OUT_OF_RANGE;

    Item *aItem = pData->aItem_;
    pData->pDestroy_(aItem[iIdx]);

    /* Shift the trailing items if necessary. */
    int32_t iShftSize = pData->iSize_ - iIdx - 1;
    if (iShftSize > 0)
        memmove(aItem + iIdx, aItem + iIdx + 1, sizeof(Item) *iShftSize);
    pData->iSize_--;

    return SUCCESS;
}

int32_t VectorResize(Vector *self, int32_t iSize)
{
    return _VectorReisze(self->pData, iSize);
}

int32_t VectorSize(Vector *self)
{
    return self->pData->iSize_;
}

int32_t VectorCapacity(Vector *self)
{
    return self->pData->iCapacity_;
}

int32_t VectorSet(Vector *self, Item item, int32_t iIdx)
{
    VectorData *pData = self->pData;

    /* Check for illegal index. */
    if ((iIdx < 0) || (iIdx >= pData->iSize_))
        return FAIL_OUT_OF_RANGE;

    /* Clean the resource hold by the old item. */
    Item *aItem = pData->aItem_;
    pData->pDestroy_(aItem[iIdx]);

    aItem[iIdx] = item;
    return SUCCESS;
}

int32_t VectorGet(Vector *self, Item *pItem, int32_t iIdx)
{
    VectorData *pData = self->pData;

    /* Check for illegal index. */
    if ((iIdx < 0) || (iIdx >= pData->iSize_))
        return FAIL_OUT_OF_RANGE;

    *pItem = pData->aItem_[iIdx];
    return SUCCESS;
}

void VectorSetDestroy(Vector *self, void (*pFunc) (Item))
{
    self->pData->pDestroy_ = pFunc;
}


/*===========================================================================*
 *               Implementation for internal operations                      *
 *===========================================================================*/
void _VectorItemDestroy(Item item) {}

int32_t _VectorReisze(VectorData *pData, int32_t iSizeNew)
{
    /* Clean the trailing items if the new size is smaller than the old one.*/
    if (iSizeNew < pData->iSize_) {
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
    return (aItemNew)? SUCCESS : FAIL_NO_MEMORY;
}
