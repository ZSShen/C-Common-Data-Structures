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
 void _DListDeinit(DLinkedList *pData);


/**
 * @brief The default clean strategy for the resource hold by an item.
 *
 * @param item          The requested item
 */
void _DListItemDestroy(Item item);


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

int32_t DListDeinit(DLinkedList **ppObj) {return 0;}

int32_t DListPushFront(DLinkedList *pObj, Item item) {return 0;}

int32_t DListPushBack(DLinkedList *pObj, Item item) {return 0;}

int32_t DListInsert(DLinkedList *pObj, Item item, int32_t iIdx) {return 0;}

int32_t DListPopFront(DLinkedList *pObj) {return 0;}

int32_t DListPopBack(DLinkedList *pObj) {return 0;}

int32_t DListDelete(DLinkedList *pObj, int32_t iIdx) {return 0;}

int32_t DListSetFront(DLinkedList *pObj, Item item) {return 0;}

int32_t DListSetBack(DLinkedList *pObj, Item item) {return 0;}

int32_t DListSetAt(DLinkedList *pObj, Item item, int32_t iIdx) {return 0;}

int32_t DListGetFront(DLinkedList *pObj, Item *pItem) {return 0;}

int32_t DListGetBack(DLinkedList *pObj, Item *pItem) {return 0;}

int32_t DListGetAt(DLinkedList *pObj, Item *pItem, int32_t iIdx) {return 0;}

int32_t DListResize(DLinkedList *pObj, int32_t iSize) {return 0;}

int32_t DListSize(DLinkedList *pObj) {return 0;}

int32_t DListReverse(DLinkedList *pObj) {return 0;}

void DListSetDestroy(DLinkedList *pObj, void (*pFunc) (Item)) {}


/*===========================================================================*
 *               Implementation for internal operations                      *
 *===========================================================================*/
 void _DListDeinit(DLinkedList *pData)
 {

 }

void _DListItemDestroy(Item item) {}