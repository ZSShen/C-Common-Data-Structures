/**
 * @file balanced_tree.h The balanced binary search tree.
 */

#ifndef _BALANCED_TREE_H_
#define _BALANCED_TREE_H_

#include "../util.h"

/** BalTreeData is the data type for the container private information. */
typedef struct _BalTreeData BalTreeData;

/** The implementation for basic binary search tree. */
typedef struct _BalancedTree {
    /** The container private information. */
    BalTreeData *pData;

    /** Insert an item into the tree.
        @see BalTreeInsert */
    int32_t (*insert) (struct _BalancedTree*, Item, bool);

    /** Search for the the item having the same order with the designated one.
        @see BalTreeSearch */
    int32_t (*search) (struct _BalancedTree*, Item, Item*);

    /** Delete the item having the same order with the designated one.
        @see BalTreeDelete */
    int32_t (*delete) (struct _BalancedTree*, Item, bool);

    /** Get the item having the maximum order of the tree.
        @see BalTreeMaximum */
    int32_t (*maximum) (struct _BalancedTree*, Item*);

    /** Get the item having the minimum order of the tree.
        @see BalTreeMinimum */
    int32_t (*minimum) (struct _BalancedTree*, Item*);

    /** Get the immediate successor of the designated item.
        @see BalTreeSuccessor */
    int32_t (*successor) (struct _BalancedTree*, Item, Item*);

    /** Get the immediate predecessor of the designated item.
        @see BalTreePredecessor */
    int32_t (*predecessor) (struct _BalancedTree*, Item, Item*);

    /** Get the number of stored item.
        @see BalTreeSize */
    int32_t (*size) (struct _BalancedTree*);

    /** Set the user defined item comparison method.
        @see BalTreeSetCompare */
    int32_t (*set_compare) (struct _BalancedTree*, int32_t (*) (Item, Item));

    /** Set the user defined item clean method.
        @see BalTreeSetDestroy */
    int32_t (*set_destroy) (struct _BalancedTree*, void (*) (Item));
} BalancedTree;


/**
 * @brief The constructor for BalancedTree.
 *
 * @param ppObj         The double pointer to the to be constructed tree
 *
 * @retval SUCC
 * @retval ERR_NOMEM    Insufficient memory for tree construction
 */
int32_t BalTreeInit(BalancedTree **ppObj);

/**
 * @brief The destructor for BalancedTree.
 *
 * If the knob is on, it also runs the resource clean method for all the items.
 *
 * @param ppObj         The double pointer to the to be destructed tree
 * @param bClean        The knob to clean item resource
 */
void BalTreeDeinit(BalancedTree **ppObj, bool bClean);

/**
 * @brief Insert an item into the tree.
 *
 * This function inserts an item into the tree. If the order of the designated
 * item is the same with a certain one stored in the tree, that item will be
 * replaced. Also, if the knob is on, it runs the resource clean method for the
 * replaced item.
 *
 * @param self          The pointer to the BalancedTree structure
 * @param item          The designated item
 * @param bClean        The knob to clean item resource
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_NOMEM    Insufficient memory for tree extension
 */
int32_t BalTreeInsert(BalancedTree *self, Item item, bool bClean);

/**
 * @brief Search for the item having the same order with the designated one.
 *
 * This function searches for an item having the same order with the designated
 * one. If such item can be found, it will be returned by the third parameter.
 * Otherwise, the error code is returned and the third parameter is updated with
 * NULL.
 *
 * @param self          The pointer to the BalancedTree structure
 * @param itemIn        The designated item
 * @param pItemOut      The pointer to the returned item
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_NODATA   Non-existent item
 */
int32_t BalTreeSearch(BalancedTree *self, Item itemIn, Item *pItemOut);

/**
* @brief Delete the item having the same order with the designated one.
 *
 * This function first searches for the item having the same order with the
 * designated one. If such item can be found, it will be removed. Also, if the
 * knob is on, the function runs the resource clean method for the removed item.
 *
 * @param self          The pointer to the BalancedTree structure
 * @param item          The designated item
 * @param bClean        The knob to clean item resource
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_NODATA   Non-existent item
 */
int32_t BalTreeDelete(BalancedTree *self, Item item, bool bClean);

/**
 * @brief Return the item having the maximum order of the tree.
 *
 * @param self          The pointer to the BalancedTree structure
 * @param pItem         The pointer to the returned item
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_IDX      Empty tree
 */
int32_t BalTreeMaximum(BalancedTree *self, Item *pItem);

/**
 * @brief Return the item having the minimum order of the tree.
 *
 * @param self          The pointer to the BalancedTree structure
 * @param pItem         The pointer to the returned item
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_IDX      Empty tree
 */
int32_t BalTreeMinimum(BalancedTree *self, Item *pItem);

/**
 * @brief Return the item which is the immediate successor of the designated item.
 *
 * @param self          The pointer to the BalancedTree structure
 * @param itemIn        The designated item
 * @param pItemOut      The pointer to the returned item
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_NODATA   Non-existent immediate successor
 */
int32_t BalTreeSuccessor(BalancedTree *self, Item itemIn, Item *pItemOut);

/**
 * @brief Return the item which is the immediate predecessor of the designated item.
 *
 * @param self          The pointer to the BalancedTree structure
 * @param itemIn        The designated item
 * @param pItemOut      The pointer to the returned item
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_NODATA   Non-existent immediate predecessor
 */
int32_t BalTreePredecessor(BalancedTree *self, Item itemIn, Item *pItemOut);

/**
 * @brief Return the number of stored items.
 *
 * @param self          The pointer to the BalancedTree structure.
 *
 * @return              The number of stored items
 * @retval ERR_NOINIT   Uninitialized container
 */
int32_t BalTreeSize(BalancedTree *self);

/**
 * @brief Set the user defined item comparison method.
 *
 * @param self          The pointer to the BalancedTree structure
 * @param pFunc         The function pointer to the custom method
 *
 * @retval ERR_NOINIT   Uninitialized container
 */
int32_t BalTreeSetCompare(BalancedTree *self, int32_t (*pFunc) (Item, Item));

/**
 * @brief Set the user defined item clean method.
 *
 * @param self          The pointer to the BalancedTree structure
 * @param pFunc         The function pointer to the custom method
 *
 * @retval ERR_NOINIT   Uninitialized container
 */
int32_t BalTreeSetDestroy(BalancedTree *self, void (*pFunc) (Item));


#endif

