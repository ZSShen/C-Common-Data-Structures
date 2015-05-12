#include "list/dlinked_list.h"


/*===========================================================================*
 *                  Hide the private data of the tree                        *
 *===========================================================================*/
 typedef struct _DListNode {
    Item item;
    struct _DListNode *pPrev;
    struct _DListNode *pNext;
 } DListNode;

struct _DListData {
    int32_t iSize_;
    DListNode *pHead_;
    void (*pDestroy_)(Item);
};


/*===========================================================================*
 *                  Definition for internal operations                       *
 *===========================================================================*/
/**
 * @brief Traverse all the nodes and clean the allocated resource.
 *
 * @param pData         The pointer to the list private data
 */
 void _DListDeinit(DListData *pData);


/**
 * @brief The default clean strategy for the resource hold by an item.
 *
 * @param item          The requested item
 */
void _DListItemDestroy(Item item);


#define NEW_NODE(pNew, item)                                                    \
            do {                                                                \
                pNew = (DListNode*)malloc(sizeof(DListNode));                   \
                if (!pNew)                                                      \
                    return ERR_NOMEM;                                           \
                pNew->item = item;                                              \
            } while (0);


#define PUSH_NODE(pNew, pHead)                                                  \
            do {                                                                \
                pNew->pNext = pHead;                                            \
                pNew->pPrev = pHead->pPrev;                                     \
                pHead->pPrev->pNext = pNew;                                     \
                pHead->pPrev = pNew;                                            \
            } while (0);


#define INSERT_NODE(pNew, pTrack)                                               \
            do {                                                                \
                pNew->pNext = pTrack;                                           \
                pNew->pPrev = pTrack->pPrev;                                    \
                pTrack->pPrev->pNext = pNew;                                    \
                pTrack->pPrev = pNew;                                           \
            } while (0);


/*===========================================================================*
 *         Implementation for the container supporting operations            *
 *===========================================================================*/
int32_t DListInit(DLinkedList **ppObj)
{
    *ppObj = (DLinkedList*)malloc(sizeof(DLinkedList));
    if (!(*ppObj))
        return ERR_NOMEM;
    DLinkedList *pObj = *ppObj;

    pObj->pData = (DListData*)malloc(sizeof(DListData));
    if (!(pObj->pData)) {
        free(*ppObj);
        *ppObj = NULL;
        return ERR_NOMEM;
    }
    DListData *pData = pObj->pData;

    pData->iSize_ = 0;
    pData->pHead_ = NULL;
    pData->pDestroy_ = _DListItemDestroy;

    pObj->push_front = DListPushFront;
    pObj->push_back = DListPushBack;
    pObj->insert = DListInsert;

    pObj->pop_front = DListPopFront;
    pObj->pop_back = DListPopBack;
    pObj->delete = DListDelete;

    pObj->set_front = DListSetFront;
    pObj->set_back = DListSetBack;
    pObj->set_at = DListSetAt;

    pObj->get_front = DListGetFront;
    pObj->get_back = DListGetBack;
    pObj->get_at = DListGetAt;

    pObj->resize = DListResize;
    pObj->size = DListSize;
    pObj->reverse = DListReverse;
    pObj->set_destroy = DListSetDestroy;

    return SUCC;
}

void DListDeinit(DLinkedList **ppObj)
{
    if (!(*ppObj))
        goto EXIT;

    DListData *pData = (*ppObj)->pData;
    if (!pData)
        goto FREE_LIST;

    _DListDeinit(pData);
    free(pData);

FREE_LIST:
    free(*ppObj);
    *ppObj = NULL;
EXIT:
    return;
}

int32_t DListPushFront(DLinkedList *pObj, Item item)
{
    DListNode *pNew;
    NEW_NODE(pNew, item);

    DListData *pData = pObj->pData;
    if (!pData->pHead_)
        pNew->pPrev = pNew->pNext = pNew;
    else
        PUSH_NODE(pNew, pData->pHead_);

    pData->pHead_ = pNew;
    pData->iSize_++;

    return SUCC;
}

