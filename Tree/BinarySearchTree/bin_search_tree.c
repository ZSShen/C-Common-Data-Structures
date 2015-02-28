#include "bin_search_tree.h"


/*===========================================================================*
 *                  Hide the private data of the tree                        *
 *===========================================================================*/
struct _TreeNode {
    Item item;
    struct _TreeNode *pParent;
    struct _TreeNode *pLeft;
    struct _TreeNode *pRight;
};

struct _BSTreeData {
    uint32_t _uiSize;
    TreeNode *_pRoot;
    int32_t (*_pCompare)(Item, Item);
    void (*_pDestroy)(Item);
};

#define DIRECTION_LEFT      (0)
#define DIRECTION_RIGHT     (1)


/*===========================================================================*
 *             Definition for the exported member operations                 *
 *===========================================================================*/
/**
 * The constructor for BinSearchTree.
 *
 * @param self          The pointer to the BinSearchTree structure.
 *
 * @return              SUCCESS: Operation success.
 *                      FAIL_NO_MEMORY   : Insufficient memory space.
 */
int32_t BSTreeInit(BinSearchTree *self);

/**
 * The destructor for BinSearchTree.
 */
void BSTreeDeinit(BinSearchTree *self);

/**
 * Insert the requested item into the proper location of the tree.
 */
int32_t BSTreeInsert(BinSearchTree *self, Item item);

/**
 * Check whethere the tree has the requested item.
 */
int32_t BSTreeSearch(BinSearchTree *self, Item item);

/**
 * Delete the requested item from the tree and adjust the tree structure.
 */
int32_t BSTreeDelete(BinSearchTree *self, Item item);


/*===========================================================================*
 *                  Definition for internal functions                        *
 *===========================================================================*/
/**
 * Traverse all the nodes and release the allocated memory space.
 *
 * @param pCurr      The pointer to the node currently traversed.
 * @param func      The pointer to the function which describes the node item deallocation strategy.
 */
void _BSTreeDeinit(TreeNode *pCurr, void (*func) (Item));

/**
 * Return the node with minimal order in the subtree rooted by the requested node.
 *
 * @param pCurr      The pointer to the requested node.
 *
 * @return           Non-NULL: The pointer to the target node.
 *                   NULL    : The subtree is empty.
 */
TreeNode* _BSTreeMinimal(TreeNode *pCurr);

/**
 * Return the node with maximal order in the subtree rooted by the requested node.
 *
 * @param pCurr      The pointer to the requested node.
 *
 * @return           Non-NULL: The pointer to the target node.
 *                   NULL    : The subtree is empty.
 */
TreeNode* _BSTreeMaximal(TreeNode *pCurr);

/**
 * Return the successor of the requested node.
 *
 * @param pCurr      The pointer to the requested node.
 *
 * @return           Non-NULL: The pointer to the target node.
 *                   NULL    : No successor can be found.
 */
TreeNode* _BSTreeSuccessor(TreeNode *pCurr);

/**
 * Return the predecessor of the requested node.
 *
 * @param pCurr      The pointer to the requested node.
 *
 * @return           Non-NULL: The pointer to the target node.
 *                   NULL    : No predecessor can be found.
 */
TreeNode* _BSTreePredecessor(TreeNode *pCurr);

/**
 * Return the node containing the requested item.
 *
 * @param self       The pointer to the BinSearchTree structure.
 * @param item      The pointer to the requested item.
 *
 * @return           Non-NULL: The pointer to the target node.
 *                   NULL    : No matched node.
 */
TreeNode* _BSTreeSearch(BinSearchTree *self, Item item);

/**
 * This function is the default comparison strategy for the items of a pair of tree nodes.
 *
 * @param pSrc          The pointer to the source item.
 * @param pDst          The pointer to the target item.
 *
 * @return        > 0 : The source item goes after the target one with certain ordering criteria.
 *                  0 : The source item equals to the target one with certain ordering criteria.
 *                < 0 : The source item goes before the target one with certain ordering criteria.
 */
int32_t _BSTreeItemCompare(Item pSrc, Item pTge);


/**
 * This function is the default deallcation strategy for an item.
 *
 * @param item         The pointer to the item which is to be deallocated.
 */
void _BSTreeItemDestroy(Item item);


/*===========================================================================*
 *           Implementation for the exported member operations               *
 *===========================================================================*/
int32_t BSTreeInit(BinSearchTree *self)
{
    self->pData = (BSTreeData*)malloc(sizeof(BSTreeData));
    if (!self->pData)
        return FAIL_NO_MEMORY;

    self->pData->_pRoot = NULL;
    self->pData->_uiSize = 0;
    self->pData->_pCompare = _BSTreeItemCompare;
    self->pData->_pDestroy = _BSTreeItemDestroy;

    self->insert = BSTreeInsert;
    self->delete = BSTreeDelete;
    self->search = BSTreeSearch;
    self->size = BSTreeSize;

    self->maximum = BSTreeMaximum;
    self->minimum = BSTreeMinimum;
    self->successor = BSTreeSuccessor;
    self->predecessor = BSTreePredecessor;

    self->set_compare = BSTreeSetCompare;
    self->set_destroy = BSTreeSetDestroy;

    return SUCCESS;
}

