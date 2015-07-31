/**
 * @file linked_list.h The doubly linked list.
 */

#ifndef _LINKED_LIST_H_
#define _LINKED_LIST_H_

#include "../util.h"

/** ListData is the data type for the container private information. */
typedef struct _ListData ListData;

/** The implementation for doubly linked list. */
typedef struct _LinkedList {
    /** The container private information */
    ListData *pData;

    /** Push an item to the head of the list.
        @see ListPushFront */
    int32_t (*push_front) (struct _LinkedList*, Item);

    /** Push an item to the tail of the list.
        @see ListPushBack */
    int32_t (*push_back) (struct _LinkedList*, Item);

    /** Insert an item to the designated index of the list.
        @see ListInsert */
    int32_t (*insert) (struct _LinkedList*, Item, int32_t);

    /** Pop an item from the head of the list.
        @see ListPopFront */
    int32_t (*pop_front) (struct _LinkedList*);

    /** Pop an item from the tail of the list.
        @see ListPopBack */
    int32_t (*pop_back) (struct _LinkedList*);

    /** Delete an item from the designated index of the list
        @see ListDelete */
    int32_t (*delete) (struct _LinkedList*, int32_t);

    /** Set an item at the head of the list.
        @see ListSetFront */
    int32_t (*set_front) (struct _LinkedList*, Item);

    /** Set an item at the tail of the list.
        @see ListSetBack */
    int32_t (*set_back) (struct _LinkedList*, Item);

    /** Set an item at the designated index of the list.
        @see ListSetAt */
    int32_t (*set_at) (struct _LinkedList*, Item, int32_t);

    /** Get an item from the head of the list.
        @see ListGetFront */
    int32_t (*get_front) (struct _LinkedList*, Item*);

    /** Get an item from the tail of the list.
        @see ListGetBack */
    int32_t (*get_back) (struct _LinkedList*, Item*);

    /** Get an item from the designated index of the list.
        @see ListGetAt */
    int32_t (*get_at) (struct _LinkedList*, Item*, int32_t);

    /** Return the number of stored items.
        @see ListSize */
    int32_t (*size) (struct _LinkedList*);

    /** Reverse the list.
        @see ListReverse */
    int32_t (*reverse) (struct _LinkedList*);

    /** Iterate through the list till the tail end.
        @see ListIterate */
    int32_t (*iterate) (struct _LinkedList*, bool, Item*);

    /** Reversely iterate through the list till the head end.
        @see ListReverseIterate */
    int32_t (*reverse_iterate) (struct _LinkedList*, bool, Item*);

    /** Set the custom item resource clean method.
        @see ListSetDestroy */
    int32_t (*set_destroy) (struct _LinkedList*, void (*) (Item));
} LinkedList;


/*===========================================================================*
 *             Definition for the exported member operations                 *
 *===========================================================================*/
/**
 * @brief The constructor for LinkedList.
 *
 * @param ppObj         The double pointer to the to be constructed list
 *
 * @retval SUCC
 * @retval ERR_NOMEM    Insufficient memory for list construction
 */
int32_t ListInit(LinkedList **ppObj);

/**
 * @brief The destructor for LinkedList.
 *
 * If the custom resource clean method is set, it also runs the resource clean
 * method for all the items.
 *
 * @param ppObj         The double pointer to the to be destructed list
 */
void ListDeinit(LinkedList **ppObj);

/**
 * @brief Push an item to the head of the list.
 *
 * @param self          The pointer to the LinkedList structure
 * @param item          The designated item
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_NOMEM    Insufficient memory for list extension
 */
int32_t ListPushFront(LinkedList *self, Item item);

/**
 * @brief Push an item to the tail of the list.
 *
 * @param self          The pointer to the LinkedList structure
 * @param item          The designated item
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_NOMEM    Insufficient memory for list extension
 */
int32_t ListPushBack(LinkedList *self, Item item);

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
 * @param self          The pointer to the LinkedList structure
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
int32_t ListInsert(LinkedList *self, Item item, int32_t iIdx);

/**
 * @brief Pop an item from the head of the list.
 *
 * This function removes an item from the head of the list. If the custom
 * resource clean method is set, it also runs the resource clean method for the
 * removed item.
 *
 * @param self          The pointer to the LinkedList structure
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_IDX      Empty list
 */
int32_t ListPopFront(LinkedList *self);

/**
 * @brief Pop an item from the tail of the list.
 *
 * This function removes an item from the tail of the list. If the custom
 * resource clean method is set, it also runs the resource clean method for the
 * removed item.
 *
 * @param self          The pointer to the LinkedList structure
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_IDX      Empty list
 */
