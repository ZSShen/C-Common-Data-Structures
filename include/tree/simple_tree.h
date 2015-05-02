/**
 * @file simple_tree.h The basic binary search tree.
 */

#ifndef _SIMPLE_TREE_H_
#define _SIMPLE_TREE_H_

#include "../util.h"

/** SimTreeData is the data type for the container private information. */
typedef struct _SimTreeData SimTreeData;

/** The implementation for basic binary search tree. */
typedef struct _SimpleTree {
    /** The container private information. */
    SimTreeData *pData;

    /** The operator to insert an item into the tree.
        @see SimTreeInsert */
    int32_t (*insert) (struct _SimpleTree*, Item);

    /** The operator to search for the item having the same order with the
        designated one.
        @see SimTreeSearch */
    int32_t (*search) (struct _SimpleTree*, Item, Item*);

    /** The operator to delete the item having the same order with the
        designated one.
        @see SimTreeDelete */
    int32_t (*delete) (struct _SimpleTree*, Item);

    /** The operator to get the item with the maximum order of the tree.
        @see SimTreeMaximum */
    int32_t (*maximum) (struct _SimpleTree*, Item*);

    /** The operator to get the item with the minimum order of the tree.
        @see SimTreeMinimum */
    int32_t (*minimum) (struct _SimpleTree*, Item*);

    /** The operator to get the immediate successor of the designated item.
        @see SimTreeSuccessor */
    int32_t (*successor) (struct _SimpleTree*, Item, Item*);

    /** The operator to get the immediate predecessor of the designated item.
        @see SimTreePredecessor */
    int32_t (*predecessor) (struct _SimpleTree*, Item, Item*);

    /** The operator to get the number of stored item.
        @see SimTreeSize */
    int32_t (*size) (struct _SimpleTree*);

    /** The operator to set the user defined order comparison strategy for
        a pair of items.
        @see SimTreeSetCompare */
    void (*set_compare) (struct _SimpleTree*, int32_t (*) (Item, Item));

    /** The operator to set the user defined clean strategy for the resource
        hold by an item.
        @see SimTreeSetDestroy */
    void (*set_destroy) (struct _SimpleTree*, void (*) (Item));
} SimpleTree;


/**
 * @brief The constructor for SimpleTree.
 *
 * @param ppObj         The double pointer to the to be constructed tree
 *
 * @retval SUCC
 * @retval ERR_NOMEM    Insufficient memory for tree construction
 */
int32_t SimTreeInit(SimpleTree **ppObj);

/**
 * @brief The destructor for SimpleTree.
 *
 * @param ppObj         The double pointer to the to be destructed tree
 */
void SimTreeDeinit(SimpleTree **ppObj);

/**
 * @brief Insert an item into the tree.
 *
 * This function inserts the designated item into the proper position of the
 * tree to maintain the structure correctness.
 *
 * @param self          The pointer to the SimpleTree structure
 * @param item          The designated item.
 *
 * @retval SUCC
 * @retval ERR_NOMEM    Insufficient memory for item insertion
 *
 * @note If the order of the designated item is the same with a certain one
 * stored in the tree, the old item will be cleaned by the resource deallocation
 * function so that the designated one can take the position.
 */
int32_t SimTreeInsert(SimpleTree *self, Item item);

/**
 * @brief Search for the item having the same order with the designated one.
 *
 * This function searches for an item having the same order with the designated
 * one. If such item can be found, it will be returned by the third parameter.
 * Otherwise, the error code is returned and the third parameter is returned
 * with NULL value.
 *
 * @param self          The pointer to the SimpleTree structure
 * @param itemIn        The designated item
 * @param pItemOut      The pointer to the returned item
 *
 * @retval SUCC
 * @retval ERR_NODATA   The target item cannot be found
 */
int32_t SimTreeSearch(SimpleTree *self, Item itemIn, Item *pItemOut);

/**
 * @brief Delete the item having the same order with the designated one from
 *thetree.
 *
 * This function first searches for the item having the same order with the
 * designated one. If such item can be found, the item will be cleaned by the
 * resource deallocation function.
 *
 * @param self          The pointer to the SimpleTree structure
 * @param item          The designated item
 *
 * @retval SUCC
 * @retval ERR_NODATA   The target item cannot be found
 */
int32_t SimTreeDelete(SimpleTree *self, Item item);

/**
 * @brief Return the item with the maximum order of the tree.

 * @param self          The pointer to the SimpleTree structure
 * @param pItem         The pointer to the returned item
 *
 * @retval SUCC
 * @retval ERR_IDX      The tree is empty
 */
int32_t SimTreeMaximum(SimpleTree *self, Item *pItem);

/**
 * @brief Return the item with the minimum order of the tree.
 *
 * @param self          The pointer to the SimpleTree structure
 * @param pItem         The pointer to the returned item
 *
 * @retval SUCC
 * @retval ERR_IDX      The tree is empty
 */
int32_t SimTreeMinimum(SimpleTree *self, Item *pItem);

/**
 * @brief Return the item which is the immediate successor of the designated item.
 *
 * @param self         The pointer to the SimpleTree structure
 * @param itemIn       The designated item
 * @param pItemOut     The pointer to the returned item
 *
 * @retval SUCC
 * @retval ERR_NODATA  The immediate successor cannot be found
 */
int32_t SimTreeSuccessor(SimpleTree *self, Item itemIn, Item *pItemOut);

/**
 * @brief Return the item which is the immediate predecessor of the designated item.
 *
 * @param self         The pointer to the SimpleTree structure
 * @param itemIn       The designated item
 * @param pItemOut     The pointer to the returned item
 *
 * @retval SUCC
 * @retval ERR_NODATA  The immediate predecessor cannot be found
 */
int32_t SimTreePredecessor(SimpleTree *self, Item itemIn, Item *pItemOut);

/**
 * @brief Return the number of items stored in the tree.
 *
 * @param self          The pointer to the SimpleTree structure.
 *
 * @return              The number of stored items
 */
int32_t SimTreeSize(SimpleTree *self);

/**
 * @brief Set the user defined order comparison strategy for a pair of items.
 *
 * @param self          The pointer to the SimpleTree structure
 * @param pFunc         The pointer to the user defined function
 */
void SimTreeSetCompare(SimpleTree *self, int32_t (*pFunc) (Item, Item));

/**
 * @brief Set the user defined clean strategy for the resource hold by an item.
 *
 * @param self          The pointer to the SimpleTree structure.
 * @param pFunc         The pointer to the user defined function.
 */
void SimTreeSetDestroy(SimpleTree *self, void (*pFunc) (Item));

#endif
