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

    /** The operator to insert an item into the tree.
        @see BalTreeInsert */
    int32_t (*insert) (struct _BalancedTree*, Item);

    /** The operator to check for the existence of the item having the same
        order with the requested one.
        @see BalTreeSearch */
    int32_t (*search) (struct _BalancedTree*, Item, Item*);

    /** The operator to delete the item having the same order with the
        requested one.
        @see BalTreeDelete */
    int32_t (*delete) (struct _BalancedTree*, Item);

    /** The operator to get the item with the maximum order of the tree.
        @see BalTreeMaximum */
    int32_t (*maximum) (struct _BalancedTree*, Item*);

    /** The operator to get the item with the minimum order of the tree.
        @see BalTreeMinimum */
    int32_t (*minimum) (struct _BalancedTree*, Item*);

    /** The operator to get the immediate successor of the requested item.
        @see BalTreeSuccessor */
    int32_t (*successor) (struct _BalancedTree*, Item, Item*);

    /** The operator to get the immediate predecessor of the requested item.
        @see BalTreePredecessor */
    int32_t (*predecessor) (struct _BalancedTree*, Item, Item*);

    /** The operator to get the number of stored item.
        @see BalTreeSize */
    int32_t (*size) (struct _BalancedTree*);

    /** The operator to set the user defined order comparison strategy for a
        a pair of items.
        @see BalTreeSetCompare */
    void (*set_compare) (struct _BalancedTree*, int32_t (*) (Item, Item));

    /** The operator to set the user defined clean strategy for the resource
        hold by an item.
        @see BalTreeSetDestroy */
    void (*set_destroy) (struct _BalancedTree*, void (*) (Item));
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
 * @param ppObj         The double pointer to the to be destructed tree
 */
void BalTreeDeinit(BalancedTree **ppObj);

/**
 * @brief Insert an item into the tree.
 *
 * This function inserts the requested item into the proper position of the
 * tree to maintain the structure invariant.
 *
 * @param self          The pointer to the BalancedTree structure
 * @param item          The requested item.
 *
 * @retval SUCC
 * @retval ERR_NOMEM    Insufficient memory for item insertion
 *
 * @note If the order of the requested item is the same with a certain one
 * already stored in the tree. The old item will be cleaned by the configured
 * policy so that the requested one can take the position.
 */
int32_t BalTreeInsert(BalancedTree *self, Item item);

/**
 * @brief Check for the existence of the item having the same order with the
 * requested one.
 *
 * This function checks if there is already an item having the same order with
 * the requested one in the tree. If such item can be found, it will be returned
 * by the third parameter. In other words, this function can be used to query
 * the item existence and it can also be to acquire the found item. Otherwise,
 * the no data error code is returned and the third parameter is updated with
 * NULL value.
 *
 * @param self          The pointer to the BalancedTree structure
 * @param itemIn        The requested item
 * @param pItemOut      The pointer to the returned item
 *
 * @retval SUCC
 * @retval ERR_NODATA   The target item cannot be found
 */
int32_t BalTreeSearch(BalancedTree *self, Item itemIn, Item *pItemOut);

/**
 * @brief Delete the item having the same order with the requested one from the
 * tree.
 *
 * This function first searches for the item having the same order with the
 * requested one in the tree. If such item can be found, the item will be cleaned
 * by the configured policy.
 *
 * @param self          The pointer to the BalancedTree structure
 * @param item          The requested item
 *
 * @retval SUCC
 * @retval ERR_NODATA   The target item cannot be found
 */
int32_t BalTreeDelete(BalancedTree *self, Item item);

/**
 * @brief Return the item with the maximum order of the tree.

 * @param self          The pointer to the BalancedTree structure
 * @param pItem         The pointer to the returned item
 *
 * @retval SUCC
 * @retval ERR_IDX      The tree is empty
 */
int32_t BalTreeMaximum(BalancedTree *self, Item *pItem);

/**
 * @brief Return the item with the minimum order of the tree.
 *
 * @param self          The pointer to the BalancedTree structure
 * @param pItem         The pointer to the returned item
 *
 * @retval SUCC
 * @retval ERR_IDX      The tree is empty
 */
int32_t BalTreeMinimum(BalancedTree *self, Item *pItem);

/**
 * @brief Return the item which is the immediate successor of the requested item.
 *
 * @param self         The pointer to the BalancedTree structure
 * @param itemIn       The requested item
 * @param pItemOut     The pointer to the returned item
 *
 * @retval SUCC
 * @retval ERR_NODATA  The immediate successor cannot be found
 */
int32_t BalTreeSuccessor(BalancedTree *self, Item itemIn, Item *pItemOut);

/**
 * @brief Return the item which is the immediate predecessor of the requested item.
 *
 * @param self         The pointer to the BalancedTree structure
 * @param itemIn       The requested item
 * @param pItemOut     The pointer to the returned item
 *
 * @retval SUCC
 * @retval ERR_NODATA  The immediate predecessor cannot be found
 */
int32_t BalTreePredecessor(BalancedTree *self, Item itemIn, Item *pItemOut);

/**
 * @brief Return the number of items stored in the tree.
 *
 * @param self          The pointer to the BalancedTree structure.
 *
 * @return              The number of stored items
 */
int32_t BalTreeSize(BalancedTree *self);

/**
 * @brief Set the user defined order comparison strategy for a pair of items.
 *
 * @param self          The pointer to the BalancedTree structure.
 * @param pFunc         The pointer to the user defined function.
 *
 * This function sets the user defined item comparison strategy.
 */
void BalTreeSetCompare(BalancedTree *self, int32_t (*pFunc) (Item, Item));

/**
 * @brief Set the user defined clean strategy for the resource hold by an item.
 *
 * @param self          The pointer to the BalancedTree structure.
 * @param pFunc         The pointer to the user defined function.
 */
void BalTreeSetDestroy(BalancedTree *self, void (*pFunc) (Item));


#endif

