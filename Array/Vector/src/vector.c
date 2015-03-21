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
}

void VectorDeinit(Vector **ppObj)
{
    return;
}


/*===========================================================================*
 *               Implementation for internal functions                       *
 *===========================================================================*/
void _VectorItemDestroy(Item item)
{ return; }