void BSTreeDeinit(BinSearchTree *self)
{
    if (self) {
        if (self->pData) {
            _BSTreeDeinit(self->pData->_pRoot, self->pData->_pDestroy);
            free(self->pData);
        }
        free(self);
    }
    return;
}

int32_t BSTreeInsert(BinSearchTree *self, Item item)
{
    int32_t iOrder;
    int8_t cDirect;
    TreeNode *pNew, *pCurr, *pParent;

    pNew = (TreeNode*)malloc(sizeof(TreeNode));
    if (!pNew)
        return FAIL_NO_MEMORY;
    pNew->item = item;
    pNew->pParent = NULL;
    pNew->pLeft = NULL;
    pNew->pRight = NULL;

    pParent = NULL;
    pCurr = self->pData->_pRoot;
    while (pCurr) {
        pParent = pCurr;
        iOrder = self->pData->_pCompare(item, pCurr->item);
        if (iOrder > 0) {
            pCurr = pCurr->pRight;
            cDirect = DIRECTION_RIGHT;
        }
        else if (iOrder < 0) {
            pCurr = pCurr->pLeft;
            cDirect = DIRECTION_LEFT;
        }
        else {
            free(pNew);
            return FAIL_DATA_CONFLICT;
        }
    }

    /* Arrive at the proper position. */
    pNew->pParent = pParent;
    if (pParent) {
        if (cDirect == DIRECTION_LEFT)
            pParent->pLeft = pNew;
        else
            pParent->pRight = pNew;
    } else
        self->pData->_pRoot = pNew;

    /* Increase the size. */
    self->pData->_uiSize++;

    return SUCCESS;
}

int32_t BSTreeSearch(BinSearchTree *self, Item item)
{
    TreeNode *pFind;
    pFind = _BSTreeSearch(self, item);
    if (pFind)
        return SUCCESS;
    return FAIL_NO_DATA;
}

int32_t BSTreeDelete(BinSearchTree *self, Item item)
{
    TreeNode *pCurr, *pChild, *pSucc, *pParent;

    pCurr = _BSTreeSearch(self, item);
    if (pCurr) {
        /* The specified node has no child. */
        if ((!pCurr->pLeft) && (!pCurr->pRight)) {
            if (pCurr->pParent) {
                if (pCurr == pCurr->pParent->pLeft)
                    pCurr->pParent->pLeft = NULL;
                else
                    pCurr->pParent->pRight = NULL;
            } else
                self->pData->_pRoot = NULL;

            self->pData->_pDestroy(pCurr->item);
            free(pCurr);
        } else {
            /* The specified node has two children. */
            if ((pCurr->pLeft) && (pCurr->pRight)) {
                pSucc = _BSTreeSuccessor(pCurr);

                pChild = pSucc->pLeft;
                if (!pChild)
                    pChild = pSucc->pRight;

                if (pChild)
                    pChild->pParent = pSucc->pParent;

                if (pSucc == pSucc->pParent->pLeft)
                    pSucc->pParent->pLeft = pChild;
                else
                    pSucc->pParent->pRight = pChild;

                self->pData->_pDestroy(pCurr->item);
                pCurr->item = pSucc->item;
                free(pSucc);
            }
            /* The specified node has one child. */
            else {
                pChild = pCurr->pLeft;
                if (!pChild)
                    pChild = pCurr->pRight;

                pChild->pParent = pCurr->pParent;

                if (pCurr->pParent) {
                    if (pCurr == pCurr->pParent->pLeft)
                        pCurr->pParent->pLeft = pChild;
                    else
                        pCurr->pParent->pRight = pChild;
                } else
                    self->pData->_pRoot = pChild;

                self->pData->_pDestroy(pCurr->item);
                free(pCurr);
            }
        }

        /* Decrease the size. */
        self->pData->_uiSize--;
        return SUCCESS;
    }

    return FAIL_NO_DATA;
}

/**
 * BSTreeMinimum(): Return the node with minimum order of the tree.
 */
TreeNode* BSTreeMinimum(BinSearchTree *self)
{
    return _BSTreeMinimal(self->pData->_pRoot);
}


/**
 * BSTreeMaximum(): Return the node with maximum order of the tree.
 */
TreeNode* BSTreeMaximum(BinSearchTree *self)
{
    return _BSTreeMaximal(self->pData->_pRoot);
}


