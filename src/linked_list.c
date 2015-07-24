#include "list/linked_list.h"


/*===========================================================================*
 *                  Hide the private data of the tree                        *
 *===========================================================================*/
typedef struct _ListNode {
   Item item;
   struct _ListNode *pPrev;
   struct _ListNode *pNext;
} ListNode;

struct _ListData {
    bool bUserDestroy_;
    int32_t iSize_;
    ListNode *pHead_;
    void (*pDestroy_) (Item);
};


/*===========================================================================*
 *                  Definition for internal operations                       *
 *===========================================================================*/
/**
 * @brief Traverse all the nodes and clean the allocated resource.
 *
 * If the custom resource clean method is set, it also runs the resource clean
 * method for all the items.
 *
 * @param pData         The pointer to the list private data
 */
 void _ListDeinit(ListData *pData);

/**
 * @brief The default item resource clean method.
 *
 * @param item          The designated item
 */
void _ListItemDestroy(Item item);


#define CHECK_INIT(self)                                                        \
            do {                                                                \
                if (!self)                                                      \
                    return ERR_NOINIT;                                          \
                if (!(self->pData))                                             \
                    return ERR_NOINIT;                                          \
            } while (0);

#define NEW_NODE(pNew, item)                                                    \
            do {                                                                \
                pNew = (ListNode*)malloc(sizeof(ListNode));                   \
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

#define LINK_NEIGHBOR(pTrack)                                                   \
            do {                                                                \
                pPred = pTrack->pPrev;                                          \
                pSucc = pTrack->pNext;                                          \
                pPred->pNext = pSucc;                                           \
                pSucc->pPrev = pPred;                                           \
            } while (0);


/*===========================================================================*
 *         Implementation for the container supporting operations            *
 *===========================================================================*/
int32_t ListInit(LinkedList **ppObj)
{
    *ppObj = (LinkedList*)malloc(sizeof(LinkedList));
    if (!(*ppObj))
        return ERR_NOMEM;
    LinkedList *pObj = *ppObj;

    pObj->pData = (ListData*)malloc(sizeof(ListData));
    if (!(pObj->pData)) {
        free(*ppObj);
        *ppObj = NULL;
        return ERR_NOMEM;
    }
    ListData *pData = pObj->pData;

    pData->bUserDestroy_ = false;
    pData->iSize_ = 0;
    pData->pHead_ = NULL;
    pData->pDestroy_ = _ListItemDestroy;

    pObj->push_front = ListPushFront;
    pObj->push_back = ListPushBack;
    pObj->insert = ListInsert;

    pObj->pop_front = ListPopFront;
    pObj->pop_back = ListPopBack;
    pObj->delete = ListDelete;

    pObj->set_front = ListSetFront;
    pObj->set_back = ListSetBack;
    pObj->set_at = ListSetAt;

    pObj->get_front = ListGetFront;
    pObj->get_back = ListGetBack;
    pObj->get_at = ListGetAt;

    pObj->size = ListSize;
    pObj->reverse = ListReverse;
    pObj->set_destroy = ListSetDestroy;

    return SUCC;
}

void ListDeinit(LinkedList **ppObj)
{
    if (!(*ppObj))
        goto EXIT;

    ListData *pData = (*ppObj)->pData;
    if (!pData)
        goto FREE_LIST;

    _ListDeinit(pData);
    free(pData);

FREE_LIST:
    free(*ppObj);
    *ppObj = NULL;
EXIT:
    return;
}

int32_t ListPushFront(LinkedList *self, Item item)
{
    CHECK_INIT(self);

    ListNode *pNew;
    NEW_NODE(pNew, item);

    ListData *pData = self->pData;
    if (!pData->pHead_)
        pNew->pPrev = pNew->pNext = pNew;
    else
        PUSH_NODE(pNew, pData->pHead_);

    pData->pHead_ = pNew;
    pData->iSize_++;

    return SUCC;
}

int32_t ListPushBack(LinkedList *self, Item item)
{
    CHECK_INIT(self);

    ListNode *pNew;
    NEW_NODE(pNew, item);

    ListData *pData = self->pData;
    if (!pData->pHead_) {
        pNew->pPrev = pNew->pNext = pNew;
        pData->pHead_ = pNew;
    } else
        PUSH_NODE(pNew, pData->pHead_);

    pData->iSize_++;

    return SUCC;
}

int32_t ListInsert(LinkedList *self, Item item, int32_t iIdx)
{
    CHECK_INIT(self);

    ListData *pData = self->pData;
    if (abs(iIdx) > pData->iSize_)
        return ERR_IDX;

    ListNode *pNew;
    NEW_NODE(pNew, item);
    if (!pData->pHead_) {
        pNew->pPrev = pNew->pNext = pNew;
        pData->pHead_ = pNew;
    } else {
        int32_t i;
        ListNode *pTrack = pData->pHead_;

        /* Insert node to the proper position with forward indexing. */
        if (iIdx >= 0) {
            for (i = iIdx ; i > 0 ; i--)
                pTrack = pTrack->pNext;
            INSERT_NODE(pNew, pTrack);
            if (iIdx == 0)
                pData->pHead_ = pNew;
        }
        /* Insert node to the proper position with backward indexing. */
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

int32_t ListPopFront(LinkedList *self)
{
    CHECK_INIT(self);

    ListData *pData = self->pData;
    if (!pData->pHead_)
        return ERR_IDX;

    ListNode *pHead = pData->pHead_;
    if (pHead == pHead->pPrev) {
        if (pData->bUserDestroy_)
            pData->pDestroy_(pHead->item);
        free(pHead);
        pData->pHead_ = NULL;
    } else {
        pHead->pPrev->pNext = pHead->pNext;
        pHead->pNext->pPrev = pHead->pPrev;
        pData->pHead_ = pHead->pNext;
        if (pData->bUserDestroy_)
            pData->pDestroy_(pHead->item);
        free(pHead);
    }

    pData->iSize_--;

    return SUCC;
}

int32_t ListPopBack(LinkedList *self)
{
    CHECK_INIT(self);

    ListData *pData = self->pData;
    if (!pData->pHead_)
        return ERR_IDX;

    ListNode *pHead = pData->pHead_;
    if (pHead == pHead->pNext) {
        if (pData->bUserDestroy_)
            pData->pDestroy_(pHead->item);
        free(pHead);
        pData->pHead_ = NULL;
    } else {
        ListNode *pTail = pHead->pPrev;
        pTail->pPrev->pNext = pHead;
        pHead->pPrev = pTail->pPrev;
        if (pData->bUserDestroy_)
            pData->pDestroy_(pTail->item);
        free(pTail);
    }

    pData->iSize_--;

    return SUCC;
}

int32_t ListDelete(LinkedList *self, int32_t iIdx)
{
    CHECK_INIT(self);

    ListData *pData = self->pData;
    ListNode *pTrack, *pPred, *pSucc;

    /* Delete node from the designated index with forward indexing. */
    if (iIdx >= 0) {
        if (iIdx >= pData->iSize_)
            return ERR_IDX;
        pTrack = pData->pHead_;
        while (iIdx > 0) {
            pTrack = pTrack->pNext;
            iIdx--;
        }
        LINK_NEIGHBOR(pTrack);
    }
    /* Delete node from the designated index with backward indexing. */
    else {
        if (-iIdx > pData->iSize_)
            return ERR_IDX;
        pTrack = pData->pHead_;
        while (iIdx < 0) {
            pTrack = pTrack->pPrev;
            iIdx++;
        }
        LINK_NEIGHBOR(pTrack);
    }

    if (pData->iSize_ == 1)
        pData->pHead_ = NULL;
    else {
        if (pTrack == pData->pHead_)
            pData->pHead_ = pSucc;
    }

    if (pData->bUserDestroy_)
        pData->pDestroy_(pTrack->item);
    free(pTrack);

    pData->iSize_--;

    return SUCC;
}

int32_t ListSetFront(LinkedList *self, Item item)
{
    CHECK_INIT(self);

    ListData *pData = self->pData;
    if (!pData->pHead_)
        return ERR_IDX;

    if (pData->bUserDestroy_)
        pData->pDestroy_(pData->pHead_->item);
    pData->pHead_->item = item;

    return SUCC;
}

int32_t ListSetBack(LinkedList *self, Item item)
{
    CHECK_INIT(self);

    ListData *pData = self->pData;
    if (!pData->pHead_)
        return ERR_IDX;

    ListNode *pTail = pData->pHead_->pPrev;
    if (pData->bUserDestroy_)
        pData->pDestroy_(pTail->item);
    pTail->item = item;

    return SUCC;
}

int32_t ListSetAt(LinkedList *self, Item item, int32_t iIdx)
{
    CHECK_INIT(self);

    /* Replace item at the designated index with forward indexing. */
    ListData *pData = self->pData;
    if (iIdx >= 0) {
        if (iIdx >= pData->iSize_)
            return ERR_IDX;

        ListNode *pTrack = pData->pHead_;
        while (iIdx > 0) {
            pTrack = pTrack->pNext;
            iIdx--;
        }
        if (pData->bUserDestroy_)
            pData->pDestroy_(pTrack->item);
        pTrack->item = item;
    }
    /* Replace item at the designated index with backward indexing. */
    if (iIdx < 0) {
        if (-iIdx > pData->iSize_)
            return ERR_IDX;

        ListNode *pTrack = pData->pHead_;
        while (iIdx < 0) {
            pTrack = pTrack->pPrev;
            iIdx++;
        }
        if (pData->bUserDestroy_)
            pData->pDestroy_(pTrack->item);
        pTrack->item = item;
    }

    return SUCC;
}

int32_t ListGetFront(LinkedList *self, Item *pItem)
{
    CHECK_INIT(self);
    if (!pItem)
        return ERR_GET;

    ListData *pData = self->pData;
    if (!pData->pHead_) {
        *pItem = NULL;
        return ERR_IDX;
    }

    *pItem = pData->pHead_->item;
    return SUCC;
}

int32_t ListGetBack(LinkedList *self, Item *pItem)
{
    CHECK_INIT(self);
    if (!pItem)
        return ERR_GET;

    ListData *pData = self->pData;
    if (!pData->pHead_) {
        *pItem = NULL;
        return ERR_IDX;
    }

    *pItem = pData->pHead_->pPrev->item;
    return SUCC;
}

int32_t ListGetAt(LinkedList *self, Item *pItem, int32_t iIdx)
{
    CHECK_INIT(self);
    if (!pItem)
        return ERR_GET;

    /* Forward indexing to the target node. */
    ListData *pData = self->pData;
    if (iIdx >= 0) {
        if (iIdx >= pData->iSize_) {
            *pItem = NULL;
            return ERR_IDX;
        }
        int i;
        ListNode *pTrack = pData->pHead_;
        for (i = 0 ; i < iIdx ; i++)
            pTrack = pTrack->pNext;
        *pItem = pTrack->item;
    }
    /* Backward indexing to the target node. */
    else {
        if (-iIdx > pData->iSize_) {
            *pItem = NULL;
            return ERR_IDX;
        }
        int i;
        ListNode *pTrack = pData->pHead_;
        for (i = 0 ; i < -iIdx ; i++)
            pTrack = pTrack->pPrev;
        *pItem = pTrack->item;
    }

    return SUCC;
}

int32_t ListSize(LinkedList *self)
{
    CHECK_INIT(self);
    return self->pData->iSize_;
}

int32_t ListReverse(LinkedList *self)
{
    CHECK_INIT(self);

    ListData *pData = self->pData;
    ListNode *pHead = pData->pHead_;
    ListNode *pTail = pData->pHead_->pPrev;

    int32_t iRge = pData->iSize_;
    while (iRge > 1) {
        Item itemHead = pHead->item;
        Item itemTail = pTail->item;
        pHead->item = itemTail;
        pTail->item = itemHead;
        pHead = pHead->pNext;
        pTail = pTail->pPrev;
        iRge -= 2;
    }

    return SUCC;
}

int32_t ListSetDestroy(LinkedList *self, void (*pFunc) (Item))
{
    CHECK_INIT(self);
    self->pData->bUserDestroy_ = true;
    self->pData->pDestroy_ = pFunc;
    return SUCC;
}


/*===========================================================================*
 *               Implementation for internal operations                      *
 *===========================================================================*/
void _ListDeinit(ListData *pData)
{
    ListNode *pCurr = pData->pHead_;
    if (pCurr) {
        do {
            ListNode *pPred = pCurr;
            pCurr = pCurr->pNext;
            if (pData->bUserDestroy_)
                pData->pDestroy_(pPred->item);
            free(pPred);
        } while (pCurr != pData->pHead_);
    }
    return;
}

void _ListItemDestroy(Item item) {}
