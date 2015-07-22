/**
 * @file binary_tree.h The basic binary search tree.
 */

#ifndef _BINARY_TREE_H_
#define _BINARY_TREE_H_

#include "../util.h"

/** BinTreeData is the data type for the container private information. */
typedef struct _BinTreeData BinTreeData;

/** The implementation for basic binary search tree. */
typedef struct _BinaryTree {
    /** The container private information. */
    BinTreeData *pData;

    /** Insert an item into the tree.
        @see BinTreeInsert */
    int32_t (*insert) (struct _BinaryTree*, Item);

    /** Search for the item having the same order with the designated one.
        @see BinTreeSearch */
    int32_t (*search) (struct _BinaryTree*, Item, Item*);

    /** Delete the item having the same order with the designated one.
        @see BinTreeDelete */
    int32_t (*delete) (struct _BinaryTree*, Item);

    /** Get the item having the maximum order of the tree.
        @see BinTreeMaximum */
    int32_t (*maximum) (struct _BinaryTree*, Item*);

    /** Get the item having the minimum order of the tree.
        @see BinTreeMinimum */
    int32_t (*minimum) (struct _BinaryTree*, Item*);

    /** Get the immediate successor of the designated item.
        @see BinTreeSuccessor */
    int32_t (*successor) (struct _BinaryTree*, Item, Item*);

    /** Get the immediate predecessor of the designated item.
        @see BinTreePredecessor */
    int32_t (*predecessor) (struct _BinaryTree*, Item, Item*);

    /** Get the number of stored item.
        @see BinTreeSize */
    int32_t (*size) (struct _BinaryTree*);

    /** Set the user defined item comparison method.
        @see BinTreeSetCompare */
    int32_t (*set_compare) (struct _BinaryTree*, int32_t (*) (Item, Item));

    /** Set the user defined item clean method.
        @see BinTreeSetDestroy */
    int32_t (*set_destroy) (struct _BinaryTree*, void (*) (Item));
} BinaryTree;


/**
 * @brief The constructor for BinaryTree.
 *
 * @param ppObj         The double pointer to the to be constructed tree
 *
 * @retval SUCC
 * @retval ERR_NOMEM    Insufficient memory for tree construction
 */
int32_t BinTreeInit(BinaryTree **ppObj);

/**
 * @brief The destructor for BinaryTree.
 *
 * If the custom resource clean method is set, it also runs the clean method for
 * all the items.
 *
 * @param ppObj         The double pointer to the to be destructed tree
 */
void BinTreeDeinit(BinaryTree **ppObj);

/**
 * @brief Insert an item into the tree.
 *
 * This function inserts an item into the tree. If the order of the designated
 * item is the same with a certain one stored in the tree, that item will be
 * replaced. Also, if the custom resource clean method is set, it also runs the
 * clean method for the replaced item.
 *
 * @param self          The pointer to the BinaryTree structure
 * @param item          The designated item
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_NOMEM    Insufficient memory for tree extension
 */
int32_t BinTreeInsert(BinaryTree *self, Item item);

/**
 * @brief Search for the item having the same order with the designated one.
 *
 * This function searches for an item having the same order with the designated
 * one. If such item can be found, it will be returned by the third parameter.
 * Otherwise, the error code is returned and the third parameter is updated with
 * NULL.
 *
 * @param self          The pointer to the BinaryTree structure
 * @param itemIn        The designated item
 * @param pItemOut      The pointer to the returned item
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_NODATA   Non-existent item
 * @retval ERR_GET      Invalid parameter to store returned item
 */
int32_t BinTreeSearch(BinaryTree *self, Item itemIn, Item *pItemOut);

/**
 * @brief Delete the item having the same order with the designated one.
 *
 * This function first searches for the item having the same order with the
 * designated one. If such item can be found, it will be removed. Also, if the
 * custom resource clean method is set, it also runs the clean method for the
 * removed item.
 *
 * @param self          The pointer to the BinaryTree structure
 * @param item          The designated item
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_NODATA   Non-existent item
 */
int32_t BinTreeDelete(BinaryTree *self, Item item);

/**
 * @brief Return the item having the maximum order of the tree.
 *
 * @param self          The pointer to the BinaryTree structure
 * @param pItem         The pointer to the returned item
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_IDX      Empty tree
 * @retval ERR_GET      Invalid parameter to store returned item
 */
int32_t BinTreeMaximum(BinaryTree *self, Item *pItem);

/**
 * @brief Return the item having the minimum order of the tree.
 *
 * @param self          The pointer to the BinaryTree structure
 * @param pItem         The pointer to the returned item
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_IDX      Empty tree
 * @retval ERR_GET      Invalid parameter to store returned item
 */
int32_t BinTreeMinimum(BinaryTree *self, Item *pItem);

/**
 * @brief Return the item which is the immediate successor of the designated item.
 *
 * @param self          The pointer to the BinaryTree structure
 * @param itemIn        The designated item
 * @param pItemOut      The pointer to the returned item
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_NODATA   Non-existent immediate successor
 * @retval ERR_GET      Invalid parameter to store returned item
 */
int32_t BinTreeSuccessor(BinaryTree *self, Item itemIn, Item *pItemOut);

/**
 * @brief Return the item which is the immediate predecessor of the designated item.
 *
 * @param self          The pointer to the BinaryTree structure
 * @param itemIn        The designated item
 * @param pItemOut      The pointer to the returned item
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_NODATA   Non-existent immediate predecessor
 * @retval ERR_GET      Invalid parameter to store returned item
 */
int32_t BinTreePredecessor(BinaryTree *self, Item itemIn, Item *pItemOut);

/**
 * @brief Return the number of stored items.
 *
 * @param self          The pointer to the BinaryTree structure.
 *
 * @return              The number of stored items
 * @retval ERR_NOINIT   Uninitialized container
 */
int32_t BinTreeSize(BinaryTree *self);

/**
 * @brief Set the custom item comparison method.
 *
 * @param self          The pointer to the BinaryTree structure
 * @param pFunc         The function pointer to the custom method
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 */
int32_t BinTreeSetCompare(BinaryTree *self, int32_t (*pFunc) (Item, Item));

/**
 * @brief Set the custom item resource clean method.
 *
 * @param self          The pointer to the BinaryTree structure
 * @param pFunc         The function pointer to the custom method
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 */
int32_t BinTreeSetDestroy(BinaryTree *self, void (*pFunc) (Item));

#endif
