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
typedef struct _SimTreeNode SimTreeNode;
typedef struct _SimTreeData SimTreeData;

typedef struct _SimpleTree {
    SimTreeData *pData;
    int32_t (*insert) (struct _SimpleTree*, Item);
    int32_t (*search) (struct _SimpleTree*, Item*);
    int32_t (*delete) (struct _SimpleTree*, Item);
    int32_t (*maximum) (struct _SimpleTree*, Item*);
    int32_t (*minimum) (struct _SimpleTree*, Item*);
    int32_t (*successor) (struct _SimpleTree*, Item*);
    int32_t (*predecessor) (struct _SimpleTree*, Item*);
    uint32_t (*size) (struct _SimpleTree*);
    void (*set_compare) (struct _SimpleTree*, int32_t (*) (Item, Item));
    void (*set_destroy) (struct _SimpleTree*, void (*) (Item));
} SimpleTree;


/**
 * The constructor for SimpleTree.
 *
 * @param ppObj         The double pointer to the to be constructed tree.
 *
 * @return              SUCCESS
 *                      FAIL_NO_MEMORY
 */
int32_t SimTreeInit(SimpleTree **ppObj);

/**
 * The destructor for SimpleTree.
 *
 * @param ppObj          The double pointer to the to be destructed tree.
 */
void SimTreeDeinit(SimpleTree **ppObj);

/**
 * @param self          The pointer to the SimpleTree structure.
 * @param item          The requested item.
 *
 * @return              SUCCESS
 *                      FAIL_NO_MEMORY
 *                      FAIL_DATA_CONFLICT
 *
 * This function inserts the requested item into the proper location of the tree.
 * It will fail when:
 *     1. Insufficient memory space.
 *     2. The requested item conflicts with the one stored in the tree.
 */
int32_t SimTreeInsert(SimpleTree *self, Item item);

/**
 * @param self          The pointer to the SimpleTree structure.
 * @param pItem         The pointer to the requested item.
 *
 * @return              SUCCESS
 *                      FAIL_NO_DATA
 *
 * This function checks whether the tree has the requested item. If the input
 * item can be found, it's content is replaced with the content of the item
 * found in the tree.
 * It will fail when:
 *     1. The requested item cannot be found in the tree.
 */
int32_t SimTreeSearch(SimpleTree *self, Item *pItem);

/**
 * @param self          The pointer to the SimpleTree structure.
 * @param item          The requested item.
 *
 * @return              SUCCESS
 *                      FAIL_NO_DATA
 *
 * This function deletes the requested item from the tree and adjusts the tree structure.
 * It will fail when:
 *     1. The requested item cannot be found in the tree.
 */
int32_t SimTreeDelete(SimpleTree *self, Item item);

/**
 * @param self          The pointer to the SimpleTree structure.
 * @param pItem         The pointer to the returned item.
 *
 * @return              SUCCESS
 *                      FAIL_NO_DATA
 *
 * This function returns the item with the maximum order of the tree.
 * It will fail when:
 *     1. The tree is empty.
 */
int32_t SimTreeMaximum(SimpleTree *self, Item *pItem);

/**
 * @param self          The pointer to the SimpleTree structure.
 * @param pItem         The pointer to the returned item.
 *
 * @return              SUCCESS
 *                      FAIL_NO_DATA
 *
 * This function returns the item with the minimum order of the tree.
 * It will fail when:
 *     1. The tree is empty.
 */
int32_t SimTreeMinimum(SimpleTree *self, Item *pItem);

/**
 * @param self         The pointer to the SimpleTree structure.
 * @param pItem        The pointer to the returned item.
 *
 * @return             SUCCESS
 *                     FAIL_NO_DATA
 *
 * This function returns the item which is the successor of the requested one.
 * It will fail when:
 *     1. The successor cannot be found.
 */
int32_t SimTreeSuccessor(SimpleTree *self, Item *pItem);

/**
 * @param self         The pointer to the SimpleTree structure.
 * @param pItem        The pointer to the returned item.
 *
 * @return             SUCCESS
 *                     FAIL_NO_DATA
 *
 * This function returns the item which is the predecessor of the requested one.
 * It will fail when:
 *     1. The predecessor cannot be found.
 */
int32_t SimTreePredecessor(SimpleTree *self, Item *pItem);

/**
 * uint32_t (*size) (SimpleTree *self)
 *
 * @param self          The pointer to the SimpleTree structure.
 *
 * @return              Tree size
 *
 * This function returns the size of the tree.
 */
uint32_t SimTreeSize(SimpleTree *self);

/**
 * void (*set_compare) (SimpleTree *self, int32_t (*pFunc) (Item, Item))
 *
 * @param self          The pointer to the SimpleTree structure.
 * @param pFunc         The pointer to the user defined function.
 *
 * This function sets the user defined item comparison strategy.
 */
void SimTreeSetCompare(SimpleTree *self, int32_t (*pFunc) (Item, Item));

/**
 * @param self          The pointer to the SimpleTree structure.
 * @param pFunc         The pointer to the user defined function.
 *
 * This function sets the user defined item deallocation strategy.
 */
void SimTreeSetDestroy(SimpleTree *self, void (*pFunc) (Item));

#endif
