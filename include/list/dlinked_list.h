/**
 * @file dlinked_list The doubly linked list.
 */

#ifndef _DLINKED_LIST_H_
#define _DLINKED_LIST_H_

#include "../util.h"

typedef struct _DListData DListData;

typedef struct _DLinkedList {
	DListData *pData;

	int32_t (*push_front) (struct _DLinkedList*, Item);

	int32_t (*push_back) (struct _DLinkedList*, Item);

	int32_t (*insert) (struct _DLinkedList*, Item, int32_t);

	int32_t (*pop_front) (struct _DLinkedList*);

	int32_t (*pop_back) (struct _DLinkedList*);

	int32_t (*delete) (struct _DLinkedList*, int32_t);

	int32_t (*set_front) (struct _DLinkedList*, Item);

	int32_t (*set_back) (struct _DLinkedList*, Item);

	int32_t (*set_at) (struct _DLinkedList*, Item, int32_t);

	int32_t (*get_front) (struct _DLinkedList*, Item*);

	int32_t (*get_back) (struct _DLinkedList*, Item*);

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
 * @param ppObj			The double pointer to the to be constructed list
 *
 * @retval SUCC
 * @retval ERR_NOMEM	Insufficient memory for list construction
 */
int32_t DListInit(DLinkedList **ppObj);

int32_t DListDeinit(DLinkedList **ppObj);

int32_t DListPushFront(DLinkedList *pObj, Item item);

int32_t DListPushBack(DLinkedList *pObj, Item item);

int32_t DListInsert(DLinkedList *pObj, Item item, int32_t iIdx);

int32_t DListPopFront(DLinkedList *pObj);

int32_t DListPopBack(DLinkedList *pObj);

int32_t DListDelete(DLinkedList *pObj, int32_t iIdx);

int32_t DListSetFront(DLinkedList *pObj, Item item);

int32_t DListSetBack(DLinkedList *pObj, Item item);

int32_t DListSetAt(DLinkedList *pObj, Item item, int32_t iIdx);

int32_t DListGetFront(DLinkedList *pObj, Item *pItem);

int32_t DListGetBack(DLinkedList *pObj, Item *pItem);

int32_t DListGetAt(DLinkedList *pObj, Item *pItem, int32_t iIdx);

int32_t DListResize(DLinkedList *pObj, int32_t iSize);

int32_t DListSize(DLinkedList *pObj);

int32_t DListReverse(DLinkedList *pObj);

void DListSetDestroy(DLinkedList *pObj, void (*pFunc) (Item));

#endif