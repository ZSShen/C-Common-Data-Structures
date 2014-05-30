#ifndef _RED_BLACK_TREE_H_
#define _RED_BLACK_TREE_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


/* Wrapper for red black tree structure initialization. */
#define RedBlackTree_init(p)        p = NULL; \
                                    p = (RedBlackTree*)malloc(sizeof(RedBlackTree)); \
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

    RedBlackNode*   (*insert)     (struct _RedBlackTree*, void*);
    void            (*delete)     (struct _RedBlackTree*, RedBlackNode*);
    unsigned long   (*size)       (struct _RedBlackTree*);
    RedBlackNode*   (*search)     (struct _RedBlackTree*, void*);
    RedBlackNode*   (*maximum)    (struct _RedBlackTree*);
    RedBlackNode*   (*minimum)    (struct _RedBlackTree*);
    RedBlackNode*   (*successor)  (struct _RedBlackTree*, RedBlackNode*);  
    RedBlackNode*   (*predecessor)(struct _RedBlackTree*, RedBlackNode*);

    void            (*set_compare)(struct _RedBlackTree*, int (*)(const void*, const void*));
    void            (*set_destroy)(struct _RedBlackTree*, void (*)(void*));
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
 *
 * @return              Non-NULL: The pointer to the successfully inserted node containing the requested item.
 *                      NULL    : Fail to insert the requested item due to unsuccessful memory allocation. 
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
RedBlackNode* RBTreeSearch(RedBlackTree *self, void *pItem);


/**
 * This function returns the size of the tree.
 * 
 * @param self          The pointer to the RedBlackTree structure.
 * 
 * @return              unsigned long: The size.
 */
unsigned long RBTreeSize(RedBlackTree *self);


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


/*
 * This function sets the item comparison strategy with the one defined by user.
 *
 * @param self          The pointer to the RedBlackTree structure.
 * @param pFunc         The pointer to the customized function.
 */
void RBTreeSetCompare(RedBlackTree *self, int (*pFunc)(const void*, const void*));


/*
 * This function sets the item destroy strategy with the one defined by user.
 * @param self          The pointer to the RedBlackTree structure.
 * @param pFunc         The pointer to the customized function.
 */
void RBTreeSetDestroy(RedBlackTree *self, void (*pFunc)(void*));

#endif

