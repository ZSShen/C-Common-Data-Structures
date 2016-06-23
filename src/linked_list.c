#include "container/linked_list.h"


/*===========================================================================*
 *                        The container private data                         *
 *===========================================================================*/
typedef struct _ListNode {
   Item item;
   struct _ListNode *pPrev;
   struct _ListNode *pNext;
} ListNode;

struct _LinkedListData {
    int32_t iSize_;
    ListNode *pHead_;
    ListNode *pIter_, *pPred_;
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
 void _LinkedListDeinit(LinkedListData *pData);

/**
 * @brief The default item resource clean method.
 *
 * @param item          The designated item
 */
void _LinkedListItemDestroy(Item item);


#define CHECK_INIT(self)                                                        \
            do {                                                                \
                if (!self)                                                      \
                    return ERR_NOINIT;                                          \
                if (!(self->pData))                                             \
                    return ERR_NOINIT;                                          \
            } while (0);

#define NEW_NODE(pNew, item)                                                    \
            do {                                                                \
                pNew = (ListNode*)malloc(sizeof(ListNode));                     \
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
 *               Implementation for the exported operations                  *
 *===========================================================================*/
int32_t LinkedListInit(LinkedList **ppObj)
{
    *ppObj = (LinkedList*)malloc(sizeof(LinkedList));
    if (!(*ppObj))
        return ERR_NOMEM;
    LinkedList *pObj = *ppObj;

    pObj->pData = (LinkedListData*)malloc(sizeof(LinkedListData));
    if (!(pObj->pData)) {
        free(*ppObj);
        *ppObj = NULL;
        return ERR_NOMEM;
    }
    LinkedListData *pData = pObj->pData;

    pData->iSize_ = 0;
    pData->pHead_ = NULL;
    pData->pIter_ = NULL;
    pData->pPred_ = NULL;
    pData->pDestroy_ = NULL;

    pObj->push_front = LinkedListPushFront;
    pObj->push_back = LinkedListPushBack;
    pObj->insert = LinkedListInsert;

    pObj->pop_front = LinkedListPopFront;
    pObj->pop_back = LinkedListPopBack;
    pObj->remove = LinkedListRemove;

    pObj->set_front = LinkedListSetFront;
    pObj->set_back = LinkedListSetBack;
    pObj->set_at = LinkedListSetAt;

    pObj->get_front = LinkedListGetFront;
    pObj->get_back = LinkedListGetBack;
    pObj->get_at = LinkedListGetAt;

    pObj->size = LinkedListSize;
    pObj->reverse = LinkedListReverse;

    pObj->iterate = LinkedListIterate;
    pObj->reverse_iterate = LinkedListReverseIterate;
    pObj->replace = LinkedListReplace;

    pObj->set_destroy = LinkedListSetDestroy;

    return SUCC;
}

void LinkedListDeinit(LinkedList **ppObj)
{
    if (!(*ppObj))
        goto EXIT;

    LinkedListData *pData = (*ppObj)->pData;
    if (!pData)
        goto FREE_LIST;

    _LinkedListDeinit(pData);
    free(pData);

FREE_LIST:
    free(*ppObj);
    *ppObj = NULL;
EXIT:
    return;
}

int32_t LinkedListPushFront(LinkedList *self, Item item)
{
    CHECK_INIT(self);

    ListNode *pNew;
    NEW_NODE(pNew, item);

    LinkedListData *pData = self->pData;
    if (!pData->pHead_)
        pNew->pPrev = pNew->pNext = pNew;
    else
        PUSH_NODE(pNew, pData->pHead_);

    pData->pHead_ = pNew;
    pData->iSize_++;

    return SUCC;
}

int32_t LinkedListPushBack(LinkedList *self, Item item)
{
    CHECK_INIT(self);

    ListNode *pNew;
    NEW_NODE(pNew, item);

    LinkedListData *pData = self->pData;
    if (!pData->pHead_) {
        pNew->pPrev = pNew->pNext = pNew;
        pData->pHead_ = pNew;
    } else
        PUSH_NODE(pNew, pData->pHead_);

    pData->iSize_++;

    return SUCC;
}

int32_t LinkedListInsert(LinkedList *self, Item item, int32_t iIdx)
{
    CHECK_INIT(self);

    LinkedListData *pData = self->pData;
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

int32_t LinkedListPopFront(LinkedList *self)
{
    CHECK_INIT(self);

    LinkedListData *pData = self->pData;
    if (!pData->pHead_)
        return ERR_IDX;

    ListNode *pHead = pData->pHead_;
    if (pHead == pHead->pPrev) {
        if (pData->pDestroy_)
            pData->pDestroy_(pHead->item);
        free(pHead);
        pData->pHead_ = NULL;
    } else {
        pHead->pPrev->pNext = pHead->pNext;
        pHead->pNext->pPrev = pHead->pPrev;
        pData->pHead_ = pHead->pNext;
        if (pData->pDestroy_)
            pData->pDestroy_(pHead->item);
        free(pHead);
    }

    pData->iSize_--;

    return SUCC;
}

int32_t LinkedListPopBack(LinkedList *self)
{
    CHECK_INIT(self);

    LinkedListData *pData = self->pData;
    if (!pData->pHead_)
        return ERR_IDX;

    ListNode *pHead = pData->pHead_;
    if (pHead == pHead->pNext) {
        if (pData->pDestroy_)
            pData->pDestroy_(pHead->item);
        free(pHead);
        pData->pHead_ = NULL;
    } else {
        ListNode *pTail = pHead->pPrev;
        pTail->pPrev->pNext = pHead;
        pHead->pPrev = pTail->pPrev;
        if (pData->pDestroy_)
            pData->pDestroy_(pTail->item);
        free(pTail);
    }

    pData->iSize_--;

    return SUCC;
}

int32_t LinkedListRemove(LinkedList *self, int32_t iIdx)
{
    CHECK_INIT(self);

    LinkedListData *pData = self->pData;
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

    if (pData->pDestroy_)
        pData->pDestroy_(pTrack->item);
    free(pTrack);

    pData->iSize_--;

    return SUCC;
}

int32_t LinkedListSetFront(LinkedList *self, Item item)
{
    CHECK_INIT(self);

    LinkedListData *pData = self->pData;
    if (!pData->pHead_)
        return ERR_IDX;

    if (pData->pDestroy_)
        pData->pDestroy_(pData->pHead_->item);
    pData->pHead_->item = item;

    return SUCC;
}

int32_t LinkedListSetBack(LinkedList *self, Item item)
{
    CHECK_INIT(self);

    LinkedListData *pData = self->pData;
    if (!pData->pHead_)
        return ERR_IDX;

    ListNode *pTail = pData->pHead_->pPrev;
    if (pData->pDestroy_)
        pData->pDestroy_(pTail->item);
    pTail->item = item;

    return SUCC;
}

int32_t LinkedListSetAt(LinkedList *self, Item item, int32_t iIdx)
{
    CHECK_INIT(self);

    /* Replace item at the designated index with forward indexing. */
    LinkedListData *pData = self->pData;
    if (iIdx >= 0) {
        if (iIdx >= pData->iSize_)
            return ERR_IDX;

        ListNode *pTrack = pData->pHead_;
        while (iIdx > 0) {
            pTrack = pTrack->pNext;
            iIdx--;
        }
        if (pData->pDestroy_)
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
        if (pData->pDestroy_)
            pData->pDestroy_(pTrack->item);
        pTrack->item = item;
    }

    return SUCC;
}

int32_t LinkedListGetFront(LinkedList *self, Item *pItem)
{
    CHECK_INIT(self);
    if (!pItem)
        return ERR_GET;

    LinkedListData *pData = self->pData;
    if (!pData->pHead_) {
        *pItem = NULL;
        return ERR_IDX;
    }

    *pItem = pData->pHead_->item;
    return SUCC;
}

int32_t LinkedListGetBack(LinkedList *self, Item *pItem)
{
    CHECK_INIT(self);
    if (!pItem)
        return ERR_GET;

    LinkedListData *pData = self->pData;
    if (!pData->pHead_) {
        *pItem = NULL;
        return ERR_IDX;
    }

    *pItem = pData->pHead_->pPrev->item;
    return SUCC;
}

int32_t LinkedListGetAt(LinkedList *self, Item *pItem, int32_t iIdx)
{
    CHECK_INIT(self);
    if (!pItem)
        return ERR_GET;

    /* Forward indexing to the target node. */
    LinkedListData *pData = self->pData;
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

int32_t LinkedListSize(LinkedList *self)
{
    CHECK_INIT(self);
    return self->pData->iSize_;
}

int32_t LinkedListReverse(LinkedList *self)
{
    CHECK_INIT(self);

    LinkedListData *pData = self->pData;
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

int32_t LinkedListIterate(LinkedList *self, bool bReset, Item *pItem)
{
    CHECK_INIT(self);
    LinkedListData *pData = self->pData;

    if (bReset) {
        pData->pIter_ = pData->pHead_;
        pData->pPred_ = NULL;
        return SUCC;
    }

    if (!pItem)
        return ERR_GET;

    if (!pData->pIter_) {
        *pItem = NULL;
        return END;
    }

    ListNode *pCurr = pData->pIter_;
    *pItem = pCurr->item;
    pData->pPred_ = pCurr;
    pData->pIter_ = pCurr->pNext;
    if (pData->pIter_ == pData->pHead_)
        pData->pIter_ = NULL;

    return SUCC;
}

int32_t LinkedListReverseIterate(LinkedList *self, bool bReset, Item *pItem)
{
    CHECK_INIT(self);
    LinkedListData *pData = self->pData;

    if (bReset) {
        pData->pIter_ = (pData->pHead_)? pData->pHead_->pPrev : NULL;
        pData->pPred_ = NULL;
        return SUCC;
    }

    if (!pItem)
        return ERR_GET;

    if (!pData->pIter_) {
        *pItem = NULL;
        return END;
    }

    ListNode *pCurr = pData->pIter_;
    *pItem = pCurr->item;
    pData->pPred_ = pCurr;
    pData->pIter_ = pCurr->pPrev;
    if (pData->pIter_ == pData->pHead_->pPrev)
        pData->pIter_ = NULL;

    return SUCC;
}

int32_t LinkedListReplace(LinkedList *self, Item item)
{
    CHECK_INIT(self);
    LinkedListData *pData = self->pData;

    if (!pData->pPred_)
        return ERR_IDX;

    if (pData->pDestroy_) {
        pData->pDestroy_(pData->pPred_->item);
        pData->pPred_->item = item;
    }

    return SUCC;
}

int32_t LinkedListSetDestroy(LinkedList *self, void (*pFunc) (Item))
{
    CHECK_INIT(self);
    self->pData->pDestroy_ = pFunc;
    return SUCC;
}


/*===========================================================================*
 *               Implementation for internal operations                      *
 *===========================================================================*/
void _LinkedListDeinit(LinkedListData *pData)
{
    ListNode *pCurr = pData->pHead_;
    if (pCurr) {
        do {
            ListNode *pPred = pCurr;
            pCurr = pCurr->pNext;
            if (pData->pDestroy_)
                pData->pDestroy_(pPred->item);
            free(pPred);
        } while (pCurr != pData->pHead_);
    }
    return;
}
