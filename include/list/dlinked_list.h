/**
 * @file dlinked_list.h The doubly linked list.
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

    /** Push an item to the head of the list.
        @see DListPushFront */
    int32_t (*push_front) (struct _DLinkedList*, Item);

    /** Push an item to the tail of the list.
        @see DListPushBack */
    int32_t (*push_back) (struct _DLinkedList*, Item);

    /** Insert an item to the designated index of the list.
        @see DListInsert */
    int32_t (*insert) (struct _DLinkedList*, Item, int32_t);

    /** Pop an item from the head of the list.
        @see DListPopFront */
    int32_t (*pop_front) (struct _DLinkedList*);

    /** Pop an item from the tail of the list.
        @see DListPopBack */
    int32_t (*pop_back) (struct _DLinkedList*);

    /** Delete an item from the designated index of the list
        @see DListDelete */
    int32_t (*delete) (struct _DLinkedList*, int32_t);

    /** Set an item at the head of the list.
        @see DListSetFront */
    int32_t (*set_front) (struct _DLinkedList*, Item);

    /** Set an item at the tail of the list.
        @see DListSetBack */
    int32_t (*set_back) (struct _DLinkedList*, Item);

    /** Set an item at the designated index of the list.
        @see DListSetAt */
    int32_t (*set_at) (struct _DLinkedList*, Item, int32_t);

    /** Get an item from the head of the list.
        @see DListGetFront */
    int32_t (*get_front) (struct _DLinkedList*, Item*);

    /** Get an item from the tail of the list.
        @see DListGetBack */
    int32_t (*get_back) (struct _DLinkedList*, Item*);

    /** Get an item from the designated index of the list.
        @see DListGetAt */
    int32_t (*get_at) (struct _DLinkedList*, Item*, int32_t);

    /** Return the number of stored items.
        @see DListSize */
    int32_t (*size) (struct _DLinkedList*);

    /** Reverse the list.
        @see DListReverse */
    int32_t (*reverse) (struct _DLinkedList*);

    /** Set the custom item resource clean method.
        @see DListSetDestroy */
    int32_t (*set_destroy) (struct _DLinkedList*, void (*) (Item));
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
 * If the custom resource clean method is set, it also runs the resource clean
 * method for all the items.
 *
 * @param ppObj         The double pointer to the to be destructed list
 */
void DListDeinit(DLinkedList **ppObj);

/**
 * @brief Push an item to the head of the list.
 *
 * @param self          The pointer to the DLinkedList structure
 * @param item          The designated item
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_NOMEM    Insufficient memory for list extension
 */
int32_t DListPushFront(DLinkedList *self, Item item);

/**
 * @brief Push an item to the tail of the list.
 *
 * @param self          The pointer to the DLinkedList structure
 * @param item          The designated item
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_NOMEM    Insufficient memory for list extension
 */
int32_t DListPushBack(DLinkedList *self, Item item);

/**
 * @brief Insert an item to the designated index of the list.
 *
 * This function inserts an item to the designated index of the list and shifts
 * the trailing items one position to the tail. The index can be positive or
 * negative, but its absolute value should be smaller than or equal to the list
 * size.
 * The operation supports both forward and backward indexing. For the former one,
 * the list is traversed from the head to the tail. And for the later one, the
 * direction is reversed.
 * To illustrate the behavior, let N denote the list size. Thus for forward indexing
 * , index (0) and index (N-1) point to the head and tail. And for backward indexing,
 * index (-1) and index (-N) point to the list tail and head.
 * But no matter which indexing method is applied, the list always grows from the
 * head to the tail.
 *
 * @param self          The pointer to the DLinkedList structure
 * @param item          The designated item
 * @param iIdx          The designated index
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_NOMEM    Insufficient memory for list extension
 * @retval ERR_IDX      Out of range indexing
 *
 * @note The absolute value of the index should be smaller than or equal to the
 * list size.
 */
int32_t DListInsert(DLinkedList *self, Item item, int32_t iIdx);

/**
 * @brief Pop an item from the head of the list.
 *
 * This function removes an item from the head of the list. If the custom
 * resource clean method is set, it also runs the resource clean method for the
 * removed item.
 *
 * @param self          The pointer to the DLinkedList structure
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_IDX      Empty list
 */
int32_t DListPopFront(DLinkedList *self);

/**
 * @brief Pop an item from the tail of the list.
 *
 * This function removes an item from the tail of the list. If the custom
 * resource clean method is set, it also runs the resource clean method for the
 * removed item.
 *
 * @param self          The pointer to the DLinkedList structure
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_IDX      Empty list
 */
