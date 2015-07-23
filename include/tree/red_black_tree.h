/**
 * @file red_black_tree.h The balanced binary search tree.
 */

#ifndef _REDBLACK_TREE_H_
#define _REDBLACK_TREE_H_

#include "../util.h"

/** RBTreeData is the data type for the container private information. */
typedef struct _RBTreeData RBTreeData;

/** The implementation for balanced binary search tree. */
typedef struct _RedBlackTree {
    /** The container private information. */
    RBTreeData *pData;

    /** Insert an item into the tree.
        @see RBTreeInsert */
    int32_t (*insert) (struct _RedBlackTree*, Item);

    /** Search for the the item having the same order with the designated one.
        @see RBTreeSearch */
    int32_t (*search) (struct _RedBlackTree*, Item, Item*);

    /** Delete the item having the same order with the designated one.
        @see RBTreeDelete */
    int32_t (*delete) (struct _RedBlackTree*, Item);

    /** Get the item having the maximum order of the tree.
        @see RBTreeMaximum */
    int32_t (*maximum) (struct _RedBlackTree*, Item*);

    /** Get the item having the minimum order of the tree.
        @see RBTreeMinimum */
    int32_t (*minimum) (struct _RedBlackTree*, Item*);

    /** Get the immediate successor of the designated item.
        @see RBTreeSuccessor */
    int32_t (*successor) (struct _RedBlackTree*, Item, Item*);

    /** Get the immediate predecessor of the designated item.
        @see RBTreePredecessor */
    int32_t (*predecessor) (struct _RedBlackTree*, Item, Item*);

    /** Get the number of stored item.
        @see RBTreeSize */
    int32_t (*size) (struct _RedBlackTree*);

    /** Set the custom item comparison method.
        @see RBTreeSetCompare */
    int32_t (*set_compare) (struct _RedBlackTree*, int32_t (*) (Item, Item));

    /** Set the custom item resource clean method.
        @see RBTreeSetDestroy */
    int32_t (*set_destroy) (struct _RedBlackTree*, void (*) (Item));
} RedBlackTree;


/**
 * @brief The constructor for RedBlackTree.
 *
 * @param ppObj         The double pointer to the to be constructed tree
 *
 * @retval SUCC
 * @retval ERR_NOMEM    Insufficient memory for tree construction
 */
int32_t RBTreeInit(RedBlackTree **ppObj);

/**
 * @brief The destructor for RedBlackTree.
 *
 * If the custom resource clean method is set, it also runs the clean method for
 * all the items.
 *
 * @param ppObj         The double pointer to the to be destructed tree
 */
void RBTreeDeinit(RedBlackTree **ppObj);

/**
 * @brief Insert an item into the tree.
 *
 * This function inserts an item into the tree. If the order of the designated
 * item is the same with a certain one stored in the tree, that item will be
 * replaced. Also, if the custom resource clean method is set, it runs the clean
 * method for the replaced item.
 *
 * @param self          The pointer to the RedBlackTree structure
 * @param item          The designated item
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_NOMEM    Insufficient memory for tree extension
 */
int32_t RBTreeInsert(RedBlackTree *self, Item item);

/**
 * @brief Search for the item having the same order with the designated one.
 *
 * This function searches for an item having the same order with the designated
 * one. If such item can be found, it will be returned by the third parameter.
 * Otherwise, the error code is returned and the third parameter is updated with
 * NULL.
 *
 * @param self          The pointer to the RedBlackTree structure
 * @param itemIn        The designated item
 * @param pItemOut      The pointer to the returned item
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_NODATA   Non-existent item
 * @retval ERR_GET      Invalid parameter to store returned item
 */
int32_t RBTreeSearch(RedBlackTree *self, Item itemIn, Item *pItemOut);

/**
* @brief Delete the item having the same order with the designated one.
 *
 * This function first searches for the item having the same order with the
 * designated one. If such item can be found, it will be removed. Also, if the
 * custom resource clean method is set, it runs the clean method for the removed
 * item.
 *
 * @param self          The pointer to the RedBlackTree structure
 * @param item          The designated item
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_NODATA   Non-existent item
 * @retval ERR_GET      Invalid parameter to store returned item
 */
int32_t RBTreeDelete(RedBlackTree *self, Item item);

/**
 * @brief Return the item having the maximum order of the tree.
 *
 * @param self          The pointer to the RedBlackTree structure
 * @param pItem         The pointer to the returned item
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_IDX      Empty tree
 * @retval ERR_GET      Invalid parameter to store returned item
 */
int32_t RBTreeMaximum(RedBlackTree *self, Item *pItem);

/**
 * @brief Return the item having the minimum order of the tree.
 *
 * @param self          The pointer to the RedBlackTree structure
 * @param pItem         The pointer to the returned item
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_IDX      Empty tree
 * @retval ERR_GET      Invalid parameter to store returned item
 */
int32_t RBTreeMinimum(RedBlackTree *self, Item *pItem);

/**
 * @brief Return the item which is the immediate successor of the designated item.
 *
 * @param self          The pointer to the RedBlackTree structure
 * @param itemIn        The designated item
 * @param pItemOut      The pointer to the returned item
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_NODATA   Non-existent immediate successor
 * @retval ERR_GET      Invalid parameter to store returned item
 */
int32_t RBTreeSuccessor(RedBlackTree *self, Item itemIn, Item *pItemOut);

/**
 * @brief Return the item which is the immediate predecessor of the designated item.
 *
 * @param self          The pointer to the RedBlackTree structure
 * @param itemIn        The designated item
 * @param pItemOut      The pointer to the returned item
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_NODATA   Non-existent immediate predecessor
 * @retval ERR_GET      Invalid parameter to store returned item
 */
int32_t RBTreePredecessor(RedBlackTree *self, Item itemIn, Item *pItemOut);

/**
 * @brief Return the number of stored items.
 *
 * @param self          The pointer to the RedBlackTree structure.
 *
 * @return              The number of stored items
 * @retval ERR_NOINIT   Uninitialized container
 */
int32_t RBTreeSize(RedBlackTree *self);

/**
 * @brief Set the custom item comparison method.
 *
 * @param self          The pointer to the RedBlackTree structure
 * @param pFunc         The function pointer to the custom method
 *
 * @retval ERR_NOINIT   Uninitialized container
 */
int32_t RBTreeSetCompare(RedBlackTree *self, int32_t (*pFunc) (Item, Item));

/**
 * @brief Set the custom item resource clean method.
 *
 * @param self          The pointer to the RedBlackTree structure
 * @param pFunc         The function pointer to the custom method
 *
 * @retval ERR_NOINIT   Uninitialized container
 */
int32_t RBTreeSetDestroy(RedBlackTree *self, void (*pFunc) (Item));


#endif

