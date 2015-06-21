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

    /** Insert an item into the tree.
        @see SimTreeInsert */
    int32_t (*insert) (struct _SimpleTree*, Item, bool);

    /** Search for the item having the same order with the designated one.
        @see SimTreeSearch */
    int32_t (*search) (struct _SimpleTree*, Item, Item*);

    /** Delete the item having the same order with the designated one.
        @see SimTreeDelete */
    int32_t (*delete) (struct _SimpleTree*, Item, bool);

    /** Get the item having the maximum order of the tree.
        @see SimTreeMaximum */
    int32_t (*maximum) (struct _SimpleTree*, Item*);

    /** Get the item having the minimum order of the tree.
        @see SimTreeMinimum */
    int32_t (*minimum) (struct _SimpleTree*, Item*);

    /** Get the immediate successor of the designated item.
        @see SimTreeSuccessor */
    int32_t (*successor) (struct _SimpleTree*, Item, Item*);

    /** Get the immediate predecessor of the designated item.
        @see SimTreePredecessor */
    int32_t (*predecessor) (struct _SimpleTree*, Item, Item*);

    /** Get the number of stored item.
        @see SimTreeSize */
    int32_t (*size) (struct _SimpleTree*);

    /** Set the user defined item comparison method.
        @see SimTreeSetCompare */
    int32_t (*set_compare) (struct _SimpleTree*, int32_t (*) (Item, Item));

    /** Set the user defined item clean method.
        @see SimTreeSetDestroy */
    int32_t (*set_destroy) (struct _SimpleTree*, void (*) (Item));
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
 * If the knob is on, it also runs the resource clean method for all the items.
 *
 * @param ppObj         The double pointer to the to be destructed tree
 * @param bClean        The knob to clean item resource
 */
void SimTreeDeinit(SimpleTree **ppObj, bool bClean);

/**
 * @brief Insert an item into the tree.
 *
 * This function inserts an item into the tree. If the order of the designated
 * item is the same with a certain one stored in the tree, that item will be
 * replaced. Also, if the knob is on, it runs the resource clean method for the
 * replaced item.
 *
 * @param self          The pointer to the SimpleTree structure
 * @param item          The designated item
 * @param bClean        The knob to clean item resource
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_NOMEM    Insufficient memory for tree extension
 */
int32_t SimTreeInsert(SimpleTree *self, Item item, bool bClean);

/**
 * @brief Search for the item having the same order with the designated one.
 *
 * This function searches for an item having the same order with the designated
 * one. If such item can be found, it will be returned by the third parameter.
 * Otherwise, the error code is returned and the third parameter is updated with
 * NULL.
 *
 * @param self          The pointer to the SimpleTree structure
 * @param itemIn        The designated item
 * @param pItemOut      The pointer to the returned item
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_NODATA   Non-existent item
 */
int32_t SimTreeSearch(SimpleTree *self, Item itemIn, Item *pItemOut);

/**
 * @brief Delete the item having the same order with the designated one.
 *
 * This function first searches for the item having the same order with the
 * designated one. If such item can be found, it will be removed. Also, if the
 * knob is on, the function runs the resource clean method for the removed item.
 *
 * @param self          The pointer to the SimpleTree structure
 * @param item          The designated item
 * @param bClean        The knob to clean item resource
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_NODATA   The target item cannot be found
 */
int32_t SimTreeDelete(SimpleTree *self, Item item, bool bClean);

/**
 * @brief Return the item having the maximum order of the tree.

 * @param self          The pointer to the SimpleTree structure
 * @param pItem         The pointer to the returned item
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_IDX      The tree is empty
 */
int32_t SimTreeMaximum(SimpleTree *self, Item *pItem);

/**
 * @brief Return the item having the minimum order of the tree.
 *
 * @param self          The pointer to the SimpleTree structure
 * @param pItem         The pointer to the returned item
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_IDX      The tree is empty
 */
int32_t SimTreeMinimum(SimpleTree *self, Item *pItem);

/**
 * @brief Return the item which is the immediate successor of the designated item.
 *
 * @param self          The pointer to the SimpleTree structure
 * @param itemIn        The designated item
 * @param pItemOut      The pointer to the returned item
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_NODATA   The immediate successor cannot be found
 */
int32_t SimTreeSuccessor(SimpleTree *self, Item itemIn, Item *pItemOut);

/**
 * @brief Return the item which is the immediate predecessor of the designated item.
 *
 * @param self          The pointer to the SimpleTree structure
 * @param itemIn        The designated item
 * @param pItemOut      The pointer to the returned item
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_NODATA   The immediate predecessor cannot be found
 */
int32_t SimTreePredecessor(SimpleTree *self, Item itemIn, Item *pItemOut);

/**
 * @brief Return the number of stored items.
 *
 * @param self          The pointer to the SimpleTree structure.
 *
 * @return              The number of stored items
 * @retval ERR_NOINIT   Uninitialized container
 */
int32_t SimTreeSize(SimpleTree *self);

/**
 * @brief Set the user defined item comparison method.
 *
 * @param self          The pointer to the SimpleTree structure
 * @param pFunc         The function pointer to the custom method
 *
 * @retval ERR_NOINIT   Uninitialized container
 */
int32_t SimTreeSetCompare(SimpleTree *self, int32_t (*pFunc) (Item, Item));

/**
 * @brief Set the user defined item clean method.
 *
 * @param self          The pointer to the SimpleTree structure
 * @param pFunc         The function pointer to the custom method
 *
 * @retval ERR_NOINIT   Uninitialized container
 */
int32_t SimTreeSetDestroy(SimpleTree *self, void (*pFunc) (Item));

#endif
