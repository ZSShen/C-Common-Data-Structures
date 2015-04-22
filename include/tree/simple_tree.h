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

    /** The operator to check for the existence of the item having the same
        order with the requested one.
        @see SimTreeSearch */
    int32_t (*search) (struct _SimpleTree*, Item, Item*);

    /** The operator to delete the item having the same order with the
        requested one.
        @see SimTreeDelete */
    int32_t (*delete) (struct _SimpleTree*, Item);

    /** The operator to get the item with the maximum order of the tree.
        @see SimTreeMaximum */
    int32_t (*maximum) (struct _SimpleTree*, Item*);

    /** The operator to get the item with the minimum order of the tree.
        @see SimTreeMinimum */
    int32_t (*minimum) (struct _SimpleTree*, Item*);

    /** The operator to get the immediate successor of the requested item.
        @see SimTreeSuccessor */
    int32_t (*successor) (struct _SimpleTree*, Item, Item*);

    /** The operator to get the immediate predecessor of the requested item.
        @see SimTreePredecessor */
    int32_t (*predecessor) (struct _SimpleTree*, Item, Item*);

    /** The operator to get the number of stored item.
        @see SimTreeSize */
    int32_t (*size) (struct _SimpleTree*);

    /** The operator to set the user defined order comparison strategy for a
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
 * This function inserts the requested item into the proper position of the
 * tree to maintain the structure invariant.
 *
 * @param self          The pointer to the SimpleTree structure
 * @param item          The requested item.
 *
 * @retval SUCC
 * @retval ERR_NOMEM    Insufficient memory for item insertion
 *
 * @note If the order of the requested item is the same with a certain one
 * already stored in the tree. The old item will be cleaned by the configured
 * policy so that the requested one can take the position.
 */
int32_t SimTreeInsert(SimpleTree *self, Item item);

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
 * @param self          The pointer to the SimpleTree structure
 * @param itemIn        The requested item
 * @param pItemOut      The pointer to the returned item
 *
 * @retval SUCC
 * @retval ERR_NODATA   The target item cannot be found
 */
int32_t SimTreeSearch(SimpleTree *self, Item itemIn, Item *pItemOut);

/**
 * @brief Delete the item having the same order with the requested one from the
 * tree.
 *
 * This function first searches for the item having the same order with the
 * requested one in the tree. If such item can be found, the item will be cleaned
 * by the configured policy.
 *
 * @param self          The pointer to the SimpleTree structure
 * @param item          The requested item
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
 * @brief Return the item which is the immediate successor of the requested item.
 *
 * @param self         The pointer to the SimpleTree structure
 * @param itemIn       The requested item
 * @param pItemOut     The pointer to the returned item
 *
 * @retval SUCC
 * @retval ERR_NODATA  The immediate successor cannot be found
 */
int32_t SimTreeSuccessor(SimpleTree *self, Item itemIn, Item *pItemOut);

/**
 * @brief Return the item which is the immediate predecessor of the requested item.
 *
 * @param self         The pointer to the SimpleTree structure
 * @param itemIn       The requested item
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
 * @param self          The pointer to the SimpleTree structure.
 * @param pFunc         The pointer to the user defined function.
 *
 * This function sets the user defined item comparison strategy.
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
