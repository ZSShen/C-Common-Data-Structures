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
 *                  Definition for internal functions                        *
 *===========================================================================*/
/**
 * This function is the default deallcation strategy for an item.
 *
 * @param item         The requested item.
 */
void _VectorItemDestroy(Item item);


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


/*===========================================================================*
 *               Implementation for internal functions                       *
 *===========================================================================*/
void _VectorItemDestroy(Item item)
{ return; }