/**
 * BSTreeSuccessor(): Return the successor of the designated node.
 */
TreeNode* BSTreeSuccessor(BinSearchTree *self, TreeNode *pCurNode)
{
    return _BSTreeSuccessor(pCurNode);
}


/**
 * BSTreePredecessor(): Return the predecessor of the designated node.
 */
TreeNode* BSTreePredecessor(BinSearchTree *self, TreeNode *pCurNode)
{
    return _BSTreePredecessor(pCurNode);
}


/**
 * BSTreeSize(): Return the size of the tree.
 */
uint32_t BSTreeSize(BinSearchTree *self)
{
    return self->pData->_uiSize;
}


/**
 * BSTreeSetCompare(): Set the item comparison strategy with the one defined by user.
 */
void BSTreeSetCompare(BinSearchTree *self, int32_t (*pFunc)(Item, Item))
{
    self->pData->_pCompare = pFunc;
    return;
}


/**
 * BSTreeSetDestroy(): Set the item deallocation strategy with the one defined by user.
 */
void BSTreeSetDestroy(BinSearchTree *self, void (*pFunc)(Item))
{
    self->pData->_pDestroy = pFunc;
    return;
}


/*===========================================================================*
 *               Implementation for internal functions                       *
 *===========================================================================*/
void _BSTreeDeinit(TreeNode *pCurr, void (*func) (Item))
{
    if (pCurr) {
        _BSTreeDeinit(pCurr->pLeft, func);
        _BSTreeDeinit(pCurr->pRight, func);
        func(pCurr->item);
        free(pCurr);
    }
    return;
}

TreeNode* _BSTreeMinimal(TreeNode *pCurr)
{
    TreeNode *pParent;
    pParent = NULL;
    while (pCurr) {
        pParent = pCurr;
        pCurr = pCurr->pLeft;
    }
    return pParent;
}

TreeNode* _BSTreeMaximal(TreeNode *pCurr)
{
    TreeNode *pParent;
    pParent = NULL;
    while (pCurr) {
        pParent = pCurr;
        pCurr = pCurr->pRight;
    }
    return pParent;
}

TreeNode* _BSTreeSuccessor(TreeNode *pCurr)
{
    if (pCurr) {
        /* Case 1: The minimal node of the non-null right subtree. */
        if (pCurr->pRight)
            pCurr = _BSTreeMinimal(pCurr->pRight);

        /* Case 2: The ancestor which considers the designated node as the maximal node of
           its left subtree. */
        else {
            while((pCurr->pParent) && (pCurr == pCurr->pParent->pRight))
                pCurr = pCurr->pParent;
            pCurr = pCurr->pParent;
        }
    }
    return pCurr;
}

TreeNode* _BSTreePredecessor(TreeNode *pCurr)
{
    if (pCurr) {
        /* Case 1: The maximal node of the non-null left subtree. */
        if (pCurr->pLeft)
            pCurr = _BSTreeMaximal(pCurr->pLeft);

        /* Case 2: The ancestor which considers the designated node as the minimal node of
           its right subtree. */
        else {
            while((pCurr->pParent) && (pCurr == pCurr->pParent->pLeft))
                pCurr = pCurr->pParent;
            pCurr = pCurr->pParent;
        }
    }
    return pCurr;
}

TreeNode* _BSTreeSearch(BinSearchTree *self, Item item)
{
    int32_t iOrder;
    TreeNode *pCurr = self->pData->_pRoot;
    while(pCurr) {
        iOrder = self->pData->_pCompare(item, pCurr->item);
        if (iOrder == 0)
            break;
        else {
            if (iOrder > 0)
                pCurr = pCurr->pRight;
            else
                pCurr = pCurr->pLeft;
        }
    }
    return pCurr;
}


/**
 * _BSTreeItemCompare(): The default comparison strategy for the items of a pair of tree nodes.
 * Note: It considers source and target items as primitive data and
 *       gives the larger order to the one with larger value.
 */
int32_t _BSTreeItemCompare(Item pSrc, Item pTge)
{
    int32_t nSrc, nTge;

#if __x86_64__
    int64_t nTmp;
    nTmp = (int64_t)pSrc;
    nSrc = (int32_t)nTmp;
    nTmp = (int64_t)pTge;
    nTge = (int32_t)nTmp;
#else
    nSrc = (int32_t)pSrc;
    nTge = (int32_t)pTge;
#endif

    return nSrc - nTge;
}


/**
 * This function is the default deallcation strategy for an item.
 *
 * @param item         The pointer to the item which is to be deallocated.
 */
void _BSTreeItemDestroy(Item item);

/**
 * _BSTreeItemDestroy(): The default deallocation strategy for a node item.
 * Note: By default, the item is a primitive data, and thus no further operations are required.
 */
void _BSTreeItemDestroy(Item item)
{ return; }