int32_t ListPopBack(LinkedList *self);

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
 * @param self          The pointer to the LinkedList structure
 * @param iIdx          The designated index
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_IDX      Out of range indexing
 */
int32_t ListDelete(LinkedList *self, int32_t iIdx);

/**
 * @brief Set an item at the head of the list.
 *
 * This function replaces the head item with the designated one. If the custom
 * resource clean method is set, it also runs the resource clean method for the
 * replaced item.
 *
 * @param self          The pointer to the LinkedList structure
 * @param item          The designated item
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_IDX      Empty list
 */
int32_t ListSetFront(LinkedList *self, Item item);

/**
 * @brief Set an item at the tail of the list.
 *
 * This function replaces the tail item with the designated one. If the custom
 * resource clean method is set, it also runs the resource clean method for the
 * replaced item.
 *
 * @param self          The pointer to the LinkedList structure
 * @param item          The designated item
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_IDX      Empty list
 */
int32_t ListSetBack(LinkedList *self, Item item);

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
 * @param self          The pointer to the LinkedList structure
 * @param item          The designated item
 * @param iIdx          The designated index
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_IDX      Out of range index
 */
int32_t ListSetAt(LinkedList *self, Item item, int32_t iIdx);

/**
 * @brief Get an item from the head of the list.
 *
 * @param self          The pointer to the LinkedList structure
 * @param pItem         The pointer to the returned item
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_IDX      Empty list
 * @retval ERR_GET      Invalid parameter to store returned item
 *
 * @note If the exception occurs, the second parameter will be updated with NULL.
 */
int32_t ListGetFront(LinkedList *self, Item *pItem);

/**
 * @brief Get an item from the tail of the list.
 *
 * @param self          The pointer to the LinkedList structure
 * @param pItem         The pointer to the returned item
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_IDX      Empty list
 * @retval ERR_GET      Invalid parameter to store returned item
 *
 * @note If the exception occurs, the second parameter will be updated with NULL.
 */
int32_t ListGetBack(LinkedList *self, Item *pItem);

/**
 * @brief Get an item from the designated index of the list.
 *
 * The operation supports both forward and backward indexing. Let N denote the
 * list size and i denote the index.
 * For forward indexing, inequality 0 <= i < N must be fitted.
 * For backward indexing, inequality 0 < i <= -N must be fitted.
 *
 * @param self          The pointer to the LinkedList structure
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
int32_t ListGetAt(LinkedList *self, Item *pItem, int32_t iIdx);

/**
 * @brief Return the number of stored items.
 *
 * @param self          The pointer to the LinkedList structure
 *
 * @return              The number of stored item
 * @retval ERR_NOINIT   Uninitialized container
 */
int32_t ListSize(LinkedList *self);

/**
 * @brief Reverse the list.
 *
 * @param self          The pointer to the LinkedList structure
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 */
int32_t ListReverse(LinkedList *self);

/**
 * @brief Iterate through the list till the tail end.
 *
 * Before iterating through the list, it is necessary to pass bReset := true and
 * pItem := NULL for iterator initialization.
 * After initialization, you can pass bReset := false and pItem := the relevant
 * pointer to get the returned item at each iteration.
 *
 * @param self          The pointer to the LinkedList structure
 * @param bReset        The knob to reset the iteration
 * @param pItem         The pointer to the returned item
 *
 * @retval SUCC
 * @retval END          Beyond the tail end
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_IDX      Empty list
 * @retval ERR_GET      Invalid parameter to store returned item
 */
int32_t ListIterate(LinkedList *self, bool bReset, Item *pItem);

/**
 * @brief Reversely iterate through the list till the head end.
 *
 * Before reversely iterating through the list, it is necessary to pass
 * bReset := true and pItem := NULL for iterator initialization.
 * After initialization, you can pass bReset := false and pItem := the relevant
 * pointer to get the returned item at each iteration.
 *
 * @param self          The pointer to the LinkedList structure
 * @param bReset        The knob to reset the iteration
 * @param pItem         The pointer to the returned item
 *
 * @retval SUCC
 * @retval END          Beyond the head end
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_IDX      Empty list
 * @retval ERR_GET      Invalid parameter to store returned item
 */
int32_t ListReverseIterate(LinkedList *self, bool bReset, Item *pItem);

/**
 * @brief Set the custom item resource clean method.
 *
 * @param self          The pointer to the LinkedList structure
 * @param pFunc         The function pointer to the custom method
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 */
int32_t ListSetDestroy(LinkedList *self, void (*pFunc) (Item));

#endif
