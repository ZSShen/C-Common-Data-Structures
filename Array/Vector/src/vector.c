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
 int32_t VectorResize(Vector *self, uint32_t uiSize);


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

int32_t VectorResize(Vector *self, uint32_t uiSize)
{
    return _VectorReisze(self->pData, uiSize);
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