int32_t DListPopBack(DLinkedList *self);

/**
 * @brief Remove an item from the designated index of the list.
 *
 * This function removes an item from the designated index of the list and shifts
 * the trailing items one position to the head. If the custom resource clean
 * method is set, it also runs the resource clean method for the removed item.
 * The operation supports both forward and backward indexing. Let N denote the
 * list size and i denote the index.
 * For forward indexing, inequality 0 <= i < N must be fitted.
 * For backward indexing, inequality 0 < i <= -N must be fitted.
 *
 * @param self          The pointer to the DLinkedList structure
 * @param iIdx          The designated index
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_IDX      Out of range indexing
 */
int32_t DListDelete(DLinkedList *self, int32_t iIdx);

/**
 * @brief Set an item at the head of the list.
 *
 * This function replaces the head item with the designated one. If the custom
 * resource clean method is set, it also runs the resource clean method for the
 * replaced item.
 *
 * @param self          The pointer to the DLinkedList structure
 * @param item          The designated item
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_IDX      Empty list
 */
int32_t DListSetFront(DLinkedList *self, Item item);

/**
 * @brief Set an item at the tail of the list.
 *
 * This function replaces the tail item with the designated one. If the custom
 * resource clean method is set, it also runs the resource clean method for the
 * replaced item.
 *
 * @param self          The pointer to the DLinkedList structure
 * @param item          The designated item
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_IDX      Empty list
 */
int32_t DListSetBack(DLinkedList *self, Item item);

/**
 * @brief Replace an item at the designated index of the list.
 *
 * This function replaces the item at the designated index with the designated
 * one. If the custom resource clean method is set, it also runs the resource
 * clean method for the replaced item.
 * The operation supports both forward and backward indexing. Let N denote the
 * list size and i denote the index.
 * For forward indexing, inequality 0 <= i < N must be fitted.
 * For backward indexing, inequality 0 < i <= -N must be fitted.
 *
 * @param self          The pointer to the DLinkedList structure
 * @param item          The designated item
 * @param iIdx          The designated index
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_IDX      Out of range index
 */
int32_t DListSetAt(DLinkedList *self, Item item, int32_t iIdx);

/**
 * @brief Get an item from the head of the list.
 *
 * @param self          The pointer to the DLinkedList structure
 * @param pItem         The pointer to the returned item
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_IDX      Empty list
 * @retval ERR_GET      Invalid parameter to store returned item
 *
 * @note If the exception occurs, the second parameter will be updated with NULL.
 */
int32_t DListGetFront(DLinkedList *self, Item *pItem);

/**
 * @brief Get an item from the tail of the list.
 *
 * @param self          The pointer to the DLinkedList structure
 * @param pItem         The pointer to the returned item
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_IDX      Empty list
 * @retval ERR_GET      Invalid parameter to store returned item
 *
 * @note If the exception occurs, the second parameter will be updated with NULL.
 */
int32_t DListGetBack(DLinkedList *self, Item *pItem);

/**
 * @brief Get an item from the designated index of the list.
 *
 * The operation supports both forward and backward indexing. Let N denote the
 * list size and i denote the index.
 * For forward indexing, inequality 0 <= i < N must be fitted.
 * For backward indexing, inequality 0 < i <= -N must be fitted.
 *
 * @param self          The pointer to the DLinkedList structure
 * @param pItem         The pointer to the returned item
 * @param iIdx          The designated index
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_IDX      Out of range index
 * @retval ERR_GET      Invalid parameter to store returned item
 *
 * @note If the exception occurs, the second parameter will be updated with NULL.
 */
int32_t DListGetAt(DLinkedList *self, Item *pItem, int32_t iIdx);

/**
 * @brief Return the number of stored items.
 *
 * @param self          The pointer to the DLinkedList structure
 *
 * @return              The number of stored item
 * @retval ERR_NOINIT   Uninitialized container
 */
int32_t DListSize(DLinkedList *self);

/**
 * @brief Reverse the list.
 *
 * @param self          The pointer to the DLinkedList structure
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 */
int32_t DListReverse(DLinkedList *self);

/**
 * @brief Set the custom item resource clean method.
 *
 * @param self          The pointer to the DLinkedList structure
 * @param pFunc         The function pointer to the custom method
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 */
int32_t DListSetDestroy(DLinkedList *self, void (*pFunc) (Item));

#endif
