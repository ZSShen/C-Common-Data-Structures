#ifndef _RED_BLACK_TREE_H_
#define _RED_BLACK_TREE_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


/* Wrapper for red black tree structure initialization. */
#define RedBlackTree_init(p)        p = NULL; \
                                    p = malloc(sizeof(RedBlackTree)); \
                                    if (p != NULL) \
                                        RBTreeInit(p);     

/* Wrapper for red black tree structure initialization. */
#define RedBlackTree_deinit(p)      if (p != NULL) { \
                                        RBTreeDeinit(p); \
                                        free(p); \
                                        p = NULL; \
                                    }

typedef struct _RedBlackNode {
    void*   pItem;    
    bool    bColor;
    struct _RedBlackNode    *pParent;
    struct _RedBlackNode    *pLeft;
    struct _RedBlackNode    *pRight;
} RedBlackNode;


typedef struct _RedBlackTree {
    RedBlackNode    *pRoot, *pNull;

    int             (*compare)    (const void*, const void*);
    void            (*destroy)    (void*);
    RedBlackNode*   (*insert)     (struct _RedBlackTree*, void*);
    void            (*delete)     (struct _RedBlackTree*, RedBlackNode*);
    bool            (*search)     (struct _RedBlackTree*, void*);
    RedBlackNode*   (*maximum)    (struct _RedBlackTree*);
    RedBlackNode*   (*minimum)    (struct _RedBlackTree*);
    RedBlackNode*   (*successor)  (struct _RedBlackTree*, RedBlackNode*);  
    RedBlackNode*   (*predecessor)(struct _RedBlackTree*, RedBlackNode*);
} RedBlackTree;


/* Constructor for red black tree structure. */
void RBTreeInit(RedBlackTree *self);


/* Destructor for red black tree structure. */
void RBTreeDeinit(RedBlackTree *self);


/**
 * This function inserts the new red black node to the appropriate location of the tree.
 * Since the new node insertion may violate the attributes of red black tree, the corresponding 
 * tree structure adjustment is necessary.
 *
 * @param self          The pointer to the RedBlackTree structure.
 * @param pItem         The pointer to the item which is to be inserted to the tree.
 */
RedBlackNode* RBTreeInsert(RedBlackTree *self, void *pItem);


/**
 * This function deletes the specified red black node from the tree and adjusts the tree structure.
 *
 * @param self          The pointer to the RedBlackTree structure.
 * @param pNode         The pointer to the node which is to be deleted from the tree.
 */
void RBTreeDelete(RedBlackTree *self, RedBlackNode *pNode);


/**
 * This function checks whethere the tree has the designated item.
 *
 * @param self          The pointer to the RedBlackTree structure.
 * @param pItem         The pointer to the item which is to be checked.
 *
 * @return              true : The item exists.
 *                      false: The item does not exist.
 */
bool RBTreeSearch(RedBlackTree *self, void *pItem);


/**
 * This function locates the node containing the maximum key of the tree.
 *
 * @param self          The pointer to the RedBlackTree structure.
 *
 * @return              Non-NULL: The pointer to the node containing maximum key.
 *                      NULL    : The tree is empty.
 */
RedBlackNode* RBTreeMaximum(RedBlackTree *self);


/**
 * This function locates the node containing the minimum key of the tree.
 *
 * @param self          The pointer to the RedBlackTree structure.
 *
 * @return              Non-NULL: The pointer to the node containing minimum key.
 *                      NULL    : The tree is empty.
 */
RedBlackNode* RBTreeMinimum(RedBlackTree *self);


/**
 * This function locates the successor of the designated node.
 *
 * @param self          The pointer to the RedBlackTree structure.
 *
 * @return              Non-NULL: The pointer to the successor.
 *                      NULL    : There is no successor of the designated node.
 */
RedBlackNode* RBTreeSuccessor(RedBlackTree *self, RedBlackNode *pCurNode);


/**
 * This function locates the predecessor of the designated node.
 *
 * @param self          The pointer to the RedBlackTree structure.
 *
 * @return              Non-Null: The pointer to the predecessor.
 *                      NULL    : There is no predecessor of the designated node.
 */
RedBlackNode* RBTreePredecessor(RedBlackTree *self, RedBlackNode *pCurNode);

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
int RBTreeNodeCompare(const void *pSrc, const void *pTge);


/**
 * The default function for item deallocation.
 * 
 * @param pItem         The pointer to the item which is to be deallocated.
 */
void RBTreeNodeDestroy(void *pItem);

#endif

