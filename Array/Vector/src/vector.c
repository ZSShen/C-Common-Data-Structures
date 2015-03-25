#include "vector.h"


/*===========================================================================*
 *                  Hide the private data of the tree                        *
 *===========================================================================*/
struct _VectorData {
    uint32_t uiSize_;
    uint32_t uiCapacity_;
    Item *aItem_;
    void (*pDestroy_)(Item);
};

#define DEFAULT_CAPACITY    (1)


/*===========================================================================*
 *             Definition for the exported member operations                 *
 *===========================================================================*/
/**
 * Insert the requested item to the tail of the vector.
 */
int32_t VectorPushBack(Vector *self, Item item);

/**
 * Delete the item from the tail of the vector.
 */
int32_t VectorPopBack(Vector *self);

/**
 * Insert the requested item into the designated index and shift
 * the trailing elements.
 */
int32_t VectorInsert(Vector *self, Item item, uint32_t uiIdx);

/**
 * Delete the item from the designated index and shift down the
 * elements.
 */
int32_t VectorDelete(Vector *self, uint32_t uiIdx);

/**
 * Resize the vector and handle some resource collection if necessary.
 */
inline int32_t VectorResize(Vector *self, uint32_t uiSize);

/**
 * Return the number of elements stored in the vector.
 */
inline uint32_t VectorSize(Vector *self);

/**
 * Return the size of the preallocated memory space.
 */
inline uint32_t VectorCapacity(Vector *self);

/**
 * Return the item stored in the designated index.
 */
inline Item VectorAt(Vector *self, uint32_t uiIdx);

/**
 * Set the user defined item deallocation strategy.
 */
inline void VectorSetDestroy(Vector *self, void (*pFunc) (Item));

/*===========================================================================*
 *                  Definition for internal functions                        *
 *===========================================================================*/
/**
 * This function is the default deallcation strategy for an item.
 *
 * @param item          The requested item.
 */
void _VectorItemDestroy(Item item);

/**
 * This function resizes the internal array.
 *
 * @param pData         The pointer to the vector private data.
 * @param uiSizeNew     The designated size;
 *
 * @return              SUCCESS
 *                      FAIL_NO_MEMORY;
 */
int32_t _VectorReisze(VectorData *pData, uint32_t uiSizeNew);


/*===========================================================================*
 *           Implementation for the exported member operations               *
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
    pData->uiSize_ = 0;
    pData->uiCapacity_ = DEFAULT_CAPACITY;
    pData->pDestroy_ = _VectorItemDestroy;

    pObj->push_back = VectorPushBack;
    pObj->pop_back = VectorPopBack;
    pObj->insert = VectorInsert;
    pObj->delete = VectorDelete;
    pObj->resize = VectorResize;
    pObj->size = VectorSize;
    pObj->capacity = VectorCapacity;
    pObj->at = VectorAt;
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

    uint32_t uiIdx;
    for (uiIdx = 0 ; uiIdx < pData->uiSize_ ; uiIdx)
        pData->pDestroy_(aItem[uiIdx]);

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
    if (pData->uiSize_ == pData->uiCapacity_) {
        int iRtnCode = _VectorReisze(pData, pData->uiCapacity_ * 2);
        if (iRtnCode != SUCCESS)
            return iRtnCode;
    }

    pData->aItem_[pData->uiSize_] = item;
    pData->uiSize_++;
    return SUCCESS;
}

int32_t VectorPopBack(Vector *self)
{
    VectorData *pData = self->pData;

    if (pData->uiSize_ == 0)
        return FAIL_OUT_OF_RANGE;

    pData->uiSize_--;
    Item item = pData->aItem_[pData->uiSize_];
    pData->pDestroy_(item);
    return SUCCESS;
}

int32_t VectorInsert(Vector *self, Item item, uint32_t uiIdx)
{
    VectorData *pData = self->pData;

    /* Check for illegal index. */
    if (uiIdx >= pData->uiSize_)
        return FAIL_OUT_OF_RANGE;

    /* If the internal array is full, expand it to double capacity. */
    if (pData->uiSize_ == pData->uiCapacity_) {
        int iRtnCode = _VectorReisze(pData, pData->uiCapacity_ * 2);
        if (iRtnCode != SUCCESS)
            return iRtnCode;
    }

    /* Shift the trailing elements. */
    Item *aItem = pData->aItem_;
    uint32_t uiShftSize = pData->uiSize_ - uiIdx;
    memmove(aItem + uiIdx + 1, aItem + uiIdx, sizeof(Item) * uiShftSize);
    aItem[uiIdx] = item;
    pData->uiSize_++;

    return SUCCESS;
}

int32_t VectorDelete(Vector *self, uint32_t uiIdx)
{
    VectorData *pData = self->pData;

    /* Check for illegal index. */
    if (uiIdx >= pData->uiSize_)
        return FAIL_OUT_OF_RANGE;

    Item *aItem = pData->aItem_;
    pData->pDestroy_(aItem[uiIdx]);

    /* Shift the trailing elements if necessary. */
    uint32_t uiShftSize = pData->uiSize_ - uiIdx - 1;
    if (uiShftSize > 0)
        memmove(aItem + uiIdx, aItem + uiIdx + 1, sizeof(Item) *uiShftSize);
    pData->uiSize_--;

    return SUCCESS;
}

inline int32_t VectorResize(Vector *self, uint32_t uiSize)
{
    return _VectorReisze(self->pData, uiSize);
}

inline uint32_t VectorSize(Vector *self)
{
    return self->pData->uiSize_;
}

inline uint32_t VectorCapacity(Vector *self)
{
    return self->pData->uiCapacity_;
}

inline Item VectorAt(Vector *self, uint32_t uiIdx)
{
    VectorData *pData = self->pData;
    if (uiIdx >= pData->uiSize_)
        return NULL;
    return pData->aItem_[uiIdx];
}

inline void VectorSetDestroy(Vector *self, void (*pFunc) (Item))
{
    self->pData->pDestroy_ = pFunc;
}


/*===========================================================================*
 *               Implementation for internal functions                       *
 *===========================================================================*/
void _VectorItemDestroy(Item item)
{ return; }

int32_t _VectorReisze(VectorData *pData, uint32_t uiSizeNew)
{
    /* Destroy the trailing elements if the new size is smaller than
       the old one.*/
    if (uiSizeNew < pData->uiSize_) {
        uint32_t uiIdx = uiSizeNew;
        while (uiIdx < pData->uiSize_) {
            pData->pDestroy_(pData->aItem_[uiIdx]);
            uiIdx++;
        }
        pData->uiSize_ = uiSizeNew;
    }

    Item *aItemNew = (Item*)realloc(pData->aItem_, uiSizeNew);
    if (aItemNew) {
        pData->aItem_ = aItemNew;
        pData->uiCapacity_ = uiSizeNew;
    }
    return (aItemNew)? SUCCESS : FAIL_NO_MEMORY;
}
