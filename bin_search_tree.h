#ifndef _BIN_SEARCH_TREE_H_
#define _BIN_SEARCH_TREE_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

/* Wrapper for binary search tree structure initialization. */
#define BinSearchTree_init(p)       p = NULL; \
                                    p = malloc(sizeof(BinSearchTree)); \
                                    if (p != NULL) \
                                        BSTreeInit(p);     

/* Wrapper for binary search tree structure initialization. */
#define BinSearchTree_deinit(p)     BSTreeDeinit(p); \
                                    free(p); \
                                    p = NULL;

typedef struct _TreeNode {
    void    *pItem;
    struct _TreeNode    *pParent;
    struct _TreeNode    *pLeft;
    struct _TreeNode    *pRight;
} TreeNode;


typedef struct _BinSearchTree {
    TreeNode    *pRoot;
        
    int       (*compare) (const void*, const void*);
    void      (*destroy) (void*);
    void      (*insert) (struct _BinSearchTree*, TreeNode*);
    void      (*delete) (struct _BinSearchTree*, TreeNode*);
    TreeNode* (*maximum) (struct _BinSearchTree*);
    TreeNode* (*minimum) (struct _BinSearchTree*);
    TreeNode* (*successor) (struct _BinSearchTree*, TreeNode*);  
    TreeNode* (*predecessor) (struct _BinSearchTree*, TreeNode*);
} BinSearchTree;


/* Constructor for binary search tree structure. */
void BSTreeInit(BinSearchTree *self);


/* Destructor for binary search tree structure. */
void BSTreeDeinit(BinSearchTree *self);


/**
 * This function inserts the new node to the appropriate location of the tree.
 *
 * @param self          The pointer to the BinSearchTree structure.
 * @param pNode         The pointer to the node which is to be inserted to the tree.
 */
void BSTreeInsert(BinSearchTree *self, TreeNode *pNode);


/**
 * This function deletes the specified node from the tree and adjusts the tree structure.
 *
 * @param self          The pointer to the BinSearchTree structure.
 * @param pNode         The pointer to the node which is to be deleted from the tree.
 */
void BSTreeDelete(BinSearchTree *self, TreeNode *pNode);


/**
 * This function locates the node containing the maximum key of the tree.
 *
 * @param self          The pointer to the BinSearchTree structure.
 *
 * @return              Non-NULL: The pointer to the node containing maximum key.
 *                      NULL    : The tree is empty.
 */
TreeNode* BSTreeMaximum(BinSearchTree *self);


/**
 * This function locates the node containing the minimum key of the tree.
 *
 * @param self          The pointer to the BinSearchTree structure.
 *
 * @return              Non-NULL: The pointer to the node containing minimum key.
 *                      NULL    : The tree is empty.
 */
TreeNode* BSTreeMinimum(BinSearchTree *self);


/**
 * This function locates the successor of the designated node.
 *
 * @param self          The pointer to the BinSearchTree structure.
 *
 * @return              Non-NULL: The pointer to the successor.
 *                      NULL    : There is no successor of the designated node.
 */
TreeNode* BSTreeSuccessor(BinSearchTree *self, TreeNode *pCurNode);


/**
 * This function locates the predecessor of the designated node.
 *
 * @param self          The pointer to the BinSearchTree structure.
 *
 * @return              Non-Null: The pointer to the successor.
 *                      NULL    : There is no predecessor of the designated node.
 */
TreeNode* BSTreePredecessor(BinSearchTree *self, TreeNode *pCurNode);


/**
 * The default function for node item comparison.
 *
 * @param pSrc          The pointer to the source item.
 * @param pDst          The pointer to the target item.
 *
 * @return               1: The key contained by source item is larger than the target one.
 *                       0: The key contained by source item is equal to the target one.
 *                      -1: The key contained by source item is less than the target one.
 */
int BSTreeNodeCompare(const void *pSrc, const void *pTge);


/**
 * The default function for item deallocation.
 * 
 * @param pItem         The pointer to the item which is to be deallocated.
 */
void BSTreeNodeDestroy(void *pItem);

#endif