int32_t DListPushBack(DLinkedList *pObj, Item item)
{
    DListNode *pNew;
    NEW_NODE(pNew, item);

    DListData *pData = pObj->pData;
    if (!pData->pHead_) {
        pNew->pPrev = pNew->pNext = pNew;
        pData->pHead_ = pNew;
    } else
        PUSH_NODE(pNew, pData->pHead_);

    pData->iSize_++;

    return SUCC;
}

int32_t DListInsert(DLinkedList *pObj, Item item, int32_t iIdx)
{
    DListData *pData = pObj->pData;

    /* Rejct invalid index. */
    if (abs(iIdx) > pData->iSize_)
        return ERR_IDX;

    DListNode *pNew;
    NEW_NODE(pNew, item);
    if (!pData->pHead_) {
        pNew->pPrev = pNew->pNext = pNew;
        pData->pHead_ = pNew;
    } else {
        int32_t i;
        DListNode *pTrack = pData->pHead_;

        /* Insert the node into the proper position with forward indexing. */
        if (iIdx >= 0) {
            for (i = iIdx ; i > 0 ; i--)
                pTrack = pTrack->pNext;
            INSERT_NODE(pNew, pTrack);
            if (iIdx == 0)
                pData->pHead_ = pNew;
        }
        /* Insert the node into the proper position with backward indexing. */
        else {
            for (i = 0 ; i > iIdx ; i--)
                pTrack = pTrack->pPrev;
            INSERT_NODE(pNew, pTrack);
            if (-iIdx == pData->iSize_)
                pData->pHead_ = pNew;
        }
    }

    pData->iSize_++;

    return SUCC;
}

int32_t DListPopFront(DLinkedList *pObj) {return 0;}

int32_t DListPopBack(DLinkedList *pObj) {return 0;}

int32_t DListDelete(DLinkedList *pObj, int32_t iIdx) {return 0;}

int32_t DListSetFront(DLinkedList *pObj, Item item) {return 0;}

int32_t DListSetBack(DLinkedList *pObj, Item item) {return 0;}

int32_t DListSetAt(DLinkedList *pObj, Item item, int32_t iIdx) {return 0;}

int32_t DListGetFront(DLinkedList *pObj, Item *pItem)
{
    DListData *pData = pObj->pData;
    if (!pData->pHead_) {
        *pItem = NULL;
        return ERR_IDX;
    }

    *pItem = pData->pHead_->item;
    return SUCC;
}

int32_t DListGetBack(DLinkedList *pObj, Item *pItem)
{
    DListData *pData = pObj->pData;
    if (!pData->pHead_) {
        *pItem = NULL;
        return ERR_IDX;
    }

    *pItem = pData->pHead_->pPrev->item;
    return SUCC;
}

int32_t DListGetAt(DLinkedList *pObj, Item *pItem, int32_t iIdx)
{
    DListData *pData = pObj->pData;
    if (!pData->pHead_) {
        *pItem = NULL;
        return ERR_IDX;
    }

    /* Forward indexing to the target node. */
    if (iIdx >= 0) {
        if (iIdx >= pData->iSize_)
            return ERR_IDX;
        int i;
        DListNode *pTrack = pData->pHead_;
        for (i = 0 ; i < iIdx ; i++)
            pTrack = pTrack->pNext;
        *pItem = pTrack->item;
    }
    /* Backward indexing to the target node. */
    else {
        if (-iIdx > pData->iSize_)
            return ERR_IDX;
        int i;
        DListNode *pTrack = pData->pHead_;
        for (i = 0 ; i < -iIdx ; i++)
            pTrack = pTrack->pPrev;
        *pItem = pTrack->item;
    }

    return SUCC;
}

int32_t DListResize(DLinkedList *pObj, int32_t iSize) {return 0;}

int32_t DListSize(DLinkedList *pObj) {return 0;}

int32_t DListReverse(DLinkedList *pObj) {return 0;}

void DListSetDestroy(DLinkedList *pObj, void (*pFunc) (Item)) {}


/*===========================================================================*
 *               Implementation for internal operations                      *
 *===========================================================================*/
void _DListDeinit(DListData *pData)
{
    DListNode *pCurr = pData->pHead_;
    do {
        DListNode *pPred = pCurr;
        pCurr = pCurr->pNext;
        free(pPred);
    } while (pCurr != pData->pHead_);
    return;
}

void _DListItemDestroy(Item item) {}