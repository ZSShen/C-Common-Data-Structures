#ifndef _BIN_SEARCH_TREE_H_
#define _BIN_SEARCH_TREE_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#define SUCCESS             (0)
#define FAIL                (-1)


/* Wrapper for BinSearchTree initialization. */
#define BinSearchTree_init(p)       p = (BinSearchTree*)malloc(sizeof(BinSearchTree)); \
                                    if (p) {                                           \
                                        int32_t rc = BSTreeInit(p);                    \
                                        if(rc == FAIL) {                               \
                                            BSTreeDeinit(p);                           \
                                            p = NULL;                                  \
                                        }                                              \
                                    }

/* Wrapper for BinSearchTree deinitialization. */
#define BinSearchTree_deinit(p)     BSTreeDeinit(p);                                   \
                                    p = NULL;                                          \

typedef struct _TreeNode TreeNode;
typedef struct _BSTreeData BSTreeData;

typedef struct _BinSearchTree {
    BSTreeData *pData;
    TreeNode* (*insert)     (struct _BinSearchTree*, void*);
    void      (*delete)     (struct _BinSearchTree*, TreeNode*);
    uint32_t  (*size)       (struct _BinSearchTree*);     
    bool      (*search)     (struct _BinSearchTree*, void*);
    TreeNode* (*maximum)    (struct _BinSearchTree*);
    TreeNode* (*minimum)    (struct _BinSearchTree*);
    TreeNode* (*successor)  (struct _BinSearchTree*, TreeNode*);  
    TreeNode* (*predecessor)(struct _BinSearchTree*, TreeNode*);
    void (*set_compare)(struct _BinSearchTree*, int32_t (*) (const void*, const void*));
    void (*set_destroy)(struct _BinSearchTree*, void (*) (void*));
} BinSearchTree;


/* Constructor for BinSearchTree structure. */
int32_t BSTreeInit(BinSearchTree *self);


/* Destructor for BinSearchTree structure. */
void BSTreeDeinit(BinSearchTree *self);


/**
 * This function inserts a new node storing the requested item to the appropriate position of the tree.
 *
 * @param self          The pointer to the BinSearchTree structure.
 * @param pItem         The pointer to the item which is to be inserted to the tree.
 *
 * @return              Non-NULL: The pointer to the successfully inserted node.
 *                      NULL    : The node cannot be inserted due to insufficient memory space. 
 */
TreeNode* BSTreeInsert(BinSearchTree *self, void *pItem);


/**
 * This function deletes the specified node from the tree and adjusts the tree structure.
 *
 * @param self          The pointer to the BinSearchTree structure.
 * @param pNode         The pointer to the node which is to be deleted from the tree.
 */
void BSTreeDelete(BinSearchTree *self, TreeNode *pNode);


/**
 * This function returns the size of the tree.
 *
 * @param self          The pointer to the BinSearchTree structure.
 *
 * @return              The size;
 */
uint32_t BSTreeSize(BinSearchTree *self);


/**
 * This function checks whethere the tree has the specified item.
 *
 * @param self          The pointer to the BinSearchTree structure.
 * @param pItem         The pointer to the item which is to be checked.
 *
 * @return              true : The item exists.
 *                      false: The item does not exist.
 */
bool BSTreeSearch(BinSearchTree *self, void *pItem);


/**
 * This function returns the node with maximum order of the tree.
 *
 * @param self          The pointer to the BinSearchTree structure.
 *
 * @return              Non-NULL: The pointer to the node with maximum order.
 *                      NULL    : The tree is empty.
 */
TreeNode* BSTreeMaximum(BinSearchTree *self);


/**
 * This function returns the node with minimum order of the tree.
 *
 * @param self          The pointer to the BinSearchTree structure.
 *
 * @return              Non-NULL: The pointer to the node with minimum order.
 *                      NULL    : The tree is empty.
 */
TreeNode* BSTreeMinimum(BinSearchTree *self);


/**
 * This function returns the successor of the designated node.
 *
 * @param self          The pointer to the BinSearchTree structure.
 *
 * @return              Non-NULL: The pointer to the successor.
 *                      NULL    : There is no successor for the designated node.
 */
TreeNode* BSTreeSuccessor(BinSearchTree *self, TreeNode *pCurNode);


/**
 * This function returns the predecessor of the designated node.
 *
 * @param self          The pointer to the BinSearchTree structure.
 *
 * @return              Non-Null: The pointer to the predecessor.
 *                      NULL    : There is no predecessor for the designated node.
 */
TreeNode* BSTreePredecessor(BinSearchTree *self, TreeNode *pCurNode);


/**
 * This function sets the item comparison strategy with the one defined by user.
 *
 * @param self          The pointer to the BinSearchTree structure.
 * @param pFunc         The pointer to the customized function.
 */
void BSTreeSetCompare(BinSearchTree *self, int32_t (*pFunc)(const void*, const void*));


/**
 * This function sets the item deallocation strategy with the one defined by user.
 *
 * @param self          The pointer to the BinSearchTree structure.
 * @param pFunc         The pointer to the customized function.
 */
void BSTreeSetDestroy(BinSearchTree *self, void (*pFunc)(void*));

#endif
