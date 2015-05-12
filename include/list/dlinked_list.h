/**
 * @file dlinked_list The doubly linked list.
 */

#ifndef _DLINKED_LIST_H_
#define _DLINKED_LIST_H_

#include "../util.h"

/** DListData is the data type for the container private information. */
typedef struct _DListData DListData;

/** The implementation for doubly linked list. */
typedef struct _DLinkedList {
    /** The container private information */
    DListData *pData;

    /** Push an item into the front-end of the list.
        @see DListPushFront */
    int32_t (*push_front) (struct _DLinkedList*, Item);

    /** Push an item into the back-end of the list.
        @see DListPushBack */
    int32_t (*push_back) (struct _DLinkedList*, Item);

    /** Insert an item into the designated index of the list.
        @see DListInsert */
    int32_t (*insert) (struct _DLinkedList*, Item, int32_t);

    int32_t (*pop_front) (struct _DLinkedList*);

    int32_t (*pop_back) (struct _DLinkedList*);

    int32_t (*delete) (struct _DLinkedList*, int32_t);

    int32_t (*set_front) (struct _DLinkedList*, Item);

    int32_t (*set_back) (struct _DLinkedList*, Item);

    int32_t (*set_at) (struct _DLinkedList*, Item, int32_t);

    /** Get the item from the front-end of the list.
        @see DListGetFront */
    int32_t (*get_front) (struct _DLinkedList*, Item*);

    /** Get the item from the back-end of the list.
        @see DListGetBack */
    int32_t (*get_back) (struct _DLinkedList*, Item*);

    /** Get an item from the designated index of the list.
        @see DListGetAt */
    int32_t (*get_at) (struct _DLinkedList*, Item*, int32_t);

    int32_t (*resize) (struct _DLinkedList*, int32_t);

    int32_t (*size) (struct _DLinkedList*);

    int32_t (*reverse) (struct _DLinkedList*);

    void (*set_destroy) (struct _DLinkedList*, void (*) (Item));
} DLinkedList;


/*===========================================================================*
 *             Definition for the exported member operations                 *
 *===========================================================================*/
/**
 * @brief The constructor for DLinkedList.
 *
 * @param ppObj         The double pointer to the to be constructed list
 *
 * @retval SUCC
 * @retval ERR_NOMEM    Insufficient memory for list construction
 */
int32_t DListInit(DLinkedList **ppObj);

/**
 * @brief The destructor for DLinkedList.
 *
 * @param ppObj         The double pointer to the to be destructed list
 */
void DListDeinit(DLinkedList **ppObj);

/**
 * @brief Push an item into the front-end of the list.
 *
 * @param pObj          The pointer to the DLinkedList structure
 * @param item          The designated item
 *
 * @retval SUCC
 * @retval ERR_NOMEM    Insufficient memory space for list extension
 */
int32_t DListPushFront(DLinkedList *pObj, Item item);

/**
 * @brief Push an item into the back-end of the list.
 *
 * @param pObj          The pointer to the DLinkedList structure
 * @param item          The designated item
 *
 * @retval SUCC
 * @retval ERR_NOMEM    Insufficient memory space for list extension
 */
int32_t DListPushBack(DLinkedList *pObj, Item item);

/**
 * @brief Insert an item into the designated index of the list.
 *
 * This function inserts the item into the designated index of the list and shifts
 * the trailing items one position to the back-end. The index can be positive or
 * negative, but its absolute value should be smaller than or equal to the list
 * size.
 * The operation supports both forward and backward indexing. For the former one,
 * the list is traversed from the front-end to the back-end. And for later one,
 * traversal direction is reversed.
 * To illustrate the behavior, let N denote the list size. Thus for forward indexing
 * , index 0 and index N-1 point to the head and tail. And for negative indexing,
 * index -1 and index -N point to the list tail and head.
 * But no matter which indexing method is applied, the list always grows from the
 * front-end to the back-end.
 *
 * @param pObj          The pointer to the DLinkedList structure
 * @param item          The designated item
 * @param iIdx          The designated index
 *
 * @retval SUCC
 * @retval ERR_NOMEM    Insufficient memory space for list extension
 * @retval ERR_IDX      Out of range indexing
 *
 * @note The absolute value of the index should be smaller than or equal to the
 * list size.
 */
int32_t DListInsert(DLinkedList *pObj, Item item, int32_t iIdx);

int32_t DListPopFront(DLinkedList *pObj);

int32_t DListPopBack(DLinkedList *pObj);

int32_t DListDelete(DLinkedList *pObj, int32_t iIdx);

int32_t DListSetFront(DLinkedList *pObj, Item item);

int32_t DListSetBack(DLinkedList *pObj, Item item);

int32_t DListSetAt(DLinkedList *pObj, Item item, int32_t iIdx);

/**
 * @brief Get the item from the front-end of the list.
 *
 * @param pObj          The pointer to the DLinkedList structure
 * @param pItem         The pointer to the returned item
 *
 * @retval SUCC
 * @retval ERR_IDX      Empty list
 *
 * @note If the exception occurs, the data pointed by pItem will also be updated
 * with NULL.
 */
int32_t DListGetFront(DLinkedList *pObj, Item *pItem);

/**
 * @brief Get the item from the back-end of the list.
 *
 * @param pObj          The pointer to the DLinkedList structure
 * @param pItem         The pointer to the returned item
 *
 * @retval SUCC
 * @retval ERR_IDX      Empty list
 *
 * @note If the exception occurs, the data pointed by pItem will also be updated
 * with NULL.
 */
int32_t DListGetBack(DLinkedList *pObj, Item *pItem);

/**
 * @brief Get an item from the designated index of the list.
 *
 * As insertion operation, both forward and backward indexing are accepted. Let
 * N denote the list size and i denote the index.
 * For forward indexing, inequality 0 <= i < N must be fitted.
 * For backward indexing, inequality 0 < i <= -N must be fitted.
 *
 * @param pObj          The pointer to the DLinkedList structure
 * @param pItem         The pointer to the returned item
 *
 * @retval SUCC
 * @retval ERR_IDX      Out of range index
 *
 * @note If the exception occurs, the data pointed by pItem will also be updated
 * with NULL.
 */
int32_t DListGetAt(DLinkedList *pObj, Item *pItem, int32_t iIdx);

int32_t DListResize(DLinkedList *pObj, int32_t iSize);

int32_t DListSize(DLinkedList *pObj);

int32_t DListReverse(DLinkedList *pObj);

void DListSetDestroy(DLinkedList *pObj, void (*pFunc) (Item));

#endif