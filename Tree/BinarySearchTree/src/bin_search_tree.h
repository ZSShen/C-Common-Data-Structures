#ifndef _BIN_SEARCH_TREE_H_
#define _BIN_SEARCH_TREE_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

#define SUCCESS                     (0)
#define FAIL_NO_MEMORY              (-1)
#define FAIL_NO_DATA                (-2)
#define FAIL_DATA_CONFLICT          (-3)

typedef const void* Item;
typedef struct _TreeNode TreeNode;
typedef struct _BSTreeData BSTreeData;

typedef struct _BinSearchTree {
    BSTreeData *pData;
    int32_t (*insert) (struct _BinSearchTree*, Item);
    int32_t (*search) (struct _BinSearchTree*, Item*);
    int32_t (*delete) (struct _BinSearchTree*, Item);
    int32_t (*maximum) (struct _BinSearchTree*, Item*);
    int32_t (*minimum) (struct _BinSearchTree*, Item*);
    int32_t (*successor) (struct _BinSearchTree*, Item*);
    int32_t (*predecessor) (struct _BinSearchTree*, Item*);
    uint32_t (*size) (struct _BinSearchTree*);
    void (*set_compare) (struct _BinSearchTree*, int32_t (*) (Item, Item));
    void (*set_destroy) (struct _BinSearchTree*, void (*) (Item));
} BinSearchTree;


/**
 * The constructor for BinSearchTree.
 *
 * @param ppObj         The double pointer to the to be constructed tree.
 *
 * @return              SUCCESS
 *                      FAIL_NO_MEMORY
 */
int32_t BSTreeInit(BinSearchTree **ppObj);

/**
 * The destructor for BinSearchTree.
 *
 * @param ppObj          The double pointer to the to be destructed tree.
 */
void BSTreeDeinit(BinSearchTree **ppObj);

/**
 * int32_t (*insert) (BinSearchTree *self, Item item)
 *
 * @param self          The pointer to the BinSearchTree structure.
 * @param item          The requested item.
 *
 * @return              SUCCESS
 *                      FAIL_NO_MEMORY
 *                      FAIL_DATA_CONFLICT
 *
 * This function inserts the requested item into the proper location of the tree.
 * It will fail under two conditions:
 *     1. Insufficient memory space.
 *     2. The requested item conflicts with the one stored in the tree.
 */

/**
 * int32_t (*search) (BinSearchTree *self, Item *pItem)
 *
 * @param self          The pointer to the BinSearchTree structure.
 * @param pItem         The pointer to the requested item.
 *
 * @return              SUCCESS
 *                      FAIL_NO_DATA
 *
 * This function checks whethere the tree has the requested item. It the input
 * item can be found, it will be replaced with the one stored in the tree.
 * It will fail under one condition:
 *     1. The requested item cannot be found in the tree.
 */

/**
 * int32_t (*delete) (BinSearchTree *self, Item item)
 *
 * @param self          The pointer to the BinSearchTree structure.
 * @param item          The requested item.
 *
 * @return              SUCCESS
 *                      FAIL_NO_DATA
 *
 * This function deletes the requested item from the tree and adjusts the tree structure.
 * It will fail under one condition:
 *     1. The requested item cannot be found in the tree.
 */

/**
 * int32_t (*maximum) (struct _BinSearchTree *self, Item *pItem)
 *
 * @param self          The pointer to the BinSearchTree structure.
 * @param pItem         The pointer to the returned item.
 *
 * @return              SUCCESS
 *                      FAIL_NO_DATA
 *
 * This function returns the item with the maximum order of the tree.
 * It will fail under one condition:
 *     1. The tree is empty.
 */

/**
 * int32_t (*minimum) (struct _BinSearchTree *self, Item *pItem)
 *
 * @param self          The pointer to the BinSearchTree structure.
 * @param pItem         The pointer to the returned item.
 *
 * @return              SUCCESS
 *                      FAIL_NO_DATA
 *
 * This function returns the item with the minimum order of the tree.
 * It will fail under one condition:
 *     1. The tree is empty.
 */

/**
 * int32_t (*successor) (struct _BinSearchTree *self, Item *pItem)
 *
 * @param self         The pointer to the BinSearchTree structure.
 * @param pItem        The pointer to the returned item.
 *
 * @return             SUCCESS
 *                     FAIL_NO_DATA
 *
 * This function returns the item which is the successor of the requested one.
 * It will fail under one condition:
 *     1. The successor cannot be found.
 */

/**
 * int32_t (*predecessor) (struct _BinSearchTree *self, Item *pItem)
 *
 * @param self         The pointer to the BinSearchTree structure.
 * @param pItem        The pointer to the returned item.
 *
 * @return             SUCCESS
 *                     FAIL_NO_DATA
 *
 * This function returns the item which is the predecessor of the requested one.
 * It will fail under one condition:
 *     1. The predecessor cannot be found.
 */

/**
 * uint32_t (*size) (struct _BinSearchTree*)
 *
 * @param self          The pointer to the BinSearchTree structure.
 *
 * @return              Tree size
 *
 * This function returns the size of the tree.
 */

/**
 * void (*set_compare)(struct _BinSearchTree*, int32_t (*) (Item, Item))
 *
 * @param self          The pointer to the BinSearchTree structure.
 * @param pFunc         The pointer to the user defined function.
 *
 * This function sets the user defined item comparison strategy.
 */

/**
 * void (*set_destroy)(struct _BinSearchTree*, void (*) (Item))
 *
 * @param self          The pointer to the BinSearchTree structure.
 * @param pFunc         The pointer to the user defined function.
 *
 * This function sets the user defined item deallocation strategy.
 */

#endif
